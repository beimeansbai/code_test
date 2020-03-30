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
abedec( char* cipher, char* pubfile, char * pk, int k )
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
	

	int   keep       = 0;    //是否删除中间文件
	keep = k;

	pub_file = pubfile;

	prv_file = pk;

	in_file = cipher;

	
	/*读取公钥和用户私钥*/
	pub = bswabe_pub_unserialize(suck_file(pub_file), 1);
	prv = bswabe_prv_unserialize(pub, suck_file(prv_file), 1);

	/*读取中间文件获取密文*/
	read_cpabe_file(in_file, &cph_buf, &file_len, &aes_buf);
	cph = bswabe_cph_unserialize(pub, cph_buf, 1);
	
	/*解密获得AES密钥*/
	if( !bswabe_dec(pub, prv, cph, m) )
		die("%s", bswabe_error());
	bswabe_cph_free(cph);
	
	/*解密获得明文*/
	plt = aes_128_cbc_decrypt(aes_buf, m);
	g_byte_array_set_size(plt, file_len);
	g_byte_array_free(aes_buf, 1);

	/*将明文内容输出*/
	fwrite(plt->data, 1, plt->len, stdout);
	

	if( !keep )
		unlink(in_file);


	return 0;
}

//void main()
//{
//	abedec( "middle.txt", "/home/xx/test_file/sara_priv_key", 1 );
//}
