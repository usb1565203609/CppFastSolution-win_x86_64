/***********************************************Copyright:Pluviophile******************************************/                            
/**********************************************Email:1565203609@qq.com*****************************************/
#include "PefileParsing.h"

PefileParsing* PefileParsing::__Inter = nullptr;

PefileParsing::PefileParsing()
{
	;
}

PefileParsing::PefileParsing
(
	_In_ string filepath,
	_In_ string savepath
)
{
	this->filepath = filepath;
	this->savepath = savepath;
}

void PefileParsing::setFilePath
(
	_In_ string filepath,
	_In_ string savepath
)
{
	this->filepath = filepath;
	this->savepath = savepath;
}

int PefileParsing::SaveFile
(
	_In_ BYTE* FileData,
	_In_ const DWORD FileSize
)
{
	//创建文件句柄
	HANDLE PeFileHandle = CreateFile
	(
		LPCSTR(this->savepath.c_str()),
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (PeFileHandle == INVALID_HANDLE_VALUE)
		return __FILE_SAVE_ERROR;
	DWORD BG = 0;
	int b = WriteFile(PeFileHandle, FileData, FileSize, &BG, NULL);
	if (!b)
		return __FILE_WRITE_ERROR;
	if (FileSize != BG)
		return __FILE_WRITESIZE_MISMATCH;
	CloseHandle(PeFileHandle);
	return __SUCCESS;
}

BYTE* PefileParsing::GetFilePE
(
	_Inout_ int& back,
	_Inout_ SIZE_T& fsize
)
{
	HANDLE PeFileHandle = CreateFile
	(
		LPCSTR(this->filepath.c_str()),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (PeFileHandle == INVALID_HANDLE_VALUE)
	{
		back = __FILE_OPEN_ERROR;
		return NULL;
	}
	this->File_Size = GetFileSize(PeFileHandle,NULL);
	fsize = this->File_Size;

	if (fsize >= __I386_FILE_MAX)
	{
		back = __FILE_TOO_BIG;
		return NULL;
	}
	//申请文件缓冲区内存
	BYTE* DataBuff = new BYTE[this->File_Size];
	memset(DataBuff, 0, this->File_Size);

	DWORD Rsize = 0;
	int K = ReadFile(PeFileHandle, DataBuff, this->File_Size, &Rsize, NULL);
	if (K == 0)
	{
		back = __FILE_READ_ERROR;
		return NULL;
	}
	CloseHandle(PeFileHandle);

	return DataBuff;
}

void PefileParsing::ClearPEBuff(BYTE * File_Data)
{
	delete[] File_Data;
}

PIMAGE_DOS_HEADER PefileParsing::GetFile_Dosheader
(
	_In_ BYTE* File_Data,
	_Inout_ int& back
)
{
	PIMAGE_DOS_HEADER FH= (PIMAGE_DOS_HEADER)File_Data;
	if (FH->e_magic != IMAGE_DOS_SIGNATURE)
	{
		back = __FILE_NO_PE;
		return NULL;
	}
	back = __SUCCESS;
	return FH;
}

PIMAGE_NT_HEADERS PefileParsing::GetFile_NTheader
(
	_In_ BYTE* File_Data,
	_Inout_ int& back
)
{
	int B = 0;
	PIMAGE_NT_HEADERS NTH= (PIMAGE_NT_HEADERS)
		((GetFile_Dosheader(File_Data,B)->e_lfanew)+(DWORD)File_Data);
	if (B == __FILE_NO_PE)
	{
		back = __FILE_NO_PE;
		return NULL;
	}
	if (NTH->Signature != IMAGE_NT_SIGNATURE)
	{
		back = __FILE_NO_NT;
		return NULL;
	}
	back = __SUCCESS;
	return NTH;
}

PIMAGE_FILE_HEADER PefileParsing::GetFile_header
(
	_In_ BYTE* File_Data,
	_Inout_ int& back
)
{
	int U = 0;
	PIMAGE_FILE_HEADER FFH = &GetFile_NTheader(File_Data,U)->FileHeader;
	if (U == __FILE_NO_NT)
	{
		back = __FILE_NO_NT;
		return NULL;
	}
	if (U == __FILE_NO_PE)
	{
		back = __FILE_NO_PE;
		return NULL;
	}
	back = __SUCCESS;
	return FFH;
}

PIMAGE_OPTIONAL_HEADER  PefileParsing::GetFile_Optheader
(
	_In_ BYTE * File_Data,
	_Inout_ int& back
)
{
	int O = 0;
	PIMAGE_OPTIONAL_HEADER OH = &GetFile_NTheader(File_Data,O)->OptionalHeader;
	if (O == __FILE_NO_NT)
	{
		back = __FILE_NO_NT;
		return NULL;
	}
	if (O == __FILE_NO_PE)
	{
		back = __FILE_NO_PE;
		return NULL;
	}
	back = __SUCCESS;
	return OH;
}

PIMAGE_SECTION_HEADER  PefileParsing::GetLastSection
(
	_In_ BYTE * File_Data,
	_Inout_ int& back
)
{
	int backNum = 0;
	DWORD SecNum = GetFile_header(File_Data,backNum)->NumberOfSections;
	if (SecNum == 0)
	{
		back = backNum;
		return NULL;
	}
	PIMAGE_SECTION_HEADER Fsec = IMAGE_FIRST_SECTION(GetFile_NTheader(File_Data,backNum));

	PIMAGE_SECTION_HEADER s = Fsec + (SecNum - 1);
	char buff[0x28];
	memcpy(buff, s, 0x28);
	for(int i=0;i<0x28;i++)
		if (buff[i] != '\x00')
		{
			back = __LASTSECTION_NO_NULL;
			return NULL;
		}
	back = __SUCCESS;
	return s;
}

PIMAGE_SECTION_HEADER PefileParsing::GetSectionForName
(
	_In_ BYTE* FileData,
	_In_ string secName,
	_Inout_ int& backNum
)
{
	int back = __SUCCESS;
	int secNumber = GetFile_header(FileData, back)->NumberOfSections;
	if (back != __SUCCESS)
	{
		backNum = back;
		return NULL;
	}
	PIMAGE_SECTION_HEADER FirstSec = 
		IMAGE_FIRST_SECTION(GetFile_NTheader(FileData, back));
	if (back!=__SUCCESS)
	{
		backNum = back;
		return NULL;
	}

	char CurrenSecName[8];
	memset(CurrenSecName, 0, 8);
	for (int i = 0; i < secNumber; i++)
	{
		memcpy(CurrenSecName, (char*)FirstSec[i].Name, 8);
		if (strcmp(CurrenSecName, secName.c_str()) == 0)
			return ((PIMAGE_SECTION_HEADER)FirstSec + i);
	}
	backNum = back;
	return NULL;
}

BYTE * PefileParsing::GetSectionData
(
	_In_ BYTE* FileData,
	_In_ string secName,
	_Inout_ unsigned long& Secsize,
	_In_ int& back
)
{
	int backNum = __SUCCESS;
	PIMAGE_SECTION_HEADER Sec = GetSectionForName(FileData, secName, backNum);
	if (backNum != __SUCCESS)
	{
		back = backNum;
		return NULL;
	}
	DWORD sizeSec = Sec->SizeOfRawData;
	BYTE* SectionData = new BYTE[sizeSec];

	memcpy(SectionData, FileData + (Sec->PointerToRawData), sizeSec);

	Secsize = sizeSec;
	back = backNum;
	return SectionData;
}

SIZE_T PefileParsing::Section_Alignment
(
	_In_ SIZE_T File_Size,
	_In_ SIZE_T Alignment
)
{
	return ((File_Size%Alignment == 0) ? 
		File_Size : File_Size / (Alignment - 1)*Alignment);
}

BYTE* PefileParsing::Add_NewSection
(
	_Inout_ BYTE * OldData, 
	_Inout_ SIZE_T &fsize, 
	_Inout_ int& back,
	_In_ const BYTE* Section_data,
	_In_ const BYTE* Section_name, 
	_In_ const SIZE_T Section_size
)
{
	PIMAGE_SECTION_HEADER nSection = NULL;
	PIMAGE_OPTIONAL_HEADER OptHeader = NULL;
	BYTE* NewData = NULL;

	int backNum = 0;
	OptHeader = GetFile_Optheader(OldData,backNum);
	if (OptHeader == NULL)
	{
		back = backNum;
		return NULL;
	}
	GetFile_header(OldData,backNum)->NumberOfSections++;
	if (backNum != __SUCCESS)
	{
		back = backNum;
		return NULL;
	}
	nSection = GetLastSection(OldData, backNum);
	if (nSection == NULL)
	{
		back = backNum;
		return NULL;
	}
	memcpy(nSection->Name, Section_name, 8);
	nSection->Misc.VirtualSize = (DWORD)Section_size;
	nSection->SizeOfRawData = 
		Section_Alignment(Section_size, OptHeader->FileAlignment);

	//设置区段虚拟地址，要再加0x1000否则会与前一个区段相同
	nSection->VirtualAddress =
		(nSection - 1)->VirtualAddress + Section_Alignment(
		(nSection - 1)->SizeOfRawData, OptHeader->SectionAlignment)+0x1000;
	
	nSection->PointerToRawData =
		(nSection - 1)->PointerToRawData + (nSection - 1)->SizeOfRawData;
	//设置区段属性
	nSection->Characteristics = 0xE00000E0;

	OptHeader->SizeOfImage += nSection->SizeOfRawData;

	SIZE_T NewSize = 
		nSection->PointerToRawData + nSection->SizeOfRawData;
	NewData = new BYTE[NewSize];
	memcpy(NewData, OldData, fsize);
	
	delete[] OldData;
	fsize = NewSize;
	
	memcpy((NewData + nSection->PointerToRawData), Section_data, Section_size);
	return NewData;
}

int PefileParsing::GetPEfileType
(
	_In_ BYTE* FileData,
	_Inout_ int& back
)
{
	int BK = 0;
	PIMAGE_FILE_HEADER fh = GetFile_header(FileData, BK);
	if (fh == NULL)
	{
		back = BK;
		return __ERROR;
	}
	else
		return fh->Characteristics;
}

PefileParsing::~PefileParsing()
{
	;
}

//静态访问方法
PefileParsing* PefileParsing::getInterface
(
	_In_ string path,
	_In_ string savepath
)
{
	if (__Inter == nullptr)
		__Inter = new PefileParsing(path,savepath);
	return __Inter;
}


