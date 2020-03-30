#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#ifndef MPRABS_DEBUG
#define NDEBUG
#endif
#include <assert.h>

#include <openssl/sha.h>
#include <glib.h>
#include <pbc.h>

#include "mprabs.h"
#include "private.h"
#include "common.h"

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

typedef struct TreeNode     //树节点
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

//ypedef struct Queue
//{
//	TreeNode data[AMAX];
//	int front;
//	int rear;
//} Queue;

VectorNode vector[AMAX];
AttrNode attribute[20];
int number = 0, length;      //number是属性叶节点的数量(行), length是v[]的长度(列) 
char * ac_postfix[50];
char last_error[256];


char*
mprabs_error()
{
	return last_error;
}

void
abs_raise_error(char* fmt, ...)
{
	va_list args;

#ifdef MPRABS_DEBUG
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

/*char* strcpy1(char* des, const char* source, int len)
{
	char* r=des;
   	int i;
    assert((des != NULL) && (source != NULL));
 	for(i = 0; i < len; i++)
 	{
		r[i] = source[i];
		//printf("source[i] = %c\n", source[i]);
 	}

	r[len] = '\0';
	//printf("%s\n", r);
    return r;
}*/

void
abs_element_from_string( element_t h, char* s)
{
	unsigned char* r;

	
	r = malloc(SHA_DIGEST_LENGTH);
	SHA1((unsigned char*) s, strlen(s), r);
	element_from_hash(h, r, SHA_DIGEST_LENGTH);

	free(r);
}


/*ABS初始化函数*/
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
	element_init_G1((*pub)->C,          (*pub)->p);
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
	
}

/*ABS密钥生成函数*/
mprabs_prv_t* mprabs_keygen( mprabs_pub_t* pub,	 mprabs_msk_t* msk, char** attributes )
{
	mprabs_prv_t* prv;
	//element_t g_r;
	//element_t r;
	element_t a0_inv;

	/* initialize */

	prv = malloc(sizeof(mprabs_prv_t));

	element_init_G1(prv->kbase, pub->p);
	element_init_G1(prv->k0, pub->p);
	element_init_Zr(a0_inv, pub->p);
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
		element_init_G1(c.k,        pub->p);
	
		
 		abs_element_from_string(u, c.attr);   //SHA1 HASH
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

char*
abs_bit_marker( char* base, char* tplate, int bit, char val )
{
	char* lx;
	char* rx;
	char* s;

 	lx = g_strnfill(64 - bit - 1, 'x');
	rx = g_strnfill(bit, 'x');
	s = g_strdup_printf(tplate, base, lx, !!val, rx);
	free(lx);
	free(rx);

	return s;
}

/*属性解析函数，可解析age = 5、2<age<7这种类型的属性（abe原代码）*/
void
abs_parse_attribute( GSList** l, char* a )
{
	if( !strchr(a, '=') )
		*l = g_slist_append(*l, a);
	else
	{
		int i;
		char* s;
		char* tplate;
		uint64_t value;
		int bits;

		s = malloc(sizeof(char) * strlen(a));

		if( sscanf(a, " %s = %lu # %u ", s, &value, &bits) == 3 )
		{
			/* expint */

			if( bits > 64 )
				abs_die("error parsing attribute \"%s\": 64 bits is the maximum allowed\n",
						a, value, bits);

			if( value >= ((uint64_t)1<<bits) )
				abs_die("error parsing attribute \"%s\": value %lu too big for %d bits\n",
						a, value, bits);

			tplate = g_strdup_printf("%%s_expint%02d_%%s%%d%%s", bits);
			for( i = 0; i < bits; i++ )
				*l = g_slist_append
					(*l, abs_bit_marker(s, tplate, i, !!((uint64_t)1<<i & value)));
			free(tplate);

			*l = g_slist_append
				(*l, g_strdup_printf("%s_expint%02d_%lu", s, bits, value));
		}
		else if( sscanf(a, " %s = %lu ", s, &value) == 2 )
		{
			/* flexint */

			for( i = 2; i <= 32; i *= 2 )
				*l = g_slist_append
					(*l, g_strdup_printf
					 (value < ((uint64_t)1<<i) ? "%s_lt_2^%02d" : "%s_ge_2^%02d", s, i));

			for( i = 0; i < 64; i++ )
				*l = g_slist_append
					(*l, abs_bit_marker(s, "%s_flexint_%s%d%s", i, !!((uint64_t)1<<i & value)));

			*l = g_slist_append
				(*l, g_strdup_printf("%s_flexint_%lu", s, value));
		}
		else
			abs_die("error parsing attribute \"%s\"\n"
					"(note that numerical attributes are unsigned integers)\n",	a);

 		free(s);
	}	
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
			while(s[i] != ' ' && s[i] != ')' && s[i] != '\0')
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
	//for(i = 0; i < m; i++)
	//{
	//	printf("%s", ac_postfix[i]);
	//}
	
	//return ac_postfix;
}

int* copy_string(int * a, int * b, int l)
{
	int j;
	for(j = 0; j < l; j++)
		a[j]= b[j];

	return a;

}



int preorder_traversal(TreeNode * p, int num, int * v_parent, int len, int* type, int c ) //type[0]and/or type[1] left/right
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
			for(i = 0; i < c; i++)
            	vector[num].v[i] = 0;
			vector[num].v[c] = -1;
			c++; 
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
		num = preorder_traversal(p->lchild, num, parent, l, t, c);
		t[1] = 2;
		l = length;
		num = preorder_traversal(p->rchild, num, parent, l, t, c);
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
	int c[2] = {1, 1};
	//add false warn
	if( root = NULL)
		return NULL;
	else
	{		
		if(t->data.k == 1)
		{
			vector[0].type = 1;
			copy_string(vector[0].v, v_parent, 10);
			n = preorder_traversal(t->lchild, 1, v_parent, l, type, c[0]);
			m = preorder_traversal(t->rchild, n, v_parent, l, type, c[1]);

		}
		if(t->data.k == 2)
		{
			vector[0].type = 2;
			copy_string(vector[0].v, v_parent, 10);
			type[0] = 2;
			type[1] = 1;
			n = preorder_traversal(t->lchild, 1, v_parent, l, type, c[0]);
			type[1] = 2;
			m = preorder_traversal(t->rchild, n, v_parent, l, type, c[1]);
		}
		
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

	
	//int mm[number][length];

	//for(i = 0; i < number; i++)
	//	for(j = 0; j < length; j++)
	//		mm[i][j] = m[i][j];
		
	/*int i, j;
	for(i = 0; i < number; i++)
	{
		for(j = 0; j < length; j++)
			printf("%d  ", m[i][j]);
		printf("\n");
	}*/
	return m;
	//return parsed_policy;
}

int length_char(char*s){ 
	int i; 
	for(i = 0; s[i] != '\0'; i++); // 注意 不等号
	return i;
}

/*确认属性向量v是否满足矩阵*/
int mul_matrix_satisfy(int* v, int ** M, int l, int w)
{
	int i, j;
	int r = 0;

	for(j = 0; j < w; j++)
	{
		for(i = 0; i < l; i++)
		{
			r = r + v[i] * M[i][j];
		}
		if(j == 0 && r != 1) return 0;
		if(j > 0 && r != 0) return 0;
		r = 0;
	}
	return 1;
}

void memcpy1(int * a, int * b, int n)
{
	for(int i = 0; i < n; i++)
		a[i] = b[i];
}

int get_combinations(int *a, int n, int m, int *result, int len, int *vv, int ** M, int l, int w, element_t *v)
{
	if(m == 0)
	{
		int *v1;
		v1 = malloc(sizeof(int)*l);
		
		memcpy1(v1, vv, l);
		for(int i = 0; i < len; i++)
		{
			v1[a[result[len-1-i]-1]] = 0;

		}
		if(mul_matrix_satisfy(v1, M, l, w))
		{
			for(int i = 0; i < len; i++)
			{
				vv[a[result[len-1-i]-1]] = 0;
				element_set0(v[a[result[len-1-i]-1]]);
			}
			return 1;
		} 
		return 0;
	}
	for(int i = n; i >= m; i--)
	{
		result[m-1] = i;
		if(get_combinations(a, i-1, m-1, result, len, vv, M, l, w, v)) return 1;
	}
	return 0;
}

int vector_satisfy(int* vv, int ** M, int l, int w, element_t *v)
{
	int i, j = 0, re;
	int n[10] = {0};

	if(mul_matrix_satisfy(vv, M, l, w)) return 1;
	for(i = 0; i < l; i++)
	{
		if(vv[i] == 1)
			n[j++] = i;
	}
	//j--;
	for(i = 1; i < l - 1; i++)
	{
		int r[i];
		re = get_combinations(n, j, i, r, i, vv, M, l, w, v);
		if(re)
			return 1;
	}
	
	return 0;

}


/*ABS签名函数*/
mprabs_sig_t*
mprabs_sig( mprabs_pub_t* pub, mprabs_prv_t* prv, GByteArray* m, char* policy)
{
	mprabs_sig_t* sig;
 	element_t hash;
 	char * message_hash;
	int i, j, n;
	element_t r0;
	
	int **M;

	
	sig = malloc(sizeof(mprabs_sig_t));
	

	sig->Si = g_array_new(0, 1, sizeof(mprabs_sig_s_t));
	sig->Pj = g_array_new(0, 1, sizeof(mprabs_sig_p_t));

	M = parse_policy(policy);   //span matrix
	message_hash = (char *) malloc(m->len + strlen(policy)+1);
	strncpy(message_hash, m->data, m->len);
	message_hash[m->len] = '\0';
	//element_printf("message_hash = %s\n", message_hash);
	i = 0;
	while(ac_postfix[i] != '\0')
	{
		strcat(message_hash, ac_postfix[i]);
		i++;
		
	}
    
   
    /*compute the v of the user*/
   
    element_t r[number];
    element_t ui[number];
    element_t v[number] ;
    int vv[number];
    int point[number];
  
    //int te = 0, re1 = 3, re2 = 3, re3 = 3;

    //for(i = 0 ; i < number ; i++)
    //{
    //	element_init_Zr(v[i], pub->p);
    //	element_set0(v[i]); 
    //}

   
    element_init_Zr(r0, pub->p);
    element_init_Zr(hash, pub->p);
    element_init_G1(sig->y, pub->p);
    element_init_G1(sig->w, pub->p);

    /*compute*/
    abs_element_from_string(hash, message_hash);  //HASH SHA1
    //element_printf("message_hash = %s\n", message_hash);
    element_random(r0);
    element_pow_zn(sig->y, prv->kbase, r0);
    element_pow_zn(sig->w, prv->k0, r0);

    for(i = 0; i < number; i++)
    {
    	element_init_Zr(v[i], pub->p);
    	for(j = 0; j < prv->comps->len; j++)
    	{

    		if(!strcmp(g_array_index(prv->comps, mprabs_prv_comp_t, j).attr, attribute[i].att))
			{
				element_set1(v[i]);
				vv[i] = 1;
				point[i] = j;
				break;	
			}
						
    	}
    	if(vv[i] != 1)
    	{
    		element_set0(v[i]);
			vv[i] = 0;
    	}
	}
	

	if(!vector_satisfy(vv, M, number, length, v))
		printf("The user's attributes do not satisfy the access policy!\n");

	//for(i = 0; i < number; i++)
	//{
		//element_printf("v[%d] = %B\n", i, v[i]);
		//int aaa = element_is1(v[i]);
		//printf("%d\n", aaa);
	//}
    
    for(i = 0; i < number; i++)
    {
    	mprabs_sig_s_t S;
		element_t K;
		element_t u;
		int ll;
		//element_t ri;
		//j = 0;

		ll = length_char(attribute[i].att);
		S.attr = malloc(sizeof(attribute[i].att));
		memcpy(S.attr, attribute[i].att, ll);  //S.attr = attribute[i].att;
		S.attr[ll] = '\0';
		

		element_init_G1(K,        pub->p);
		element_init_G1(u,        pub->p);
		element_init_Zr(r[i], pub->p);
		
		element_init_G1(S.ss, pub->p);

		element_random(r[i]);
		//element_printf("r[%d] = %B\n", i, r[i]);
		element_pow_zn(u, pub->g, hash);
		//element_printf("g = %B\n", pub->g);
		//element_printf("hash = %B\n", hash);
		//element_printf("u = %B\n", u);
		element_mul(u, u, pub->C);
		//element_printf("C = %B\n", pub->C);
		//element_printf("u = %B\n", u);
		element_pow_zn(u, u, r[i]);
		//element_printf("u = %B\n", u);

					//mprabs_prv_comp_t test;

			//test = g_array_index(prv->comps, mprabs_prv_comp_t, j);
			
			//int aaa = strcmp(test.attr, attribute[i].att);
		//element_printf("v[%d] = %B\n", i, v[i]);
		if(element_is1(v[i]))
		{
			element_pow_zn(K, g_array_index(prv->comps, mprabs_prv_comp_t, point[i]).k, v[i]);
			element_pow_zn(K, K, r0);
			element_mul(S.ss, K, u);
		}
		else
		{
			element_set(S.ss, u);
			//vv[i] = 0;
		}
		
		
 		element_clear(K);
		element_clear(u);
		//element_clear(ri);

		//element_printf("S[%d] = %B\n", i, S.ss);
		g_array_append_val(sig->Si, S);
    }



    for(i = 0; i < number; i++)     //acquire the hash value of attributes in policy
    {
    	element_init_Zr(ui[i], pub->p);
    	abs_element_from_string(ui[i], attribute[i].att);  //HASH SHA1

    }
/*
    element_t test1, test2, test3, test4;
    element_t ab1, ab2;
	element_init_GT(test1, pub->p);
	element_init_GT(test2, pub->p);
	element_init_GT(test3, pub->p);
	element_init_GT(test4, pub->p);
	element_init_G2(ab1, pub->p);
	element_pow_zn(ab1, pub->B[0], ui[4]);
	element_mul(ab1, pub->A[0], ab1);
	element_init_G2(ab2, pub->p);
	element_pow_zn(ab2, pub->B[1], ui[4]);
	element_mul(ab2, pub->A[1], ab2);
	//mprabs_prv_comp_t test3;
	//test3 = g_array_index(prv->comps, mprabs_prv_comp_t, 0);
	pairing_apply(test1, g_array_index(prv->comps, mprabs_prv_comp_t, 0).k, ab1, pub->p);
	pairing_apply(test2, g_array_index(prv->comps, mprabs_prv_comp_t, 0).k, ab2, pub->p);
	element_printf("tets1 = %B, test2 = %B\n", test1, test2);
	element_mul(test3, test1, test2);
	pairing_apply(test1, prv->kbase, pub->h[0], pub->p);
	pairing_apply(test2, prv->kbase, pub->h[1], pub->p);
	element_printf("tets1 = %B, test2 = %B\n", test1, test2);
	element_mul(test4, test1, test2);
	int ttt1 = element_cmp(test3, test4);
	element_printf("tets3 = %B, test4 = %B\n", test3, test4);
	element_pow_zn(ab1, pub->h[0], pub->h[1]);
	pairing_apply(test4, prv->kbase, ab1, pub->p);
	int ttt2 = element_cmp(test3, test4);
    element_printf("tets3 = %B, test4 = %B\n", test3, test4);
*/

    for(j = 0; j< length; j++)
    {
    	mprabs_sig_p_t P;
    	element_t ab;
    	element_t mij;
    	element_t m1;

    	element_init_G2(ab, pub->p);
    	element_init_G2(P.pp, pub->p);
   	 	element_init_Zr(mij, pub->p);
    	element_init_Zr(m1, pub->p);

    	for(i = 0; i < number; i++)
    	{
    		element_pow_zn(ab, pub->B[j], ui[i]);
    		element_mul(ab, pub->A[j], ab);
    		if(M[i][j] == 1)
    			element_set1(m1);
    		else if(M[i][j] == -1)
    		{
    			element_set1(m1);
    			element_neg(m1, m1);
    		}
    		else
    			element_set0(m1);
    		element_pow_zn(ab, ab, m1);
    		//element_printf("ab = %B\n", ab);
    		//element_printf("r[%d] = %B\n", i, r[i]);
    		element_pow_zn(ab, ab, r[i]);
    		if(i == 0) element_set(P.pp, ab);
    		else
    			element_mul(P.pp, P.pp, ab);

    	}
    	/*if(j == 0)
    	{
    		element_t test;
    		element_t ute;
     		element_init_GT(test, pub->p);
    		element_init_G1(ute, pub->p);
    		element_pow_zn(ute, pub->g, hash);
			element_mul(ute, ute, pub->C);
			//element_printf("u = %B\n", ute);
    		pairing_apply(test, ute, P.pp, pub->p);
    		//element_printf("p1 = %B\n", P.pp);  
    		//element_printf("test = %B\n", test); 
    		element_clear(test);
    		element_clear(ute);
    	}*/

    	element_clear(ab);
		element_clear(mij);
		element_clear(m1);

    	g_array_append_val(sig->Pj, P);

    
    }
	
	return sig;
}


/*ABS验签函数*/
int mprabs_verify(mprabs_pub_t* pub, mprabs_sig_t* sig, GByteArray* m, char* policy)
{
	//mprabs_sig_t* sig;
 	element_t hash;
 	char * message_hash;
	int i, j;
	int ** M;
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

    M = parse_policy(policy); 

	/* compute the H(m|Tac)*/
	//element_printf("m->data = %s\n", m->data);

	message_hash = (char *) malloc(m->len + strlen(policy)+1);
	//m->data[m->len] = '\0';
	strncpy(message_hash, m->data, m->len);
	message_hash[m->len] = '\0';
	//element_printf("m->data = %c\n", m->data[m->len-1]);
	//element_printf("m->data = %c\n", m->data[m->len]);
	//element_printf("m->data = %c\n", m->data[m->len+1]);
	//element_printf("m->data = %s\n", m->data);
	//element_printf("message_hash = %s\n", message_hash);
	//for(i = 0; i < m->len; i++)
	//	printf("m[%d] = %c\n", i, m->data[i]);
	i = 0;
	while(ac_postfix[i] != '\0')
	{
		strcat(message_hash, ac_postfix[i]);
		i++;
		
	}
	element_init_Zr(hash, pub->p);
	abs_element_from_string(hash, message_hash);
	//element_printf("message_hash = %s\n", message_hash);

	
	if(element_is1(sig->y)) return 0;

  	element_init_GT(e1, pub->p);
  	element_init_GT(e2, pub->p);

  	pairing_apply(e1, sig->w, pub->A0, pub->p);  //计算w和A0的配对，结果放在e1中
  	pairing_apply(e2, sig->y, pub->h0, pub->p);  //
  	//element_printf("e1 = %B, e2 = %B\n", e1, e2);

  	if(element_cmp(e1, e2)) return 0;

  	element_clear(e1);
  	element_clear(e2);

  	element_t u1[number]; 
  	element_t m1;

    for(i = 0; i < number; i++)  
    {
    	element_init_Zr(u1[i], pub->p);
    	abs_element_from_string(u1[i], attribute[i].att);  //HASH SHA1

    }


	
	element_init_G1(u, pub->p);
	
	element_pow_zn(u, pub->g, hash);
	element_mul(u, u, pub->C);
	//element_printf("u = %B\n", u);
	

    
    element_init_GT(e3, pub->p);
    element_init_GT(e4, pub->p);
    element_init_GT(e5, pub->p);
    element_init_GT(e6, pub->p);
    element_init_Zr(m1, pub->p);

    pairing_apply(e3, sig->y, pub->h[0], pub->p);  
    pairing_apply(e4, u, g_array_index(sig->Pj, mprabs_sig_p_t, 0).pp, pub->p);
    //element_printf("p1 = %B\n", g_array_index(sig->Pj, mprabs_sig_p_t, 0).pp);
    //element_printf("e4 = %B\n", e4);
    element_mul(e3, e3, e4);
    //element_printf("e3 = %B\n", e3);

    for(j = 0; j < length; j++)
    {
    	pairing_apply(e4, u, g_array_index(sig->Pj, mprabs_sig_p_t, j).pp, pub->p); 
    	for(i = 0; i < number; i++)
    	{
    		element_pow_zn(ab, pub->B[j], u1[i]);
    	    element_mul(ab, pub->A[j], ab);
    	    //element_printf("ab = %B\n", ab);
    	    if(M[i][j] == 1)
    			element_set1(m1);
    		else if(M[i][j] == -1)
    		{
    			element_set1(m1);
    			element_neg(m1, m1);
    		}
    		else
    			element_set0(m1);
    		//element_printf("m1 = %B\n", m1);
    	    element_pow_zn(ab, ab, m1);
    	    //element_printf("ab = %B\n", ab);
    	    pairing_apply(e6, g_array_index(sig->Si, mprabs_sig_s_t, i).ss, ab, pub->p); 
    	    //element_printf("S[%d] = %B\n", i, g_array_index(sig->Si, mprabs_sig_s_t, i).ss);
    	    //element_printf("e6 = %B\n", e6);
    	    if(i == 0) 
    	        element_set(e5, e6);
            else
    	        element_mul(e5, e5, e6);
    	    //element_printf("e5 = %B\n", e5);
    	}
    	if(j == 0)
    	{
    		//int ttt = element_cmp(e3, e5);
    		//element_printf("e3 = %B, e5 = %B\n", e3, e5);
    		if(element_cmp(e3, e5)) return 0;
    	}
    	else
    	{
    		if(element_cmp(e4, e5)) return 0;
    	}    	
    }

    return 1;
}