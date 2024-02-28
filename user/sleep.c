#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char* argv[])
{
	//sleep time
	if(argc < 2)
	{
		printf("error:no time\n");
		printf("use: sleep <time>\n");
	}
	else
	{
		sleep(atoi(argv[1]));
	}
	exit(0);
}
