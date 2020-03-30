#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <glib.h>
#include <pbc.h>
#include <pbc_random.h>

#include "mprabs.h"
#include "common.h"

#include "abslib.h"

/*
	TODO ensure we don't give out the same attribute more than once (esp
	as different numerical values)
*/

gint
abs_comp_string( gconstpointer a, gconstpointer b)
{
	return strcmp(a, b);
}



int
abskeygen( char* pk, char*  pub_file, char*  msk_file, char** attribute, int num )
{
	mprabs_pub_t* pub;
	mprabs_msk_t* msk;
	mprabs_prv_t* prv;

	int i, n, p;

	//char*  pub_file = "/home/xx/test_file/abs_pub_key";
	//char*  msk_file = "/home/xx/test_file/abs_master_key";
	char** attrs    = 0;

	GSList* alist = NULL;
	GSList* ap;

	char*  out_file = pk;

	//printf("%s", attribute);

    for( p = 0; p < num; p++ )
    	abs_parse_attribute(&alist, attribute[p]);

	alist = g_slist_sort(alist, abs_comp_string);
	n = g_slist_length(alist);

	attrs = malloc((n + 1) * sizeof(char*));

	i = 0;
	for( ap = alist; ap; ap = ap->next )
	{
		attrs[i++] = ap->data;
		//printf("%s\n", attrs[i-1]);
	}
	attrs[i] = 0;

	//parse_args(argc, argv);

	pub = mprabs_pub_unserialize(abs_suck_file(pub_file), 1);
	msk = mprabs_msk_unserialize(pub, abs_suck_file(msk_file), 1);

	prv = mprabs_keygen(pub, msk, attrs);
	abs_spit_file(out_file, mprabs_prv_serialize(prv), 1);

	return 0;
}

/*void main()
{
    //char* attribute;
	
	char* attr[5];
	attr[0] = "security_team";
	attr[1] = "strategy_team";
	attr[2] = "executive_level";
	attr[3] = "business_staff";
	attr[4] = "hire_data";
	abskeygen( "/home/xx/test_file/sara_abs_priv_key","/home/xx/test_file/abs_pub_key", "/home/xx/test_file/abs_master_key", attr, 5);
	
}*/
