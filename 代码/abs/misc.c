#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <pbc.h>


#include "mprabs.h"
#include "private.h"

void
abs_serialize_uint32( GByteArray* b, uint32_t k )
{
	int i;
	guint8 byte;

	for( i = 3; i >= 0; i-- )
	{
		byte = (k & 0xff<<(i*8))>>(i*8);
		g_byte_array_append(b, &byte, 1);
	}
}

uint32_t
abs_unserialize_uint32( GByteArray* b, int* offset )
{
	int i;
	uint32_t r;

	r = 0;
	for( i = 3; i >= 0; i-- )
		r |= (b->data[(*offset)++])<<(i*8);

	return r;
}

void
abs_serialize_element( GByteArray* b, element_t e )
{
	uint32_t len;
	unsigned char* buf;

	len = element_length_in_bytes(e);
	abs_serialize_uint32(b, len);

	buf = (unsigned char*) malloc(len);
	element_to_bytes(buf, e);
	g_byte_array_append(b, buf, len);
	free(buf);
}

void
abs_unserialize_element( GByteArray* b, int* offset, element_t e )
{
	uint32_t len;
	unsigned char* buf;

	len = abs_unserialize_uint32(b, offset);

	buf = (unsigned char*) malloc(len);
	memcpy(buf, b->data + *offset, len);
	*offset += len;

	element_from_bytes(e, buf);
	free(buf);
}

void
abs_serialize_string( GByteArray* b, char* s )
{
	g_byte_array_append(b, (unsigned char*) s, strlen(s) + 1);
}

char*
abs_unserialize_string( GByteArray* b, int* offset )
{
	GString* s;
	char* r;
	char c;

	s = g_string_sized_new(32);
	while( 1 )
	{
		c = b->data[(*offset)++];
		if( c && c != EOF )
			g_string_append_c(s, c);
		else
			break;
	}

	r = s->str;
	g_string_free(s, 0);

	return r;
}

GByteArray*
mprabs_pub_serialize( mprabs_pub_t* pub )
{
	GByteArray* b;
	int i;

	b = g_byte_array_new();
	abs_serialize_string(b,  pub->pairing_desc);
	//serialize_element(b, pub->p);
	abs_serialize_element(b, pub->g);
	abs_serialize_element(b, pub->h0);
	abs_serialize_element(b, pub->A0);
	for(i = 0; i < 10; i++)
		abs_serialize_element(b, pub->h[i]);
	for(i = 0; i < 10; i++)
		abs_serialize_element(b, pub->A[i]);
	for(i = 0; i < 10; i++)
		abs_serialize_element(b, pub->B[i]);
	abs_serialize_element(b, pub->C);
	return b;
}

mprabs_pub_t*
mprabs_pub_unserialize( GByteArray* b, int free )
{
	mprabs_pub_t* pub;
	int offset;
	int i;

	pub = (mprabs_pub_t*) malloc(sizeof(mprabs_pub_t));
	offset = 0;

	pub->pairing_desc = abs_unserialize_string(b, &offset);
	pairing_init_set_buf(pub->p, pub->pairing_desc, strlen(pub->pairing_desc));

	element_init_G1(pub->g,           pub->p);
	element_init_G2(pub->h0,           pub->p);
	element_init_G2(pub->A0,          pub->p);
	for(i = 0; i < 10; i++)
		element_init_G2(pub->h[i], pub->p);
	for(i = 0; i < 10; i++)
		element_init_G2(pub->A[i], pub->p);
	for(i = 0; i < 10; i++)
		element_init_G2(pub->B[i], pub->p);
	element_init_G1(pub->C, pub->p);

	abs_unserialize_element(b, &offset, pub->g);
	abs_unserialize_element(b, &offset, pub->h0);
	abs_unserialize_element(b, &offset, pub->A0);
	for(i = 0; i < 10; i++)
		abs_unserialize_element(b, &offset, pub->h[i]);
	for(i = 0; i < 10; i++)
		abs_unserialize_element(b, &offset, pub->A[i]);
	for(i = 0; i < 10; i++)
		abs_unserialize_element(b, &offset, pub->B[i]);
	abs_unserialize_element(b, &offset, pub->C);

	if( free )
		g_byte_array_free(b, 1);

	return pub;
}

GByteArray*
mprabs_msk_serialize( mprabs_msk_t* msk )
{
	GByteArray* b;

	b = g_byte_array_new();
	abs_serialize_element(b, msk->a0);
	abs_serialize_element(b, msk->a);
	abs_serialize_element(b, msk->b);

	return b;
}

mprabs_msk_t*
mprabs_msk_unserialize( mprabs_pub_t* pub, GByteArray* b, int free )
{
	mprabs_msk_t* msk;
	int offset;

	msk = (mprabs_msk_t*) malloc(sizeof(mprabs_msk_t));
	offset = 0;

	element_init_Zr(msk->a0, pub->p);
	element_init_Zr(msk->a, pub->p);
	element_init_Zr(msk->b, pub->p);
	

	abs_unserialize_element(b, &offset, msk->a0);
	abs_unserialize_element(b, &offset, msk->a);
	abs_unserialize_element(b, &offset, msk->b);

	if( free )
		g_byte_array_free(b, 1);

	return msk;
}

GByteArray*
mprabs_prv_serialize( mprabs_prv_t* prv )
{
	GByteArray* b;
	int i;

	b = g_byte_array_new();

	abs_serialize_element(b, prv->kbase);
	abs_serialize_element(b, prv->k0);
	abs_serialize_uint32( b, prv->comps->len);

	for( i = 0; i < prv->comps->len; i++ )
	{
		abs_serialize_string( b, g_array_index(prv->comps, mprabs_prv_comp_t, i).attr);
		abs_serialize_element(b, g_array_index(prv->comps, mprabs_prv_comp_t, i).k);
		
	}

	return b;
}

mprabs_prv_t*
mprabs_prv_unserialize( mprabs_pub_t* pub, GByteArray* b, int free )
{
	mprabs_prv_t* prv;
	int i;
	int len;
	int offset;

	prv = (mprabs_prv_t*) malloc(sizeof(mprabs_prv_t));
	offset = 0;

	element_init_G2(prv->kbase, pub->p);
	element_init_G2(prv->k0, pub->p);
	abs_unserialize_element(b, &offset, prv->kbase);
	abs_unserialize_element(b, &offset, prv->k0);

	prv->comps = g_array_new(0, 1, sizeof(mprabs_prv_comp_t));
	len = abs_unserialize_uint32(b, &offset);

	for( i = 0; i < len; i++ )
	{
		mprabs_prv_comp_t c;

		c.attr = abs_unserialize_string(b, &offset);

		element_init_G1(c.k,  pub->p);
		

		abs_unserialize_element(b, &offset, c.k);
		

		g_array_append_val(prv->comps, c);
	}

	if( free )
		g_byte_array_free(b, 1);

	return prv;
}

/*void
serialize_policy( GByteArray* b, bswabe_policy_t* p )
{
	int i;

	serialize_uint32(b, (uint32_t) p->k);

	serialize_uint32(b, (uint32_t) p->children->len);
	if( p->children->len == 0 )
	{
		serialize_string( b, p->attr);
		serialize_element(b, p->c);
		serialize_element(b, p->cp);
	}
	else
		for( i = 0; i < p->children->len; i++ )
			serialize_policy(b, g_ptr_array_index(p->children, i));
}

bswabe_policy_t*
unserialize_policy( bswabe_pub_t* pub, GByteArray* b, int* offset )
{
	int i;
	int n;
	bswabe_policy_t* p;

	p = (bswabe_policy_t*) malloc(sizeof(bswabe_policy_t));

	p->k = (int) unserialize_uint32(b, offset);
	p->attr = 0;
	p->children = g_ptr_array_new();

	n = unserialize_uint32(b, offset);
	if( n == 0 )
	{
		p->attr = unserialize_string(b, offset);
		element_init_G1(p->c,  pub->p);
		element_init_G1(p->cp, pub->p);
		unserialize_element(b, offset, p->c);
		unserialize_element(b, offset, p->cp);
	}
	else
		for( i = 0; i < n; i++ )
			g_ptr_array_add(p->children, unserialize_policy(pub, b, offset));

	return p;
}*/



GByteArray*
mprabs_sig_serialize( mprabs_sig_t* sig )
{
	GByteArray* b;
	int i;

	b = g_byte_array_new();
	abs_serialize_element(b, sig->y);
	abs_serialize_element(b, sig->w);
	abs_serialize_uint32( b, sig->Si->len);
	abs_serialize_uint32( b, sig->Pj->len);

	for( i = 0; i < sig->Si->len; i++ )
	{
		abs_serialize_string( b, g_array_index(sig->Si, mprabs_sig_s_t, i).attr);
		abs_serialize_element(b, g_array_index(sig->Si, mprabs_sig_s_t, i).ss);
		
	}

	for( i = 0; i < sig->Pj->len; i++ )
	{
		abs_serialize_uint32( b, g_array_index(sig->Pj, mprabs_sig_p_t, i).j);
		abs_serialize_element(b, g_array_index(sig->Pj, mprabs_sig_p_t, i).pp);
		
	}

	return b;
}

mprabs_sig_t*
mprabs_sig_unserialize( mprabs_pub_t* pub, GByteArray* b, int free )
{
	mprabs_sig_t* sig;
	int offset;
	int n, m, i;

	sig = (mprabs_sig_t*) malloc(sizeof(mprabs_sig_t));
	offset = 0;

	element_init_G1(sig->y, pub->p);
	element_init_G1(sig->w,  pub->p);
	abs_unserialize_element(b, &offset, sig->y);
	abs_unserialize_element(b, &offset, sig->w);

	n = abs_unserialize_uint32(b, &offset);
	m = abs_unserialize_uint32(b, &offset);
	

	sig->Si = g_array_new(0, 1, sizeof(mprabs_sig_s_t));
	sig->Pj = g_array_new(0, 1, sizeof(mprabs_sig_p_t));

	for( i = 0; i < n; i++ )
	{
		mprabs_sig_s_t s1;

		s1.attr = abs_unserialize_string(b, &offset);

		element_init_G1(s1.ss,  pub->p);
		

		abs_unserialize_element(b, &offset, s1.ss);
		

		g_array_append_val(sig->Si, s1);
	}

	for( i = 0; i < m; i++ )
	{
		mprabs_sig_p_t p1;

		p1.j = (int) abs_unserialize_uint32(b, &offset);

		element_init_G1(p1.pp,  pub->p);
		

		abs_unserialize_element(b, &offset, p1.pp);
		

		g_array_append_val(sig->Pj, p1);
	}


	if( free )
		g_byte_array_free(b, 1);

	return sig;
}

void
mprabs_pub_free( mprabs_pub_t* pub )
{
	int i;
	element_clear(pub->g);
	element_clear(pub->h0);
	element_clear(pub->A0);
 	for(i = 0; i < 10; i++)
 		element_clear(pub->h[i]);
 	for(i = 0; i < 10; i++)
 		element_clear(pub->A[i]);
 	for(i = 0; i < 10; i++)
 		element_clear(pub->B[i]);
 	pairing_clear(pub->p);
	free(pub->pairing_desc);
	free(pub);
}

void
mprabs_msk_free( mprabs_msk_t* msk )
{
	element_clear(msk->a0);
	element_clear(msk->a);
	element_clear(msk->b);
	free(msk);
}

void
mprabs_prv_free( mprabs_prv_t* prv )
{
	int i;
	
	element_clear(prv->kbase);
	element_clear(prv->k0);

	for( i = 0; i < prv->comps->len; i++ )
	{
		mprabs_prv_comp_t c;

		c = g_array_index(prv->comps, mprabs_prv_comp_t, i);
		free(c.attr);
		element_clear(c.k);
		
	}

	g_array_free(prv->comps, 1);

	free(prv);
}

/*void
mprabs_policy_free( mprabs_policy_t* p )
{
	int i;

	if( p->attr )
	{
		free(p->attr);
		element_clear(p->c);
		element_clear(p->cp);
	}

	for( i = 0; i < p->children->len; i++ )
		bswabe_policy_free(g_ptr_array_index(p->children, i));

	g_ptr_array_free(p->children, 1);

	free(p);
}*/

void
mprabs_sig_free( mprabs_sig_t* sig )
{
	element_clear(sig->y);
	element_clear(sig->w);
	
	int i;
	
	for( i = 0; i < sig->Si->len; i++ )
	{
		mprabs_sig_s_t s1;

		s1 = g_array_index(sig->Si, mprabs_sig_s_t, i);
		free(s1.attr);
		element_clear(s1.ss);
		
	}

	for( i = 0; i < sig->Pj->len; i++ )
	{
		mprabs_sig_p_t p1;

		p1 = g_array_index(sig->Pj, mprabs_sig_p_t, i);
		//free(&(p1.j));
		element_clear(p1.pp);
		
	}

	g_array_free(sig->Si, 1);
	g_array_free(sig->Pj, 1);


	free(sig);
}
