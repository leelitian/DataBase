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
class table
{
public:
    std::string table_name;//����
    std::vector<std::string> col_name;//�ֶ���
    std::vector<std::string> type_value;//�ֶ���������
    std::vector<int> condition;//0��ʾ��Լ������
    int col_len;//���Ը���
    int remcount = 0;//Ԫ�����
    //bool check_exist(std::string table_name);
    void create_table(std::string op, int& flag, std::vector<table>& tab);
};

#endif