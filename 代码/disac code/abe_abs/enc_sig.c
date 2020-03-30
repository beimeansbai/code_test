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
	char str[BUF_SIZE];
	int path_len;
	int len;
	
	/*获取repo路径*/
	fgets(str, STLEN, stdin);    //获取路径长度
//    printf("%s",str);
    len = atoi(str);   //转换为int型
//    printf("len:%d\n", len);
    char repo_path[len + 1];
    fgets(repo_path, len + 1, stdin);   //根据长度读取路径,读出\0
//    printf("%s",path);
	int p = len;
	while(repo_hash[p] != '/')
		p--;
	
  
	/*获取文件路径*/
	fgets(str, STLEN, stdin);   
    path_len = atoi(str);   
    char path[path_len + 1];
    fgets(path, path_len + 1, stdin);   

    /*还需要根据git log --reverse来获取最早的commit ID*/
	FILE* stream;
	char base_hash[BUF_SIZE];
	char cmd[BUF_SIZE] = "/home/xx/abe_abs/commit.sh ";
	strcat(cmd, repo_path);  
	stream = popen(cmd, "r");   //调用commit.sh获取最初的commit ID
	if (stream == NULL)
    {
        printf("popen open commit.sh error!\n");
    }
    else
    {
    	if(fgets(base_hash, 40, stream) == NULL) {    //读取40位commit ID

        printf("commit.sh has no output\n");
        
        }
    }
    pclose(stream);
	/*base_hash加上repo name*/
	int j = 0;
	for(; p <= len; p ++)
	{
		base_hash[40 + j] = repo_path[p + 1];    //base_hash = commit ID + repo name
	}
	
	/*file_id*/
	char file_id[BUF_SIZE];
	strcpy(file_id, base_hash);
	strcat(file_id, path);
	
	
	/*调用blockchain获取access policy*/
	memset(cmd, 0, sizeof(cmd));
	stpcpy(cmd, "node /home/xx/web3test/index1230-pastevent.js ");   //读取以太坊事件的代码
	strcat(cmd, file_id);
	
	char buf[BUF_SIZE];	
	memset(buf, 0, sizeof(buf));
	
    stream = popen(cmd, "r");
    if (stream == NULL)
    {
        printf("popen open index1230-pastevent file error!\n");
    }
    else
    {
    	while (fgets(buf, BUF_SIZE, stream) != NULL) {  

        if(i == 1)  strcpy(wp, buf);     //读取两个policy
        else if(i == 2)  strcpy(rp, buf);
        else printf("more than two outputs\n");
        i = i+1;
        }
    }
    pclose(stream);
    wp[strlen(wp)-1] = '\0';   //"security_team and (abc or (hire_data and business_staff))"
    rp[strlen(rp)-1] = '\0';   //"(A1 and A2) or (A5 or (A6 and A7))"
	char* abepub_file = "/home/xx/test_file/abe_pub_key";    //公钥路径
	char* abspub_file = "/home/xx/test_file/abs_pub_key";

	/*读取文件明文*/
	GByteArray* plt;     //存放文件明文的变量
	plt = g_byte_array_new();
	char str[STLEN];
	while (fgets(str, STLEN, stdin) != NULL)    
	{
		//puts(str);
		g_byte_array_append (plt, (guint8*) str, strlen(str));
	}

	/*加密及签名*/
	abeenc(plt, "/home/xx/test_file/ciphertext.txt", abepub_file, rp);      //加密结果输出到中间文件
	abssign("/home/xx/test_file/ciphertext.txt", abspub_file, "/home/xx/test_file/sara_abs_priv_key", wp, 1);
	
}
