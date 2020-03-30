#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <pbc.h>
#include <pbc_random.h>

#include "mprabs.h"
#include "common.h"
#include "abslib.h"
#include "private.h"

int abssetup(char* pub_file, char* msk_file)
{
	mprabs_pub_t* pub;
	mprabs_msk_t* msk;

	//char* pub_file = "/home/xx/test_file/abs_pub_key";
	//char* msk_file = "/home/xx/test_file/abs_master_key";

		//parse_args(argc, argv);

	mprabs_setup(&pub, &msk);
	abs_spit_file(pub_file, mprabs_pub_serialize(pub), 1);
	abs_spit_file(msk_file, mprabs_msk_serialize(msk), 1);

	return 0;
}

//void main()
//{
//	abssetup("/home/xx/test_file/abs_pub_key", "/home/xx/test_file/abs_master_key");
//}
