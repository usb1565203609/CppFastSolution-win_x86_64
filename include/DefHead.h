/***********************************************Copyright:Pluviophile******************************************/                            
/**********************************************Email:1565203609@qq.com*****************************************/
#pragma once


#define cu_long						unsigned long
#define cu_char						unsigned char
#define cu_int						unsigned int
#define cu_short					unsigned short

#define __ERROR						0X00000			
#define __SUCCESS					0X00001
//PE FILE DEFINE
#define __FILE_OPEN_ERROR			0X00002
#define __FILE_READ_ERROR			0X00003
#define __FILE_NO_PE				0X00004
#define __FILE_NO_NT				0X00005
#define __FILE_SAVE_ERROR			0X00006
#define __FILE_WRITE_ERROR			0X00007
#define __LASTSECTION_NO_NULL		0X00008
#define __FILE_WRITESIZE_MISMATCH	0X00009
#define __FILE_TOO_BIG				0X0000A

#define __SECTION_SIZE				0X00028

#define __I386_FILE_MAX				0XFFFFFFFF

//UDPSocket DEINE
#define __SOCK_WSAINIT_ERROR		0X0000B
#define __SOCK_INIT_ERROR			__SOCK_WSAINIT_ERROR+1
#define __SOCK_PORT_ERROR			__SOCK_WSAINIT_ERROR+2
#define __SOCK_IP_ERROR				__SOCK_WSAINIT_ERROR+3
#define __SOCK_LISTEN_ERROR			__SOCK_WSAINIT_ERROR+4
#define __SOCKET_CLOSE_ERROR		__SOCK_WSAINIT_ERROR+5
#define __SOCKET_LISTENNUM_TOOBIG   __SOCK_WSAINIT_ERROR+6
#define __SOCK_ACCEPT_ERROR			__SOCK_WSAINIT_ERROR+7
#define __SOCK_CONNECT_ERROR		__SOCK_WSAINIT_ERROR+8
#define __SOCK_IP_ISNULL			__SOCK_WSAINIT_ERROR+9
#define __SOCKET_NO_RECV			__SOCK_WSAINIT_ERROR+10

//base64
#define __BASE_NO_BASE64			0X00016

//SMTP
#define __SMTP_ERROR				0X00017
#define __SMTP_HELO_ERROR			__SMTP_ERROR+1
#define __SMTP_AUTH_ERROR			__SMTP_ERROR+2
#define __SMTP_USERPASSWD_ERROR		__SMTP_ERROR+3
#define __SMTP_PASSWD_ERROR			__SMTP_ERROR+4
#define __SMTP_FROM_ERROR			__SMTP_ERROR+5
#define __SMTP_RECPTO_ERROR			__SMTP_ERROR+6
#define __SMTP_QUIT_ERROR			__SMTP_ERROR+7
#define __SMTP_DATAFLAG_ERROR		__SMTP_ERROR+8
#define __SMTP_EMAILSEND_ERROR		__SMTP_ERROR+9