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
	GByteArray* cph_buf;
	GByteArray* aes_buf;
	element_t m;     //AES加密密钥

	char* pub_file = 0;
	char* out_file = 0;
	char* policy = 0;


	policy = parse_policy_lang(ac);    //解析policy

	//printf("(%s)\n", policy);
	pub_file = pubfile;    //公钥
	out_file = cipher;     //中间文件路径

	//if( !out_file )
	//	out_file = g_strdup_printf("%s.cpabe", in_file);

	//if( !policy )
	//	policy = parse_policy_lang(suck_stdin());
	

	pub = bswabe_pub_unserialize(suck_file(pub_file), 1);    //取abe公钥

  if( !(cph = bswabe_enc(pub, m, policy)) )       //对AES加密密钥m进行ABE加密，结果放在cph中
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
 
	  
	/*对文件进行AES加密*/
	file_len = plt->len;
	aes_buf = aes_128_cbc_encrypt(plt, m);
	g_byte_array_free(plt, 1);
	element_clear(m);

	/*将密文输出到中间文件*/
	write_cpabe_file(out_file, cph_buf, file_len, aes_buf);

	g_byte_array_free(cph_buf, 1);
	g_byte_array_free(aes_buf, 1);

	

	return 0;
}

//void main()
//{
//	abeenc(  "/home/xx/test_file/ciphertext.txt", "(AAA and A2) or (A5 or (BCDE and ABC))");
//}