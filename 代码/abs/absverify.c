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
	//int file_len;
	//GByteArray* plt;
	//GByteArray* cph_buf;
	//GByteArray* sig_buf;
	int result;
	

	char* pub_file = 0;
	pub_file = pubfile;
	//char* in_file  = 0;
	char* out_file = 0;
	//int   keep     = 0;

	//int** policy_m = 0;

	//keep = k; //k = 1 don't delete original file

	//in_file = sign_file;

	//policy_m = parse_policy(ac);

	//printf("(%s)\n", policy);

	out_file = cipher_file;

	//if( !out_file )
	//	out_file = g_strdup_printf("%s.cpabe", in_file);

	pub = mprabs_pub_unserialize(abs_suck_file(pub_file), 1);

	//plt = g_byte_array_new();
	//char str[STLEN];
	//while (fgets(str, STLEN, stdin) != NULL && str[0] != '\n')
	//{
	//	puts(str);
	//	g_byte_array_append (plt, (guint8*) str, strlen(str));
	//}
    //char* test_input = "/home/xx/test_file/test_input.txt";
	//abs_spit_file(test_input, plt, 1);

    //read_abs_file(test_input, &cph_buf, &file_len, &sig_buf);
    
    /*
    int i;
	int len;
	sig_buf = g_byte_array_new();
	cph_buf = g_byte_array_new();

	file_len = 0;
	for( i = 3; i >= 0; i-- )
		file_len |= fgetc(stdin)<<(i*8);*/

	/* read cph buf */
	/*
	len = 0;
	for( i = 3; i >= 0; i-- )
		len |= fgetc(stdin)<<(i*8);
	g_byte_array_set_size(cph_buf, len);
	//printf("cph_len = %d\n", len);
	fread(cph_buf->data, 1, len, stdin);*/
	//printf("(*cph_buf)->data = %s\n",(*cph_buf)->data);

	/* read sig buf */
	/*
	len = 0;
	for( i = 3; i >= 0; i-- )
		len |= fgetc(stdin)<<(i*8);
	g_byte_array_set_size(sig_buf, len);
	fread(sig_buf->data, 1, len, stdin);*/
   
	//aaaaaaa
	sig = mprabs_sig_unserialize(pub, sig_buf, 1);
	
	
	//aaaaaaa
	result = mprabs_verify(pub, sig, cph_buf, ac);

	//if( !keep )
	//	unlink(in_file);

	//aaaaaa
    if(result)
    {
    	//write_abs_file("/home/xx/test_file/signedtext.txt", sig_buf, file_len, cph_buf); 
    	//fwrite(cph_buf->data, 1, cph_buf->len, stdout);
    	abs_spit_file(out_file, cph_buf, 1);
    	/*FILE* f;
			
		if(!(f = fopen(out_file,"w")))
    	{
       		printf("file cannot open \n");

    	}
    	fwrite(cph_buf->data, 1, cph_buf->len, f);
    	//fprintf(f, "%d\n", tmpres1);
		fclose(f);
		g_byte_array_free(cph_buf, 1);*/
    	//write_abs_file(out_file, cph_buf, file_len);
    	//g_byte_array_free(cph_buf, 1);
    	//printf("Signature verification successful! The message is stored in %s.\n", out_file);
    	//printf("Signature verification successful!\n");
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