/*
	Include glib.h, pbc.h, and bswabe.h before including this file.
*/

struct mprabs_pub_s
{
	char* pairing_desc;
	pairing_t p;
	element_t g;           /* G_1 */
	//element_t h;           /* G_1 */
	element_t h0;			/* G_2 */
	element_t A0;			/* G_2 */
	element_t h[10];          /* G_2 */
	element_t A[10];		  /* G_2 */
	element_t B[10];		  /* G_2 */
	element_t C;		  /* G_1 */
	//element_t g_hat_alpha; /* G_T */
};

struct mprabs_msk_s
{
	element_t a0;    /* Z_r */
	element_t a;    /* Z_r */
	element_t b;    /* Z_r */
	//element_t g_alpha; /* G_2 */
};

typedef struct
{
	/* these actually get serialized */
	char* attr;
	element_t k;  /* G_1 */
	//element_t dp; /* G_2 */

	/* only used during dec (only by dec_merge) */
	//int used;
	//element_t z;  /* G_1 */
	//element_t zp; /* G_1 */
}
mprabs_prv_comp_t;

struct mprabs_prv_s
{
	element_t kbase;   /* G_1 */
	element_t k0;   /* G_1 */
	GArray* comps; /* bswabe_prv_comp_t's */  //attributes
};

typedef struct
{
	int k;           
	char* attr;       
}
mprabs_attribute_t;




typedef struct
{
	char* attr;
	element_t ss;  /* G_1 */
	
}
mprabs_sig_s_t;

typedef struct
{
	int j; //j shuyu t
	element_t pp;  /* G_1 */
	
}
mprabs_sig_p_t;

struct mprabs_sig_s
{
	element_t y; /* G_1 */
	element_t w;  /* G_1 */
	GArray* Si;
	GArray* Pj;
};

