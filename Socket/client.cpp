#include<iostream>
#include"client_method.cpp"
using namespace std;
int main()
{
	int port;
	string server_address;
	char* zsMessage=new char[1024];
	char flags;
	cout<<"�Ƿ�ʹ��Ĭ�����ã���y/n��"<<endl;
	cin>>flags;

	while(1)
	{
		if(flags=='y'||flags=='Y')
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
