#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int factorial(int n)
{
    fork();       // NOTE the change
    if (n == 0)
	{
        return 1;
    }
    return factorial(n-1) * n;
}

int main()
{
    int n;
    printf ("Input n: ");
    scanf("%d", &n);
    printf("fac(%d) = %d\n", n, factorial(n));

    return 0;
}
