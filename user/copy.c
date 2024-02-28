#include "kernel/types.h"
#include "user/user.h"

int main()
{
	char buf[64];
	while(1)
	{
		//read input in console by using system_call-read
		int n = read(0,buf,sizeof(buf));
		//if no input
		if(n<=0)
		{
			break;
		}
		//output by using write
		write(1,buf,n);
	}
	exit(0);
}
