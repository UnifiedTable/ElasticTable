#pragma once
#include <list>
#include <map>
#include <string>
#include <fstream>
//#include <corecrt_io.h>
using namespace std;

//配置文件读取工具

int ConfigFileInit(map<string, string>& m_mapConfigInfo, string path);//初始化配置文件，map存放所有信息，path为路径
string getValueString(map<string, string> m_mapConfigInfo, string key);//获取字符串值
int getValueInt(map<string, string> m_mapConfigInfo, string key);//获取整型值
int write_conf(string path, string key, string value);//写入配置
int delete_conf(string path, string key);//删除配置
int change_conf(string path, string key, string value);//修改配置