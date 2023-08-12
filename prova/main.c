#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

struct prova 
{
   int8_t n : 5;
}; 

void main () {
   struct prova p; 
   p.n = 17;
   int8_t n = 17;

   if ((p.n & 0b00011111) == n)
      printf("si\n");
   else 
      printf("no\n");
}


