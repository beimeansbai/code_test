int abssetup(char* pub_file, char* msk_file);
int abskeygen( char* pk, char*  pub_file, char*  msk_file, char** attribute, int num );
int abssign( char* cipher_message, char* pubfile, char* sk, char* ac, int k);
int absverify( char* cipher_file, char* pubfile, char* ac);
//int abssign1(  char* sk, char* ac);
//int abssign2(char* msg_file, char* sk, char* ac);
//int absverify1( char* cipher_file, char* ac);
//int abstest(char* cipher_file, char* ac);
//int absverify2( char* sign_file, char* cipher_file, char* ac);