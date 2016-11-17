#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	unsigned long n;

	if (argc != 2 || (n = strtoul(argv[1], NULL, 10)) >= 16) {
		fprintf(stderr, "%s <n=0...15>\n", argv[0]);
		exit(1);		
	}

	switch (n) {
		case 0: printf("0"); break;
		case 1: printf("1"); break;
		case 2: printf("2"); break;
		case 3: printf("3"); break;
		case 4: printf("4"); break;
		case 5: printf("5"); break;
		case 6: printf("6"); break;
		case 7: printf("7"); break;
		case 8: printf("8"); break;
		case 9: printf("9"); break;
		case 10: printf("10"); break;
		case 11: printf("11"); break;
		case 12: printf("12"); break;
		case 13: printf("13"); break;
		case 14: printf("14"); break;
		case 15: printf("15"); break;
	}
	printf("\n");
}
