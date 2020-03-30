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
	
    
    char* rp = "(A1 and A2) or (A5 or (A6 and A7))";
	char* abepub_file = "/home/xx/test_file/abe_pub_key";
	
	GByteArray* plt;
	plt = g_byte_array_new();
	char str[STLEN];

	int len;

	fgets(str,STLEN,stdin);    //输出路径长度
//    printf("%s",str);
    len = atoi(str);
//    printf("len:%d\n", len);

   char path[len+1];
    fgets(path,len+1,stdin);   //输出路径
//    printf("%s",path);
    getchar();
    while((fgets(str,STLEN,stdin))!=NULL)  //输出文件
{   
     g_byte_array_append (plt, (guint8*) str, strlen(str));
	//printf("%s",str);
}


	
	abeenc(plt, abepub_file, rp);
	
	
}