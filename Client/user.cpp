#include "user.h"

user::user(std::string name, std::string password, std::map<std::string, bool> permission) {
	this->name = name;
	this->password = password;
	this->permission["write"] = permission["write"];
	this->permission["read"] = permission["read"];
	this->permission["insert"] = permission["insert"];
	this->permission["select"] = permission["select"];
	this->permission["update"] = permission["update"];
	this->permission["delete"] = permission["delete"];
}
user::user() {
	this->name = "\0";
	this->password = "\0";
	this->permission["write"] = "false";
	this->permission["read"] = "false";
	this->permission["insert"] = "false";
	this->permission["select"] = "false";
	this->permission["update"] = "false";
	this->permission["delete"] = "false";
}
void user::create_user() {
    user u;
    std::string name;
    std::cout << "�������û�����" << std::endl;
    std::cin >> name;

    std::string password;
    std::cout << "���������룺" << std::endl;
    std::cin >> password;

    std::string have;
    std::cout << "��������û���Ȩ�ޣ�w->дȨ�ޣ�r->��Ȩ�ޣ�wr->��дȨ�ޣ���" << std::endl;
    std::cin >> have;

    u.name = name;
    u.password = password;
    for (int i = 0; i < have.length(); ++i)
    {
        if (have[i] == 'w')
        {
            u.permission["write"] = "true";
            u.permission["delete"] = "true";
            u.permission["insert"] = "true";
            u.permission["select"] = "true";
            u.permission["update"] = "true";
        }

        if (have[i] == 'r')
            u.permission["read"] = "true";
    }

    std::ofstream fout;
    fout.open("user.txt", std::ios::app);//��׷�ӷ�ʽ��txt�ļ�
    fout << "������" << u.name << " ���룺" << u.password
        << " дȨ�� " << u.permission["write"]
        << " ��Ȩ�� " << u.permission["read"] 
        << " ���� " << u.permission["insert"]
        << " ɾ�� " << u.permission["delete"]
        << " ɸѡ " << u.permission["select"]
        << " ���� " << u.permission["update"] << std::endl;
    fout.close();
    //std::cout << "�û������ɹ������¼��" << std::endl;
}

user user::stringToUSER(std::string s)//�洢�û��ͷ���Ȩ�޵���Ϣ
{
    user temp;
    std::string t = "";
    int i = 6;
    int len = s.size();


    //����ȡ�ļ��е����ģ�ֻ��ȡӢ��
    while (i < len)
    {
        if (s[i] == ' ') break;
        else t += s[i];
        i++;
    }
    temp.name = t;
    t = "";

    i = i + 7;
    while (i < len)
    {
        if (s[i] == ' ') break;
        else t += s[i];
        i++;
    }
    temp.password = t;
    t = "";

    i = i + 8;
    while (i < len)
    {
        if (s[i] == ' ') break;
        else t += s[i];
        i++;
    }
    temp.permission["write"] = t;
    t = "";

    i = i + 8;
    while (i < len)
    {
        if (s[i] == ' ') break;
        else t += s[i];
        i++;
    }
    temp.permission["read"] = t;
    t = "";
    return temp;
}

user user::query_user(std::string name, std::string password) {

    user tmp;
    std::ifstream fin;
    fin.open("user.txt");
    bool find = false;
    std::string s;

    while (!fin.eof())//eof��������������ж��ļ��Ƿ�Ϊ�ջ����Ƿ�����ļ���β
    {
        getline(fin, s);//��������fin�ж�ȡһ���ַ���s��
        tmp = stringToUSER(s);
        if (tmp.name == name && tmp.password == password)
        {
            fin.close();
            return tmp;
        }
    }
    fin.close();
    user t;
    return t;
}
