#include <stdio.h>

int main(int argc, char** argv, char** envp)
{
	/* print command line arguments */
	printf("argc=%d\n", argc);
	for (int i = 0; i < argc; i++) {
		printf("argv[%d]=%s\n", i, argv[i]);
	}

	/* print environment */
	printf("envp=%p\n", envp);
	if (!envp) goto out;
	for (char** env = envp; *env != 0; env++) {
		printf("env: %s\n", *env);
	}

out:
	return 0;
}
