#include"TCPSocket.h"
//client端
//一个用于测试的程序，根据自己的实际情况设置参数
//连接成功后的回调函数，可以是一个类的静态函数，但不能是成员函数
//第一个参数是连接套接字，第二个参数是其他详细信息
void io(SOCKET so, sockaddr_in ain)
{
	//回调函数，此处与服务端进行通信
	unsigned long i = TCPSend(so, "connect success!\n", sizeof("connect success!\n"));
	if (i == -1)
	{
		cout << "send error " << WSAGetLastError();
		return;
	}

	//调用收发函数
	string sendstr = "\nclient recv!\n";
	char rbuff[100];
	
	memset(rbuff , 0 , 100);
	unsigned long ssize = sendstr.length();
	unsigned long rsize = sizeof(rbuff);
	i = TCPRecvSend(so, sendstr.c_str(), ssize, rbuff, rsize);
	if (i != __SUCCESS)
	{
		cout << " rs error " << i;
		return;
	}
	cout << rbuff;
}

int main
(
	_In_ int argv,
	_In_ char*argc[]
)
{
	//定义接受详细错误信息值变量
	unsigned long backNum = 0;
	//单例模式创建类对象
	TCPSocket* s=TCPSocket::getInterface(1080, "127.0.0.1", backNum);
	//出错处理
	if (s == NULL)
	{
		cout << "init error " << backNum;
		return 0;
	}
	//主动建立连接并获取错误值
	int y = 0;
	//创建连接,主动发起连接
	int i=s->TCPConnect(io,y);
	if (i != __SUCCESS)
	{
		cout << "connect error " << i<<" "<<y;
		 s->Close();
		return 0;
	}
	//关闭连接
	i = s->Close();
	if (i != __SUCCESS)
	{
		cout << "close error" << i;
		return 0;
	}
	return 0;
}