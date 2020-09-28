#include "table.h"
/*
bool table::check_exist(std::string table_name) {
    std::ifstream fin;
    fin.open("table.txt");
    bool find = false;
    std::string s;

    while (!fin.eof())//eof��������������ж��ļ��Ƿ�Ϊ�ջ����Ƿ�����ļ���β
    {
        getline(fin, s);
        if (s.find("������" + table_name) != std::string::npos) {
            fin.close();
            return true;
        }
    }
    fin.close();
    return false;
}
*/
void table::create_table(std::string op, int& flag, std::vector<table>& tab)
{
    std::string s = "create table ";
    int f = 1;
    for (int i = 0; i < s.length(); ++i)
    {
        if (op[i] != s[i])
        {
            f = 0;
            break;
        }
    }
    if (f)
    {
        flag = 2;
        //create_table(op);
        table t;
        t.col_len = 0;
        int flag;

        std::string tmp = "create table ";
        int k = tmp.length();
        //��ȡ�������
        t.table_name = "";
        for (; (op[k] != '\n' || op[k] != '(' || op[k] != ' ') && k < op.length(); ++k)
            t.table_name += op[k];
        //��ȡ�����ֶ������ֶ�����
        std::string s;
        while (std::getline(std::cin, s) && s != ")")//ֻ�лس�����ֹgetline������ÿ�λس�����һ��ѭ���������֣���ֹ
        {
            t.col_len++;
            int f = 1;
            int i = 0;
            tmp = "";
            //����һ���ַ����������䳤��

            while (!((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')))//��ֹ�ڵڶ��п�ͷ���֣����߿ո����������޷�����
                ++i;

            for (; s[i] != ' ' && i < s.length(); ++i)//�����ַ������ֶ���������
            {
                tmp += s[i];
            }

            if (tmp.length() == 0)
            {
                std::cout << "create �﷨����" << std::endl;
                return ;
            }

            int m = 0;
            tmp = "";

            while (!((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')))//��ֹ�ֶ������ֶ������м�ո�������ɿ�
                ++i;

            for (; s[i] != '(' && s[i] != ' ' && s[i] != ',' && i < s.length(); ++i)//�����ַ������ֶ����ʹ�����
            {
                tmp += s[i];
                // m=i;
            }

            //�ж��ֶ������Ƿ���ȷ
            if ((tmp != "char") && (tmp != "int") && (tmp != "float") && (tmp != "double") && (tmp != "time") && (tmp != "date"))
            {
                std::cout << "�ֶ����ʹ���" << std::endl;
                return;
            }

            if (tmp.length() == 0)
            {
                std::cout << "create �﷨����" << std::endl;
                return;
            }

            //ȥ���˽�β�Ķ��ţ��Լ��ڶ��п�ʼ�ģ���ȫ����ɿո�
            for (int i = 0; i < s.length(); i++)
                if (s[i] == ',')
                    s[i] = ' ';
            if (s[0] == '(')
                s[0] = ' ';

            //�����Լ���Ļ�����ΪԼ������һ���ַ��������Բ�����֮ǰ�İ����ַ����жϣ����ж�ǰ�����ַ����Ƿ�����ֶ������ֶ�����֮��
            //Ȼ���ж�ʣ����ַ����Ƿ����޶�����

            std::stringstream ss(s);//������ĵ�һ��ת�����������տո��������ַ����ض�
            std::string s1;
            std::vector<std::string> in;
            while (ss >> s1)//��ȡss�е�ֵ��s1��ÿ��ѭ����ss�ͱ����տո�ض�
                in.push_back(s1);
            t.col_name.push_back(in[0]); //�ֶ���
            t.type_value.push_back(in[1]); //�ֶ�����

            std::string yueshu = "";
            for (int i = 2; i < in.size(); i++)
            {
                yueshu += in[i];//�ڶ����ַ��������ȫ��ΪԼ������
                if (i != in.size() - 1) yueshu += " ";//��Լ�������еĿո�Ҳ��ȡ����
            }

            if (yueshu == "primary key")
                t.condition.push_back(1);
            else if (yueshu == "unique")
                t.condition.push_back(2);
            else if (yueshu == "not null")
                t.condition.push_back(3);
            else t.condition.push_back(0);
            //cout << "---" << t.condition << yueshu << endl;
        }
        /*
        if (check_exist(table_name))
            goto TABLE_EXIST;
        */
        //��t�����table��������
        tab.push_back(t);
        //�洢�������ݱ�
        std::ofstream fout;
        fout.open("table.txt", std::ios::app);//��׷�ӵķ�ʽ���ļ�
        //���������뵽�ļ���
        fout << t.table_name << " " << t.col_len << std::endl;
        for (int i = 0; i < t.col_name.size(); ++i)//size()�������������е�ǰԪ�صĸ���
        {
            fout << t.col_name[i];
            fout << " " << t.type_value[i];
            fout << ' ' << t.condition[i] << std::endl;
        }
        fout << std::endl;
        fout.close();


        //������һ����֮�󣬲�����table�ĵ��л���ʾ��ͬʱ������һ���Ա���Ϊ����txt�ļ���ר�Ŵ�����һ�����е�����
        char file[30];
        std::string tt = "table/" + t.table_name + ".txt";
        for (int i = 0; i < tt.length(); ++i)
            file[i] = tt[i];
        file[tt.length()] = '\0';
        //�����txt�ļ�
        fout.open(file);
        for (int i = 0; i < t.col_name.size() - 1; ++i)
            fout << t.col_name[i] << " ";
        fout << t.col_name[t.col_name.size() - 1] << std::endl;
        fout.close();


        std::cout << "����ɹ���" << std::endl;
    }
    return ;
}
