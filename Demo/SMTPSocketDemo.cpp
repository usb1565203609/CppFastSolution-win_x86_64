#include"SMTPSocket.h"

int main()
{
	SMTPSocket s;
	int back = __SUCCESS;
	//Ҫ���͵��ʼ�����
	string sendMsgdata = "��TCPSocket.exe��(Win32): "
		"\n�Ѽ��ء�C:\Windows\SysWOW64\kernel.appcore.dll����"
		"\n�޷����һ�� PDB �ļ���"
		"��TCPSocket.exe��(Win32): �Ѽ��ء�G:\vs\Project2\Release\TCPSocket.exe�����Ѽ��ط��š�"
		"��TCPSocket.exe��(Win32) : �Ѽ��ء�C:\Windows\SysWOW64\ntdll.dll�����޷����һ�� PDB �ļ���"
		"��TCPSocket.exe��(Win32) : �Ѽ��ء�C:\Windows\SysWOW64\kernel32.dll�����޷����һ�� PDB �ļ���"
		"��TCPSocket.exe��(Win32) : �Ѽ��ء�C:\Windows\SysWOW64\KernelBase.dll�����޷����һ�� PDB �ļ���"
		"��TCPSocket.exe��(Win32) : �Ѽ��ء�C:\Windows\SysWOW64\ws2_32.dll�����޷����һ�� PDB �ļ���"
		"��TCPSocket.exe��(Win32) : �Ѽ��ء�C:\Windows\SysWOW64\rpcrt4.dll�����޷����һ�� PDB �ļ���"
		"��TCPSocket.exe��(Win32) : �Ѽ��ء�C:\Windows\SysWOW64\sspicli.dll�����޷����һ�� PDB �ļ���"
		"��TCPSocket.exe��(Win32) : �Ѽ��ء�C:\Windows\SysWOW64\cryptbase.dll�����޷����һ�� PDB �ļ���"
		"��TCPSocket.exe��(Win32) : �Ѽ��ء�C:\Windows\SysWOW64\bcryptprimitives.dll�����޷����һ�� PDB �ļ���"
		"��TCPSocket.exe��(Win32) : �Ѽ��ء�C:\Windows\SysWOW64\sechost.dll�����޷����һ�� PDB �ļ���"
		"��TCPSocket.exe��(Win32) : �Ѽ��ء�C:\Windows\SysWOW64\mswsock.dll�����޷����һ�� PDB �ļ���"
		"�߳� 0x1aa4 ���˳�������ֵΪ 0 (0x0)��";

	//����SMTP������IP��˿�
	s.setIpAddress("220.181.12.11");
	s.setPort(25);

	//��ʼ��
	if ((back=s.SMTPInit()) != __SUCCESS)
	{
		cout <<"INIT ERROR"<< back;
		return 0;
	}
	//���õ�½�˺�����Ȩ��
	s.setUserPasswd("cppfastsolution@163.com", "WBKZTXWMSKCJOFJM");
	//���÷��͵�ַ��Ŀ�ĵ�ַ�������õ�һ��ϵ�ˣ�Ҳ�ɵ��ö����Ӷ���ϵ�˽���Ⱥ��
	s.setFromandTo("cppfastsolution@163.com", "1565203609@qq.com");
	//��Ӷ�������ˣ����ö�μ���
	s.addRcpto("2737089450@qq.com");
	//�����ʼ�����������
	s.setSubjectandMsg("һ�������ʼ�",sendMsgdata,sendMsgdata.length());
	//��¼
	back=s.Login();
	if (back != __SUCCESS)
	{
		cout << "LOGIN ERROR"<<back;
		s.SMTPQuit();
		return 0;
	}
	//�����ʼ����˺���ֻ��Ⱥ���ʼ�
	back = s.SendMailAll();
	//�˺���ֻ�ɵ����ʼ�
	//back = s.SendMail();
	if (back != __SUCCESS)
	{
		cout << "SEND ERROR" << back;
		s.SMTPQuit();
		return 0;
	}
	s.SMTPQuit();
}