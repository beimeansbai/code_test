#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <pbc.h>
#include <pbc_random.h>
#include <sys/stat.h>
#include <unistd.h>
#include "bswabe.h"
#include "common.h"
#include "policy_lang.h"
#include "abelib.h"

#define STLEN 50

int
abeenc( GByteArray* plt, char * cipher, char* pubfile, char* ac)
{
	bswabe_pub_t* pub;
	bswabe_cph_t* cph;
	int file_len;
	//GByteArray* plt;
	GByteArray* cph_buf;
	GByteArray* aes_buf;
	element_t m;

	char* pub_file = 0;

	//char* in_file  = 0;
	char* out_file = 0;
	//int   keep     = 0;

	char* policy = 0;

	//keep = k; //k = 1 don't delete original file

	//in_file = plain;

	policy = parse_policy_lang(ac);

	//printf("(%s)\n", policy);
	pub_file = pubfile;
	out_file = cipher;

	//if( !out_file )
	//	out_file = g_strdup_printf("%s.cpabe", in_file);

	if( !policy )
		policy = parse_policy_lang(suck_stdin());
	//parse_args(argc, argv);

	pub = bswabe_pub_unserialize(suck_file(pub_file), 1);

  if( !(cph = bswabe_enc(pub, m, policy)) )
		die("%s", bswabe_error());
	free(policy);

	cph_buf = bswabe_cph_serialize(cph);
	bswabe_cph_free(cph);

	//plt = suck_file(in_file);
/*	plt = g_byte_array_new();
	char str[STLEN];
	while (fgets(str, STLEN, stdin) != NULL && str[0] != '\n')
	{
		//puts(str);
		g_byte_array_append (plt, (guint8*) str, strlen(str));
	}*/
    //struct stat s;
    //fstat(fileno(stdin), &s);
    //g_byte_array_set_size(plt, s.st_size);

	
	//fread(plt->data, 1, s.st_size, stdin); 

	  

	file_len = plt->len;
	aes_buf = aes_128_cbc_encrypt(plt, m);

	//char* test_input = "/home/xx/test_file/test_input.txt";
	//spit_file(test_input, plt, 1);

	g_byte_array_free(plt, 1);
	element_clear(m);

	/*int i;
	
	for( i = 3; i >= 0; i-- )
		fputc((file_len & 0xff<<(i*8))>>(i*8), stdout);*/

	/* write aes_buf */
	/*for( i = 3; i >= 0; i-- )
		fputc((aes_buf->len & 0xff<<(i*8))>>(i*8), stdout);
	fwrite(aes_buf->data, 1, aes_buf->len, stdout);*/

	/* write cph_buf */
	/*for( i = 3; i >= 0; i-- )
		fputc((cph_buf->len & 0xff<<(i*8))>>(i*8), stdout);
	fwrite(cph_buf->data, 1, cph_buf->len, stdout);*/


	write_cpabe_file(out_file, cph_buf, file_len, aes_buf);

	g_byte_array_free(cph_buf, 1);
	g_byte_array_free(aes_buf, 1);

	//if( !keep )
	//	unlink(in_file);

	return 0;
}

//void main()
//{
//	abeenc(  "/home/xx/test_file/ciphertext.txt", "(AAA and A2) or (A5 or (BCDE and ABC))");
//}