#include <stdio.h>
#include <string.h>
#include "abelib.h"
#include "abslib.h"

void main()
{
	
	abesetup();
	abssetup();
	char* attr[5];
	attr[0] = "security_team";
	attr[1] = "strategy_team";
	attr[2] = "executive_level";
	attr[3] = "business_staff";
	attr[4] = "hire_data";
	abskeygen( "/home/xx/test_file/sara_priv_key1", attr, 5);
	abeenc(  "/home/xx/test_file/ciphertext.txt", "(AAA and A2) or (A5 or (BCDE and ABC))");
	abssign( "/home/xx/test_file/ciphertext.txt", "/home/xx/test_file/sara_priv_key1", "security_team and (abc or (hire_data and business_staff))", 0);
	
}