#pragma once
#ifndef UDPSOCKET_H
#define UDPSOCKET_H
//解决windows头文件与winsock2头文件包含顺序问题
#define WIN32_LEAN_AND_MEAN


#include<iostream>
#include"DefHead.h"
#include<Windows.h>
#include<WS2tcpip.h>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

using namespace std;


class __declspec(dllexport) UDPSocket
{
public:
	//function
	//构造函数初始化UDPSocket套接字
	explicit UDPSocket();
	explicit UDPSocket
	(
		_In_ int port,
		_In_ string ip,
		_Inout_ unsigned long& back
	);

	explicit UDPSocket
	(
		_In_ int port,
		_In_ unsigned long ip,
		_Inout_ unsigned long& back
	);
	
	int SocketInit();

	int UDPRecvfrom
	(
		_Inout_ char* recvBuff,
		_Inout_ unsigned long recvSize
	);

	int UDPSendto
	(
		_In_ const char* sendBuff,
		_In_ unsigned long sendSize
	);

	~UDPSocket();

	//set方法
	void setPort(_In_ int Port);
	void setIp(_In_ string ip);

	//static function
	//单例模式接口重载
	static UDPSocket* getInterface
	(
		_In_ int port,
		_In_ string ip,
		_Inout_ unsigned long& back
	);
	static UDPSocket* getInterface
	(
		_In_ int port,
		_In_ unsigned long ip,
		_Inout_ unsigned long & back
	);

	
	//服务端监听绑定函数与服务端监听函数
	int UDPbind();
	
	//关闭套接字,如果失败返回原始WINAPI WSAGetLastError()函数错误值
	int Close();

private:
	//var
	static UDPSocket* __Inter;
	//端口与ip
	int port;
	string ip;
	unsigned long UIP;
	//套接字与addr结构体
	SOCKET MSocket;
	SOCKADDR_IN addr_in;
};
#endif	//UDPSocket_H
