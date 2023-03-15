#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int i;

    i = access(argv[1], X_OK);
    perror("J");
    printf("result: %i\n", i);
}