#include <stdio.h>
#include <string.h>
#include "abelib.h"
//#include "abslib.h"

void main()
{
	
	///char* attributes[4];
	//attributes[0] = "A1";
	//attributes[1] = "ABC";
	//attributes[2] = "BCDE";
	//attributes[3] = "hire_data";
	//abekeygen( "/home/xx/test_file/sara_priv_key", attributes, 4 );
	
	//int a = absverify( "/home/xx/test_file/ciphertext1.txt", "security_team and (abc or (hire_data and business_staff))");
	abedec( "/home/xx/test_file/ciphertext.txt", "/home/xx/test_file/sara_priv_key1", 0);
}