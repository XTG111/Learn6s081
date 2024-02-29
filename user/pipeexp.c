#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
	//save file-des
	int p[2];
	//exec func
	char* argv[2];
	argv[0] = "wc";
	argv[1] = 0;
	//create pipe use p[2]
	pipe(p);
	//child
	if(fork() == 0)
	{
		//close read
		close(0);
		dup(p[0]);
		close(p[0]);
		close(p[1]);
		//use wc
		exec("wc",argv);
	}
	//father
	else
	{
		close(p[0]);
		write(p[1],"hello world\n",12);
		close(p[1]);
	}
}
