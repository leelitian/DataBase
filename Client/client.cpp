#include<iostream>
#include"client_method.h"
using namespace std;
int main()
{
	int port;
	string server_address;
	char* zsMessage=new char[1024];
	char flag;
	cout<<"�Ƿ�ʹ��Ĭ�����ã���y/n��"<<endl;
	cin>>flag;

	while(1)
	{
		if(flag=='y'||flag=='Y')
		{
			startClient(10000,"127.0.0.1");
			cout<<"�ͻ���������ɡ�"<<endl;
			break;
		}
		else if(flag=='n'||flag=='N')
		{
			cout<<"�����������ip��ַ��"<<endl;
			cin>>server_address;
			cout<<"������������˿ںţ�"<<endl;
			cin>>port;
			startClient(port, server_address);
			cout<<"�ͻ���������ɡ�"<<endl;
			break;
		}
		else
		{
			cout<<"�������������������"<<endl;
			continue;
		}
	}


	while(1)
	{
		cout<<"������sql��䣺"<<endl;
		cin>>zsMessage;
		char* result=clientSendMsg (zsMessage);
		cout<<"�����"<<' '<<result<<endl;
	}

	cleanClient();
	return 0;
}
