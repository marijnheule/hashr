rm sol.dimacs
rm sol.hex
BASE=2147483647
PRIME=16777619
NCHAR=5         # number of characters
./HashR $1 -b $BASE -p $PRIME -n $NCHAR -x > encoded.cnf
cat encoded.cnf | grep "characters"
./allsat/core/allsat encoded.cnf -slimit=1000 | grep " 0" > sol.dimacs
cat sol.dimacs | ./decode $NCHAR > sol.hex
java PrintFNV
