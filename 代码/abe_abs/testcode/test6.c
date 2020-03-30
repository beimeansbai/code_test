#include <stdio.h>
#include <string.h>
#include "abelib.h"
#include "abslib.h"

void main()
{
	
	//abeenc1(  "/home/xx/test_file/ciphertext.txt", "(AAA and A2) or (A5 or (BCDE and ABC))");
	//abssign1( "/home/xx/test_file/sara_priv_key1", "security_team and (abc or (hire_data and business_staff))");
	abssign2( "/home/xx/test_file/signedtext.txt", "/home/xx/test_file/sara_priv_key1", "security_team and (abc or (hire_data and business_staff))");
	
}