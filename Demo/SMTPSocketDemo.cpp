#include"SMTPSocket.h"

int main()
{
	SMTPSocket s;
	int back = __SUCCESS;
	//要发送的邮件内容
	string sendMsgdata = "“TCPSocket.exe”(Win32): "
		"\n已加载“C:\Windows\SysWOW64\kernel.appcore.dll”。"
		"\n无法查找或打开 PDB 文件。"
		"“TCPSocket.exe”(Win32): 已加载“G:\vs\Project2\Release\TCPSocket.exe”。已加载符号。"
		"“TCPSocket.exe”(Win32) : 已加载“C:\Windows\SysWOW64\ntdll.dll”。无法查找或打开 PDB 文件。"
		"“TCPSocket.exe”(Win32) : 已加载“C:\Windows\SysWOW64\kernel32.dll”。无法查找或打开 PDB 文件。"
		"“TCPSocket.exe”(Win32) : 已加载“C:\Windows\SysWOW64\KernelBase.dll”。无法查找或打开 PDB 文件。"
		"“TCPSocket.exe”(Win32) : 已加载“C:\Windows\SysWOW64\ws2_32.dll”。无法查找或打开 PDB 文件。"
		"“TCPSocket.exe”(Win32) : 已加载“C:\Windows\SysWOW64\rpcrt4.dll”。无法查找或打开 PDB 文件。"
		"“TCPSocket.exe”(Win32) : 已加载“C:\Windows\SysWOW64\sspicli.dll”。无法查找或打开 PDB 文件。"
		"“TCPSocket.exe”(Win32) : 已加载“C:\Windows\SysWOW64\cryptbase.dll”。无法查找或打开 PDB 文件。"
		"“TCPSocket.exe”(Win32) : 已加载“C:\Windows\SysWOW64\bcryptprimitives.dll”。无法查找或打开 PDB 文件。"
		"“TCPSocket.exe”(Win32) : 已加载“C:\Windows\SysWOW64\sechost.dll”。无法查找或打开 PDB 文件。"
		"“TCPSocket.exe”(Win32) : 已加载“C:\Windows\SysWOW64\mswsock.dll”。无法查找或打开 PDB 文件。"
		"线程 0x1aa4 已退出，返回值为 0 (0x0)。";

	//摄制SMTP服务器IP与端口
	s.setIpAddress("220.181.12.11");
	s.setPort(25);

	//初始化
	if ((back=s.SMTPInit()) != __SUCCESS)
	{
		cout <<"INIT ERROR"<< back;
		return 0;
	}
	//设置登陆账号与授权码
	s.setUserPasswd("cppfastsolution@163.com", "WBKZTXWMSKCJOFJM");
	//设置发送地址与目的地址，可设置单一联系人，也可调用多次添加多联系人进行群发
	s.setFromandTo("cppfastsolution@163.com", "1565203609@qq.com");
	//添加多个接收人，调用多次即可
	s.addRcpto("2737089450@qq.com");
	//设置邮件主题与内容
	s.setSubjectandMsg("一个测试邮件",sendMsgdata,sendMsgdata.length());
	//登录
	back=s.Login();
	if (back != __SUCCESS)
	{
		cout << "LOGIN ERROR"<<back;
		s.SMTPQuit();
		return 0;
	}
	//发送邮件，此函数只可群发邮件
	back = s.SendMailAll();
	//此函数只可单发邮件
	//back = s.SendMail();
	if (back != __SUCCESS)
	{
		cout << "SEND ERROR" << back;
		s.SMTPQuit();
		return 0;
	}
	s.SMTPQuit();
}