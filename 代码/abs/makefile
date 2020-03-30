abslib.a:abssetup.o abskeygen.o abssign.o absverify.o common.o core.o misc.o
	ar rc abslib.a abssetup.o abskeygen.o abssign.o absverify.o common.o core.o misc.o
abssetup:abssetup.o common.o misc.o core.o
	gcc core.o common.o misc.o abssetup.o -g -o abssetup -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pbc -I /usr/local/include/pbc -lbswabe -lcrypto -lcrypto -lgmp -lglib-2.0 -Wl,-rpath /usr/local/lib -lgmp -Wl,-rpath /usr/local/lib -lpbc
abskeygen:abskeygen.o common.o misc.o core.o
	gcc core.o common.o misc.o abskeygen.o -g -o abskeygen -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pbc -I /usr/local/include/pbc -lbswabe -lcrypto -lcrypto -lgmp -lglib-2.0 -Wl,-rpath /usr/local/lib -lgmp -Wl,-rpath /usr/local/lib -lpbc
abssetup.o:abssetup.c abslib.h mprabs.h common.h
	gcc -c abssetup.c -g -o abssetup.o -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pbc -I /usr/local/include/pbc -lbswabe -lcrypto -lcrypto -lgmp -lglib-2.0 -Wl,-rpath -lgmp -Wl,-rpath -lpbc
abskeygen.o:abskeygen.c abslib.h mprabs.h common.h
	gcc -c abskeygen.c -g -o abskeygen.o -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pbc -I /usr/local/include/pbc -lbswabe -lcrypto -lcrypto -lgmp -lglib-2.0 -Wl,-rpath -lgmp -Wl,-rpath -lpbc
abssign.o:abssign.c abslib.h mprabs.h common.h
	gcc -c abssign.c -g -o abssign.o -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pbc -I /usr/local/include/pbc -lbswabe -lcrypto -lcrypto -lgmp -lglib-2.0 -Wl,-rpath -lgmp -Wl,-rpath -lpbc
absverify.o:absverify.c abslib.h mprabs.h common.h
	gcc -c absverify.c -g -o absverify.o -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pbc -I /usr/local/include/pbc -lbswabe -lcrypto -lcrypto -lgmp -lglib-2.0 -Wl,-rpath -lgmp -Wl,-rpath -lpbc
common.o:common.c common.h
	gcc -c common.c -g -o common.o -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pbc -I /usr/local/include/pbc -lbswabe -lcrypto -lcrypto -lgmp -lglib-2.0 -Wl,-rpath -lgmp -Wl,-rpath -lpbc
core.o:core.c mprabs.h private.h common.h
	gcc -c core.c -g -o core.o -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pbc -I /usr/local/include/pbc -lbswabe -lcrypto -lcrypto -lgmp -lglib-2.0 -Wl,-rpath -lgmp -Wl,-rpath -lpbc
misc.o:misc.c mprabs.h private.h
	gcc -c misc.c -g -o misc.o -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pbc -I /usr/local/include/pbc -lbswabe -lcrypto -lcrypto -lgmp -lglib-2.0 -Wl,-rpath -lgmp -Wl,-rpath -lpbc

#abssign2.o:abssign2.c abslib.h
#	gcc -c abssign2.c -g -o abssign2.o -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pbc -I /usr/local/include/pbc -lbswabe -lcrypto -lcrypto -lgmp -lglib-2.0 -Wl,-rpath -lgmp -Wl,-rpath -lpbc
#absverify1.o:absverify1.c abslib.h
#	gcc -c absverify1.c -g -o absverify1.o -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pbc -I /usr/local/include/pbc -lbswabe -lcrypto -lcrypto -lgmp -lglib-2.0 -Wl,-rpath -lgmp -Wl,-rpath -lpbc
clean:
	rm abslib.a abssetup.o abskeygen.o abssign.o absverify.o common.o core.o misc.o