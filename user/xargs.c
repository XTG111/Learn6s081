#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXSZ 512
//define the statemachine
enum state
{
	S_WAIT, //wait param input, init state or input is space
	S_ARG, //input valid param
	S_ARG_END, //valid param end
	S_ARG_LINE_END, // valid param enter "1\n"
	S_LINE_END, // space enter "1 \n"
	S_END, //end	
};
//char state
enum char_type
{
	C_SPACE,
	C_CHAR,
	C_LINE_END,
};

//get input string every char type
enum char_type get_char_type(char c)
{
	switch(c)
	{
	case ' ':
		return C_SPACE;
		break;
	case '\n':
		return C_LINE_END;
		break;
	default:
		return C_CHAR;
		break;
	}
};

//change state by the next char
enum state transform_state(enum state cur_state, enum char_type ct)
{
	switch(cur_state)
	{
	case S_WAIT:
		if (ct == C_SPACE)    return S_WAIT;
    		if (ct == C_LINE_END) return S_LINE_END;
    		if (ct == C_CHAR)     return S_ARG;
    		break;
  	case S_ARG:
    		if (ct == C_SPACE)    return S_ARG_END;
    		if (ct == C_LINE_END) return S_ARG_LINE_END;
    		if (ct == C_CHAR)     return S_ARG;
    		break;
  	case S_ARG_END:
  	case S_ARG_LINE_END:
  	case S_LINE_END:
    		if (ct == C_SPACE)    return S_WAIT;
    		if (ct == C_LINE_END) return S_LINE_END;
    		if (ct == C_CHAR)     return S_ARG;
    		break;
  	default:
    		break;
  	}
	return S_END;
};

//clear params when appear '\n'
void clearArgv(char* x_argv[MAXARG],int beg)
{
	for(int i = beg;i<MAXARG;i++)
	{
		x_argv[i] = 0;
	}
}

//main
int main(int argc,char* argv[])
{
	//if params's length > MAXARG error
	if(argc - 1 >= MAXARG)
	{
		fprintf(2,"xargs: too many arguments\n");
		exit(1);
	}
	char lines[MAXSZ];
	char* p = lines;
	char* x_argv[MAXARG] = {0};
	
	//the params input save
	for(int i = 1;i<argc;i++)
	{
		x_argv[i-1] = argv[i];
	}
	//begin index
	int arg_beg = 0;
	//end index
	int arg_end = 0;
	//cur index
	int arg_cnt = argc-1;
	//the begin state
	enum state st = S_WAIT;
	
	while(st != S_END)
	{
		if(read(0,p,sizeof(char)) != sizeof(char))
		{
			st = S_END;
		}
		else
		{
			//change state by *p
			st = transform_state(st,get_char_type(*p));
		}
		//if end index bigger than maxsz
		if(++arg_end >= MAXSZ)
		{
			fprintf(2,"xargs: arguments too long\n");
			exit(1);
		}
		switch(st)
		{
		case S_WAIT: // move arg_beg
			++arg_beg;
			break;
		case S_ARG_END: //end params, save params in x_argv
			x_argv[arg_cnt++] = &lines[arg_beg];
			arg_beg = arg_end;
			*p ='\0';
			break;
		case S_ARG_LINE_END:  // if '\n' save and ouptput
      			x_argv[arg_cnt++] = &lines[arg_beg];
      		// no break, same S_LINE_END --> output
    		case S_LINE_END:      // output
      			arg_beg = arg_end;
      			*p = '\0';
      			if (fork() == 0) 
      			{
        			exec(argv[1], x_argv);
			}
			arg_cnt = argc - 1;
      			clearArgv(x_argv, arg_cnt);
      			wait(0);
      			break;
    		default:
      			break;
		}
		++p;
	}
	exit(0);
}
