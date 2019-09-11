#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main()
{
  int result;
  int var = 123;
  result = fork();
  if (result != 0){
    printf("Parent: Var is %i\n", var);
    var++;
    printf("Parent: Var is %i\n", var);
} else {

printf("Child: Var is %i\n", var);
var--;
printf("Child: Var is %i\n", var);
}
}
