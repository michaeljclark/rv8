#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const char *hello_world = "‶Hello World″\n";

int main()
{
    if (write(0, hello_world, strlen(hello_world)) != strlen(hello_world)) exit(9);
    return 0;
}
