#include"RSA.h"

RSA::RSA()
{
    ;
}

void RSA::getKey
(
    _Inout_ string& public_key,
    _Inout_ string& private_key,
    _Inout_ string& n
)
{
    mpz_t key_p, key_q, temp_n;
    mpz_t fi, pub_key, pri_key;
    //初始化p,q,n,φ(n),公钥，私钥
    mpz_init(key_p);
    mpz_init(key_q);
    mpz_init(temp_n);
    mpz_init(fi);
    mpz_init(pub_key);
    mpz_init(pri_key);
    /*
    *生成随机1024位质数
    **/
   //随机数种子
    gmp_randstate_t grat;
    //默认生成在随机性与效率之间取一个折中
    gmp_randinit_default(grat);
    //以当前时间作为随机数种子
    gmp_randseed_ui(grat,time(NULL));
    //生成两个个1024位的随机整数
    mpz_urandomb(key_p,grat,1024);
    mpz_urandomb(key_q,grat,1024);
    //生成素数
    mpz_nextprime(key_p,key_p);
    mpz_nextprime(key_q,key_q);
    /*
    *获得n与φ(n)
    **/
    //p×q得到n
    mpz_mul(temp_n,key_q,key_p);
    //p,q分别减一后相乘得到φ(n),
    //注意结尾的ui表示32位无符号整数
    mpz_sub_ui(key_p,key_p,1);
    mpz_sub_ui(key_q,key_q,1);
    mpz_mul(fi,key_q,key_p);
    /*得到公钥e此值可取65537、17、37、47
    *,但要注意，此值除65537以外其他小值
    *做公钥得到的密文是固定不变的，也就是说安全性是不可靠的
    **/
    mpz_set_ui(pub_key,65537);

    //逆元运算
    mpz_invert(pri_key,pub_key,fi);
    //将公钥私钥与n化为字符串
    mpz_class temp_d(pri_key);
    mpz_class swap_n(temp_n);
    mpz_class temp_e(pub_key);
    public_key=temp_e.get_str();
    private_key=temp_d.get_str();
    n=swap_n.get_str();

    mpz_clear(key_p);
    mpz_clear(key_q);
    mpz_clear(temp_n);
    mpz_clear(fi);
    mpz_clear(pub_key);
    mpz_clear(pri_key);
}

string RSA::RSA_Encode
(
    _In_ const char* IN_Data,
     _Inout_ size_t& inoutLen,
    _In_ string public_key,
    _In_ string n
)
{
    mpz_t m,pub_e,temp_n;
    mpz_init(m);
    mpz_init(pub_e);
    mpz_init(temp_n);
    //取得公钥、n、明文，将输入内容统一转化为/mpz_t高精度整数
    mpz_set_str(pub_e,public_key.c_str(),10);
    mpz_set_str(temp_n,n.c_str(),10);
    string out_data;
    //对字符串循环加密，并用回车隔开
    for(int i=0;i<inoutLen;i++)
    {
        mpz_set_ui(m,(unsigned long)IN_Data[i]);
        /*
        *模幂操作，取密文
        *c=(m^e) mod n
        **/
        mpz_powm(m,m,pub_e,temp_n);
        //取得字符串
        mpz_class c_data(m);
        out_data.append(c_data.get_str());
        out_data.append("\n");
    }
    inoutLen=out_data.length();

    mpz_clear(m);
    mpz_clear(pub_e);
    mpz_clear(temp_n);
    return out_data;
}

string RSA::RSA_Decode
(
    _In_ string private_key,
    _In_ string n,
    _In_ string c_data,
    _Inout_ size_t& inoutLen
)
{
    vector<string>C_List;
    string temp_str;
    cout<<"\n\n\n\n\n\n";
    //循环拆分字符串，根据原有字符个数拆分为字符串容器列表
    //以此按顺序处理密文
    for(int i=0;i<inoutLen;i++)
    {
        if(c_data.at(i)=='\n')
        {
            C_List.push_back(temp_str);
            temp_str.clear();
            continue;
        }
        temp_str+=c_data.at(i);
    }
    //cout<<"list\n\n"<<C_List.at(0);
    
    mpz_t pri_key,temp_n,C_Data;
    mpz_init(pri_key);
    mpz_init(temp_n);
    mpz_init(C_Data);
    //将字符串转为mpz_t高精度整数
    mpz_set_str(pri_key,private_key.c_str(),10);
    mpz_set_str(temp_n,n.c_str(),10);
    string back_data;
    //循环根据容器个数来判断原本有几个字符，
    //并将其密文解析为明文
    for(int i=0;i<C_List.size();i++)
    {
        mpz_set_str(C_Data,C_List.at(i).c_str(),10);
        mpz_powm(C_Data,C_Data,pri_key,temp_n);
        mpz_class CD(C_Data);
        unsigned long lchar=CD.get_ui();
        char words=(char)lchar;
        back_data+=words;
    }
    inoutLen=back_data.length();
    mpz_clear(pri_key);
    mpz_clear(temp_n);
    mpz_clear(C_Data);
    return back_data;
}