#include <cstdio>

__thread int i;
__thread int j;

int main()
{
	i += j;
}
