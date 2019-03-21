#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <linux/sem.h>
int p1,p2;
int main (void)
{
	if((p1=fork())==0)
	{
		execv("./time",NULL);
	}
	else                                
	{
		if((p2=fork())==0)
		{
			execv("./cpu",NULL);
		}
		else 
		{
			if (fork()==0)
			{
				execv("./add",NULL);
			}
		}
	}
	return 0;
} 
