#include"UDPSocket.h"

int main()
{
	unsigned long back = 0;
	char buff[100];

	memset(buff , 0 , 100);
	UDPSocket* us = UDPSocket::getInterface(1080, "127.0.0.1", back);
	if (back != __SUCCESS)
	{
		cout << "init error " << back;
		return 0;
	}
	back=us->UDPbind();
	if (back == __ERROR)
	{
		cout << "bind error ";
		us->Close();
		return 0;
	}
	back = us->UDPRecvfrom(buff, 100);
	if (back == -1)
	{
		cout << "recv error " << back;
		us->Close();
		return 0;
	}
	cout << buff;
	return 0;
}