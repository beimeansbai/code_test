all:enc_sig verify_dec setup
.PHONY:all
enc_sig:enc_sig.c
	gcc enc_sig.c -g -o enc_sig ./abelib.a ./abslib.a -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pbc -I /usr/local/include/pbc -lbswabe -lcrypto -lcrypto -lgmp -lglib-2.0 -Wl,-rpath /usr/local/lib -lgmp -Wl,-rpath /usr/local/lib -lpbc
verify_dec:verify_dec.c
	gcc verify_dec.c -g -o verify_dec ./abelib.a ./abslib.a -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pbc -I /usr/local/include/pbc -lbswabe -lcrypto -lcrypto -lgmp -lglib-2.0 -Wl,-rpath /usr/local/lib -lgmp -Wl,-rpath /usr/local/lib -lpbc
setup:setup.c
	gcc setup.c -g -o setup ./abelib.a ./abslib.a -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pbc -I /usr/local/include/pbc -lbswabe -lcrypto -lcrypto -lgmp -lglib-2.0 -Wl,-rpath /usr/local/lib -lgmp -Wl,-rpath /usr/local/lib -lpbc
#test6:test6.c
#	gcc test6.c -g -o test6 ./abelib.a ./abslib.a -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pbc -I /usr/local/include/pbc -lbswabe -lcrypto -lcrypto -lgmp -lglib-2.0 -Wl,-rpath /usr/local/lib -lgmp -Wl,-rpath /usr/local/lib -lpbc

#test7:test7.c
#	gcc test7.c -g -o test7 ./abelib.a ./abslib.a -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pbc -I /usr/local/include/pbc -lbswabe -lcrypto -lcrypto -lgmp -lglib-2.0 -Wl,-rpath /usr/local/lib -lgmp -Wl,-rpath /usr/local/lib -lpbc
clean:
	rm enc_sig verify_dec setup
#	rm test6 test7