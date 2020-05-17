#include"UDPSocket.h"

int main()
{
	unsigned long back = 0;
	int buffsize = 0;
	UDPSocket* us = UDPSocket::getInterface(1080, "127.0.0.1", back);
	if (back != __SUCCESS)
	{
		cout << "init error " << back;
		return 0;
	}
	buffsize=us->UDPSendto("client send1", sizeof("client send1"));
	if (back == -1)
	{
		cout << "send error " << back;
		us->Close();
		return 0;
	}
	us->Close();
	return 0;
}