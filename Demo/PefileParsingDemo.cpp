#include"PefileParsing.h"

//一个用于测试的程序，实现给ect1.exe程序加一个新节区
//并将添加新节区后的可执行程序保存为t.exe，根据自己的实际情况设置参数
int main
(
	_In_ int argv,
	_In_ char*argc[]
)
{
	string filepath = "G:/vs/Project1/Release/ect1.exe";
	//要添加新节区的老文件保存路径
	string savepath = "C:/t.exe";
	//添加后的文件保存路径
	DWORD I = 0;
	BYTE* Arr = NULL;
	BYTE* NH = NULL;
	BYTE SD[0x200];
	for (int i = 0; i < 0x200; i++)
		SD[i] = '\x00';
	
	PefileParsing *h = PefileParsing::getInterface(filepath,savepath);
	SIZE_T fsize = 0;
	Arr = h->GetFilePE(I,fsize);
	//内容获取失败
	if (Arr == NULL)
	{
		cout << "OPEN ERROR！"<<I;
	}
	else
	{
		//添加新节区
		NH = h->Add_NewSection(Arr, fsize, I, SD,(BYTE*)".ioep", 0x200);
		if (NH == NULL)
		{
			cout << "add error " << I;
			return 0;
		}
		//将添加新节区后的文件数据保存到磁盘
		I=h->SaveFile(NH, fsize);
		if (I != __SUCCESS)
		{
			cout << "save error " << I;
			return 0;
		}
		//清理缓存区，防止内存泄漏
		h->ClearPEBuff(NH);
	}
	return 0;
}