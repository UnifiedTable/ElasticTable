#pragma once
#include <list>
#include <map>
#include <string>
#include <fstream>
//#include <corecrt_io.h>
using namespace std;

//�����ļ���ȡ����

int ConfigFileInit(map<string, string>& m_mapConfigInfo, string path);//��ʼ�������ļ���map���������Ϣ��pathΪ·��
string getValueString(map<string, string> m_mapConfigInfo, string key);//��ȡ�ַ���ֵ
int getValueInt(map<string, string> m_mapConfigInfo, string key);//��ȡ����ֵ
int write_conf(string path, string key, string value);//д������
int delete_conf(string path, string key);//ɾ������
int change_conf(string path, string key, string value);//�޸�����