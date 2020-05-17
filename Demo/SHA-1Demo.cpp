#include"SHA1.h"

int main()
{
	string po = "china";
	char outp[SHA1_LEN];
	SHA1::Sha1Encode(po.c_str(), po.length(), outp);
	puts(outp);
	return 0;
}
