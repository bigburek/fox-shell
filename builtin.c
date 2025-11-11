#include "uthash.h"
#include <unistd.h>
#include <stdio.h>


int builtin_cd(char** args);
int builtin_exit(char** args);
int builtin_help(char** args);
int amount_builtins(); 


struct fs_builtin {
	const char* name;
	int (*func)(char**);
	UT_hash_handle hh;
};

//Hashmap init
struct fs_builtin *builtins = NULL;

static struct fs_builtin builtin_table[] = {
    { "cd", builtin_cd },
    { "exit", builtin_exit },
    { "help", builtin_help },
};


int builtin_cd(char** args) {
	if (args[1] == NULL) 
	{
		fprintf(stderr, "fs: no argument provided to [cd]\n");
		fflush(stderr);
	} else {
		if (chdir(args[1]) != 0) {
			perror("fs");
		}
	}
	return 1;
}

int builtin_exit(char** args) {
	printf("exit called\n");
	return 0;
}

int builtin_help(char** args) {
	printf("Fox Shell\n");
	printf("Use 'man' to find information on other programs.\n");
	printf("Commands built in:\n");
	printf("\n");
	for (int i = 0; i<amount_builtins(); i++) {
		printf(" %s [args..]\n", builtin_table[i].name);
	}
	return 1;
}
int builtin_find_execute(char **args) {
    struct fs_builtin *builtin;
    HASH_FIND_STR(builtins, args[0], builtin);
    if (builtin) {
        return builtin->func(args);
    }
    
    return 2;
}

void register_builtins() {
	int builtin_count = amount_builtins();
    for (int i = 0; i < builtin_count; i++) {
        HASH_ADD_KEYPTR(hh,
                        builtins,
                        builtin_table[i].name,
                        strlen(builtin_table[i].name),
                        &builtin_table[i]);
    }
}

int amount_builtins() {
	return sizeof(builtin_table) / sizeof(builtin_table[0]);
}


