#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "fileidlib"
#define BUF_SIZE 1024
int main()
{
	//char wp[BUF_SIZE], rp[BUF_SIZE];
	//int i = 1;
	FILE * stream1;
	FILE * stream2;
	char commit[41], buf[BUF_SIZE];
	memset(commit, 0, sizeof(commit));
    //stream1 = fopen("~/gittest/abs_test/.git/FETCH_HEAD", "r");
	stream1 = popen("cat ~/gittest/abs_test/.git/FETCH_HEAD", "r");
    if (stream1 == NULL)
    {
        printf("FETCH_HEAD open error!\n");
    }
    else
    {
    	fgets(commit, 40, stream1);
    	printf("%s\n", commit);
    }
    //fclose(stream1);
    pclose(stream1);
    char command[BUF_SIZE] = "./path.sh ";
    strcat(command, commit);
    printf("%s\n", command);
    stream2 = popen(command, "r");

    if (stream2 == NULL)
    {
        printf("popen open file error!\n");
    }
    else
    {
    	while (fgets(buf, 1024, stream2) != NULL) {  

        fprintf(stdout, "%s", buf);
      
        }
    }
    pclose(stream2);
     
    return 1;
}