#ifndef TCPSocket_H
#define TCPSocket_H

//���windowsͷ�ļ���winsock2ͷ�ļ�����˳������
#define WIN32_LEAN_AND_MEAN

#include"DefHead.h"
#include <Windows.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<cstring>
#include<iostream>

using namespace std;
#pragma comment(lib,"ws2_32.lib")

//���ͺ��������ʧ�ܾͷ�����ϸ��Ϣ,�ɹ���Ϊ__SUCCESS
int TCPSend
(
	_In_ SOCKET nsocket,
	_In_ string buff,
	_In_ unsigned long buffsize
);

//���պ��������ʧ�ܾͷ�����ϸ��Ϣ,�ɹ���Ϊ__SUCCESS
__declspec(dllexport) extern unsigned long TCPRecv
(
	_In_ SOCKET nsocket,
	_In_ unsigned char* buff,
	_In_ unsigned long buffsize
);

//���պ��������ʧ�ܾͷ�����ϸ��Ϣ,�ɹ���Ϊ__SUCCESS
__declspec(dllexport) extern unsigned long TCPSendRecv
(
	_In_ SOCKET nsocket,
	_In_ string sbuff,
	_In_ unsigned long& sbuffsize,
	_Inout_ unsigned char* rbuff,
	_Inout_ unsigned long& rbuffsize
);

//�շ�����,���ʧ�ܾͷ�����ϸ��Ϣ,�ɹ���Ϊ__SUCCESS
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
	//���캯����ʼ��TCPSocket�׽���
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

	//set����
	void setPort(_In_ int Port);
	void setIp(_In_ unsigned long ip);
	void setIp(_In_ string ip);

	//static function
	//����ģʽ�ӿ�����
	static TCPSocket* getInterface
	(
		_In_ int port,
		_In_ string ip,
		_Inout_ unsigned long& back
	);
	//����ģʽ���ʽӿ�
	static TCPSocket* getInterface
	(
		_In_ int port, 
		_In_ unsigned long ip, 
		_Inout_ unsigned long& back
	);
	//����˼����󶨺��������˼�������
	int TCPbind();
	//��������ʹ�ûص�����
	//��̬������ǳ�Ա�ص���������ʹ�ô˺���
	int TCPListen
	(
		_In_ void(*callback)(SOCKET, sockaddr_in),
		_In_ int ListenNum
	);
	//�ͻ��˷��������׽��֣�ʹ�ûص�����
	int TCPConnect
	(
		_In_ void (*callback)(SOCKET,sockaddr_in),
		_In_ int& back
	);
	SOCKET* TCPConnect
	(
		_In_ int& back
	);
	//�ر��׽���,���ʧ�ܷ���ԭʼWINAPI WSAGetLastError()��������ֵ
	int Close();

private:
	//var
	static TCPSocket* __Inter;
	//�˿���ip
	int port;
	string ip;
	unsigned long UIP;
	//�׽�����addr�ṹ��
	SOCKET MSocket;
	SOCKADDR_IN addr_in;
};
#endif	//TCPSocket_H