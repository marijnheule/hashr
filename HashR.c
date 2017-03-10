#include <stdio.h>
#include <stdlib.h>

#define NCHAR		   4

//#define FNV

#ifdef FNV
  #define BASE    2147483647
  #define FACTOR    16777619
#else
  #define BASE		   0
  #define FACTOR  	  31
#endif

unsigned int maxVar;
unsigned int zero;
unsigned int nVars, nClauses;

void alphabet (int offset, int printFlag) {
  int i;
  if (printFlag == 0) { nClauses += 13; return; }

  printf("%i 0\n", offset + 6);
  for (i = 7; i < 16; i++) 
    printf("-%i 0\n", offset + i);

  printf("-%i -%i -%i 0\n", offset + 4, offset + 3, offset + 2);

  printf("%i %i %i %i %i 0\n", offset + 4, offset + 3, offset + 2, offset + 1, offset);  // 64 + 96
  printf("-%i -%i %i -%i -%i 0\n", offset + 4, offset + 3, offset + 2, offset + 1, offset);  // 91 + 123
}

void printHelp () {
  printf("usage: hashclash [HASH] [<option> ...]\n\n");
  printf("  -h      prints this help message\n");
  printf("  -n      number of characters (default: %i)\n", NCHAR);
  printf("  -b      base number for hash function (default: %i)\n", BASE);
  printf("  -p      factor (typically prime) for hash function (default: %i)\n", FACTOR);
  printf("  -a      turn on alphabet mode (default: off)\n");
  printf("  -x      turn on XOR mode (default: off)\n");
  exit(0); }

// create 32 new Boolean variables and
// assign redundant ones to false
unsigned int newVector (int size, int printFlag) {
  int i;

  if (printFlag) {
    for (i = size; i < 32; i++)
      printf("-%i 0\n" , maxVar + i); }
  else
    nClauses += 32 - size;

  maxVar += 32;
  return maxVar - 32; }

void setConstant (int offset, int constant, int printFlag) {
  if (printFlag == 0) {
    nClauses += 32; return; }

  int i;
  for (i = 0; i < 32; i++) {
    if (constant & (1 << i)) printf(" %i 0\n", offset + i);
    else                     printf("-%i 0\n", offset + i); } }

unsigned int xor    (int aOffset, int bOffset, int printFlag) {
  unsigned int output = newVector (32, printFlag);

  if (printFlag == 0) {
    nClauses += 32 * 4; return output; }

  int i;
  for (i = 0; i < 32; i++) {
    printf(" %i  %i -%i 0\n", aOffset + i, bOffset + i, output + i);
    printf("-%i  %i  %i 0\n", aOffset + i, bOffset + i, output + i);
    printf(" %i -%i  %i 0\n", aOffset + i, bOffset + i, output + i);
    printf("-%i -%i -%i 0\n", aOffset + i, bOffset + i, output + i); }

  return output;
}

unsigned int add32s (int aOffset, int bOffset, int shift, int printFlag) {
  int i;
  unsigned int carry  = newVector (32, printFlag);
  unsigned int output = newVector (32, printFlag);

  if (printFlag == 0) {
    nClauses += 443 - 11 * shift; return output; }

  for (i = 0; i <= shift; i++) {
    printf("-%i 0\n", carry + i); }

  for (i = shift; i < 31; i++) {
    printf("-%i -%i  %i 0\n", aOffset + i - shift, bOffset + i, carry + i + 1);
    printf("-%i -%i  %i 0\n", aOffset + i - shift,   carry + i, carry + i + 1);
    printf("-%i -%i  %i 0\n", bOffset + i,   carry + i, carry + i + 1);

    printf(" %i  %i -%i 0\n", aOffset + i - shift, bOffset + i, carry + i + 1);
    printf(" %i  %i -%i 0\n", aOffset + i - shift,   carry + i, carry + i + 1);
    printf(" %i  %i -%i 0\n", bOffset + i,   carry + i, carry + i + 1); }

  for (i = 0; i < shift; i++) {
    printf("-%i  %i 0\n", bOffset + i, output + i);
    printf(" %i -%i 0\n", bOffset + i, output + i); }

  for (i = shift; i < 32; i++) {
    printf(" %i  %i  %i -%i 0\n", aOffset + i - shift, bOffset + i, carry + i, output + i);
    printf(" %i -%i -%i -%i 0\n", aOffset + i - shift, bOffset + i, carry + i, output + i);
    printf("-%i  %i -%i -%i 0\n", aOffset + i - shift, bOffset + i, carry + i, output + i);
    printf("-%i -%i  %i -%i 0\n", aOffset + i - shift, bOffset + i, carry + i, output + i);

    printf("-%i -%i -%i  %i 0\n", aOffset + i - shift, bOffset + i, carry + i, output + i);
    printf("-%i  %i  %i  %i 0\n", aOffset + i - shift, bOffset + i, carry + i, output + i);
    printf(" %i -%i  %i  %i 0\n", aOffset + i - shift, bOffset + i, carry + i, output + i);
    printf(" %i  %i -%i  %i 0\n", aOffset + i - shift, bOffset + i, carry + i, output + i); }

  return output; }

unsigned int add32p (int aOffset, int bOffset, int printFlag) {
  return add32s (aOffset, bOffset, 0, printFlag); }

unsigned int mult32 (int offset, int constant, int printFlag) {
  int i;
  unsigned int output = zero;

  for (i = 0; i < 32; i++)
    if (constant & (1 << i)) {
      output = add32s (offset, output, i, printFlag); }

  return output; }

int main (int argc, char** argv) {
  unsigned int i32, o32;
  unsigned int h;

  if (argc < 2) {
    printHelp();
    exit(0);
  }

  int i;
  int xorMode = 0, alphaMode = 0;

  nVars = nClauses = 0;

  int nChar  = NCHAR;
  int base   = BASE;
  int factor = FACTOR;

  int tmp = 0;
  for (i = 1; i < argc; i++) {
    if        (argv[i][0] == '-') {
      if      (argv[i][1] == 'h') printHelp ();
      else if (argv[i][1] == 'n') nChar     = atoi (argv[i+1]);
      else if (argv[i][1] == 'b') base      = atoi (argv[i+1]);
      else if (argv[i][1] == 'p') factor    = atoi (argv[i+1]);
      else if (argv[i][1] == 'a') alphaMode = 1;
      else if (argv[i][1] == 'x') xorMode   = 1; } }

  if (nChar < 1 || nChar > 30) {
    printf("c wrong number of characters, must be in range [1:30]");
    exit(0); }

  unsigned int chars[nChar];

  printf("c hashclash using base %i, factor %i, and %i characters\n", base, factor, nChar);

  int printFlag;
  for (printFlag = 0; printFlag <= 1; printFlag++) {
    maxVar = 1;

    for (i = 0; i < nChar; i++) {
      chars[i] = newVector (16, printFlag);
      if (alphaMode)
        alphabet (chars[i], printFlag);
    }

    zero = newVector ( 0, printFlag);
    i32  = newVector (32, printFlag);

    setConstant (i32, base, printFlag);

    h = i32;
    for (i = 0; i < nChar; i++) {
      h = mult32 (h, factor, printFlag);
      if (xorMode) h = xor    (chars[i], h, printFlag);
      else         h = add32p (chars[i], h, printFlag);
    }

    o32 = 0;
    if (argc > 1) o32 = atoi (argv[1]);

    setConstant (h, o32, printFlag);

    if (printFlag == 0)
      printf("p cnf %i %i\n", maxVar - 1, nClauses);
  }
}
