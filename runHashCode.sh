rm sol.dimacs
rm sol.hex
BASE=0
PRIME=31
NCHAR=10         # number of characters
./HashR $1 -b $BASE -p $PRIME -n $NCHAR -a > encoded.cnf
cat encoded.cnf | grep "characters"
./allsat/core/allsat encoded.cnf -slimit=100 | grep " 0" > sol.dimacs
cat sol.dimacs | ./decode $NCHAR > sol.hex
java PrintHashes > results
cat results
rm sol.dimacs
rm sol.hex
rm encoded.cnf
