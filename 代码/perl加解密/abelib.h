int abesetup(char* pub_file, char* msk_file);
int abekeygen( char* pk, char*  pub_file, char*  msk_file, char** attribute, int num );
int abeenc( GByteArray* plt, char* pubfile, char* ac);
int abedec( GByteArray* aes_buf, GByteArray* cph_buf, int file_len, char* pubfile, char * pk, int k );
//int abeenc1( char * cipher, char* ac);
//int abetest( char* cipher, char * pk, int k );
//int abedec2( char* cipher, char * plain, char * pk, int k );