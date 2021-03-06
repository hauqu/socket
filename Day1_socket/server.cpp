#define _WINSOCK_DEPRECATED_NO_WARNINGS //头文件升级原因导致原有函数报错，加上这句忽略报错
#include <stdio.h>
#include <winsock2.h>//windows 下的socket2.2
#include<iostream>
#include<string>
#pragma comment(lib,"ws2_32.lib")  //使用socket必须加载库

int main(int argc, char* argv[])
{
	//初始化WSA  
	//启动一个2.2网络协议
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}
	//创建套接字  

	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("server socket error !");
		return 0;
	}

	//绑定IP和端口  
	sockaddr_in sin;
	sin.sin_family = AF_INET;//协议族
	sin.sin_port = htons(12345);//端口
	sin.sin_addr.S_un.S_addr = INADDR_ANY;//任意地址
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//开始监听  
	if (listen(slisten, SOMAXCONN))//SOMAXCONN 队列等待处理最大连接个数
	{
		printf("listen error !");
		return 0;
	}

	//循环接收数据  
	SOCKET sClient;
	
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];
	while (true)
	{
		printf("等待连接...\n");
		sClient = accept(slisten, (SOCKADDR*)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		//接收数据  
		int ret = recv(sClient, revData, 255, 0);
		if (ret > 0)
		{
			revData[ret] = 0x00;
			printf(revData);
		}

		//发送数据  
		const char* sendData = "你好，TCP客户端！\n";
		std::string temp = "这里是服务端";
		std::cin >> temp;
		sendData = temp.c_str();
		send(sClient, sendData, strlen(sendData), 0);
		
		
		//sendData = temp.c_str();
		//send(sClient, sendData, strlen(sendData), 0);
		closesocket(sClient);
	}

	closesocket(slisten);
	WSACleanup();
	return 0;
}
