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


#define STLEN 1024
void main()
{
	//char file_path[70];
	int i;
    
    char* rp = "(A1 and A2) or (A5 or (A6 and A7))";
	char* abepub_file = "/home/xx/test_file/abe_pub_key";
	
	//GByteArray* plt;
	//plt = g_byte_array_new();
	char str[STLEN];

	int len;

	fgets(str,STLEN,stdin);    //输出路径长度
    len = atoi(str);
   // printf("len:%d\n", len);

    char path[len+2];
    fgets(path,len+1,stdin);   //输出路径
    //printf("%s",path);
   
    getchar();
	GByteArray* aes_buf;
	GByteArray* cph_buf;
	cph_buf = g_byte_array_new();
	aes_buf = g_byte_array_new();

	int file_len;
	//FILE* fp;
	//fp = fopen("/home/xx/test_file/abstest/aaa.txt", "r");
	/* read real file len as 32-bit big endian int */
	file_len = 0;
	for( i = 3; i >= 0; i-- )
		file_len |= fgetc(stdin)<<(i*8);

	/* read aes buf */
	len = 0;
	for( i = 3; i >= 0; i-- )
		len |= fgetc(stdin)<<(i*8);
	g_byte_array_set_size(aes_buf, len);
	fread((aes_buf)->data, 1, len, stdin);

	/* read cph buf */
	len = 0;
	for( i = 3; i >= 0; i-- )
		len |= fgetc(stdin)<<(i*8);
	g_byte_array_set_size(cph_buf, len);
	fread((cph_buf)->data, 1, len, stdin);
	
	
	//printf("%d\n", file_len);

	
	abedec(aes_buf, cph_buf, file_len, abepub_file, "/home/xx/test_file/sara_abe_priv_key", 1);

    
}