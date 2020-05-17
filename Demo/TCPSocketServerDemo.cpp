#include"TCPSocket.h"
//server��
//һ�����ڲ��Եĳ���ʵ�ָ�ect1.exe�����һ���½���
//��������½�����Ŀ�ִ�г��򱣴�Ϊt.exe�������Լ���ʵ��������ò���

void callbackListen(SOCKET S, sockaddr_in addrin)
{
	//�ڴ˻ص������ڴ������������Լ��շ�����
	char buff[100];
	unsigned long buffsize = sizeof(buff);
	unsigned long i=TCPRecv(S, buff, buffsize);

	memset(buff , 0 , 100);
	//�жϽ��������Ƿ����
	if (i == -1)
	{
		cout << "recv error "<<WSAGetLastError();
		return;
	}
	else
		cout << buff;

	//���÷��պ���
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
	//������ϸ����ֵ����
	unsigned long y = 0;
	//��������󣬴���Ϊ��������
	TCPSocket* s = TCPSocket::getInterface(1080, "127.0.0.1", y);
	//������
	if (s == NULL)
	{
		cout << "socket init error " << y;
		return 0;
	}
	//��
	y=s->TCPbind();
	if (y == __ERROR)
	{
		cout << "bind error ";
		s->Close();
		return 0;
	}
	//��������Ҫ����һ�������Ա�����Ļص������������Ǿ�̬����
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