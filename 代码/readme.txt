abe和abs的代码中，公钥pub_file,主密钥msk_file，以及私钥和中间文件的路径都改为可设置的
所有需要的文件都在test_file文件夹中
用过的测试代码都在各自的子文件夹中

cpabe-0.11:
	其中abesetup、abekeygen、abeenc、abedec是根据cpabe写好的调用接口，合成abelib.a静态库
	由于cpabe本身有makefile，故此文件夹中没有对生成静态库提供makefile
	会用到的编译链接语句：
	gcc -c abeenc.c -g -o abeenc.o -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pbc -I /usr/local/include/pbc -lbswabe -lcrypto -lcrypto -lgmp -lglib-2.0 -Wl,-rpath -lgmp -Wl,-rpath –lpbc
	gcc abekeygen.c -o abekeygen -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pbc -I /usr/local/include/pbc -lbswabe -lcrypto -lcrypto -lgmp -lglib-2.0 -Wl,-rpath /usr/local/lib -lgmp -Wl,-rpath /usr/local/lib -lpbc common.o policy_lang.o
	ar rc abelib.a abesetup.o abekeygen.o abeenc.o abedec.o common.o policy_lang.o


abs:
	有相应的makefile
	与abe基本类似

abe_abs:
	setup是初始化程序，会生成abe和abs的公钥、主密钥和私钥，这些路径都需要输入，运行一次即可
	enc_sig是filter中与clean绑定的程序，将文件输入加密，生成中间文件，再对中间文件签名，把所有内容输出
	verify_dec是filter中与smudge绑定的程序，将pull下来的文件输入验签，生成中间文件，在对中间文件解密，把明文输出

git filter设置：
	git clone一个仓库后，或者本地git init一个仓库后进入仓库目录
	cd ./.git
	cd ./info   此时里面没有attributes文件
	echo "* filter=test" >> attributes     test是随便起的名字
	git config --global filter.test.clean "xxx/xxx/xx/test6"    
	全局设置，其他仓库如果attributes中也使用叫test的filter，会使用同样配置
	git config --global filter.test.smudge "xxx/xxx/xx/test7"
	查看此仓库的filter：
	git config filter.test.clean 
	git config filter.test.smudge 
	
	另外的查看：
	git add之后在暂存区就有签名后的文件
	git ls-files -s   显示暂存区的所有文件及其hash
	如显示100644  6cef9234rhusjkdhbvous7896t   0     abc.txt
	想查看暂存区的abc.txt，用hash前4位，把内容放abc_temp.txt中
	git cat-file -p 6cef > abc_temp.txt
	现在abc_temp.txt的内容与本地生成的signedtext.txt文件相同