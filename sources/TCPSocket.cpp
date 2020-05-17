#include "TCPSocket.h"
TCPSocket* TCPSocket::__Inter = nullptr;

TCPSocket::TCPSocket()
{
	;
}

TCPSocket::TCPSocket
(
	_In_ int port, 
	_In_ unsigned long ip,
	_Inout_ unsigned long & back
)
{
	this->port = port;
	this->UIP = ip;

	back = __SUCCESS;
	//初始化WSA
	unsigned short Version = MAKEWORD(2, 2);
	WSADATA WsaData;
	if (WSAStartup(Version, &WsaData) != 0)
	{
		back = __SOCK_WSAINIT_ERROR;
		return;
	}
	//初始化套接字
	this->MSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->MSocket==INVALID_SOCKET)
	{
		back=__SOCK_INIT_ERROR;
		return;
	}
	if ((this->port <= 0) || (this->port >= 65535))
	{
		back=__SOCK_PORT_ERROR;
		return;
	}
	
	if (this->UIP != INADDR_ANY)
	{
		back = __SOCK_IP_ERROR;
		return;
	}
	//初始化结构体
	this->addr_in.sin_family = AF_INET;
	this->addr_in.sin_port = htons(this->port);
	this->addr_in.sin_addr.S_un.S_addr = this->UIP;
}


TCPSocket::TCPSocket
(
	_In_ int port, 
	_In_ string ip,
	_Inout_ unsigned long & back
)
{
	this->port = port;
	this->ip = ip;

	back = __SUCCESS;
	//初始化WSA
	unsigned short Version = MAKEWORD(2, 2);
	WSADATA WsaData;
	if (WSAStartup(Version, &WsaData) != 0)
	{
		back = __SOCK_WSAINIT_ERROR;
		return;
	}
	//初始化套接字
	this->MSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->MSocket == INVALID_SOCKET)
	{
		back = __SOCK_INIT_ERROR;
		return;
	}
	if ((this->port <= 0) || (this->port >= 65535))
	{
		back = __SOCK_PORT_ERROR;
		return;
	}

	//初始化结构体
	this->addr_in.sin_family = AF_INET;
	this->addr_in.sin_port = htons(this->port);
	inet_pton(
		AF_INET, 
		this->ip.c_str(), 
		&this->addr_in.sin_addr.S_un.S_addr);
}

int TCPSocket::SocketInit()
{
	//如果未初始化ip
	if (this->ip.empty())
		return __SOCK_IP_ISNULL;
	//初始化WSA
	unsigned short Version = MAKEWORD(2, 2);
	WSADATA WsaData;
	if (WSAStartup(Version, &WsaData) != 0)
		return __SOCK_WSAINIT_ERROR;
	//初始化套接字
	this->MSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (this->MSocket == INVALID_SOCKET)
		return __SOCK_INIT_ERROR;

	if ((this->port <= 0) || (this->port >= 65535))
		return __SOCK_PORT_ERROR;

	//初始化结构体
	this->addr_in.sin_family = AF_INET;
	this->addr_in.sin_port = htons(this->port);
	inet_pton(
		AF_INET,
		this->ip.c_str(),
		&this->addr_in.sin_addr.S_un.S_addr);

	return __SUCCESS;
}

TCPSocket::~TCPSocket()
{
	;
}

void TCPSocket::setPort(_In_ int Port)
{
	this->port = Port;
}

void TCPSocket::setIp(_In_ unsigned long ip)
{
	this->UIP = ip;
}

void TCPSocket::setIp(_In_ string ip)
{
	this->ip = ip;
}

int TCPSocket::TCPbind()
{
	if (bind(
		this->MSocket,
		(LPSOCKADDR)&this->addr_in,
		sizeof(this->addr_in)) == SOCKET_ERROR)
	{
		return __ERROR;
	}
	return __SUCCESS;
}

int TCPSocket::TCPListen
(
	_In_ void(*callback)(SOCKET, sockaddr_in),
	_In_ int ListenNum
)
{
	if (ListenNum > 128)
		return __SOCKET_LISTENNUM_TOOBIG;
	if (listen(this->MSocket, ListenNum) == SOCKET_ERROR)
		return __SOCK_LISTEN_ERROR;
	while (true)
	{
		sockaddr_in addr;
		int addrsize = sizeof(addr);
		SOCKET s = accept(this->MSocket, (SOCKADDR*)&addr, &addrsize);
		if (s == INVALID_SOCKET)
			return __SOCK_ACCEPT_ERROR;
		callback(s, addr);
		closesocket(s);
	}
	return __SUCCESS;
}

int TCPSocket::TCPConnect
(
	_In_ void(*callback)(SOCKET, sockaddr_in),
	_In_ int& back
)
{
	if (connect(this->MSocket,
		(sockaddr*)&this->addr_in,
		sizeof(this->addr_in)) == SOCKET_ERROR)
	{
		back = WSAGetLastError();
		return __SOCK_CONNECT_ERROR;
	}
	callback(this->MSocket,this->addr_in);
	return __SUCCESS;
}

SOCKET * TCPSocket::TCPConnect(int & back)
{
	if (connect(this->MSocket,
		(sockaddr*)&this->addr_in,
		sizeof(this->addr_in)) == SOCKET_ERROR)
	{
		back = WSAGetLastError();
		return NULL;
	}
	return  (&this->MSocket);
}

int TCPSocket::Close()
{
	if (closesocket(this->MSocket) == SOCKET_ERROR)
	{
		return WSAGetLastError();
		WSACleanup();
	}
	WSACleanup();
	return __SUCCESS;
}

TCPSocket * TCPSocket::getInterface
(
	_In_ int port, 
	_In_ string ip, 
	_Inout_ unsigned long & back
)
{
	unsigned long backNum = 0;
	if (__Inter == nullptr)
	{
		__Inter = new TCPSocket(port, ip, backNum);
		if (backNum != __SUCCESS)
		{
			back = backNum;
			return NULL;
		}
	}
	back = __SUCCESS;
	return __Inter;
}

TCPSocket * TCPSocket::getInterface
(
	_In_ int port, 
	_In_ unsigned long ip, 
	_Inout_ unsigned long & back
)
{
	unsigned long backNum = 0;
	if (__Inter == nullptr)
	{
		__Inter = new TCPSocket(port, ip, backNum);
		if (backNum != __SUCCESS)
		{
			back = backNum;
			return NULL;
		}
	}
	back = __SUCCESS;
	return __Inter;
}

int TCPSend
(
	_In_ SOCKET nsocket,
	_In_ string buff,
	_In_ unsigned long buffsize
)
{
	unsigned long sendsize = send(nsocket, buff.c_str(), buffsize, 0);
	return sendsize;
}

unsigned long TCPRecv
(
	_In_ SOCKET nsocket,
	_In_ unsigned char* buff,
	_In_ unsigned long buffsize
)
{
	unsigned long rsize = recv(nsocket, (char*)buff, buffsize, 0);
	return rsize;
}

unsigned long TCPSendRecv
(
	_In_ SOCKET nsocket,
	_In_ string sbuff,
	_In_ unsigned long& sbuffsize,
	_Inout_ unsigned char* rbuff,
	_Inout_ unsigned long& rbuffsize
)
{
	unsigned long sendSize = send(nsocket, sbuff.c_str(), sbuffsize, 0);
	if (sendSize == SOCKET_ERROR)
	{
		sbuffsize = 0;
		return WSAGetLastError();
	}
	unsigned long i = recv(nsocket, (char*)rbuff, rbuffsize, 0);
	if (i == SOCKET_ERROR)
	{
		rbuffsize = 0;
		return WSAGetLastError();
	}
	rbuffsize = i;
	return __SUCCESS;
}

unsigned long TCPRecvSend
(
	_In_ SOCKET nsocket,
	_In_ string sbuff,
	_In_ unsigned long& sbuffsize,
	_Inout_ unsigned char* rbuff,
	_Inout_ unsigned long& rbuffsize
)
{
	unsigned long tempSize = recv(nsocket, (char*)rbuff, rbuffsize, 0);
	if (tempSize == SOCKET_ERROR)
	{
		rbuffsize = 0;
		return WSAGetLastError();
	}
	if (tempSize == 0)
		return __SOCKET_NO_RECV;
	rbuffsize = tempSize;
	
	unsigned long sendSize = send(nsocket, sbuff.c_str(), sbuffsize, 0);
	if (sendSize == SOCKET_ERROR)
	{
		sbuffsize = 0;
		return WSAGetLastError();
	}

	sbuffsize = sendSize;
	return __SUCCESS;
}
