#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <pbc.h>
#include <pbc_random.h>

#include "bswabe.h"
#include "common.h"
#include "abelib.h"

int
abetest( char* cipher, char * pk, int k )
{
	bswabe_pub_t* pub;
	bswabe_prv_t* prv;
	int file_len;
	GByteArray* aes_buf;
	GByteArray* plt;
	GByteArray* cph_buf;
	bswabe_cph_t* cph;
	element_t m;

	char* pub_file   = 0;
	char* prv_file   = 0;
	char* in_file    = 0;
	//char* out_file   = 0;

	int   keep       = 0;

	keep = k;

	pub_file = "/home/xx/test_file/abe_pub_key";

	prv_file = pk;

	in_file = cipher;

	//out_file = plain;

    //if( !out_file )
	//{
	//	if(  strlen(in_file) > 6 &&
	//			!strcmp(in_file + strlen(in_file) - 6, ".cpabe") )
	//		out_file = g_strndup(in_file, strlen(in_file) - 6);
	//	else
	//		out_file = strdup(in_file);
	//}
	
	//if( keep && !strcmp(in_file, out_file) )
	//	die("cannot keep input file when decrypting file in place (try -o)\n");
	//parse_args(argc, argv);

	pub = bswabe_pub_unserialize(suck_file(pub_file), 1);
	prv = bswabe_prv_unserialize(pub, suck_file(prv_file), 1);

	read_cpabe_file(in_file, &cph_buf, &file_len, &aes_buf);

	cph = bswabe_cph_unserialize(pub, cph_buf, 1);
	if( !bswabe_dec(pub, prv, cph, m) )
		die("%s", bswabe_error());
	bswabe_cph_free(cph);

	plt = aes_128_cbc_decrypt(aes_buf, m);
	g_byte_array_set_size(plt, file_len);
	g_byte_array_free(aes_buf, 1);

	//spit_file(out_file, plt, 1);
	fwrite(plt->data, 1, plt->len, stdout);
	//printf("The decrypted information is stored in the %s.\n", out_file);

	if( !keep )
		unlink(in_file);


	return 0;
}

void main()
{
	abetest("/home/xx/test_file/ciphertext.txt", "/home/xx/test_file/sara_priv_key", 1);
}
