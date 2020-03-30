#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>  
#include <sys/wait.h>
#include <glib.h>
#include <pbc.h>
#include <pbc_random.h>
#include <sys/stat.h>
#include <unistd.h>
#include "abelib.h"
#include "abslib.h"

#define BUF_SIZE 1024
void main()
{
	char wp[BUF_SIZE], rp[BUF_SIZE];
	int i = 1;
	//abesetup();
	//abssetup();
	//char* attr[5];
	//attr[0] = "security_team";
	//attr[1] = "strategy_team";
	//attr[2] = "executive_level";
	//attr[3] = "business_staff";
	//attr[4] = "hire_data";
	//abskeygen( "/home/xx/test_file/sara_priv_key1", attr, 5);
	FILE* stream;
	char buf[BUF_SIZE];
	memset(buf, 0, sizeof(buf));
    stream = popen("node /home/xx/web3test/index1230-pastevent.js git/test/a.txt93nfsnv2", "r");

    if (stream == NULL)
    {
        printf("popen open file error!\n");
    }
    else
    {
    	while (fgets(buf, BUF_SIZE, stream) != NULL) {  

        //fprintf(stdout, "%s", buf);
        if(i == 1)  strcpy(wp, buf);
        else if(i == 2)  strcpy(rp, buf);
        else printf("more than two outputs\n");
        i = i+1;
        }
    }
    pclose(stream);
    wp[strlen(wp)-1] = '\0';   //"security_team and (abc or (hire_data and business_staff))"
    rp[strlen(rp)-1] = '\0';   //"(A1 and A2) or (A5 or (A6 and A7))"
	char* abepub_file = "/home/xx/test_file/abe_pub_key";
	char* abspub_file = "/home/xx/test_file/abs_pub_key";

	GByteArray* plt;
	plt = g_byte_array_new();
	char str[STLEN];
	while (fgets(str, STLEN, stdin) != NULL && str[0] != '\n')
	{
		//puts(str);
		g_byte_array_append (plt, (guint8*) str, strlen(str));
	}

	
	abeenc(plt, "/home/xx/test_file/ciphertext.txt", abepub_file, rp);
	abssign("/home/xx/test_file/ciphertext.txt", abspub_file, "/home/xx/test_file/sara_abs_priv_key", wp, 1);
	
}
