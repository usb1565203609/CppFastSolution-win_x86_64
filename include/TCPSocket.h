#ifndef TCPSocket_H
#define TCPSocket_H

//解决windows头文件与winsock2头文件包含顺序问题
#define WIN32_LEAN_AND_MEAN

#include"DefHead.h"
#include <Windows.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<cstring>
#include<iostream>

using namespace std;
#pragma comment(lib,"ws2_32.lib")

//发送函数，如果失败就返回详细信息,成功则为__SUCCESS
int TCPSend
(
	_In_ SOCKET nsocket,
	_In_ string buff,
	_In_ unsigned long buffsize
);

//接收函数，如果失败就返回详细信息,成功则为__SUCCESS
__declspec(dllexport) extern unsigned long TCPRecv
(
	_In_ SOCKET nsocket,
	_In_ unsigned char* buff,
	_In_ unsigned long buffsize
);

//发收函数，如果失败就返回详细信息,成功则为__SUCCESS
__declspec(dllexport) extern unsigned long TCPSendRecv
(
	_In_ SOCKET nsocket,
	_In_ string sbuff,
	_In_ unsigned long& sbuffsize,
	_Inout_ unsigned char* rbuff,
	_Inout_ unsigned long& rbuffsize
);

//收发函数,如果失败就返回详细信息,成功则为__SUCCESS
__declspec(dllexport) extern unsigned long TCPRecvSend
(
	_In_ SOCKET nsocket,
	_In_ string sbuff,
	_In_ unsigned long& sbuffsize,
	_Inout_ unsigned char* rbuff,
	_Inout_ unsigned long& rbuffsize
);

class __declspec(dllexport) TCPSocket
{
public:
	//function
	//构造函数初始化TCPSocket套接字
	explicit TCPSocket();
	explicit TCPSocket
	(
		_In_ int port,
		_In_ unsigned long ip,
		_Inout_ unsigned long& back
	);
	explicit TCPSocket
	(
		_In_ int port, 
		_In_ string ip,
		_Inout_ unsigned long& back
	);
	int SocketInit();
	~TCPSocket();

	//set方法
	void setPort(_In_ int Port);
	void setIp(_In_ unsigned long ip);
	void setIp(_In_ string ip);

	//static function
	//单例模式接口重载
	static TCPSocket* getInterface
	(
		_In_ int port,
		_In_ string ip,
		_Inout_ unsigned long& back
	);
	//单例模式访问接口
	static TCPSocket* getInterface
	(
		_In_ int port, 
		_In_ unsigned long ip, 
		_Inout_ unsigned long& back
	);
	//服务端监听绑定函数与服务端监听函数
	int TCPbind();
	//监听函数使用回调机制
	//静态函数或非成员回调函数才能使用此函数
	int TCPListen
	(
		_In_ void(*callback)(SOCKET, sockaddr_in),
		_In_ int ListenNum
	);
	//客户端发起连接套接字，使用回调机制
	int TCPConnect
	(
		_In_ void (*callback)(SOCKET,sockaddr_in),
		_In_ int& back
	);
	SOCKET* TCPConnect
	(
		_In_ int& back
	);
	//关闭套接字,如果失败返回原始WINAPI WSAGetLastError()函数错误值
	int Close();

private:
	//var
	static TCPSocket* __Inter;
	//端口与ip
	int port;
	string ip;
	unsigned long UIP;
	//套接字与addr结构体
	SOCKET MSocket;
	SOCKADDR_IN addr_in;
};
#endif	//TCPSocket_H