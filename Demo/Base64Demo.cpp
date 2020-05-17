#include"Base64.h"

int main()
{
	string y = "A";
	string s = Base64::Base64Encode(y, y.length());
	cout << s;
	s = Base64::Base64Decode(s.c_str(), s.length());
	cout <<"\n"<<s;
}