#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <pbc.h>
#include <pbc_random.h>
#define STLEN 50

void main()
{
	GByteArray* message;

	message = g_byte_array_new();

	//int ch;

	int i;
	int j = 4234;
	//while((ch = fgetc(stdin)) != '\n')
	//{
	//	g_byte_array_append (message, (guint8*)ch, 1);
	//}
	char str[STLEN];
	while (fgets(str, STLEN, stdin) != NULL && str[0] != '\n')
	{
		fputs(str, stdout);
		//g_byte_array_append (message, (guint8*) str, strlen(str));
	}


	//fprintf(stdout, "the length s %d\n", message->len);
	//for( i = 3; i >= 0; i-- )
	//	fputc('a'+((message->len & 0xff<<(i*8))>>(i*8)), stdout);
	//fwrite(message->data, 1, message->len, stdout);

	
	//fprintf(stdout, "-----2222-----\n");
	//for( i = 0; i < message->len; i++)
	//	fputc(message->data[i], stdout);
	//fputc(j, stdout);
	//fprintf(stdout, "%d\n", j);
}