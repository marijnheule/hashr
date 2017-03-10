/************ decode.c *** [Marijn Heule] *********/
/* This program converts a satisfying assigngment */
/* in DIMACS format into a list of hexidecimal    */
/* strings, which are supposed to have the same   */
/* hash value.                                    */
/**************************************************/

#include <stdio.h>
#include <stdlib.h>

#define WORDS	3

int main (int argc, char** argv) {
  unsigned int count = 0, c = 0, size = 0;

  int words = WORDS;
  if (argc > 1) words = atoi (argv[1]);

  while (1) {
    int lit;
    int tmp = fscanf (stdin, " %i ", &lit);
    if (tmp == EOF) break;
    if (lit == 0) { printf("\n"); count = 0; }
    else {
      unsigned int b = (lit > 0);
      c |= (b << size);
      size++; }
    if (size == 32) {
      if (count < words) {
        printf("%04x",c);
        count++; }
      c = 0;
      size = 0; }
  }
}
