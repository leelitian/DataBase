#include <iostream>
#include <WINSOCK2.H>
#include <string>

#include "client.h"
using namespace std;

int main(int argc, char** argv)
{
	int port;
	string server_address;
	char* zsMessage = new char[MSGSIZE];
	char flags;

	while (1)
	{
		cout << "�Ƿ�ʹ��Ĭ�����ã���y/n��" << endl;
		cin >> flags;
		if (flags == 'y' || flags == 'Y')
		{
			startClient(10000, "127.0.0.1");
			cout << "�ͻ���������ɡ�" << endl;
			break;
		}
		else if (flags == 'n' || flags == 'N')
		{
			cout << "�����������ip��ַ��" << endl;
			cin >> server_address;
			cout << "������������˿ںţ�" << endl;
			cin >> port;
			startClient(port, server_address);
			cout << "�ͻ���������ɡ�" << endl;
			break;
		}
		else
		{
			cout << "�������������������" << endl;
			continue;
		}
	}

	string name, password;
	char user_choose;
main_page:
	system("cls");

	cout << "                          *************************" << endl;
	cout << "                          *   ��ӭ����HNUDBMS  *" << endl;
	cout << "                          *************************" << endl;
	cout << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
	cout << "��ѡ����Ҫ���еĲ���" << endl;
	cout << "�û���¼-->1" << endl;
	cout << "ע���û�-->2" << endl;

	cin >> user_choose;
	while (true)
	{
		if (user_choose == '1')
			goto usr_load;
		else if (user_choose == '2')
		{
			cout << "�������û�����" << endl;
			cin >> name;

			cout << "���������룺" << endl;
			cin >> password;

			string have;
			cout << "��������û���Ȩ�ޣ�w->дȨ�ޣ�r->��Ȩ�ޣ�wr->��дȨ�ޣ���" << endl;
			cin >> have;

			string result = clientSendMsg("2 " + name + " " + password + " " + have);
			break;
		}
		else
		{
			cout << "�Բ��������������" << endl;
			goto main_page;
		}
	}
	goto main_page;
usr_load:
	while (true)
	{
		printf("�û���: ");
		cin >> name;
		printf("����:");
		cin >> password;

		string result = clientSendMsg("1 " + name + " " + password);

		// �����û���������
		if (result == "FAILED")
		{
			cout << "�û������������" << endl;
			continue;
		}
		else
		{
			cout << name << "��ã���ӭʹ��DBMS\n��ӵ������Ȩ�ޣ�" << endl;
			cout << result.substr(3);
			break;
		}
	}

	string operations;

	cin.get();
	while (true)
	{
		cout << "----------------------------------------------------" << endl;
		cout << "������SQL���" << endl;
		cout << "----------------------------------------------------" << endl;

		getline(cin, operations);		//���������ж����ַ�������operations����
		if(operations == "quit")
			goto main_page;
		string result = clientSendMsg(operations);
		cout << result << endl;
	}
	return 0;
}