#define _WINSOCK_DEPRECATED_NO_WARNINGS //ͷ�ļ�����ԭ����ԭ�к����������������Ա���
#include <stdio.h>
#include <winsock2.h>//windows �µ�socket2.2
#include<iostream>
#include<string>
#pragma comment(lib,"ws2_32.lib")  //ʹ��socket������ؿ�

int main(int argc, char* argv[])
{
	//��ʼ��WSA  
	//����һ��2.2����Э��
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}
	//�����׽���  

	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("server socket error !");
		return 0;
	}

	//��IP�Ͷ˿�  
	sockaddr_in sin;
	sin.sin_family = AF_INET;//Э����
	sin.sin_port = htons(12345);//�˿�
	sin.sin_addr.S_un.S_addr = INADDR_ANY;//�����ַ
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//��ʼ����  
	if (listen(slisten, SOMAXCONN))//SOMAXCONN ���еȴ�����������Ӹ���
	{
		printf("listen error !");
		return 0;
	}

	//ѭ����������  
	SOCKET sClient;
	
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];
	while (true)
	{
		printf("�ȴ�����...\n");
		sClient = accept(slisten, (SOCKADDR*)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		//��������  
		int ret = recv(sClient, revData, 255, 0);
		if (ret > 0)
		{
			revData[ret] = 0x00;
			printf(revData);
		}

		//��������  
		const char* sendData = "��ã�TCP�ͻ��ˣ�\n";
		std::string temp = "�����Ƿ����";
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
