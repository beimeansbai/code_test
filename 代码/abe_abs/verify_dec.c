#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>  
#include <sys/wait.h>
#include <glib.h>
#include <pbc.h>
#include <pbc_random.h>
#include "abelib.h"
#include "abslib.h"
#define BUF_SIZE 1024
void main()
{
	char wp[BUF_SIZE], rp[BUF_SIZE];
	int i = 1;
	//char* attributes[4];
	//attributes[0] = "A1";
	//attributes[1] = "ABC";
	//attributes[2] = "BCDE";
	//attributes[3] = "hire_data";
	//abekeygen( "/home/xx/test_file/sara_priv_key", attributes, 4 );
    GByteArray* cph_buf;
    GByteArray* sig_buf;
    int file_len;
    int j;
    int len;
    //char ch;

    sig_buf = g_byte_array_new();
    cph_buf = g_byte_array_new();
    file_len = 0;
    for( j = 3; j >= 0; j-- )
        file_len |= fgetc(stdin)<<(j*8);

    /* read cph buf */
    len = 0;
    for( j = 3; j >= 0; j-- )
        len |= fgetc(stdin)<<(j*8);
    g_byte_array_set_size(cph_buf, len);
    fread(cph_buf->data, 1, len, stdin);
   
    /* read sig buf */
    len = 0;
    for( j = 3; j >= 0; j-- )
        len |= fgetc(stdin)<<(j*8);
    g_byte_array_set_size(sig_buf, len);
    fread(sig_buf->data, 1, len, stdin);

    GByteArray* buf;
    buf = g_byte_array_new();
    g_byte_array_append (buf, file_len, strlen(file_len));


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

	absverify( file_len, cph_buf, sig_buf, "/home/xx/test_file/ciphertext1.txt", abspub_file, wp);
	abedec( "/home/xx/test_file/ciphertext1.txt", abepub_file, "/home/xx/test_file/sara_abe_priv_key", 1);
}
