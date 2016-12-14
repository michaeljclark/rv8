#include <cstdio>

extern "C" int module_2(int, int);

int main()
{
	int result = module_2(5, 5);
	printf("result=%d\n", result);
}
