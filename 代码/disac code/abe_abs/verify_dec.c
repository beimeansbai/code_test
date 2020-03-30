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
	char* abepub_file = "/home/xx/test_file/abe_pub_key";
	char* abspub_file = "/home/xx/test_file/abs_pub_key";
	
   
   /*读取文件密文和签名*/
	GByteArray* cph_buf;
    GByteArray* sig_buf;
    int file_len;
    //int j;
       

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

    

    /*验签及解密*/
	absverify( file_len, cph_buf, sig_buf, "/home/xx/test_file/ciphertext1.txt", abspub_file, wp);
	abedec( "/home/xx/test_file/ciphertext1.txt", abepub_file, "/home/xx/test_file/sara_abe_priv_key", 1);
}
