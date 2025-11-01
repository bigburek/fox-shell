#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

struct args {
	char** tokens;
	int size;
} typedef args;


args split_line(char* line);
void free_lines(char** tokens, int length);
int lanuch_process(char** args);

int main (void) {
	char* line = NULL;
	size_t len = 0;
	ssize_t nread;

	args arguments;
	int status;	
	do {
		printf("> ");
		nread=getline(&line, &len, stdin);
		if (nread == -1)
		{
			perror("getline");
				return 1;
		}
		
		arguments = split_line(line);
		status = lanuch_process(arguments.tokens);	
		free_lines(arguments.tokens, arguments.size);	
		} while (status);
	free(line);
	return 0;
}

args split_line(char* line) {
	int tokens_size = 4;
	int index = 0;
	char** tokens = malloc(tokens_size *sizeof(char*));
	char* token = strtok(line, " \t\r\n");
	while (token != NULL) {
		if ( index >= tokens_size - 1) {
			tokens_size *= 2;
			char** temporary = realloc (tokens, tokens_size * sizeof(char*));
			//realloc would work in most cases but if it fails and returns NULL it will create a memory leak so instead we make a tmp variable to realloc, 
			//then check for fail and free the memory if it failed. then if it didnt we update the old variable with the new pointers
			if (!temporary) {
				free_lines(tokens, index);
				fprintf(stderr, "bs: allocation error\n");
				exit(EXIT_FAILURE);
			}
			tokens = temporary;
		}
		tokens[index] = strdup(token);
			//also check for failure here and free
			if (!tokens[index]) 
			{
				free_lines(tokens, index); 
				fprintf(stderr, "bs: allocation error\n");
				exit(EXIT_FAILURE);
			}
		//printf("%s %s \n", "Token: ", token);
		index++;
		token = strtok(NULL, " \t\r\n");
	}
	tokens[index] = NULL; //ADDS NULL AT THE END OF ARRAY SO WE CAN ITERATE IT SAFELY
	args a;
	a.tokens = tokens;
	a.size = index;
	return a;
}

void free_lines(char** tokens, int length) {
	for (int i = 0; i < length; i++) free(tokens[i]);
	free(tokens);
}

int lanuch_process(char** args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) { //execvp is a variant of exec(), expects a program name and then a an array of strings
      perror("bs"); //prints system's error msg along with bs
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("bs");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED); //wait till child process is done/terminated
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

