#include <unistd.h>

int main()
{
    return write(0, "Hello World\n", 12);
}
