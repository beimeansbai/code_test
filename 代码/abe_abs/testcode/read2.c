#include <stdio.h>
#include <string.h>
#include <ctype.h>
//#include <unistd.h>
//#include <glib.h>
//#include <pbc.h>
//#include <pbc_random.h>

//#include "mprabs.h"
//#include "common.h"

//#include "abslib.h"

void main()
{
	//GByteArray* message;
	char *in_file;
	in_file = "/home/xx/test_file/plain.txt";
	//scanf("%s", in_file);
	//printf("%s\n", in_file);

	FILE *fp;
	fp = fopen(in_file, "rb");

	int n;
	unsigned char buf[16];
	unsigned long count = 0;
	while((n = fread(buf, sizeof(char), 16, fp)) > 0) {
		int i;

		printf("%08lx ", count);

		for(i=0; i<n; i++) {
			printf("%02x ", (unsigned)buf[i]);
		}

		if(n < 16) {
			for(i=n; i<16; i++) printf("    ");
		}

		for(i=0; i<n; i++) {
			putchar(isprint(buf[i]) ? buf[i] : '.');
		}

		putchar('\n');

		count += 16;

	}
	fclose(fp);

}