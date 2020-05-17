#include "Base64.h"

Base64::Base64()
{
}

string Base64::Base64Encode
(
	_In_ string tsrc,
	_In_ cu_long srcsize 
)
{
	const cu_char* src = (const cu_char*)tsrc.c_str();
	cu_char* tempBuff = new cu_char[3];
	string destStr;
	//取到最后剩几个数据
	int lastNumb = srcsize % 3;

	//分为三组处理
	int i;
	for (i = 0; i < (srcsize / 3); i++)
	{
		memset(tempBuff, 0x00, 3);
		memcpy(tempBuff, (src+i*3), 3);

		destStr += base64tab[(tempBuff[0] >> 2)];
		destStr += base64tab[(((tempBuff[0] & 0x03) << 4) | (tempBuff[1] >> 4))];
		destStr += base64tab[(((tempBuff[1] & 0x0f) << 2) | (tempBuff[2] >> 6))];
		destStr += base64tab[(tempBuff[2] & 0x3f)];
	}
	//剩余数据处理
	if (lastNumb == 1)
	{
		memset(tempBuff, 0x00, 3);
		memcpy(tempBuff, (src + i * 3), 1);
		destStr += base64tab[(tempBuff[0] >> 2)];
		destStr += base64tab[(tempBuff[0] & 0x03) << 4];
		destStr.append("==");
	}
	if (lastNumb == 2)
	{
		memset(tempBuff, 0x00, 2);
		memcpy(tempBuff, (src + i * 3), 2);
		destStr += base64tab[(tempBuff[0] >> 2)];
		destStr += base64tab[(((tempBuff[0] & 0x03) << 4) | (tempBuff[1] >> 4))];
		destStr += base64tab[((tempBuff[1] & 0x0f) << 2)];
		destStr.append("=");
	}

	delete[] tempBuff;
	return destStr;
}

string Base64::Base64Decode
(
	_In_ string tsrc,
	_In_ cu_long srcsize
)
{
	const cu_char *src = (const cu_char*)tsrc.c_str();
	string backStr;
	cu_char* tempBuff = new cu_char[4];
	if (srcsize % 4)
		return "";
	int i;
	for (i = 0; i < (srcsize / 4); i++)
	{
		memset(tempBuff, 0x00, 4);
		memcpy(tempBuff, src + (i * 4), 4);

		backStr += (cu_char)(Base64Detab[tempBuff[0]] << 2) |
					(Base64Detab[tempBuff[1]] >> 4);
		if (tempBuff[2] != '=')
			backStr += (cu_char)((Base64Detab[tempBuff[1]] & 0x0f) << 4) |
			(Base64Detab[tempBuff[2]] >> 2);
		if (tempBuff[3] != '=')
			backStr += (cu_char)(Base64Detab[tempBuff[2]] << 6) |
			Base64Detab[tempBuff[3]];

	}
	delete[] tempBuff;
	return backStr;
}


