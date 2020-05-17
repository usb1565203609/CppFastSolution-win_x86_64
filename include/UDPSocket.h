#pragma once
#ifndef UDPSOCKET_H
#define UDPSOCKET_H
//���windowsͷ�ļ���winsock2ͷ�ļ�����˳������
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
	//���캯����ʼ��UDPSocket�׽���
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

	//set����
	void setPort(_In_ int Port);
	void setIp(_In_ string ip);

	//static function
	//����ģʽ�ӿ�����
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

	
	//����˼����󶨺��������˼�������
	int UDPbind();
	
	//�ر��׽���,���ʧ�ܷ���ԭʼWINAPI WSAGetLastError()��������ֵ
	int Close();

private:
	//var
	static UDPSocket* __Inter;
	//�˿���ip
	int port;
	string ip;
	unsigned long UIP;
	//�׽�����addr�ṹ��
	SOCKET MSocket;
	SOCKADDR_IN addr_in;
};
#endif	//UDPSocket_H
