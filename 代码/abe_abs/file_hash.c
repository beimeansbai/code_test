#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <pbc.h>
#include <pbc_random.h>
#include "fileid.h"
#define STLEN 50
/*
char* file_hash(GByteArray* buf)
{

}
*/
char* file_hash(GByteArray* buf)
{
	
    FILE* f1;
    if((f1 = fopen("/home/xx/test_file/middle.txt", "w")) == NULL )
		printf( "The file was not opened\n" );
	fwrite(buf->data, 1, buf->len, f1);
    fclose(f1);
    
/*
    GString* s;
	gchar* r;
	char c;
	j = 0;
	s = g_string_sized_new(32);
	while( 1 )
	{
		c = buf->data[j++];
		if( c && c != EOF )
			g_string_append_c(s, c);
		else
			break;
	}
	r = s->str;
	g_string_free(s, 0);
	printf("%s\n", r);


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
    char* hash;
    hash = (char *)malloc(sizeof(char)*40);
    char* command = "/home/xx/abe_abs/hash.sh /home/xx/test_file/middle.txt";
   // printf("%s\n", command);
    stream = popen(command, "r");

    if (stream == NULL)
    {
        printf("popen open file error!\n");
    }
    else
    {
    	fgets(hash, 41, stream);
      
        //printf("hash: %s\n", hash);
    }
    pclose(stream);
 	return hash;

}
