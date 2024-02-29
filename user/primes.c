#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//primes
//left pipe -- pleft[2]
__attribute__((noreturn))
void findprimes(int pleft[2])
{
	//read the num-list first number
	//it's must be the prime
	int p;
	read(pleft[0],&p,sizeof(p));
	//p = -1 --> finish
	if(p == -1)
	{
		exit(0);
	}
	printf("prime %d\n",p);

	//next progress find next prime
	int pright[2];
	pipe(pright);
	
	//child progress 
	if(fork() == 0)
	{
		//not need use father2child pipe write
		close(pright[1]);
		//not need use 2father pipe read
		close(pleft[0]);
		//**func**
		findprimes(pright);
	}
	//father progress -- current -- clear some no-prime
	//which k*p
	else
	{
		//close father2child pipe read
		close(pright[0]);
		int buf;
		//clear
		while(read(pleft[0],&buf,sizeof(buf)) && buf != -1)
		{
			if(buf % p != 0)
			{
				//not k*p write in pright for child progress
				write(pright[1],&buf,sizeof(buf));
			}
		}
		buf = -1;
		//finish signal
		write(pright[1],&buf,sizeof(buf));
		//wait child progress finish
		wait(0);
		exit(0);
	}
}

int main(int argc,char* argv[])
{
	int p[2];
	pipe(p);
	
	//child progress
	if(fork() == 0)
	{
		//fisrt child progress
		//not write for pipe
		close(p[1]);
	      	findprimes(p);
		exit(0);	
	}
	//father progress
	else
	{
		//not need read for pipe
		close(p[0]);
		int i;
		//write 2->35 in pipe left
		for(i = 2;i<36;i++)
		{
			write(p[1],&i,sizeof(i));
		}
		//finish sigal
		i = -1;
		write(p[1],&i,sizeof(i));
	}
	//wait the first child progress finish then all prime find
	wait(0);
	exit(0);
}
