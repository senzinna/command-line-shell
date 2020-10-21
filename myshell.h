// My functions

void printInfo(void);
void command(char *raw);
void changedir(char *raw);
void printEnviron(void);
void myDir(char *raw);
void child_run(char *raw);
void RunFromFile(char *argv);
void RunWithoutFile(void);
void Search(char *raw);
void HelpPath(char *arg);
void PWD(void);

// colors
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
