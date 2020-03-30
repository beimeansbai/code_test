#include <stdio.h>
#include <string.h>
#include "abelib.h"
#include "abslib.h"

void main()
{
	
	char abepub_file[80];
	char abemsk_file[80];
	char abspub_file[80];
	char absmsk_file[80];
	char abeprv_file[80];
	char absprv_file[80];
	printf("Please input the path of ABE public key file:\n");
	scanf("%s", abepub_file);
	//printf("%s\n", abepub_file);
	printf("Please input the path of ABE master key file:\n");
	scanf("%s", abemsk_file);
	//printf("%s\n", abemsk_file);
	printf("Please input the path of ABS public key file:\n");
	scanf("%s", abspub_file);

	printf("Please input the path of ABS master key file:\n");
	scanf("%s", absmsk_file);
	printf("Please input the path of ABE private key file:\n");
	scanf("%s", abeprv_file);
	printf("Please input the path of ABS private key file:\n");
	scanf("%s", absprv_file);
	//printf("%s\n", absprv_file);

	//abesetup(abepub_file, abemsk_file);
	//abssetup(abspub_file, absmsk_file);
	char* attr[5];
	attr[0] = "security_team";
	attr[1] = "strategy_team";
	attr[2] = "executive_level";
	attr[3] = "business_staff";
	attr[4] = "hire_data";
	
	abskeygen( abeprv_file, abspub_file, absmsk_file, attr, 5);  //sign key
	char* attributes[4];
	attributes[0] = "A0";
	attributes[1] = "A6";
	attributes[2] = "A7";
	attributes[3] = "B3";
	abekeygen( absprv_file, abepub_file, abemsk_file, attributes, 4 );  //dec key
}