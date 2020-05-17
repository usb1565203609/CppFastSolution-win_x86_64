#ifndef SMTPSOCKET_H
#define SMTPSOCKET_H

#include"Base64.h"
#include"TCPSocket.h"
#include"DefHead.h"
#include<vector>

class __declspec(dllexport)SMTPSocket
{
public:
	explicit SMTPSocket();
	//设置私有属性的set方法
	void setIpAddress(string ip);
	void setPort(int port);
	void setHostName(string hostname);
	//用户名与密码设置
	void setUserPasswd
	(
		_In_ string user,
		_In_ string passwd
	);
	void setFromandTo
	(
		_In_ string from,
		_In_ string sendto
	);
	void setSubjectandMsg
	(
		_In_ string subject,
		_In_ string Msg,
		_In_ unsigned MsgSize
	);
	void addRcpto(string rcpto);
	//初始化与登录函数
	int SMTPInit();
	int Login();
	//邮件发送
	int SendMail();
	int SendMailAll();
	//连接关闭函数
	int SMTPQuit();

	static SMTPSocket* __Inter;
	static SMTPSocket* getInterface();
	~SMTPSocket();

private:
	string ipAddress;
	int port;

	string Base64Uname;
	string username;
	string Base64Passwd;
	string Passwd;
	string from;
	string sendTo;
	string subject;
	string Msg;
	string HostName="Administrator";
	
	unsigned long MsgSize = 0;
	vector<string> Tolist;
	SOCKET* MSocket;
	TCPSocket* S;
};
#endif

