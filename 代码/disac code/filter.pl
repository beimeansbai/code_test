#!/usr/bin/perl
#
# Example implementation for the Git filter protocol version 2
# See Documentation/gitattributes.txt, section "Filter Protocol"
#
# Please note, this pass-thru filter is a minimal skeleton. No proper
# error handling was implemented.
#

use strict;
use warnings;

my $MAX_PACKET_CONTENT_SIZE = 65516;

sub packet_bin_read {               #读取输入
	my $buffer;
	my $bytes_read = read STDIN, $buffer, 4;   #从stdin读取4字节数据到buffer,这4字节是包的大小
	if ( $bytes_read == 0 ) {

		# EOF - Git stopped talking to us!
		exit();
	}
	elsif ( $bytes_read != 4 ) {    #read返回值为真正读取的字节数,若不是四字节则报错
		die "invalid packet: '$buffer'";
	}
	my $pkt_size = hex($buffer);   #hex十六进制转换成十进制
	if ( $pkt_size == 0 ) {    #包的大小为0
		return ( 1, "" );
	}
	elsif ( $pkt_size > 4 ) {
		my $content_size = $pkt_size - 4;
		$bytes_read = read STDIN, $buffer, $content_size;   #如果包长度大于4字节,则继续读取包的内容
		if ( $bytes_read != $content_size ) {     #读取的数据大小与传入的长度值不符则报错
			die "invalid packet ($content_size bytes expected; $bytes_read bytes read)";
		}
		return ( 0, $buffer );     #返回读取的数据
	}
	else {
		die "invalid packet size: $pkt_size";
	}
}

sub packet_txt_read {
	my ( $res, $buf ) = packet_bin_read();
	unless ( $buf =~ s/\n$// ) {                               #除非buf中的所有\n替换为空,否则报错
		die "A non-binary line MUST be terminated by an LF.";
	}
	return ( $res, $buf );
}

sub packet_bin_write {
	my $buf = shift;     #将参数数组的第一个元素删除并返回该元素
	print STDOUT sprintf( "%04x", length($buf) + 4 );   #16进制输出4字节长度,不足前面补零
	print STDOUT $buf;      #输出文件内容
	STDOUT->flush();
}

sub packet_txt_write {
	packet_bin_write( $_[0] . "\n" );
}

sub packet_flush {
	print STDOUT sprintf( "%04x", 0 );
	STDOUT->flush();
}

#git与filter通信时,会先输入以下的内容
( packet_txt_read() eq ( 0, "git-filter-client" ) ) || die "bad initialize";
( packet_txt_read() eq ( 0, "version=2" ) )         || die "bad version";
( packet_bin_read() eq ( 1, "" ) )                  || die "bad version end";
#filter进行回应
packet_txt_write("git-filter-server");
packet_txt_write("version=2");
packet_flush();

( packet_txt_read() eq ( 0, "capability=clean" ) )  || die "bad capability";
( packet_txt_read() eq ( 0, "capability=smudge" ) ) || die "bad capability";
( packet_txt_read() eq ( 0, "capability=delay" ) ) || die "bad capability";
( packet_bin_read() eq ( 1, "" ) )                  || die "bad capability end";

packet_txt_write("capability=clean");
packet_txt_write("capability=smudge");
packet_flush();

#git发送命令和文件路径
while (1) {
	my ($command)  = packet_txt_read() =~ /^command=(.+)$/;
	my ($pathname) = packet_txt_read() =~ /^pathname=(.+)$/;

	if ( $pathname eq "" ) {
		die "bad pathname '$pathname'";
	}

	packet_bin_read();

	my $input = "";
	{
		binmode(STDIN);
		my $buffer;
		my $done = 0;
		while ( !$done ) {
			( $done, $buffer ) = packet_bin_read();  #读取文件内容
			$input .= $buffer;    #input = input和buffer合并
		}
	}

	#my $path = $ENV{'PWD'};    #输出文件绝对路径
	#$path .= "/";
	#$path .= $pathname;
#	my $path = $pathname;    #输出相对路径
#	my $len = length($path);
#	my $in = $len;
#	$in .= "\n";
#	$in .= $path;
#	$in .= "\n";
#	$in .= $input;  


	my $in;   #设置$in作为输入变量
	my $path = $pathname;  #相对路径
	my $len;
	my $repo_path = $ENV{'PWD'};   #filter所在路径，即repo所在路径
    use IPC::Run qw(run);   
    my ($output, $err);   #output接收输出变量

	$len = length($repo_path);
	$in = $len;
	$in .= $repo_path; 
	$len = length($path);
	$in .= $len;
	$in .= $path;
	$in .= $input;   #文件内容
	
	if ( $command eq "clean" ) {
		### Perform clean here ###
			
		run ['/home/xx/abe_abs/enc_sig'], \$in, \$output, \$err;   #调用clean的程序
	}
	elsif ( $command eq "smudge" ) {
		### Perform smudge here ###
		$output = '';
		run ['/home/xx/abe_abs/verify_dec'], \$in, \$output, \$err;    #调用smudge程序
	}
	else {
		die "bad command '$command'";
	}

	packet_txt_write("status=success");
	packet_flush();
	while ( length($output) > 0 ) {
		my $packet = substr( $output, 0, $MAX_PACKET_CONTENT_SIZE );
		packet_bin_write($packet);   #向git输送经过filter的文件内容
		if ( length($output) > $MAX_PACKET_CONTENT_SIZE ) {
			$output = substr( $output, $MAX_PACKET_CONTENT_SIZE );
		}
		else {
			$output = "";
		}
	}
	packet_flush();    # flush content!
	packet_flush();    # empty list, keep "status=success" unchanged!

}
