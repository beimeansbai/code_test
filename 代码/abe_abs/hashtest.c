#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <pbc.h>
#include <pbc_random.h>
#include "fileid.h"
#define STLEN 50

void main()
{
	GByteArray* cph_buf;
    GByteArray* sig_buf;
    int file_len;
    int j;
    int len;
    
    
    GByteArray* buf;
	buf = g_byte_array_new();
	char str[STLEN];
	while (fgets(str, STLEN, stdin) != NULL && str[0] != '\n')
	{
		//puts(str);
		g_byte_array_append (buf, (guint8*) str, strlen(str));
	}

	//printf("%s\n", buf->data);
    //printf("%d\n", buf->len);
    //printf("read success!\n");

    fwrite(buf->data, 1, buf->len, stdout);
    printf("aaa\n");
    /*
    sig_buf = g_byte_array_new();
    cph_buf = g_byte_array_new();
    file_len = 0;
    for( j = 3; j >= 0; j-- )
    {
        file_len |= buf->data[3-j]<<(j*8);

    }
   
    
    len = 0;
    for( j = 3; j >= 0; j-- )
        len |= buf->data[7-j]<<(j*8);
    g_byte_array_set_size(cph_buf, len);
    for(j = 0; j <= len; j++)
    	g_byte_array_append (cph_buf, &(buf->data[8+j]), 1);
    int a = 8 + j;

    
    len = 0;
    for( j = 3; j >= 0; j-- )
        len |= fgetc(stdin)<<(j*8);
    g_byte_array_set_size(sig_buf, len);
    for(j = 0; j <= len; j++)
    	g_byte_array_append (sig_buf, &(buf->data[a+j]), 1);
	*/
    //printf("length: %d\n", buf->len);
	
	char* hash;
	hash = file_hash(buf);
	printf("%s", hash);
	


}