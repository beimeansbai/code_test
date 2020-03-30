/*
  Include glib.h and pbc.h before including this file. Note that this
  file should be included at most once.
*/

#if defined (__cplusplus)
extern "C" {
#endif

/*
  A public key.
*/
typedef struct mprabs_pub_s mprabs_pub_t;

/*
  A master secret key.
*/
typedef struct mprabs_msk_s mprabs_msk_t;

/*
  A private key.
*/
typedef struct mprabs_prv_s mprabs_prv_t;

/*
  A ciphertext. Note that this library only handles encrypting a
  single group element, so if you want to encrypt something bigger,
  you will have to use that group element as a symmetric key for
  hybrid encryption (which you do yourself).
*/
typedef struct mprabs_sig_s mprabs_sig_t;

/*
  Generate a public key and corresponding master secret key, and
  assign the *pub and *msk pointers to them. The space used may be
  later freed by calling bswabe_pub_free(*pub) and
  bswabe_msk_free(*msk).
*/
void mprabs_setup( mprabs_pub_t** pub, mprabs_msk_t** msk );

/*
  Generate a private key with the given set of attributes. The final
  argument should be a null terminated array of pointers to strings,
  one for each attribute.
*/
void abs_parse_attribute( GSList** l, char* a );

mprabs_prv_t* mprabs_keygen( mprabs_pub_t* pub,  mprabs_msk_t* msk, char** attributes );
/*
  Pick a random group element and encrypt it under the specified
  access policy. The resulting ciphertext is returned and the
  element_t given as an argument (which need not be initialized) is
  set to the random group element.

  After using this function, it is normal to extract the random data
  in m using the pbc functions element_length_in_bytes and
  element_to_bytes and use it as a key for hybrid encryption.

  The policy is specified as a simple string which encodes a postorder
  traversal of threshold tree defining the access policy. As an
  example,

    "foo bar fim 2of3 baf 1of2"

  specifies a policy with two threshold gates and four leaves. It is
  not possible to specify an attribute with whitespace in it (although
  "_" is allowed).

  Numerical attributes and any other fancy stuff are not supported.

  Returns null if an error occured, in which case a description can be
  retrieved by calling bswabe_error().
*/
mprabs_sig_t* mprabs_sig( mprabs_pub_t* pub, mprabs_prv_t* prv, GByteArray* m, char* policy);
/*
  Decrypt the specified ciphertext using the given private key,
  filling in the provided element m (which need not be initialized)
  with the result.

  Returns true if decryption succeeded, false if this key does not
  satisfy the policy of the ciphertext (in which case m is unaltered).
*/

int mprabs_verify( mprabs_pub_t* pub, mprabs_sig_t* sig, GByteArray* m, char* policy);
/*
  Exactly what it seems.
*/

char* abs_bit_marker( char* base, char* tplate, int bit, char val );


GByteArray* mprabs_pub_serialize( mprabs_pub_t* pub );
GByteArray* mprabs_msk_serialize( mprabs_msk_t* msk );
GByteArray* mprabs_prv_serialize( mprabs_prv_t* prv );
GByteArray* mprabs_sig_serialize( mprabs_sig_t* cph );

/*
  Also exactly what it seems. If free is true, the GByteArray passed
  in will be free'd after it is read.
*/
mprabs_pub_t* mprabs_pub_unserialize( GByteArray* b, int free );
mprabs_msk_t* mprabs_msk_unserialize( mprabs_pub_t* pub, GByteArray* b, int free );
mprabs_prv_t* mprabs_prv_unserialize( mprabs_pub_t* pub, GByteArray* b, int free );
mprabs_sig_t* mprabs_sig_unserialize( mprabs_pub_t* pub, GByteArray* b, int free );

/*
  Again, exactly what it seems.
*/
void mprabs_pub_free( mprabs_pub_t* pub );
void mprabs_msk_free( mprabs_msk_t* msk );
void mprabs_prv_free( mprabs_prv_t* prv );
void mprabs_sig_free( mprabs_sig_t* cph );

/*
  Return a description of the last error that occured. Call this after
  bswabe_enc or bswabe_dec returns 0. The returned string does not
  need to be free'd.
*/
char* mprabs_error();

#if defined (__cplusplus)
} // extern "C"
#endif
