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

int
absverify( int file_len, GByteArray* cph_buf, GByteArray* sig_buf, char* cipher_file, char* pubfile, char* ac)
{
	mprabs_pub_t* pub;
	mprabs_sig_t* sig;
	int result;
	

	char* pub_file = 0;
	pub_file = pubfile;
	char* out_file = 0;
	
	out_file = cipher_file;   //中间文件

	
    /*读取公钥*/
	pub = mprabs_pub_unserialize(abs_suck_file(pub_file), 1);

	/*进行验签*/
	sig = mprabs_sig_unserialize(pub, sig_buf, 1);
	result = mprabs_verify(pub, sig, cph_buf, ac);

	
    if(result)
    {
    	abs_spit_file(out_file, cph_buf, 1);    //验签成功就将密文输出到中间文件
    	
    }
    	
    else
    {
    	printf("Signature verification failed!\n");
    	g_byte_array_free(cph_buf, 1);
    	return 0;
    }
    
    return 1;
}

//void main()
//{
	//int a = absverify( "signedtext.txt", "ciphertext1.txt", "((security_team and hire_data) or (sysadmin and abc)) or business_staff", 1);
	//int a = absverify( "signedtext.txt", "ciphertext1.txt", "security_team and (abc or (hire_data and business_staff))", 1);
//}