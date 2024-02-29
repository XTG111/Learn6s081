#include "kernel/types.h"
#include "user/user.h"

int main()
{	
	//system call exe
	//echo
	char* argv[] = {"echo","this","is","echo",0};
	exec("echo",argv);
	printf("exec error\n");
	exit(0);
}
