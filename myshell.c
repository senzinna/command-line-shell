#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include "myshell.h"

// Used to stop from closing whrn Ctr+C is hit
void sigintHandler(int sig_num)
{
	signal(SIGINT, sigintHandler);
	printf("\nCannot be terminated using Ctr+C\nPress enter to keep going ... ");
	fflush(stdout);
}

// Reads raw input from user
char buff[1024];

int main(int argc, char *argv[]) {

	// Set help path
	HelpPath(argv[0]);
	
	// Update SHELL in environ
	PWD();
	
	if ( argc < 2)
	{
		RunWithoutFile();
	}
	
	else
	{
		RunFromFile(argv[1]);
	}
return 0;
}


// This function runs the shells without a batchfille
void RunWithoutFile()
{
	while(1)
		{
			// Prints my name and into
			signal(SIGINT, sigintHandler);
			printInfo();
			
			// Puts input into buffer
			fgets(buff,1024,stdin);
			
			command( buff );
			
			// Clear command and argument or else pressing enter causes the same command
			int i=0;
			for (i=0; i < 1024; i++)
			{
				buff[i]='\0';
			}
		}
}


// This function runs the shell wtith commands from the batchfile
void RunFromFile(char *file)
{
	signal(SIGINT, sigintHandler);
	FILE *fp;
	fp = fopen (file, "r");
	
	char temp[1024];
	
	if (fp != NULL)
	{
		printf("\nRunning from file: %s\n\n",file);
		while(fgets(temp, 1024, fp) != NULL)
		{
			printInfo();
			printf("%s",temp);
			command(temp );
		}
	}
	else
	{
		// starts the shell as normal if not file is passed it
		printf("No batch file found starting normally\n");
		RunWithoutFile();
	}
	printf("Done reading from file\n");
	fclose(fp);
}