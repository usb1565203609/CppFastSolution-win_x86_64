#include"RSA.h"

int main()
{
    string e,d,n;
    RSA* a=new RSA();
    string input;
    cout<<"请输入明文：";
    cin>>input;
    a->getKey(e,d,n);
    size_t out_len=input.length();
    string c=a->RSA_Encode(input.c_str(),out_len,e,n);
    cout<<"\n明文:\n"<<input<<"\n"<<"密文:\n"<<c<<"\n长度:\n"<<out_len;
    string m=a->RSA_Decode(d,n,c,out_len);
    cout<<"\n\n密文:\n"<<c<<"\n明文:\n"<<m<<"\n长度:\n"<<out_len;
    return 0;
}