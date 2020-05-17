#pragma once
#include<iostream>
#include<string>

#define SHA1_LEN 41
using namespace std;


class __declspec(dllexport) SHA1
{
public:
	explicit SHA1();
	static void Sha1Encode
	(
		_In_ const char* text,
		_In_ size_t len,
		_Inout_ char* outData
	);
	static bool Sha1StrValidate
	(
		_In_ const char* input_1,
		_In_ const char* input_2
	);
private:
	static inline void CharToUlong
	(
		_In_ const unsigned char* char_data,
		_Inout_ unsigned int* ulong_data
	);
};

