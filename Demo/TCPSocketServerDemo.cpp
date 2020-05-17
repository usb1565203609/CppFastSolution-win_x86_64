#include"TCPSocket.h"
//server端
//一个用于测试的程序，实现给ect1.exe程序加一个新节区
//并将添加新节区后的可执行程序保存为t.exe，根据自己的实际情况设置参数

void callbackListen(SOCKET S, sockaddr_in addrin)
{
	//在此回调函数内处理连接请求以及收发数据
	char buff[100];
	unsigned long buffsize = sizeof(buff);
	unsigned long i=TCPRecv(S, buff, buffsize);

	memset(buff , 0 , 100);
	//判断接收数据是否出错
	if (i == -1)
	{
		cout << "recv error "<<WSAGetLastError();
		return;
	}
	else
		cout << buff;

	//调用发收函数
	string sendstr = "\nthis is server\n";
	char recvstr[100];

	memset(recvstr , 0 , 100);
	unsigned long ssize = sendstr.length();
	unsigned long rsize = 100;
	i = TCPSendRecv(S, sendstr.c_str(), ssize, recvstr, rsize);
	if (i == -1)
	{
		cout << "sr error " << WSAGetLastError();
		return;
	}
	else
	{
		cout << recvstr;
	}
}

int main
(
	_In_ int argv,
	_In_ char*argc[]
)
{
	//定义详细错误值变量
	unsigned long y = 0;
	//创建类对象，此例为单例访问
	TCPSocket* s = TCPSocket::getInterface(1080, "127.0.0.1", y);
	//出错处理
	if (s == NULL)
	{
		cout << "socket init error " << y;
		return 0;
	}
	//绑定
	y=s->TCPbind();
	if (y == __ERROR)
	{
		cout << "bind error ";
		s->Close();
		return 0;
	}
	//监听，需要传入一个非类成员函数的回调函数，可以是静态函数
	y = s->TCPListen(callbackListen, 5);
	if (y != __SUCCESS)
	{
		cout << "listen error " << y;
		s->Close();
		return 0;
	}
	y=s->Close();
	if (y != __SUCCESS)
	{
		cout << "close error " << y;
		return 0;
	}
	return 0;
}