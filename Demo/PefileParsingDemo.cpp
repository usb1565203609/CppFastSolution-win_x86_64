#include"PefileParsing.h"

//һ�����ڲ��Եĳ���ʵ�ָ�ect1.exe�����һ���½���
//��������½�����Ŀ�ִ�г��򱣴�Ϊt.exe�������Լ���ʵ��������ò���
int main
(
	_In_ int argv,
	_In_ char*argc[]
)
{
	string filepath = "G:/vs/Project1/Release/ect1.exe";
	//Ҫ����½��������ļ�����·��
	string savepath = "C:/t.exe";
	//��Ӻ���ļ�����·��
	DWORD I = 0;
	BYTE* Arr = NULL;
	BYTE* NH = NULL;
	BYTE SD[0x200];
	for (int i = 0; i < 0x200; i++)
		SD[i] = '\x00';
	
	PefileParsing *h = PefileParsing::getInterface(filepath,savepath);
	SIZE_T fsize = 0;
	Arr = h->GetFilePE(I,fsize);
	//���ݻ�ȡʧ��
	if (Arr == NULL)
	{
		cout << "OPEN ERROR��"<<I;
	}
	else
	{
		//����½���
		NH = h->Add_NewSection(Arr, fsize, I, SD,(BYTE*)".ioep", 0x200);
		if (NH == NULL)
		{
			cout << "add error " << I;
			return 0;
		}
		//������½�������ļ����ݱ��浽����
		I=h->SaveFile(NH, fsize);
		if (I != __SUCCESS)
		{
			cout << "save error " << I;
			return 0;
		}
		//������������ֹ�ڴ�й©
		h->ClearPEBuff(NH);
	}
	return 0;
}