#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#ifndef BSWABE_DEBUG
#define NDEBUG
#endif
#include <assert.h>

#include <openssl/sha.h>
#include <glib.h>
#include <pbc.h>

#include "mprabs.h"
#include "private.h"

#define TYPE_A_PARAMS \
"type a\n" \
"q 87807107996633125224377819847540498158068831994142082" \
"1102865339926647563088022295707862517942266222142315585" \
"8769582317459277713367317481324925129998224791\n" \
"h 12016012264891146079388821366740534204802954401251311" \
"822919615131047207289359704531102844802183906537786776\n" \
"r 730750818665451621361119245571504901405976559617\n" \
"exp2 159\n" \
"exp1 107\n" \
"sign1 1\n" \
"sign0 1\n"


#define AMAX 50

typedef struct ElementType
{
	int k;   //or = 1; and = 2
	char* att; //leaf node is attribute string, other node is null

} ElementType;

typedef struct TreeNode
{
	ElementType data;
	struct TreeNode *lchild, *rchild;
} TreeNode;

typedef struct TreeStack
{
	TreeNode *buf[AMAX];
	int n;
}TreeStack;

typedef struct VectorNode
{
	int type; //type of node or = 1, and = 2, att = 0
	int v[10];  //vector
	//char *attr;
} VectorNode;

typedef struct AttrNode
{
	char * att;
	int num;
} AttrNode;

//typedef struct AttrArray
//{
//	int length;
//	AttrNode attr[20];
//} AttrArry;

VectorNode vector[MAX];
AttrNode attribute[20];
int number = 0, length;      //number is the number of attribute leaf(hang), length means the length of v[](lie) 
char * ac_postfix[50];
char last_error[256];

char*
bswabe_error()
{
	return last_error;
}

void
raise_error(char* fmt, ...)
{
	va_list args;

#ifdef BSWABE_DEBUG
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	exit(1);
#else
	va_start(args, fmt);
	vsnprintf(last_error, 256, fmt, args);
	va_end(args);
#endif
}

void
element_from_string( element_t h, char* s )
{
	unsigned char* r;

	r = malloc(SHA_DIGEST_LENGTH);
	SHA1((unsigned char*) s, strlen(s), r);
	element_from_hash(h, r, SHA_DIGEST_LENGTH);

	free(r);
}



void
mprabs_setup( mprabs_pub_t** pub, mprabs_msk_t** msk )
{
	element_t c;
	int i, num = 10;

	/* initialize */
 
	*pub = malloc(sizeof(mprabs_pub_t));
	*msk = malloc(sizeof(mprabs_msk_t));

	(*pub)->pairing_desc = strdup(TYPE_A_PARAMS);
	pairing_init_set_buf((*pub)->p, (*pub)->pairing_desc, strlen((*pub)->pairing_desc));

	element_init_G1((*pub)->g,           (*pub)->p);
	element_init_G2((*pub)->h0,          (*pub)->p);
	element_init_G2((*pub)->A0,          (*pub)->p);
	//element_init_G1((*pub)->c,           (*pub)->p);
    for( i = 0; i < num; i++ )
    	element_init_G2((*pub)->h[i],          (*pub)->p);
	for( i = 0; i < num; i++ )
    	element_init_G2((*pub)->A[i],          (*pub)->p);
    for( i = 0; i < num; i++ )
    	element_init_G2((*pub)->B[i],          (*pub)->p);
	element_init_Zr(c,               (*pub)->p);
	element_init_Zr((*msk)->a0,        (*pub)->p);
	element_init_Zr((*msk)->a,        (*pub)->p);
	element_init_Zr((*msk)->b,        (*pub)->p);
	//element_init_G2((*msk)->g_alpha,     (*pub)->p);

	/* compute */

 	element_random((*msk)->a0);
 	element_random((*msk)->a);
 	element_random((*msk)->b);
 	element_random(c);
 	
	element_random((*pub)->g);
	element_random((*pub)->h0);


	for( i = 0; i < num; i++ )
    	element_random((*pub)->h[i]);
	//element_random((*pub)->gp);

    element_pow_zn((*pub)->A0, (*pub)->h0, (*msk)->a0);
    
    for( i = 0; i < num; i++ )
    {
    	element_pow_zn((*pub)->A[i], (*pub)->h[i], (*msk)->a);
    	element_pow_zn((*pub)->B[i], (*pub)->h[i], (*msk)->b);
    }
    element_pow_zn((*pub)->C, (*pub)->g, c);
	//element_pow_zn((*msk)->g_alpha, (*pub)->gp, alpha);
	//element_pow_zn((*pub)->h, (*pub)->g, (*msk)->beta);
  //pairing_apply((*pub)->g_hat_alpha, (*pub)->g, (*msk)->g_alpha, (*pub)->p);
}

mprabs_prv_t* mprabs_keygen( mprabs_pub_t* pub,	 mprabs_msk_t* msk, char** attributes )
{
	mprabs_prv_t* prv;
	element_t g_r;
	element_t r;
	element_t a0_inv;

	/* initialize */

	prv = malloc(sizeof(mprabs_prv_t));

	element_init_G1(prv->kbase, pub->p);
	element_init_G1(prv->k0, pub->p);
	//element_init_G2(prv->d, pub->p);
	//element_init_G2(g_r, pub->p);
	//element_init_Zr(r, pub->p);
	//element_init_Zr(beta_inv, pub->p);

	prv->comps = g_array_new(0, 1, sizeof(mprabs_prv_comp_t));

	/* compute */

	element_random(prv->kbase);
	element_invert(a0_inv, msk->a0);
 	element_pow_zn(prv->k0, prv->kbase, a0_inv);

 	
	//element_mul(prv->d, msk->g_alpha, g_r);
	//element_invert(beta_inv, msk->beta);
	//element_pow_zn(prv->d, prv->d, beta_inv);

	while( *attributes )
	{
		mprabs_prv_comp_t c;
		element_t u;
		element_t rp;
		element_t rp_inv;

		c.attr = *(attributes++);

		element_init_Zr(rp,        pub->p);
		element_init_Zr(rp_inv,        pub->p);
		element_init_Zr(u,        pub->p);
	
		
 		element_from_string(u, c.attr);   //SHA1 HASH
 		element_mul(rp, msk->b, u);
 		element_add(rp, rp, msk->a);
 		element_invert(rp_inv, rp);

		element_pow_zn(c.k, prv->kbase, rp_inv);

		element_clear(u);
		element_clear(rp);
		element_clear(rp_inv);

		g_array_append_val(prv->comps, c);
	}

	return prv;
}

TreeStack* creat_empty_stack()
{
	TreeStack* pstack;

	pstack =  (TreeStack *)malloc(sizeof(TreeStack));  
    pstack->n = -1;  
  
    return pstack;  
} 

/* 入栈 */ 
int  push_stack(TreeStack *p, TreeNode *data)  
{  
    p->n++;  
    p->buf[p->n] = data;  
  
    return 0;  
}  
  
/* 出栈 */  
TreeNode *pop_stack(TreeStack *p)  
{  
    TreeNode *data;  
  
    data = p->buf[p->n];  
    p->n --;  
  
    return data;  
}  
  
/* 判断栈空 */  
int is_empty_stack(TreeStack *p)  
{  
    if(p->n == -1)  
        return 1;  
    else  
        return 0;  
}  

/* 创建后缀表达式树 */  
TreeNode* create_access_tree(char** policy, TreeStack* p)
{
	 int i = 0;  
    TreeNode *current;  
    TreeNode *left, *right;  
    //current = (TreeNode *)malloc(sizeof(TreeNode));
  
    while(policy[i])  
    {  
        if(policy[i] == "and"|| policy[i] == "or")  
        {  
            current = (TreeNode *)malloc(sizeof(TreeNode)); 
            if(policy[i] == "and")
            	current->data.k = 2;  
            else
            	current->data.k = 1;
            current->data.att = "\0";
            right = pop_stack(p);  
            left = pop_stack(p);  
            current->lchild = left;  
            current->rchild = right;  
            push_stack(p, current);  
        }  
        else  
        {  
            current = (TreeNode *)malloc(sizeof(TreeNode));  
            current->data.att = policy[i]; 
            current->data.k = 0;  
            current->lchild = NULL;  
            current->rchild = NULL;  
            push_stack(p, current);


        }  
        i++;  
    }  
    return p->buf[p->n];  


}

void infix_to_postfix( char* s )
{
	char symbol[50] = {0}, att[20] = {0};
	int i, p, n = 0, m = 0;
	int len;
	//char * ac_postfix[50];
	char * and = "and"; 
	char * or = "or";

	//ac_postfix = (char **)malloc(sizeof(char*) * 50);  

	len = strlen(s);

	for(i = 0; i < len; i++)
	{
		if(s[i] == '(')
		{
			symbol[n] = s[i];
			n++;
		}
		else if(s[i] == ')')
		{
			while(((symbol[n-1] == '1') || (symbol[n-1] == '2')) && n > 0 )
			{
				if(symbol[n-1] == '1')
					ac_postfix[m++] = or;
				if(symbol[n-1] == '2')
					ac_postfix[m++] = and;
				n--;
			}
			if(symbol[n-1] == '(')
				symbol[--n] = '\0';
		}
		else if(s[i] == 'o' && s[i+1] == 'r' )
		{
			symbol[n++] = '1';
			i++;
		}
		else if(s[i] == 'a' && s[i+1] == 'n' && s[i+2] == 'd' )
		{
			symbol[n++] = '2';
			i = i + 2;
		}
		else if(s[i] == ' ') ;
		else
		{
			p = 0;
			while(s[i] != ' ' && s[i] != ')')
			{
				att[p++] = s[i];
				i++;
			}
			att[p] = '\0';
			ac_postfix[m] = (char *)malloc(sizeof(char )*20);
			memcpy(ac_postfix[m], att, sizeof(att));
			//ac_postfix[m++] = att;	
			m++;
			i--;
			
		}
    
	}
	if (symbol[n-1] == '1')
		ac_postfix[m] = or;
	else
		ac_postfix[m] = and;
	ac_postfix[++m] = '\0';
	
	//return ac_postfix;
}

int* copy_string(int * a, int * b, int l)
{
	int j;
	for(j = 0; j < l; j++)
		a[j]= b[j];

	return a;

}

int preorder_traversal(TreeNode * p, int num, int * v_parent, int len, int* type ) //type[0]and/or type[1] left/right
{
	int t[2], i, j, l;
	int parent[10] = {0}; 
	l = len;
	copy_string(parent, v_parent, 10);
	if(p)
	{
		vector[num].type = p->data.k;
		t[0] = p->data.k;
		if(type[0] == 1)   //parent = or
		{
			copy_string(vector[num].v, parent, 10);
			//for(j = 0; j < 10; j++)
			//	vector[num].v[j]= v_parent[j];
			
		}
		else if(type[0] == 2 && type[1] == 1)  //parent = and leftchild
		{
			copy_string(vector[num].v, parent, 10);
            vector[num].v[l] = 1;
            l++;
            if(l > length)
            	length++;
		}
		else   //parent = and rightchild
		{
			//for(i = 0; i < c; i++)
				vector[num].v[0] = 0;
			vector[num].v[1] = -1;
			//c++; 
		}
		//vector[num]->attr = p->data->att;
		if(p->data.k == 0)
		{
			attribute[number].att = p->data.att;
			attribute[number].num = num;
			number++;
		}
		//for(i = len; i < 10; i++)
		//	vector[num].v[i] = 0;
		copy_string(parent, vector[num].v, 10);
		num++;
		//for(j = 0; j < 10; j++)
		//	v_parent[j] = vector[num].v[j];
		t[1] = 1;
		num = preorder_traversal(p->lchild, num, parent, l, t);
		t[1] = 2;
		num = preorder_traversal(p->rchild, num, parent, l, t);
	}
	return num;
}
/*create the span program matrix*/
int** create_span_program(TreeNode * root)
{
	//VectorNode * m, s;
	int n, m;
	TreeNode * t = root;
	int v_parent[10] = {0};
	int i, j, type[2] = {1,1};  //The default root node is or,so type[0]=1

	//m = (VectorNode *)malloc(sizeof(VectorNode) * MAX);
	//v_parent = (int *)malloc(sizeof(int) * 10);
	//c = 0;
	v_parent[0] = 1;
	length = 1;
	int l = 1;

	//add false warn
	if( root = NULL)
		return NULL;
	else
	{		
		if(t->data.k == 1)
		{
			vector[0].type = 1;
			copy_string(vector[0].v, v_parent, 10);

		}
		n = preorder_traversal(t->lchild, 1, v_parent, l, type);
		m = preorder_traversal(t->rchild, n, v_parent, l, type);
		
		int **matrix;
		matrix = (int **)malloc(sizeof(int *)*number);
		for(i = 0; i < number; i++)
		{
			matrix[i] = (int *)malloc(sizeof(int)*length);
			for(j = 0; j < length; j++)
			{
				if(vector[attribute[i].num].v[j] == 0 || vector[attribute[i].num].v[j] == 1 || vector[attribute[i].num].v[j] == -1)
					matrix[i][j] = vector[attribute[i].num].v[j];
				else 
					matrix[i][j] = 0;
			}
		}	
	//	for(i = 0; i < number; i++)
	//{
	//	for(j = 0; j < length; j++)
	//		printf("%d  ", matrix[i][j]);
	//	printf("\n");
	//}
		return matrix;

	}


}

//char* parse_policy( char* s )
int** parse_policy (char* ac)
{
	//char* parsed_policy[50]; 
	TreeNode* T;
	TreeStack* p;
	
	//int i, j;
	//char* s = "((a1 and a2) or a3) or (a4 and (a5 or a6))";


    number = 0;

	//parsed_policy = (char **)malloc(sizeof(char*) * 50);

	infix_to_postfix(ac);

	T = (TreeNode *)malloc(sizeof(TreeNode));

	p = creat_empty_stack();

	T = create_access_tree(ac_postfix, p);

	int **m = create_span_program(T);

	return m;
	//int mm[number][length];

	//for(i = 0; i < number; i++)
	//	for(j = 0; j < length; j++)
	//		mm[i][j] = m[i][j];
		

	//for(i = 0; i < number; i++)
	//{
	//	for(j = 0; j < length; j++)
	//		printf("%d  ", m[i][j]);
	//	printf("\n");
	//}

	//return parsed_policy;
}

GArray* attribute_row(char* ac)
{
	TreeNode* T;
	TreeStack* p;
	number = 0;
	GArray* a;
	int i;
	mprabs_attribute_t at;

	//parsed_policy = (char **)malloc(sizeof(char*) * 50);
	a = g_array_new(0, 1, sizeof(mprabs_attribute_t));

	infix_to_postfix(ac);

	T = (TreeNode *)malloc(sizeof(TreeNode));

	p = creat_empty_stack();

	T = create_access_tree(ac_postfix, p);

	//int **m = create_span_program(T);
	
	for (i = 0; i < number; i++)
	{
		at.attr = attribute[i].att;
		at.k = i;
		g_ptr_array_add(a, at);
	}
	

	return a;
    
}

int* l_and_t(char* ac)
{
	TreeNode* T;
	TreeStack* p;
	number = 0;
	int a[2];

	//parsed_policy = (char **)malloc(sizeof(char*) * 50);

	infix_to_postfix(ac);

	T = (TreeNode *)malloc(sizeof(TreeNode));

	p = creat_empty_stack();

	T = create_access_tree(ac_postfix, p);

	a[0] = number;
	a[1] = length;

	return a;
}

char* policy_postfix(char * ac)
{
	char* p;
	infix_to_postfix(ac);

	int i = 0;

	while(ac_postfix[i] != '\0')
	{
		p[i] = ac_postfix[i];
		i++;
	}
	p[i] = '\0';

	return p;

}

mprabs_sig_t*
mprabs_sig( mprabs_pub_t* pub, mprabs_prv_t* prv, GByteArray* m, char* policy, int ** M)
{
	mprabs_sig_t* sig;
 	element_t hash;
 	char* policy_p;
 	char * message_hash;
	int i, j, n;
	element_t r0;
	GArray* a;
	int num*;


	sig = malloc(sizeof(mprabs_sig_t));
	message_hash = (char *) malloc(m->len + strlen(policy_p));

	sig->s = g_array_new(0, 1, sizeof(mprabs_sig_s_t));
	sig->p = g_array_new(0, 1, sizeof(mprabs_sig_p_t));


	policy_p = policy_postfix(policy);
	strcpy(message_hash, m->data);
    strcat(message_hash, policy_p);

    num = l_and_t(policy);

    /*compute the v of the user*/
    a = attribute_row(policy);
    element_t r[num[0]];
    element_t u[num[0]];
    int v[a.len] = {0};
   
    element_init_Zr(r0, pub->p);
    element_init_G2(hash, pub->p);

    /*compute*/
    element_from_string(hash, message_hash);  //HASH SHA1
    element_random(r0);
    element_pow_zn(sig->y, prv->kbase, r0);
    element_pow_zn(sig->w, prv->k0, r0);
    
    for(i = 0; i < num[0]; i++)
    {
    	mprabs_sig_s_t S;
		element_t K;
		element_t u;
		//element_t ri;
		j = 0;

		

		S.attr = a.data[i];

		element_init_Zr(K,        pub->p);
		element_init_Zr(u,        pub->p);
	
		element_random(r[i]);
		element_pow_zn(u, pub->g, hash);
		element_mul(u, u, pub->C);
		element_pow_zn(u, u, r[i]);

		for(n = 0; n < prv->comps.len; n++)
		{
			if(prv->comps[j]->att == a->data[i]->attr)
			{
				v[i] = 1;
				break;				
			}

			
		}
		element_pow_zn(K, prv->comps[j]->k, v[i]);
		element_pow_zn(K, K, r0);
		element_mul(S.s, K, u);
		
 		element_clear(K);
		element_clear(u);
		//element_clear(ri);

		g_array_append_val(sig->s, S);
    }

    

    for(i = 0; i < num[0]; i++)  
    {
    	element_init_G2(u[i], pub->p);
    	element_from_string(u[i], a->data[i].attr);  //HASH SHA1

    }

    mprabs_sig_p_t P;
    element_t ab;
    element_t mij;

    element_init_G2(ab, pub->p);
    element_init_Zr(mij, pub->p);

    for(j = 0; j< num[1]; j++)
    {
    	for(i = 0; i < num[0]; i++)
    	{
    		element_pow_zn(ab, pub->B[j], u[i]);
    		element_mul(ab, ab, pub->A[j]);
    		element_mul(mij, M[i][j], r[i]);
    		element_pow_zn(ab, ab, mij);
    		if(i == 0) P.p = ab;
    		else
    			element_mul(P.p, P.p, ab);

    	}

    	element_clear(ab);
		element_clear(mij);

    	g_array_append_val(sig->p, P);

    
    }
	
	return sig;
}


int
mprabs_verify( mprabs_pub_t* pub, mprabs_sig_t* sig, GByteArray* m, char* policy, int ** M)
{
	//mprabs_sig_t* sig;
 	element_t hash;
 	char* policy_p;
 	char * message_hash;
	int i, j;
	int num[2];
	element_t e1;
	element_t e2;
	element_t e3; 
	element_t e4;
	element_t e5;
	element_t e6;
	element_t u;
	element_t ab;
    element_t mij;

    element_init_G2(ab, pub->p);
    element_init_Zr(mij, pub->p);

	/* compute the H(m|Tac)*/
	message_hash = (char *) malloc(m.len + strlen(policy_p));
	element_init_G2(hash, pub->p);
	


	policy_p = policy_postfix(policy);
	strcpy(message_hash, m.data);
    strcat(message_hash, policy_p);

    element_from_string(hash, message_hash);
    num = l_and_t(policy);

  	if(sig->y == 1) return false;

  	element_init_GT(e1, pub->p);
  	element_init_GT(e2, pub->p);

  	pairing_apply(e1, sig->w, pub->A0, pub->p);  //计算w和A0的配对，结果放在e1中
  	pairing_apply(e2, sig->y, pub->h0, pub->p);  //

  	if(element_cmp(e1, e2)) return false;

  	element_t u[num[0]]; 

    for(i = 0; i < num[0]; i++)  
    {
    	element_init_G2(u[i], pub->p);
    	element_from_string(u[i], a.pdata[i]);  //HASH SHA1

    }


	
	element_init_G1(u, pub->p);
	
	element_pow_zn(u, pub->g, hash);
	element_mul(u, u, pub->C);
	

    j = 0;
    element_init_GT(e3, pub->p);
    element_init_GT(e4, pub->p);
    element_init_GT(e5, pub->p);
    element_init_GT(e6, pub->p);

    pairing_apply(e3, sig->y, pub->h[0], pub->p);  
    pairing_apply(e4, u, sig->p[0], pub->p);
    element_mul(e3, e3, e4);

    //element_clear(e4);

  	for(i = 0; i < num[0]; i++)
  	{
  		element_pow_zn(ab, pub->B[j], u[i]);
    	element_mul(ab, ab, pub->A[j]);
    	element_pow_zn(ab, ab, M[i][j]);
    	pairing_apply(e6, sig->s[i], ab, pub->p); 
    	if(i == 0) 
    		e5 = e6;
    	else
    		element_mul(e5, e5, e6);
    }

    if(element_cmp(e3, e5)) return false;

    for(j = 1; j < num[1]; j++)
    {
    	pairing_apply(e4, u, sig->p[j], pub->p); 
    	for(i = 0; i < num[0]; i++)
    	{
    		element_pow_zn(ab, pub->B[j], u[i]);
    	    element_mul(ab, ab, pub->A[j]);
    	    element_pow_zn(ab, ab, M[i][j]);
    	    pairing_apply(e6, sig->s[i], ab, pub->p); 
    	    if(i == 0) 
    	        e5 = e6;
            else
    	        element_mul(e5, e5, e6);
    	}
    	if(element_cmp(e4, e5)) return false;
    }

    return true;
}