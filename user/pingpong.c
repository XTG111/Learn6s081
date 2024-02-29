#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
	//pipe
	int pf2c[2],pc2f[2];
	pipe(pf2c);
	pipe(pc2f);
	//father progress
	if(fork()!=0)
	{
		//close file-des not use
		close(pf2c[0]);
		close(pc2f[1]);
		//write bit in father2child pipe
		write(pf2c[1],"a",1);
		char buf;
		//read bit from child2father pipe
		read(pc2f[0],&buf,1);
		//after read printf
		printf("%d: received pong\n",getpid());
		close(pf2c[1]);
		close(pc2f[0]);
		wait(0);
	}
	//child progress
	else
	{
		close(pf2c[1]);
		close(pc2f[0]);
		char buf;
		//read bit from father2child pipe
		read(pf2c[0],&buf,1);
		printf("%d: received ping\n",getpid());
		//writ bit in child2father pipe
		write(pc2f[1],&buf,1);
		close(pf2c[0]);
		close(pc2f[1]);
	}
	exit(0);
}
