#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

struct prova
{
  unsigned char n : 5;
};

void main()
{
  struct prova p;
  p.n = 11;
  int8_t n = 17;

  printf("char %c\n", p.n);

  if (p.n == n)
    printf("si\n");
  else
    printf("no\n");
}
