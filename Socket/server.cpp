#include <WINSOCK2.H>
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include "Thread.h"
#include "Thread.cpp"
#include "slove.cpp"

#define SERVER_PORT		10000
#define MSGSIZE			1024
//#define

#pragma comment(lib, "ws2_32.lib")
using namespace std;


class CMyTask: public CTask
{
	public:
		CMyTask() = default;
		int Run()
		{
			//printf("%s\n", (char*)m_ptrData);
			//int x = rand()%4 + 1;
			//sleep(x);
			//return 0;

			int connfd= GetConnFd();
			cout<<"���ڼ������߳�"<<pthread_self()<<"���ӵĿͻ�����Ϣ"<<endl;
			while(1)
			{
				char *szMessage;
				char *zsMessage;
				int retSize = recv(connfd, szMessage, MSGSIZE, 0);
				if (retSize < 0)
				{
					cout << "���߳�"<<pthread_self()<<"�󶨵Ŀͻ����ж�����" << endl;
					break;
				}
				szMessage[retSize] = '\0';
				cout<<"���յ����߳�"<<pthread_self()<<"���ӵĿͻ�����Ϣ��"<<szMessage<<endl;


				zsMessage=deal(szMessage);

				int sendSize = send(connfd, zsMessage, MSGSIZE, 0);
				if (sendSize < 0)
				{
					cout << "����ʧ�ܣ���������Ϊ���߳�"<<pthread_self()<<"���ӵĿͻ��������жϣ�" << endl;
					break;
				}
				cout<<"�ɹ���Ӧ���߳�"<<pthread_self()<<"���ӵĿͻ���  ";
				cout<<"���ڼ������߳�"<<pthread_self()<<"���ӵĿͻ�����Ϣ"<<endl;

			}
			closesocket(connfd);
			return 0;
		}
};

int main()
{
	WSADATA wsaData;
	int sListen;

	SOCKET localServer;
	SOCKET localClient;
	//SOCKET
	SOCKADDR_IN local;
	SOCKADDR_IN client;

	int retSize;
	int sendSize;
	WSAStartup(0x0202, &wsaData);

	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	local.sin_family = AF_INET;
	local.sin_port = htons(SERVER_PORT);
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sListen, (struct sockaddr *) &local, sizeof(SOCKADDR_IN));

	listen(sListen, 1);
	CThreadPool Pool(5);


	while (TRUE)
	{
		int addrSize = sizeof(SOCKADDR_IN);
		int sClient = accept(sListen, (struct sockaddr *) &client, &addrSize);
		if(sClient<0)
		{
			printf("cli connect failed.");
			// throw  std::exception();

		}
		//�յ��ͻ������󣬼���ӵ��������ȥ
		else
		{
			CTask* ta=new CMyTask;
			ta->SetConnFd(sClient);
			Pool.AddTask(ta);
		}
	}
	return 0;
}

