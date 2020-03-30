#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <pbc.h>
#include <pbc_random.h>

#include "mprabs.h"
#include "common.h"

#include "abslib.h"

#define STLEN 50
/* cipher_message- the name of the file need to be signed
   cipher_sign- the name of the outfile(cipher+signature)
   sk- the name of the private key file
*/
int
abstest(char* cipher_file, char* ac)
{
	mprabs_pub_t* pub;
	mprabs_prv_t* prv;
	mprabs_sig_t* sig;
	int file_len;
	GByteArray* cph_buf;
	GByteArray* sig_buf;
	

	char* pub_file = "/home/xx/test_file/abs_pub_key";
	//char* prv_file = 0;
	char* in_file  = 0;
	//char* out_file = 0;
	//int   keep     = 0;

	//char* policy = 0;

	//prv_file = sk;

	//keep = k; //k = 1 don't delete original file

	in_file = cipher_file;

	//int **policy_m = parse_policy(ac);

	//out_file = cipher_sign;

	//if( !out_file )
	//	out_file = g_strdup_printf("%s.abs", in_file);

	//if( !policy )
		//policy = parse_policy_lang(suck_stdin());
		//printf("parse policy false")
	
	pub = mprabs_pub_unserialize(abs_suck_file(pub_file), 1);
	//prv = mprabs_prv_unserialize(pub, abs_suck_file(prv_file), 1);

	int i;
	int len;
	

	sig_buf = g_byte_array_new();
	cph_buf = g_byte_array_new();

	
	//read_abs_file("/home/xx/gittest1/abs_test/abc.txt", &cph_buf, &file_len, &sig_buf);
	
	/* read real file len as 32-bit big endian int */
	file_len = 0;
	for( i = 3; i >= 0; i-- )
		file_len |= fgetc(stdin)<<(i*8);

	/* read cph buf */
	len = 0;

	for( i = 3; i >= 0; i-- )
		len |= fgetc(stdin)<<(i*8);
	g_byte_array_set_size(cph_buf, len);
	//printf("cph_len = %d\n", len);
	fread(cph_buf->data, 1, len, stdin);
	//printf("(*cph_buf)->data = %s\n",(*cph_buf)->data);

	/* read sig buf */
	len = 0;
	for( i = 3; i >= 0; i-- )
		len |= fgetc(stdin)<<(i*8);
	g_byte_array_set_size(sig_buf, len);
	fread(sig_buf->data, 1, len, stdin);
    //message = abs_suck_file(in_file);//read the plaintext
    
	//write_abs_file("/home/xx/test_file/signedtext.txt", sig_buf, file_len, message);  //copy in_file, add the sig to get out_file
	for(i = 3; i >= 0; i-- )
		fputc((file_len & 0xff<<(i*8))>>(i*8), stdout);

	/* write cph_buf */
	for( i = 3; i >= 0; i-- )
		fputc((cpu_buf->len & 0xff<<(i*8))>>(i*8), stdout);
	//printf("cph_len = %d\n", cph_buf->len);
	//for( j = 0; j < message->len; j++)
	fwrite(cph_buf->data, 1, cph_buf->len, stdout);
		//fputc(message->data[j], stdout);

	/* write sig_buf */
	for( i = 3; i >= 0; i-- )
		fputc((sig_buf->len & 0xff<<(i*8))>>(i*8), stdout);
	fwrite(sig_buf->data, 1, sig_buf->len, stdout);
	//for( j = 0; j < sig_buf->len; j++)
		//fputc(sig_buf->data[j], stdout);

	
	g_byte_array_free(sig_buf, 1);
	g_byte_array_free(message, 1);
	

	//if( !keep )
	//	unlink(in_file);

	return 0;
}

//void main()
//{

	//abssign( "ciphertext.txt", "signedtext.txt", "kevin_priv_key1", "((security_team and hire_data) or (sysadmin and abc)) or business_staff", 1);
	//abssign( "ciphertext.txt", "signedtext.txt", "kevin_priv_key1", "security_team and (abc or (hire_data and business_staff))", 1);
//}

