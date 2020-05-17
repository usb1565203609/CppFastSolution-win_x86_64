#ifndef RSA_H
#define RSA_H

#include<iostream>
#include<string>
#include<vector>
#include<time.h>
#include<C_EncryCore.h>

using namespace std;


class __declspec(dllexport) RSA:public CEncryCore
{
    public:
        explicit RSA();
        void getKey
        (
            _Inout_ string& public_key,
            _Inout_ string& private_key,
            _Inout_ string& n
        );
        string RSA_Encode
        (
            _In_  const char* IN_Data,
            _Inout_ size_t& inoutLen,
            _In_ string public_key,
            _In_ string n
        );
        string RSA_Decode
        (
            _In_ string private_key,
            _In_ string n,
            _In_ string c_data,
            _Inout_ size_t& inoutLen
        );

};
#endif