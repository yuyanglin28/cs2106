#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int factorial(int n)
{
  fork();   
    if (n == 0)
	{
            // NOTE the change
        return 1;
    }

    return factorial(n-1) * n;
}

int main()
{
    printf("fac(2) = %d\n", factorial(2));

    return 0;
}
