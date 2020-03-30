#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <pbc.h>
#include <pbc_random.h>

#include <bswabe.h>
#include "common.h"
#include "abelib.h"




int abesetup(char* pub_file, char* msk_file)
{
	bswabe_pub_t* pub;
	bswabe_msk_t* msk;

	//char* pub_file = "/home/xx/test_file/abe_pub_key";
	//char* msk_file = "/home/xx/test_file/abe_master_key";

	//parse_args(argc, argv);

	bswabe_setup(&pub, &msk);
	spit_file(pub_file, bswabe_pub_serialize(pub), 1);
	spit_file(msk_file, bswabe_msk_serialize(msk), 1);

	return 0;
}

//void main()
//{
//	abesetup("/home/xx/test_file/abe_pub_key", "/home/xx/test_file/abe_master_key");
//}