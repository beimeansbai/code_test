#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "fileid.h"
#define BUF_SIZE 1024
#define STLEN 50
int main()
{
	//char wp[BUF_SIZE], rp[BUF_SIZE];
	//int i = 1;
    char * hashlist[10];  //假设不超过10个文件
    char * pathlist[10];

    /*获取文件的hash值*/
    char * hash;
    hash = (char *)malloc(sizeof(char)*41);
    
    FILE* f1;
    FILE* f2;     //直接读仓库中的文件
    if((f1 = fopen("/home/xx/test_file/middle.txt", "w")) == NULL )   //把git filter输入的文件内容暂存,进行hash
        printf( "The file was not opened\n" );
    if((f2 = fopen("/home/xx/gittest/abs_test/test.txt", "r")) == NULL )   
        printf( "The file was not opened\n" );
    char str[STLEN];
    while (fgets(str, STLEN, f2) != NULL && str[0] != '\n')    //之前的判断语句有问题 不加&&之后的
    {
        //fputs(str, f1);
        fwrite(str, sizeof(char), strlen(str), f1);
        //fwrite(str, sizeof(char), strlen(str), stdout);
    }
    
    fclose(f1);


    FILE * stream;
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
        printf("hash: %s\n", hash);
    }

    pclose(stream);




    int j;
    int flag = 0;

	FILE * stream1;
	FILE * stream2;
	char commit[41], buf[BUF_SIZE];
	memset(commit, 0, sizeof(commit));
    
    /*获取git fetch的commit ID*/
	stream1 = popen("cat ~/gittest/abs_test/.git/FETCH_HEAD", "r");
    if (stream1 == NULL)
    {
        printf("FETCH_HEAD open error!\n");
    }
    else
    {
    	fgets(commit, 40, stream1);
    	//printf("%s\n", commit);
    }
    //fclose(stream1);
    pclose(stream1);

    int i = 0;
    /*根据commit ID进入相应的仓库中查找对应的文件信息*/
    char command1[BUF_SIZE] = "/home/xx/abe_abs/path.sh ";
    strcat(command1, commit);
    //printf("%s\n", command);
    stream2 = popen(command1, "r");

    if (stream2 == NULL)
    {
        printf("popen open file error!\n");
    }
    else
    {
    	while (fgets(buf, 1024, stream2) != NULL) 
        {  
            fprintf(stdout, "%s", buf);
            hashlist[i] = (char *)malloc(sizeof(char)*41);
            pathlist[i] = (char *)malloc(sizeof(char)*(strlen(buf)-53));
            strncpy(hashlist[i], buf+12, 40);
            hashlist[i][40] = '\0';
            strncpy(pathlist[i], buf+53, strlen(buf)-54);
            pathlist[strlen(buf)-52] = '\0';
            printf("%s\n", hashlist[i]);
            printf("%s\n", pathlist[i]);
            if(strcmp(hash, hashlist[i]) == 0)
            {
                printf("the file path is : %s\n", pathlist[i]);
                flag = 1;
                break;
            }


            i++;
        }
        
    }
    pclose(stream2);


    
    if(flag == 0) printf("cannot find the path of hash %s \n", hash);

    for(j = 0; j < i; j++)
    {
        free(hashlist[j]);
        free(pathlist[j]);
    }
    
    free(hash);

    return 1;
    
}