#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char **argv, char **envp)
{
	(void ) argc;
	while(*argv && *(argv + 1))
	{
		argv++;
		int i = 0;
		while(argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
			i++;


	}
}