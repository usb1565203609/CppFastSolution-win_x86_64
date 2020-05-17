#include "SMTPSocket.h"


SMTPSocket* SMTPSocket::__Inter = nullptr;

SMTPSocket::SMTPSocket()
{
	;
}


void SMTPSocket::setIpAddress(string ip)
{
	this->ipAddress = ip;
}

void SMTPSocket::setPort(int port)
{
	this->port = port;
}

void SMTPSocket::setHostName(string hostname)
{
	this->HostName = hostname;
}

void SMTPSocket::setUserPasswd
(
	_In_ string user,
	_In_ string passwd
)
{
	this->Base64Uname = Base64::Base64Encode(user, user.length());
	this->Base64Passwd = Base64::Base64Encode(passwd, passwd.length());

	this->username = user;
	this->Passwd = passwd;
}

void SMTPSocket::setFromandTo
(
	_In_ string from, 
	_In_ string sendto)
{
	this->from = from;
	this->sendTo = sendto;
	this->Tolist.push_back(sendto);
}

void SMTPSocket::setSubjectandMsg
(
	_In_ string subject,
	_In_ string Msg,
	_In_ unsigned MsgSize
)
{
	this->subject = subject;
	this->Msg = Base64::Base64Encode(Msg,Msg.length());
	this->MsgSize = this->Msg.length();
}

void SMTPSocket::addRcpto(string rcpto)
{
	this->Tolist.push_back(rcpto);
}

int SMTPSocket::SMTPInit()
{
	int back = __SUCCESS;
	this->S = new TCPSocket();
	this->S->setIp(this->ipAddress);
	this->S->setPort(this->port);
	this->S->SocketInit();

	this->MSocket = this->S->TCPConnect(back);
	if (this->MSocket == NULL)
		return back;

	unsigned char recvbuff[200];
	unsigned long rbuffsize = sizeof(recvbuff);
	memset(recvbuff, 0, 200);

	back = TCPRecv(*(this->MSocket),recvbuff, rbuffsize);
	if (back == SOCKET_ERROR)
		return back;

	Sleep(500);
	return __SUCCESS;
}

int SMTPSocket::Login()
{
	char temp[5];
	memset(temp, 0, 5);
	
	if ((this->Base64Passwd.empty()) || (this->Base64Uname.empty()))
		return __SMTP_USERPASSWD_ERROR;
	//HELO
	int back = __SUCCESS;
	string helo = "helo ";
	helo.append(this->HostName).append("\r\n");
	unsigned long sbuffsize = helo.length();
	unsigned char recvbuff[100];
	unsigned long rbuffsize = sizeof(recvbuff);
	memset(recvbuff, 0, 100);
	back=TCPSendRecv(*(this->MSocket), helo, sbuffsize, recvbuff, rbuffsize);
	if (back == SOCKET_ERROR)
		return back;

	memcpy(temp, recvbuff, 3);
	string backcode = temp;
	string s = "250";
	if (backcode != s)
		return __SMTP_HELO_ERROR;

	//auth login
	memset(recvbuff, 0, 100);
	string auth = "auth login\r\n";
	unsigned long ausize = auth.length();
	rbuffsize = sizeof(recvbuff);
	back = TCPSendRecv(*(this->MSocket), auth.c_str(), ausize, recvbuff, rbuffsize);
	if (back == SOCKET_ERROR)
		return back;
	if (strcmp((const char*)recvbuff, "334 dXNlcm5hbWU6\r\n") != 0)
		return __SMTP_AUTH_ERROR;
	
	//send base64username
	memset(recvbuff, 0, 100);
	auth.clear();
	auth = this->Base64Uname;
	auth.append("\r\n");
	ausize = auth.length();
	rbuffsize = sizeof(recvbuff);
	back = TCPSendRecv(*(this->MSocket), auth.c_str(), ausize, recvbuff, rbuffsize);
	if (back == SOCKET_ERROR)
		return back;

	//send base64passwd
	memset(recvbuff, 0, 100);
	auth.clear();
	auth = this->Base64Passwd;
	auth.append("\r\n");
	ausize = auth.length();
	rbuffsize = sizeof(recvbuff);
	back = TCPSendRecv(*(this->MSocket), auth.c_str(), ausize, recvbuff, rbuffsize);
	if (back == SOCKET_ERROR)
		return back;

	memcpy(temp, recvbuff, 3);
	backcode = temp;
	s = "235";
	if (backcode != s)
		return __SMTP_PASSWD_ERROR;

	return __SUCCESS;
}

int SMTPSocket::SendMail()
{
	char temp[5];
	memset(temp, 0, 5);

	int back = __SUCCESS;
	string mailFrom = "mail from:<";
	string mailTo = "rcpt to:<";
	mailFrom.append(this->from).append(">\r\n");
	mailTo.append(this->sendTo).append(">\r\n");

	unsigned long mailFromsize = mailFrom.length();
	unsigned long mailTosize = mailTo.length();

	unsigned char recvbuff[200];
	unsigned long rbuffsize = sizeof(recvbuff);

	//set from
	memset(recvbuff, 0, 200);
	back = TCPSendRecv(*(this->MSocket), mailFrom,
		mailFromsize, recvbuff, rbuffsize);
	if (back == SOCKET_ERROR)
		return back;

	memcpy(temp, recvbuff, 3);
	string backcode = temp;
	string s = "250";
	if (backcode != s)
		return __SMTP_FROM_ERROR;

	//set to
	memset(recvbuff, 0, 200);
	rbuffsize = sizeof(recvbuff);

	back = TCPSendRecv(*(this->MSocket), mailTo,
		mailTosize, recvbuff, rbuffsize);
	if (back == SOCKET_ERROR)
		return back;
	
	memset(temp, 0, 5);
	memcpy(temp, recvbuff, 3);
    backcode = temp;
	s = "250";
	if (backcode != s)
		return __SMTP_RECPTO_ERROR;
	//send data
	memset(recvbuff, 0, 200);
	rbuffsize = sizeof(recvbuff);

	string sd = "data\r\n";
	unsigned long sds = sd.length();

	back = TCPSendRecv(*(this->MSocket), sd, sds, recvbuff, rbuffsize);

	memset(temp, 0, 5);
	memcpy(temp, recvbuff, 3);
	backcode = temp;
	s = "354";
	if (backcode != s)
		return __SMTP_DATAFLAG_ERROR;
	//start send!
	memset(recvbuff, 0, 200);
	rbuffsize = sizeof(recvbuff);
	
	string Mime =
		"MIME-Version: 1.0\r\n"
		"Content - Type: text / plain; charset = UTF - 8\r\n"
		"Content-Transfer-Encoding: base64\r\n"
		"X - Mailer : Microsoft Outlook Express 6.00.2900.2869\r\n\r\n";

	string sendData;
	sendData.append("From: ").append(this->from).append("\r\n");
	sendData.append("To: ").append(this->sendTo).append("\r\n");
	sendData.append("Subject: ").append(this->subject).append("\r\n");
	sendData.append(Mime);

	sendData.append(this->Msg).append("\r\n.\r\n");
	unsigned long sendSize = sendData.length();

	back = TCPSendRecv(*(this->MSocket), sendData,
		sendSize, recvbuff, rbuffsize);
	if (back == SOCKET_ERROR)
		return back;
	memset(temp, 0, 5);
	memcpy(temp, recvbuff, 3);

	backcode = temp;
	s = "250";
	if (backcode != s)
		return __SMTP_EMAILSEND_ERROR;

	return __SUCCESS;
}

int SMTPSocket::SendMailAll()
{
	char temp[5];
	int back = __SUCCESS;
	string mailFrom = "mail from:<";
	
	mailFrom.append(this->from).append(">\r\n");

	unsigned long mailFromsize = mailFrom.length();

	unsigned char recvbuff[200];
	unsigned long rbuffsize = sizeof(recvbuff);

	//set from
	memset(recvbuff, 0, 200);
	back = TCPSendRecv(*(this->MSocket), mailFrom,
		mailFromsize, recvbuff, rbuffsize);
	if (back == SOCKET_ERROR)
		return back;

	memset(temp, 0, 5);
	memcpy(temp, recvbuff, 3);
	string backcode = temp;
	string s = "250";
	if (backcode != s)
		return __SMTP_FROM_ERROR;

	//set to
	for (int i = 0; i < this->Tolist.size(); i++)
	{
		memset(recvbuff, 0, 200);
		rbuffsize = sizeof(recvbuff);

		string mailTo = "rcpt to:<";
		mailTo.append(this->Tolist.at(i)).append(">\r\n");
		unsigned long mailTosize = mailTo.length();

		back = TCPSendRecv(*(this->MSocket), mailTo,
			mailTosize, recvbuff, rbuffsize);
		if (back == SOCKET_ERROR)
			return back;

		memset(temp, 0, 5);
		memcpy(temp, recvbuff, 3);
		backcode = temp;
		s = "250";
		if (backcode != s)
			return __SMTP_RECPTO_ERROR;
	}
	//send data
	memset(recvbuff, 0, 200);
	rbuffsize = sizeof(recvbuff);

	string sd = "data\r\n";
	unsigned long sds = sd.length();

	back = TCPSendRecv(*(this->MSocket), sd, sds, recvbuff, rbuffsize);

	memset(temp, 0, 5);
	memcpy(temp, recvbuff, 3);
	backcode = temp;
	s = "354";
	if (backcode != s)
		return __SMTP_DATAFLAG_ERROR;
	//start send!
	memset(recvbuff, 0, 200);
	rbuffsize = sizeof(recvbuff);

	string Mime =
		"MIME-Version: 1.0\r\n"
		"Content - Type: text / plain; charset = UTF - 8\r\n"
		"Content-Transfer-Encoding: base64\r\n"
		"X - Mailer : Microsoft Outlook Express 6.00.2900.2869\r\n\r\n";

	string sendData;
	sendData.append("From: ").append(this->from).append("\r\n");
	sendData.append("Subject: ").append(this->subject).append("\r\n");
	sendData.append(Mime);

	sendData.append(this->Msg).append("\r\n.\r\n");
	unsigned long sendSize = sendData.length();

	back = TCPSendRecv(*(this->MSocket), sendData,
		sendSize, recvbuff, rbuffsize);
	if (back == SOCKET_ERROR)
		return back;
	memset(temp, 0, 5);
	memcpy(temp, recvbuff, 3);

	backcode = temp;
	s = "250";
	if (backcode != s)
		return __SMTP_EMAILSEND_ERROR;

	return __SUCCESS;
}

int SMTPSocket::SMTPQuit()
{
	int back = 0;
	back = TCPSend(*(this->MSocket), "quit\r\n", sizeof("quit\r\n"));
	if (back == SOCKET_ERROR)
	{
		this->S->Close();
		return __SMTP_QUIT_ERROR;
	}
	this->S->Close();
	return 0;
}

SMTPSocket * SMTPSocket::getInterface()
{
	if (__Inter == nullptr)
		__Inter = new SMTPSocket();

	return __Inter;
}

SMTPSocket::~SMTPSocket()
{
	delete this->S;
}
