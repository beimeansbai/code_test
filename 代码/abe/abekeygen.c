#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <glib.h>
#include <pbc.h>
#include <pbc_random.h>

#include "bswabe.h"
#include "common.h"
#include "policy_lang.h"
#include "abelib.h"

/*
	TODO ensure we don't give out the same attribute more than once (esp
	as different numerical values)
*/

gint
comp_string( gconstpointer a, gconstpointer b)
{
	return strcmp(a, b);
}



int
abekeygen( char* pk, char*  pub_file, char*  msk_file, char** attribute, int num )
{
	bswabe_pub_t* pub;
	bswabe_msk_t* msk;
	bswabe_prv_t* prv;

	int i, n, p;

	//char*  pub_file = "/home/xx/test_file/abe_pub_key";
	//char*  msk_file = "/home/xx/test_file/abe_master_key";
	char** attrs    = 0;

	GSList* alist;
	GSList* ap;

	char*  out_file = pk;

	//printf("%s", attribute);

    for( p = 0; p < num; p++ )
    	parse_attribute(&alist, attribute[p]);

	alist = g_slist_sort(alist, comp_string);
	n = g_slist_length(alist);

	attrs = malloc((n + 1) * sizeof(char*));

	i = 0;
	for( ap = alist; ap; ap = ap->next )
		attrs[i++] = ap->data;
	attrs[i] = 0;

	//parse_args(argc, argv);

	pub = bswabe_pub_unserialize(suck_file(pub_file), 1);
	msk = bswabe_msk_unserialize(pub, suck_file(msk_file), 1);

	prv = bswabe_keygen(pub, msk, attrs);
	spit_file(out_file, bswabe_prv_serialize(prv), 1);

	return 0;
}

/*void main()
{
    //char* attribute;
	
	char* attributes[4];
	attributes[0] = "A0";
	attributes[1] = "A6";
	attributes[2] = "A7";
	attributes[3] = "B3";
	abekeygen( "/home/xx/test_file/sara_abe_priv_key", "/home/xx/test_file/abe_pub_key", "/home/xx/test_file/abe_master_key",attributes, 4 );
	
}*/