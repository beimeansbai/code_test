/*
	Include glib.h and pbc.h before including this file.

	This contains data structures and procedures common throughout the
	tools.
*/

/*
	TODO if ssl sha1 not available, use built in one (sha1.c)
*/

char*       abs_suck_file_str( char* file );
char*       abs_suck_stdin();
GByteArray* abs_suck_file( char* file );

void        abs_spit_file( char* file, GByteArray* b, int free );

void read_abs_file( char* file,    GByteArray** sig_buf,
											int* file_len, GByteArray** cph_buf );

void write_abs_file( char* file,   GByteArray* sig_buf,
											 int file_len, GByteArray* cph_buf );

void abs_die(char* fmt, ...);

//GByteArray* aes_128_cbc_encrypt( GByteArray* pt, element_t k );
//GByteArray* aes_128_cbc_decrypt( GByteArray* ct, element_t k );

//#define CPABE_VERSION PACKAGE_NAME "%s " PACKAGE_VERSION "\n" \
"\n" \
"Parts Copyright (C) 2006, 2007 John Bethencourt and SRI International.\n" \
"This is free software released under the GPL, see the source for copying\n" \
"conditions. There is NO warranty; not even for MERCHANTABILITY or FITNESS\n" \
"FOR A PARTICULAR PURPOSE.\n" \
"\n" \
"Report bugs to John Bethencourt <bethenco@cs.berkeley.edu>.\n"
