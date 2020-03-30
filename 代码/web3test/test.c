#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>  
#include <sys/wait.h>
#include <string.h>
#define BUF_SIZE 1024 
int main()
{
	char wp[BUF_SIZE], rp[BUF_SIZE];
	int i = 1;
	//system("node /home/xx/web3test/index1230-pastevent.js ");
	//printf("execve done \n");
/*	if(fork() == 0){
		char *argv[] = {"node", "/home/xx/web3test/index1230-pastevent.js", NULL};
		char *envp[] = {0, NULL};
		if(execve("/usr/bin/env", argv, envp) < 0){
			printf("error on exec\n");
			exit(0);
		}
	}
	else{
		wait(&childpid);
		printf("execve done %d\n",childpid);
	}*/	
	FILE* stream;
	char buf[1024], path[1024];
	memset(buf, 0, sizeof(buf));
    stream = popen("node /home/xx/web3test/index1230-pastevent.js git/test/a.txt93nfsnv2", "r");

    if (stream == NULL)
    {
        printf("popen open file error!\n");
    }
    else
    {
    	while (fgets(buf, 1024, stream) != NULL) {  

        fprintf(stdout, "%s", buf);
        if(i == 1)  strcpy(wp, buf);
        else if(i == 2)  strcpy(rp, buf);
        else printf("more than two outputs\n");
        i = i+1;
        }
    }
    pclose(stream);
    wp[strlen(wp)-1] = '\0';
    rp[strlen(rp)-1] = '\0';
    getcwd(path,sizeof(path));  
   	printf("current working directory: %s\n", path);
    printf("wp: %s, %ld\n", wp, strlen(wp)); 
    printf("rp: %s, %ld\n", rp, strlen(rp)); 
    return 1;
}