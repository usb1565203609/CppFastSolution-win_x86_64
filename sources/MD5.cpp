#include"MD5.h"

MD5::MD5()
{
	nullptr;
}

void MD5::MD5Encode
(
	_In_ const char* text,
	_In_ size_t len,
	_Inout_ char* dst
)
{
	//分组
	short output[16];
	char dest[16];
	memset(dest, 0, SHORT_MD5_LEN);
	memset(dst, 0, CHAR_MD5_LEN);
	//四组幻数
	unsigned int h[] = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };
	static const unsigned int k[64] =
	{
		0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
		0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
		0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
		0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
		0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
		0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
		0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
		0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
		0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
		0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
		0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
		0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
		0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
	};
	//四轮循环(GG,FF,HH,II)每轮循环每一步所要位移的位数
	static const unsigned int qz[] =
	{
		7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
		5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
		4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
		6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
	};
	//每一轮所要读取的元素下表
	static const unsigned int s[] =
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
		1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12,
		5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2,
		0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9
	};

	unsigned int i = 0, j = 0;
	//N*512+448
	//N=(数据长度+64(bit))/512(bit)，长度+8是为了防止数据长度>=56
	//任意数据长度+64bit刚好是512倍数，最后+8空出存放数据原始长度的位置
	size_t n_len = ((len + 8) / 64) * 64 + 56 + 8;
	unsigned char *n_text = (unsigned char *)malloc(n_len);
	memset(n_text, 0x00, n_len);
	memcpy(n_text, text, len);
	//末尾添加二进制1000 0000
	n_text[len] = 0x80;
	//追加长度
	//注意此处末尾添加的是一个64位的数据！！！
	unsigned char len_s[8];
	memset(len_s, 0x00, 8);
	unsigned long temp_len = 0x00000000;
	temp_len = (unsigned long)(len * 8);
	//此处注意，只拷贝4个字节数据，因为
	//unsigned long只有四个字节
	memcpy(len_s, &temp_len, sizeof(unsigned long));
	memcpy(n_text + (n_len-8), len_s, 8);

	//每64字节（512位）
	//处理一次,因为填充过后的数刚好是64的倍数
	for (j = 0; j < n_len; j += 64)
	{
		unsigned int H[4] = { 0,0,0,0 };
		memcpy(H, h, 4 * sizeof(unsigned int));
		//分段拷贝内容，以供处理多组数据
		unsigned char temp_text[64];
		memset(temp_text, 0x00, 64);
		memcpy(temp_text, n_text + j, 64);

		//一共循环64次，分为四组
		for (i = 0; i < 64; i++)
		{
			//四组非线性函数运算，用这个开关语句来判断是第几组
			// 0~16第一组，16~32第二组
			//32~48第三组，48~64第四组
			unsigned int R = 0, f = 0, tmp = 0;
			switch ((int)i / 16)
			{
			//H[1]=X,H[2]=Y,H[3]=Z
			//F(X,Y,Z)
			case 0: f = (H[1] & H[2]) | ((~H[1]) & H[3]); break;
			//G(X,Y,Z)
			case 1: f = (H[3] & H[1]) | (H[2] & (~H[3])); break;
			//H(X,Y,Z)
			case 2: f = H[1] ^ H[2] ^ H[3]; break;
			//I
			case 3: f = H[2] ^ (H[1] | (~H[3]));  break;
			}
			//abcd分别交换位置
			tmp  = H[3];
			H[3] = H[2];
			H[2] = H[1];
			//R=(a+?(bcd)+M?+ti),四个字节一运算不是一个字节
			R = H[0] + f + k[i] + (((unsigned int *)temp_text)[s[i]]);
			//b+((a+?(bcd)+M?+ti)<<s))
			H[1] = H[1] + ((R << qz[i]) | (R >> (32 - qz[i])));
			H[0] = tmp;
		}
		//每轮循环结束后，ABCD分别与abcd相加
		for (i = 0; i < 4; i++) h[i] += H[i];
	}

	free(n_text);
	memcpy(dest, h, 16);

	//与0xff位与将高位的ff变为00
	for (int i = 0; i < 16; i++)
		output[i] = dest[i] & 0xff;
	//将十六进制数据打印成字符串
	for (int i = 0; i < SHORT_MD5_LEN; i++)
		sprintf_s(dst + i * 2, 3, "%02x", output[i]);
}


bool MD5::MD5StrValidate
(
	_In_ const char * input1, 
	_In_ const char * input2
)
{
	if (strcmp(input1, input2) == 0)
		return true;
	return false;
}
