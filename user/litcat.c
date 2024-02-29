#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
	//byte buf
	char buf[512];
	//byte num
	int n;
	for(;;)
	{
		//read
		n = read(0,buf,sizeof(buf));
		if(n == 0)
		{
			break;
		}
		if(n < 0)
		{
			fprintf(2,"read error\n");
			exit(1);
		}
		//write
		//if res not n -- error
		if(write(1,buf,n)!=n)
		{
			fprintf(2,"write error\n");
			exit(1);
		}
	}
	exit(0);
}
