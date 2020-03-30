#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glib.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include <pbc.h>

#include "common.h"




FILE*
abs_fopen_read_or_die( char* file )
{
	FILE* f;

	if( !(f = fopen(file, "r")) )
		abs_die("can't read file: %s\n", file);

	return f;
}

FILE*
abs_fopen_write_or_die( char* file )
{
	FILE* f;

	if( !(f = fopen(file, "w")) )
		abs_die("can't write file: %s\n", file);

	return f;
}

GByteArray*
abs_suck_file( char* file )
{
	FILE* f;
	GByteArray* a;
	struct stat s;
	//int i;

	a = g_byte_array_new();
	stat(file, &s);
	//printf("length = %d\n", s.st_size);
	g_byte_array_set_size(a, s.st_size);

	f = abs_fopen_read_or_die(file);
	fread(a->data, 1, s.st_size, f);
	//for(i = 0; i < a->len; i++)
	//	printf("a->data[%d] = %c\n", i, a->data[i]);
	//printf("a->data = %s\n", a->data);
	fclose(f);

	return a;
}

char*
abs_suck_file_str( char* file )
{
	GByteArray* a;
	char* s;
	unsigned char zero;

	a = abs_suck_file(file);
	zero = 0;
	g_byte_array_append(a, &zero, 1);
	s = (char*) a->data;
	g_byte_array_free(a, 0);

	return s;
}

char*
abs_suck_stdin()
{
	GString* s;
	char* r;
	int c;

	s = g_string_new("");
	while( (c = fgetc(stdin)) != EOF )
		g_string_append_c(s, c);

	r = s->str;
	g_string_free(s, 0);

	return r;
}

void
abs_spit_file( char* file, GByteArray* b, int free )
{
	FILE* f;

	f = abs_fopen_write_or_die(file);
	fwrite(b->data, 1, b->len, f);
	fclose(f);

	if( free )
		g_byte_array_free(b, 1);
}

void read_abs_file( char* file,    GByteArray** cph_buf,
											int* file_len, GByteArray** sig_buf )
{
	FILE* f;
	int i;
	int len;

	*sig_buf = g_byte_array_new();
	*cph_buf = g_byte_array_new();

	f = abs_fopen_read_or_die(file);

	/* read real file len as 32-bit big endian int */
	*file_len = 0;
	for( i = 3; i >= 0; i-- )
		*file_len |= fgetc(f)<<(i*8);

	/* read cph buf */
	len = 0;
	for( i = 3; i >= 0; i-- )
		len |= fgetc(f)<<(i*8);
	g_byte_array_set_size(*cph_buf, len);
	//printf("cph_len = %d\n", len);
	fread((*cph_buf)->data, 1, len, f);
	//printf("(*cph_buf)->data = %s\n",(*cph_buf)->data);

	/* read sig buf */
	len = 0;
	for( i = 3; i >= 0; i-- )
		len |= fgetc(f)<<(i*8);
	g_byte_array_set_size(*sig_buf, len);
	fread((*sig_buf)->data, 1, len, f);
	
	fclose(f);
}

void
write_abs_file( char* file,   GByteArray* sig_buf,
									int file_len, GByteArray* cph_buf )
{
	FILE* f;
	int i;

	f = abs_fopen_write_or_die(file);

	/* write real file len as 32-bit big endian int */
	for(i = 3; i >= 0; i-- )
		fputc((file_len & 0xff<<(i*8))>>(i*8), f);

	/* write sig_buf */
	for( i = 3; i >= 0; i-- )
		fputc((sig_buf->len & 0xff<<(i*8))>>(i*8), f);
	fwrite(sig_buf->data, 1, sig_buf->len, f);
	
	/* write cph_buf */
	for( i = 3; i >= 0; i-- )
		fputc((cph_buf->len & 0xff<<(i*8))>>(i*8), f);
	//printf("cph_len = %d\n", cph_buf->len);
	//for( j = 0; j < message->len; j++)
	fwrite(cph_buf->data, 1, cph_buf->len, f);
		//fputc(message->data[j], stdout);

	fclose(f);
}

void
abs_die(char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	exit(1);
}
