#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>

#include "myshell.h"

// path used for pwd and environ
char path[1024];

int status;

char helppath[1024]={'\0'};
// used for environ
extern char **environ;

// Stores the name of the file being printed to
char new_file[1024] = {'\0'};

// flag to see if > or >> is inputed
int out_flag = 0;

// Flag to see if < is inputed
int in = 0;

// Used to update SHELL in environ
void PWD(void)
{
	char SHELL[1024];
	strcpy(SHELL, "SHELL=");
	strcat(SHELL, getcwd(NULL,0));
	putenv(SHELL);
}

// sets the starting path so that help can be called from anywhere
void HelpPath (char *a)
{
	getcwd(helppath,sizeof(helppath));
	strcat(helppath,"/readme.txt");
}


// Goes though the buffer and pulls sets the output file to new_file
void search( char *raw)
{
	int j=0;
	for (j=0; j < 1024; j++)
	{
		new_file[j] = '\0';
	}
	
	
	char ch;
	int t=0;
	int place;
	out_flag = 0;
	in =0;
	
	int counter =0;
	for (counter =0; counter < 1024; counter++)
		{
			if (raw[counter]=='>')
			{
				out_flag =1;
				counter++;
				if (raw[counter]=='>')
					{
						out_flag = 2;
						counter++;
					}
				
	  		place=counter+1;
			ch = raw[place];
			while (ch !='\n')
				{
					new_file[t] = raw[place];
					t++;
					place++;
					ch = raw[place];
				}
				new_file[strlen(new_file)] = '\0';
			}
			
			else if(raw[counter]=='<')
			{
				in = 1;
				raw[counter] = raw[counter -1];
			}
		}
}

// Print Function
void printInfo (void)
{
	getcwd(path, sizeof(path));
	printf(ANSI_COLOR_MAGENTA "MyShell:" ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET "$ " ,path);
}


// check the users input for the correct command
void command (char *raw)
{
	// searcher if I/O redirect is needed
	search(raw);
	
	// clr and clear
	if ((strncmp(raw , "clr", 3) == 0) || (strncmp(raw , "clear", 5) == 0))
	{
		printf("\x1B[2J\x1B[1;1H");
	}
	
	
	// pause
	else if (strncmp(raw, "pause", 5) == 0)
	{
		printf("Shell is paused. Press enter to continue ...\n");
		
		system("stty -echo");
		
		char temp = getchar();
		while(temp != '\n')
			{
				temp = getchar();
			}
		
		system("stty echo");
	}
	
	
	// cd
	else if (strncmp(raw, "cd",2)==0)
	{
		changedir(&raw[3]);
	}
	
	
	// dir
	if ((strncmp(raw , "dir", 3) == 0))
	{
		// displays the content of a path provided
		if(&raw[4] != '\0')
		{
			char MyLs[1024];
			snprintf(MyLs,sizeof(MyLs), "ls -al %s", &raw[4]);
			child_run(MyLs);
		}
		
		// if the uses just types "cd"
		else
			{
				char ls[]= "ls -al . " ;
				child_run(ls);
			}
	}


	// environ
	else if (strncmp(raw, "environ", 7)==0 )
	{
		if (out_flag == 1)
		{
			FILE *fp;
			fp = fopen(new_file,"w");
			int i;
			for (i = 0 ; environ[i] != NULL; i++)
			{
				fprintf(fp,"%d %s\n",i,environ[i]);
			}
			fclose(fp);
		}
		else if (out_flag == 2)
		{
		
			FILE *fp;
			fp = fopen(new_file,"a");
			int i;
			for (i = 0 ; environ[i] != NULL; i++)
				{
					fprintf(fp,"%d %s\n",i,environ[i]);
				}
				fclose(fp);
		}
		
		else
		{
			printEnviron();
		}
	}
	
	
	// prints helps
	else if (strncmp(raw, "help",4)==0)
	{
		// make another fork() because the other child_run breaks the input into
		// tokens, but i set the path myself it was easier to make a simple fork command
		
		pid_t pid;
		pid = fork();
			
		if (pid == 0)
		{
			// if > was entered
			if (out_flag == 1)
			{
				freopen(new_file,"w",stdout);
			}
			// if >> was entered
			else if ( out_flag == 2)
			{
				freopen(new_file,"a",stdout);
			}
			
			execlp("cat", "cat", helppath, NULL);
			exit(0);
		}
		else
		{
				waitpid(pid, &status, WUNTRACED);
		}
	}
	
	// when user just hits enter
	else if (raw[0]== '\n')
	{
		// do nothing
		;
	}
	
	
	// quit
	else if (strncmp(raw, "quit",4)==0)
	{
		exit(0);
	}
	
	// everything else
	else
	{
		if (strncmp(raw,"cd",2) !=0)
		{
			child_run(raw);
		}
	}
}



// function for changing directory
void changedir (char *raw /*, char *arg */ )
{
	// if not directory entered prints currnt path, ex "cd"
	if (raw[0]=='\0')
		{
			printf("%s\n", path);
			return;
		}
	else 
		{
			raw[strlen(raw) - 1 ] = '\0';
		}
	
	int i = chdir(raw);
	
	// return 0 if error
	if (i!=0)
		{
			printf("Error changing directory '%s'\n", raw);
		}
		
		// Updates environ
		else
			{
				char *newp;
				newp = malloc(sizeof(getcwd(NULL,0))+5);
				strcpy(newp, "PWD=");
				strcat(newp, getcwd(NULL,0));
				putenv(newp);
			}
}


// print environ
void printEnviron(void)
{
	int i;
	
	for (i = 0 ; environ[i] != NULL; i++)
		{
			printf("%d %s\n",i,environ[i]);
		}
}


// function used to run other commands and to set background commands using &
void child_run(char *raw)
{
	char *args[256] = {'\0'};
	int count = 0;
	int running = 0;
	char *token = strtok(raw, " \t\n");
	
	// parse through and fill array
	while(token != NULL)
	{
		if (strncmp(token,"&",1)==0)
		{
			running = 1;
		}
		else
		{
			args[count] = token;
			count ++;
		}
		
		token = strtok(NULL, " \t\n");
	}

	// set last element to NULL	
	args[count] = NULL;
	
	pid_t pid=0;
	
	pid= fork();

	// Child
	if (pid ==0)
	{
		// if > was entered
		if (out_flag == 1)
		{
			freopen(new_file,"w",stdout);
			int i=0;
			for (i=0; i < 1024;i++)
			{
				if (strncmp(args[i],">",1)==0)
				{
					args[i]= NULL;
					break;
				}
			}
		}
		
		// if >> was entered
		else if ( out_flag == 2)
		{
			freopen(new_file,"a",stdout);
			int i=0;
			for (i=0; i < 1024;i++)
				{
					if (strncmp(args[i],">",1)==0)
					{
						args[i]= NULL;
						break;
					}
				}
		}		
		
		// if not I/O redirection was entered
		execvp(args[0],args);
		exit(0);
	}
	
	// Used to kill any childern that havent died
	if (pid == 0)
	{
		exit(0);
	}
	
	// Parent wait if & was not entered
	else
	{
		if (!running)
		{
			waitpid(pid, &status, WUNTRACED);
		}
	}
}