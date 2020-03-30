#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fileid.h"
#define STLEN 50

char* file_hash(char* hash1)
{
	//GByteArray* cph_buf;
    //GByteArray* sig_buf;
    //int file_len;
    //int j;
    //int len;
    FILE* f1;
    //FILE* f2;
    if((f1 = fopen("/home/xx/test_file/middle.txt", "w")) == NULL )   //把git filter输入的文件内容暂存,进行hash
        printf( "The file was not opened\n" );
   // if((f2 = fopen("/home/xx/gittest/abs_test/test.txt", "r")) == NULL )   
    //    printf( "The file was not opened\n" );
    char str[STLEN];
    while (fgets(str, STLEN, stdin) != NULL)
    {
        //fputs(str, f1);
        fwrite(str, sizeof(char), strlen(str), f1);
    }
    
    fclose(f1);
    //fclose(f2);

    /*
    GByteArray* buf;
	buf = g_byte_array_new();
	char str[STLEN];
	while (fgets(str, STLEN, stdin) != NULL && str[0] != '\n')    //读取git filter传入的内容
	{
		//puts(str);
		g_byte_array_append (buf, (guint8*) str, strlen(str));
	}
    */

	//printf("%s\n", buf->data);
    //printf("%d\n", buf->len);
    //printf("read success!\n");

    
    //if(fgets(str, STLEN, stdin) != NULL) ;//假设文件内容不超过STLEN
    //int len = strlen(str);

    //char * hash1;
    //hash1 = (char *)malloc(sizeof(char)*41);
    FILE * stream;
    //char* hash;
    //hash = (char *)malloc(sizeof(char)*41);
    char* command = "/home/xx/abe_abs/hash.sh /home/xx/test_file/middle.txt";  //对文件求hash(在终端测试此命令可行)
   // printf("%s\n", command);
    stream = popen(command, "r");

    if (stream == NULL)
    {
        printf("popen open file error!\n");
    }
    else
    {
        fgets(hash1, 41, stream);
        hash1[40] = '\0';
        printf("hash: %s\n", hash1);
    }

    pclose(stream);

      //根据传入的char*型变量hash1作为输出

    //printf("%s\n", hash1); 

    //fwrite(buf->data, 1, buf->len, stdout);   //输出测试
    
    //free(hash1);
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
	
	/*
    char* hash;
	hash = file_hash(buf);   //求文件内容的hash
	printf("%s", hash);     //输出测试
	*/

    return hash1;
}