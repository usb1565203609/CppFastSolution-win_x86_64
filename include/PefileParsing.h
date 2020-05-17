#ifndef PefileParsing_H
#define PefileParsing_H
#define CREATE_API_DU _declspec(dllimport)

#include"DefHead.h"

#include<Windows.h>
#include<winnt.h>
#include<iostream>


using namespace std;

/**************************************************************************
*	PefileParsing一个实现PE文件加壳器类，具体实现思路：		     *
*	1.先打开要被加壳的宿主文件，并将其内容读入内存			  *
*	  相关类方法：	setFilePath实现设置文件路径			*
*					GetFilePE获取文件内容并读入内存	  *
*	2.解析PE文件，解析出其各种PE文件头，找到最后一个区段，			 *
*	  并在最后一个区段后新加一个区段用于存放壳代码指令			 *
*                      Copyright:Pluviophile                              *
*                      Email:1565203609@qq.com                            *
***************************************************************************/

class  __declspec(dllexport) PefileParsing
{
public:
	static PefileParsing* __Inter;
	//functions
	explicit PefileParsing();
	explicit PefileParsing
	(
		_In_ string filepath,
		_In_ string savepath
	);
	void setFilePath
	(
		_In_ string filepath,
		_In_ string savepath
	);

	BYTE* Add_NewSection													//添加新区段
	(
		_Inout_ BYTE* OldData,
		_Inout_ SIZE_T &fsize,
		_Inout_ int& back,
		_In_ const BYTE* Section_data,
		_In_ const BYTE* Section_name,
		_In_ const SIZE_T Section_size
	);

	int SaveFile
	(
		_In_ BYTE*FileData,
		_In_ const DWORD FileSize
	);																	//保存文件
	BYTE* GetFilePE
	(
		_Inout_ int& back,
		_Inout_ SIZE_T& fsize
	);																	//获取文件数据
	
	~PefileParsing();
	
	//static function
	static void ClearPEBuff(_In_ BYTE* File_Data);						//清理释放PE文件内存,防止内存泄漏
	
	static PefileParsing *getInterface									//单例模式访问接口
	(
		_In_ string filepath,
		_In_ string savepath
	);
	static int GetPEfileType											//判断PE文件类型
	(
		_In_ BYTE* FileData,
		_Inout_ int& back
	);
	//static variable
	static PIMAGE_DOS_HEADER GetFile_Dosheader
	(
		_In_ BYTE* File_Data,
		_Inout_ int& back
	);																	//DOS header
	static PIMAGE_NT_HEADERS GetFile_NTheader
	(
		_In_ BYTE* File_Data,
		_Inout_ int& back
	);																	//NT header
	static PIMAGE_FILE_HEADER GetFile_header
	(
		_In_ BYTE* File_Data,
		_Inout_ int& back
	);																	//FILE header
	static PIMAGE_OPTIONAL_HEADER GetFile_Optheader
	(
		_In_ BYTE* File_Data,
		_Inout_ int& back
	);																	//OPTIONAL header
	static PIMAGE_SECTION_HEADER GetLastSection
	(
		_In_ BYTE* File_Data,
		_Inout_ int& back
	);																	//获取最后一个区段

	static PIMAGE_SECTION_HEADER GetSectionForName
	(
		_In_ BYTE* FileData,
		_In_ string secName,
		_Inout_ int& backNum
	);

	static BYTE* GetSectionData
	(
		_In_ BYTE* FileData,
		_In_ string secName,
		_Inout_ unsigned long& Secsize,
		_In_ int& back
	);

	static SIZE_T Section_Alignment											//对齐区段
	(
		_In_ SIZE_T File_Size,
		_In_ SIZE_T Alignment
	);

private:
	//variables
	DWORD64 File_Size;
	string filepath;
	string savepath;

	//functions
	
	

	
};
#endif // PefileParsing_H


