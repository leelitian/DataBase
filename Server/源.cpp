#include <iostream>
#include <io.h>
#include <direct.h>
#include "logger.h"
#include "user.h"
#include "table.h"

#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include "Thread.h"
using namespace std;

vector<table> Tab, copyTab;
user U;

void split(const string& s, vector<string>& tokens, const char& delim = ' ')
{
	tokens.clear();
	size_t lastPos = s.find_first_not_of(delim, 0);
	size_t pos = s.find(delim, lastPos);
	while (lastPos != string::npos)
	{
		tokens.emplace_back(s.substr(lastPos, pos - lastPos));
		lastPos = s.find_first_not_of(delim, pos);
		pos = s.find(delim, lastPos);
	}
}

void init()
{
	initLogger("info_log_file", "warn_log_file", "error_log_file");
	std::string prefix = "table";
	if (_access(prefix.c_str(), 0) == -1)	//����ļ��в�����
		_mkdir(prefix.c_str());				//�򴴽�

	ifstream table_in("table.txt", ios::in);
	string tab_name, col_name, type_name;
	int cnt, cond;
	while (table_in >> tab_name >> cnt)
	{
		table tmp_tab;
		tmp_tab.table_name = tab_name;
		tmp_tab.col_len = cnt;
		for (int i = 0; i < cnt; ++i)
		{
			table_in >> col_name >> type_name >> cond;
			tmp_tab.col_name.push_back(col_name);
			tmp_tab.type_value.push_back(type_name);
			tmp_tab.condition.push_back(cond);
		}
		Tab.push_back(tmp_tab);
	}
	table_in.close();
}

string help(string op)
{
	string ret;
	//help database��������������ݱ����Ϣ��ͬʱ��ʾ��������ͣ�
	if (op == "help database")
	{
		if (Tab.size() == 0) return "�����ݿ�����������!\n";
		//�����
		for (int i = 0; i < Tab.size(); ++i)
		{
			ret += "----------------��ʾ��" + Tab[i].table_name + " ������------------------\n";
			for (int j = 0; j < Tab[i].col_name.size(); ++j)//��ʾ����
				ret += Tab[i].col_name[j] += "   \n";
			for (int j = 0; j < Tab[i].col_name.size(); ++j)//��ʾ��������
				ret += Tab[i].type_value[j] += " \n";
		}

		return ret;
	}

	//����help table �������������ݱ����������Ե���ϸ��Ϣ��
	if (op.substr(0, 10) == "help table")
	{
		string name = op.substr(11);

		bool flag = false;
		for (int k = 0; k < Tab.size(); ++k)
		{
			if (Tab[k].table_name == name)
			{
				ret += "---------------- ��ʾ��" + Tab[k].table_name + "������------------------\n";

				ifstream fin;
				string tt = "table/"+Tab[k].table_name + ".txt";
				char file[30];
				for (int i = 0; i < tt.length(); ++i)
					file[i] = tt[i];
				file[tt.length()] = '\0';
				fin.open(file);
				string t;
				while (getline(fin, t))
					ret += t + "\n";
				ret += "\n";
				flag = true;
				return ret;
			}
		}

		return name + "�����ڣ�\n";
	}

	return "NULL";
}

//�������
string insert_a(string op)
{
	string s = "insert into ";
	if (op.substr(0, 12) != s)
		return "NULL";

	//ȷ�����ĸ����в�������,���жϱ��Ƿ����
	string table_name = "";
	int i = s.length();
	for (; i < op.length() && op[i] != ' '; ++i)
		table_name += op[i];

	int k = -1;
	for (int j = 0; j < Tab.size(); ++j)
	{
		if (Tab[j].table_name == table_name)
		{
			k = j;		//k�����˲��뵽��������еĵڼ�������
			break;
		}
	}
	if (k == -1)
	{
		return table_name + "�����ڣ�\n";
	}

	//�ж�����ʽ
	s = "values(";
	++i;
	for (int j = 0; j < s.length(); ++j, ++i)
	{
		if (op[i] != s[j])
		{
			return "insert�﷨����\n";
		}
	}
	//ȷ�����������
	vector<string> vec;
	string temp = "";
	while (i < op.length())
	{
		if (op[i] == ',' || op[i] == ')')//�������Ż��������žͷ�������
		{
			vec.push_back(temp);
			temp = "";
		}
		else if (op[i] == '\'' || op[i] == ' ')//�����źͿո�ʲôҲ����
			;
		else
			temp += op[i];
		++i;
	}
	if (vec.size() != Tab[k].col_name.size())
	{
		return "�����Ԫ����ñ���Ӧ��\n";
	}
	//�ж�Լ������
	for (int i = 0; i < Tab[k].col_name.size(); ++i)//�����жϸ����ֶ�
	{
		if (Tab[k].condition[i] == 1 && vec[i] == "")
			return "��������Ϊ�գ�\n";
		else if (Tab[k].condition[i] == 3 && vec[i] == "")
			return "Լ������not null�����ܽ��в��룡\n";
		else if (Tab[k].condition[i] == 2 && vec[i] == "null")
			return "Լ������unique�����ܽ��в��룡\n";
	}

	ofstream fout;
	temp = "table/" + Tab[k].table_name + ".txt";
	char file[30];
	for (int i = 0; i < temp.length(); ++i)
		file[i] = temp[i];
	file[temp.length()] = '\0';
	fout.open(file, ios::app);
	for (int i = 0; i < vec.size() - 1; ++i)
		fout << vec[i] << " ";
	fout << vec[vec.size() - 1] << endl;
	fout.close();

	// Tab[k].remcount++;
	return  "������1����¼\n";
}

//ִ��ɾ������
string delete_b(string& table_name, string& key)
{
	if (key.front() == '\'' || key.back() == '\'')
		key = string(key.begin() + 1, key.end() - 1);

	string ret = "";
	table_name = "table/" + table_name + ".txt";			//table_name���ļ�����key��Ҫ���ҵĹؼ���
	fstream file;
	file.open(table_name.c_str());	//�����ص�ǰ�ַ��������ַ���ַ

	ofstream tempfile("temp.txt", ios::out | ios::trunc);		//ios::trunc����ļ��Ѵ�������ɾ�����ļ�
	string line;

	int delete_num = 0;
	while (getline(file, line))
	{
		if (line.find(key) == string::npos)	//���ĳһ��û���ҵ�Ҫ���ҵĹؼ��֣�foundΪĬ��ֵ-1.
			tempfile << line << endl;		//��û�йؼ��������е����ݴ�����ʱ�ļ�in2.txt�����൱��ɾ���˴��ؼ��ֵ�һ��
		else
			delete_num++;
	}
	tempfile.close();
	file.close();

	ofstream output;
	output.open(table_name.c_str());
	fstream tmp_file("temp.txt");
	while (getline(tmp_file, line))
	{
		output << line << endl;
	}
	output.close();
	tmp_file.close();

	system("del temp.txt");		//ɾ����ʱ�ļ�

	table ttt;
	for (int counts = 0; counts < Tab.size(); counts++)
	{
		if (table_name == Tab[counts].table_name)
		{
			ttt = Tab[counts];
		}
	}
	ttt.remcount -= delete_num;

	ret += "�ɹ�ɾ����";
	ret += to_string(delete_num);
	ret += "��Ԫ��\n";
	return ret;
}

// �ж�ɾ������ʽ
string delete_a(string op)
{
	vector<string> tokens;
	split(op, tokens);
	
	if (tokens[0] != "delete" || tokens[1] != "from" || tokens[3] != "where")
		return "NULL";

	string table_name = tokens[2];

	int k = -1;
	for (int j = 0; j < Tab.size(); ++j)
	{
		if (Tab[j].table_name == table_name)
		{
			k = j;
			break;
		}
	}
	if (k == -1)
		return table_name + "�����ڣ�\n";

	vector<string> cmp;
	split(tokens[4], cmp, '=');

	return delete_b(table_name, cmp[1]);
}

string select_b(string& table_name, string& key, string& colname)
{
	if (key.front() == '\'' || key.back() == '\'')
		key = string(key.begin() + 1, key.end() - 1);

	string ret = "";
	table t;
	t.table_name = "table/" + table_name;
	t.table_name += ".txt";//ttname���ļ�����tt��Ҫ���ҵĹؼ���
	fstream file;
	int found;
	file.open(t.table_name.c_str());//c_str��Borland��װ��String���е�һ�������������ص�ǰ�ַ��������ַ���ַ

	string line;
	while (!file.eof())
	{
		getline(file, line);//������ȡtxt�ļ�������
		//tt���ǹؼ��֣���select_t()�����л�ȡ��
		found = line.find(key);	//found��ֵΪҪ���ҵĹؼ����������е��±�
		if (found != -1)		//���ĳһ��û���ҵ�Ҫ���ҵĹؼ��֣�foundΪĬ��ֵ-1.
		{
			ret += line;
			ret += '\n';
		}
	}
	return ret;
	file.close();
}

//��ѯ���
string select_a(string op)
{
	string ret;
	vector<string> tokens;
	split(op, tokens);

	if (tokens[0] != "select" || tokens[2] != "from" || tokens[4] != "where")
		return "NULL";
//select sname from student where sno='001'
	string colname = tokens[1];
	string table_name = tokens[3];

	//��ʵ����
	int k = -1;
	for (int j = 0; j < Tab.size(); ++j)
	{
		if (Tab[j].table_name == table_name)
		{
			k = j;
			break;
		}
	}
	if (k == -1)
	{
		return table_name + "�����ڣ�\n";
	}

	string where = "";

	//��ȡѡ����������ƣ����ؼ��ֵ�����where sno='201401060634'����sno
	vector<string> cmp;
	split(tokens[5], cmp, '=');

	ret += "��ѯ�ɹ���\n";
	ret += select_b(table_name, cmp[1], colname);
	
	return ret;
}

string update_b(string& table_name, string& t1, string& t2)
{
	if (t1.front() == '\'' || t1.back() == '\'')
		t1 = string(t1.begin() + 1, t1.end() - 1);

	if (t2.front() == '\'' || t2.back() == '\'')
		t2 = string(t2.begin() + 1, t2.end() - 1);

	int numcounts = 0;
	table_name =  "table/" + table_name + ".txt";
	fstream file;

	file.open(table_name.c_str());//c_str()�������ص�ǰ�ַ��������ַ���ַ
	ofstream tempfile("temp.txt", ios::out | ios::trunc);
	string line;
	while (getline(file, line))
	{
		if (line.find(t2) == string::npos)				//���ĳһ��û���ҵ�Ҫ���ҵĹؼ��֣�foundΪĬ��ֵ-1.
			tempfile << line << endl;
		else
		{
			size_t pos = line.find(t2);
			while (pos != string::npos)
			{
				line.replace(pos, t2.length(), t1);//���µĴ��滻��ָ���Ĵ�, ��t1�滻ָ���ַ�������ʼλ��pos��ʼ����Ϊlen���ַ�
				numcounts++;
				pos = line.find(t2);//��������ָ���Ĵ���ֱ�����еĶ��ҵ�Ϊֹ
			}
			tempfile << line << endl;
		}
	}
	tempfile.close();
	file.close();
	ofstream outfile1;
	outfile1.open(table_name.c_str());
	fstream file1("temp.txt");
	while (getline(file1, line))
	{
		outfile1 << line << endl;
	}
	outfile1.close();
	file1.close();
	system("del temp.txt");//ɾ����ʱ�ļ�
	return "�ɹ��޸���" + to_string(numcounts) + "��Ԫ�飡\n";

}
// �������
string update_a(string op)
{
	string ret;
	vector<string> tokens;
	split(op, tokens);

	if (tokens[0] != "update" || tokens[2] != "set" || tokens[4] != "where")
		return "NULL";

	string name = tokens[1];
	int k = -1;
	for (int j = 0; j < Tab.size(); ++j)
	{
		if (Tab[j].table_name == name)
		{
			k = j;
			break;
		}
	}
	if (k == -1)
	{
		return name + "�����ڣ�\n";
	}

	vector<string> cmp1;
	split(tokens[3], cmp1, '=');
	vector<string> cmp2;
	split(tokens[5], cmp2, '=');

	return update_b(name, cmp1[1], cmp2[1]);

}

// grant select on table student to sly;
string grant(string op)
{
	string result = "NULL";
	vector<string> tokens;
	split(op, tokens);

	if (tokens[0] != "grant" || tokens[2] != "on" || tokens[3] != "table" || tokens[5] != "to")
		return "NULL";

	user tempuser = U;
	string privilege = tokens[1];

	if (privilege == "insert" || privilege == "delete" || privilege == "update" || privilege == "select")
	{
		tempuser.permission[privilege] = "true";
		result = "�ɹ������û�" + privilege + "Ȩ��";
	}

	U = tempuser;
	return result;
}

// revoke select on table student from sly;
string revoke(string op)
{
	string result = "NULL";
	vector<string> tokens;
	split(op, tokens);

	if (tokens[0] != "revoke" || tokens[2] != "on" || tokens[3] != "table" || tokens[5] != "from")
		return "NULL";

	user tempuser = U;
	string privilege = tokens[1];

	if (privilege == "insert" || privilege == "delete" || privilege == "update" || privilege == "select")
	{
		tempuser.permission[privilege] = "false";
		result = "�ɹ������û�" + privilege + "Ȩ��";
	}

	U = tempuser;
	return result;
}

void Roll_back()
{
	copyTab = Tab;
}

string deal(string recv)
{
	vector<string> tokens;
	split(recv, tokens);
	string ret;
	
	if (tokens[0] == "1")
	{
		user ut = ut.query_user(tokens[1], tokens[2]);
		if (ut.name != tokens[1] || ut.password != tokens[2]) return "FAILED";
		else
		{
			U = ut;

			if (U.permission["write"] == "true" && U.permission["read"] == "true")
			{
				ret += "Create       Help       Select\n";
				ret += "Update       Insert     Delete\n";
			}
			else if (U.permission["write"] == "false" && U.permission["read"] == "true")
			{
				ret += "Help         Select\n";
			}
			else if (U.permission["write"] == "true" && U.permission["read"] == "false")
			{
				ret += "Create       Update       Insert     Delete\n";
			}
			else
				ret += "NULL";
			return "OK " + ret;
		}
	}
	else if (tokens[0] == "2")
	{
		user u;
		u.create_user(tokens[1], tokens[2], tokens[3]);
		return "OK";
	}
	else
	{
		table tabt;
		if (U.permission["write"] == "true")
		{
			ret = help(recv);
			if (ret != "NULL") return ret;
			ret = insert_a(recv);
			if (ret != "NULL") return ret;
			ret = grant(recv);
			if (ret != "NULL") return ret;
			ret = revoke(recv);
			if (ret != "NULL") return ret;
			ret = tabt.create_table(recv, Tab);
			if (ret != "NULL") return ret;
			ret = delete_a(recv);
			if (ret != "NULL") return ret;
			ret = select_a(recv);
			if (ret != "NULL") return ret;
			ret = update_a(recv);
			if (ret != "NULL") return ret;

			return "�����������������������\n";
		}
		else
		{
			if (U.permission["select_u"] == "true")
			{
				ret = select_a(recv);
				if (ret != "NULL") return ret;
			}
			if (U.permission["insert_u"] == "true")
			{
				ret = insert_a(recv);
				if (ret != "NULL") return ret;
			}
			if (U.permission["delete_u"] == "true")
			{
				ret = delete_a(recv);
				if (ret != "NULL") return ret;
			}
			if (U.permission["update_u"] == "true")
			{
				ret = update_a(recv);
				if (ret != "NULL") return ret;
			}

			ret = select_a(recv);
			if (ret != "NULL") return ret;
			return "�����������������������\n";
		}
		return ret;
	}
	return ret;
}

class CMyTask : public CTask
{
public:
	CMyTask() = default;
	int Run()
	{
		int connfd = GetConnFd();
		char szMessage[MSGSIZE];

		while (1)
		{
			int retSize = recv(connfd, szMessage, MSGSIZE, 0);
			if (retSize < 0)
			{
				break;
			}
			szMessage[retSize] = '\0';

			string recieve(szMessage, szMessage + retSize);
			LOG(log_rank::INFO) << szMessage << endl;
			std::cout << "recv: " + recieve << endl;		//�յ�
			if (recieve == "quit")
				break;
			string zs = deal(recieve);
			std::cout << "send: " + zs << endl;			// ����
			int sendSize = send(connfd, zs.c_str(), strlen(zs.c_str()), 0);
			if (sendSize < 0) break;
		}
		closesocket(connfd);
		return 0;
	}
};

int main(int argc, char** argv)
{
	init();

	WSADATA wsaData;
	int sListen;

	//SOCKET
	SOCKADDR_IN local;
	SOCKADDR_IN client;

	WSAStartup(0x0202, &wsaData);

	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	local.sin_family = AF_INET;
	local.sin_port = htons(SERVER_PORT);
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sListen, (struct sockaddr*)&local, sizeof(SOCKADDR_IN));

	listen(sListen, 1);
	CThreadPool Pool(5);

	while (TRUE)
	{
		int addrSize = sizeof(SOCKADDR_IN);
		int sClient = accept(sListen, (struct sockaddr*)&client, &addrSize);
		if (sClient < 0)
		{
			printf("cli connect failed.");
		}
		//�յ��ͻ������󣬼���ӵ��������ȥ
		else
		{
			CTask* ta = new CMyTask;
			ta->SetConnFd(sClient);
			Pool.AddTask(ta);
		}
	}

	return 0;
}