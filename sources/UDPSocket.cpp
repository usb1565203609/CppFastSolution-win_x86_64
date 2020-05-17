#include "UDPSocket.h"

UDPSocket* UDPSocket::__Inter = nullptr;

UDPSocket::UDPSocket()
{
	;
}


UDPSocket::UDPSocket
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
	this->MSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
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

UDPSocket::UDPSocket(int port, unsigned long ip, unsigned long & back)
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
	this->MSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
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

int UDPSocket::SocketInit()
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
	this->MSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

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

int UDPSocket::UDPRecvfrom
(
	_Inout_ char * recvBuff, 
	_In_ unsigned long recvSize
)
{
	int addrSize = sizeof(this->addr_in);
	int backSize = recvfrom(this->MSocket, recvBuff,
		recvSize, 0,
		(SOCKADDR*)&this->addr_in, &addrSize);

	if (backSize == SOCKET_ERROR)
		return SOCKET_ERROR;

	return backSize;
}

int UDPSocket::UDPSendto
(
	_In_ const char * sendBuff, 
	_In_ unsigned long sendSize
)
{
	int backLen=sendto(this->MSocket,
		sendBuff, sendSize, 0, 
		(SOCKADDR*)&this->addr_in, 
		sizeof(this->addr_in));

	if (backLen == SOCKET_ERROR)
		return SOCKET_ERROR;
	return backLen;
}

UDPSocket::~UDPSocket()
{
	;
}

void UDPSocket::setPort(_In_ int Port)
{
	this->port = Port;
}

void UDPSocket::setIp(_In_ string ip)
{
	this->ip = ip;
}

int UDPSocket::UDPbind()
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


int UDPSocket::Close()
{
	if (closesocket(this->MSocket) == SOCKET_ERROR)
	{
		return WSAGetLastError();
		WSACleanup();
	}
	WSACleanup();
	return __SUCCESS;
}

UDPSocket * UDPSocket::getInterface
(
	_In_ int port,
	_In_ string ip,
	_Inout_ unsigned long & back
)
{
	unsigned long backNum = 0;
	if (__Inter == nullptr)
	{
		__Inter = new UDPSocket(port, ip, backNum);
		if (backNum != __SUCCESS)
		{
			back = backNum;
			return NULL;
		}
	}
	back = __SUCCESS;
	return __Inter;
}

UDPSocket * UDPSocket::getInterface
(
	_In_ int port,
	_In_ unsigned long ip,
	_Inout_ unsigned long & back
)
{
	unsigned long backNum = 0;
	if (__Inter == nullptr)
	{
		__Inter = new UDPSocket(port, ip, backNum);
		if (backNum != __SUCCESS)
		{
			back = backNum;
			return NULL;
		}
	}
	back = __SUCCESS;
	return __Inter;
}



