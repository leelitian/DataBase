#ifndef __TABLE__
#define __TABLE__
#include <iostream>
#include <cstring>
#include <string.h>
#include <fstream>
#include <vector>
#include <cstdio>
#include <windows.h>
#include <cstring>
#include <sstream>
using namespace std;

class table
{
public:
    std::string table_name;                 //����
    std::vector<std::string> col_name;      //�ֶ���
    std::vector<std::string> type_value;    //�ֶ���������
    std::vector<int> condition;             //0��ʾ��Լ������
    bool check_exist(std::string table_name);
    int col_len = 0;//���Ը���
    int remcount = 0;//Ԫ�����
    //bool check_exist(std::string table_name);
    string create_table(std::string op, std::vector<table>& tab);
};

#endif