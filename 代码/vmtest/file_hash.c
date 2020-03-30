#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fileid.h"
#define STLEN 50

char* file_hash(char* hash)
//void main()
{
	
    //char buf[STLEN];
    //scanf("%s", buf);

    /*
    FILE* f1;
    if((f1 = fopen("/home/xx/test_file/middle.txt", "w")) == NULL )   //把git filter输入的文件内容暂存,进行hash
		printf( "The file was not opened\n" );
	fputs(buf, f1);
    fclose(f1);
    */
/*
	FILE * stream;
	char hash[41];
	memset(hash, 0, sizeof(hash));
	gchar * sh = "./hash.sh ";
	GString* co;
	co = g_string_new(sh);
    co = g_string_append(co, r);
    printf("command: %s\n", co->str);
    char* command;
    command = co->str;
    */
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
    	fgets(hash, 41, stream);
        hash[40] = '\0';
        //printf("hash: %s\n", hash);
    }

    pclose(stream);
    //char c[41] = {0};
    //strcpy(c, hash);
    //free(hash);
    return hash;

}
