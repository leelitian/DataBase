#include<iostream>
#include<string>
#define MSGSIZE			1024  
using namespace std;

char* deal(char* sql)
{
	char sql_sql[MSGSIZE];
	string tmp=sql;
	tmp="�յ��ַ�����"+tmp; 
	tmp.copy(sql_sql,tmp.length(),0);
	sql_sql[tmp.length()]='\0';
	return sql_sql;
}
