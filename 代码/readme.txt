abe��abs�Ĵ����У���Կpub_file,����Կmsk_file���Լ�˽Կ���м��ļ���·������Ϊ�����õ�
������Ҫ���ļ�����test_file�ļ�����
�ù��Ĳ��Դ��붼�ڸ��Ե����ļ�����

cpabe-0.11:
	����abesetup��abekeygen��abeenc��abedec�Ǹ���cpabeд�õĵ��ýӿڣ��ϳ�abelib.a��̬��
	����cpabe������makefile���ʴ��ļ�����û�ж����ɾ�̬���ṩmakefile
	���õ��ı���������䣺
	gcc -c abeenc.c -g -o abeenc.o -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pbc -I /usr/local/include/pbc -lbswabe -lcrypto -lcrypto -lgmp -lglib-2.0 -Wl,-rpath -lgmp -Wl,-rpath �Clpbc
	gcc abekeygen.c -o abekeygen -I /usr/include/glib-2.0 -I /usr/lib/x86_64-linux-gnu/glib-2.0/include -I /usr/include/pbc -I /usr/local/include/pbc -lbswabe -lcrypto -lcrypto -lgmp -lglib-2.0 -Wl,-rpath /usr/local/lib -lgmp -Wl,-rpath /usr/local/lib -lpbc common.o policy_lang.o
	ar rc abelib.a abesetup.o abekeygen.o abeenc.o abedec.o common.o policy_lang.o


abs:
	����Ӧ��makefile
	��abe��������

abe_abs:
	setup�ǳ�ʼ�����򣬻�����abe��abs�Ĺ�Կ������Կ��˽Կ����Щ·������Ҫ���룬����һ�μ���
	enc_sig��filter����clean�󶨵ĳ��򣬽��ļ�������ܣ������м��ļ����ٶ��м��ļ�ǩ�����������������
	verify_dec��filter����smudge�󶨵ĳ��򣬽�pull�������ļ�������ǩ�������м��ļ����ڶ��м��ļ����ܣ����������

git filter���ã�
	git cloneһ���ֿ�󣬻��߱���git initһ���ֿ�����ֿ�Ŀ¼
	cd ./.git
	cd ./info   ��ʱ����û��attributes�ļ�
	echo "* filter=test" >> attributes     test������������
	git config --global filter.test.clean "xxx/xxx/xx/test6"    
	ȫ�����ã������ֿ����attributes��Ҳʹ�ý�test��filter����ʹ��ͬ������
	git config --global filter.test.smudge "xxx/xxx/xx/test7"
	�鿴�˲ֿ��filter��
	git config filter.test.clean 
	git config filter.test.smudge 
	
	����Ĳ鿴��
	git add֮�����ݴ�������ǩ������ļ�
	git ls-files -s   ��ʾ�ݴ����������ļ�����hash
	����ʾ100644  6cef9234rhusjkdhbvous7896t   0     abc.txt
	��鿴�ݴ�����abc.txt����hashǰ4λ�������ݷ�abc_temp.txt��
	git cat-file -p 6cef > abc_temp.txt
	����abc_temp.txt�������뱾�����ɵ�signedtext.txt�ļ���ͬ