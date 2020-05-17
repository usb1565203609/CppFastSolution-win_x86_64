#include "SHA1.h"

SHA1::SHA1()
{
	nullptr;
}

void SHA1::Sha1Encode
(
	_In_ const char* text,
	_In_ size_t len,
	_Inout_ char* outData
)
{
	/*
	*常量初始化
	*/
	unsigned long k = 0, f = 0;
	unsigned int temp_ul_text[80];
	unsigned long temp_A = 0;
	unsigned long h[] = { 0x67452301,0xefcdab89,0x98badcfe,0x10325476,0xc3d2e1f0 };
	/*
	*开始填充原文
	*/
	size_t n_len = ((len + 8) / 64) * 64 + 56 + 8;
	unsigned char*n_text = (unsigned char*)malloc(n_len);
	memset(n_text, 0x00, n_len);
	memcpy(n_text, text, len);
	n_text[len] = 0x80;
	unsigned char c_lens[8];
	memset(c_lens, 0x00, 8);
	unsigned long temp_len = (unsigned long)(len * 8);
	memcpy(c_lens, &temp_len, sizeof(unsigned long));
	/*
	*颠倒数据长度存储的端序
	*/
	for (int i = 7; i >= 4; i--)
	{
		int y = c_lens[i];
		c_lens[i] = c_lens[7-i];
		c_lens[7-i] = y;
	}
	memcpy(n_text + (n_len - 8), c_lens, 8);
	
	/*
	*分组计算，512bits一组
	*/
	for (int i = 0; i < n_len; i += 64)
	{
		//A=H[0],B=H[1],C=H[2],D=H[3],E=H[4]
		unsigned long H[5] = { 0,0,0,0,0 };
		unsigned char temp_text[64];

		memset(temp_text, 0x00, 64);
		memset(temp_ul_text, 0x00, 80*sizeof(unsigned long));
		memcpy(H, h, 5 * (sizeof(unsigned long)));
		memcpy(temp_text, (n_text + i), 64);

		CharToUlong(temp_text, temp_ul_text);

		for (int j = 0; j < 80; j++)
		{
			switch ((int)j/20)
			{
			case 0:
				k = 0x5a827999;
				f = (H[1] & H[2]) | ((~H[1]) & H[3]);
				break;
			case 1:
				k = 0x6ed9eba1;
				f = H[1] ^ H[2] ^ H[3];
				break;
			case 2:
				k = 0x8f1bbcdc;
				f = (H[1] & H[2]) | (H[1] & H[3]) | (H[2] & H[3]);
				break;
			case 3:
				k = 0xca62c1d6;
				f = H[1] ^ H[2] ^ H[3];
				break;
			default:
				break;
			}
			temp_A = ((H[0] << 5) | (H[0] >> 27)) + f + H[4] + temp_ul_text[j] + k;
			H[4] = H[3];
			H[3] = H[2];
			H[2] = (H[1] << 30) | (H[1] >> 2);
			H[1] = H[0];
			H[0] = temp_A;
		}
		for (int k = 0; k < 5; k++)
			h[k] += H[k];
	}
	free(n_text);
	//字符串格式化返回输出结果
	for (int o = 0; o < 5; o++)
	{
		sprintf_s(outData, 41, "%lx", h[o]);
		outData += 8;
	}
}

bool SHA1::Sha1StrValidate
(
	_In_ const char* input_1,
	_In_ const char* input_2
)
{
	if (!strcmp(input_1, input_2))
		return true; 
	return false;
}

inline void SHA1::CharToUlong
(
	_In_ const unsigned char* char_data,
	_Inout_ unsigned int* ulong_data
) 
{
	for (int i = 0; i < 16; i++)
	{
		int j = 4 * i;
		ulong_data[i] = ((unsigned int)char_data[j]) << 24 | 
						((unsigned int)char_data[1 + j]) << 16 | 
						((unsigned int)char_data[2 + j]) << 8 |
						((unsigned int)char_data[3 + j]) << 0;
	}
	for (int i = 16; i < 80; i++)
	{
		ulong_data[i] = ulong_data[i - 16] ^ ulong_data[i - 14] ^ 
						ulong_data[i - 8] ^ ulong_data[i - 3];
		ulong_data[i] = (ulong_data[i] << 1) | (ulong_data[i] >> 31);
	}
}
