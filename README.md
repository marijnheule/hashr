HashR is a generator of strings that have
the same hash value for a given hash function.

Install: ./build.sh

The installation script will compile five 
programs:
- hashr.c : given a hash function and a 
  hash value, hashr will produce a SAT 
  formula (in the DIMACS format) which 
  satisfying assimentments result in the given 
  hash value.
- allsat : this program is a modification of 
  the MiniSAT solver (version 2.2). This 
  modification enables the solver to generate 
  *all* solutions for the input formula (in 
  DIMACS format). Since the number of solutions 
  can be huge, the solver has a new parameter 
  which speficies after how many solutions it 
  should terminate (default: 1000 solutions). 
- decode.c : this program will transform
  satisfying assignments in the DIMACS 
  format to a file of hexidecimal numbers
  (one line per assignment). Each group of
  four hex numbers represent a single Java
  character (16 bits).  
- PrintHashes.java : this program transforms
  a list of hex numbers into a string and
  computes the hashCode for that string using
  the default Java hash function (base = 0, 
  factor = 31).
- PrintFNV.java : this program transforms
  a list of hex numbers into a string and
  computes the hashCode for that string using
  the stronger Fowler-Noll-Vo hash function 
  (base = 2147483647, factor = 16777619, xor).

Example:

  After installation, HashR can be tested
  using the following two scripts:

  ./runHashCode.sh  1234567890
  ./runFNV.sh       1234567890

  The number of characters is set to 5 and can
  easily be changed by editing the scripts. 
