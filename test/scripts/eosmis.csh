#!/bin/tcsh

set USERDIR = "jkarancs/crab"

eos ls eos/cms/store/caf/user/$USERDIR | sed 's;_; ;g' | sed 's;\.; ;g'| awk '{ print $(NF-1) }' | sed 's;^000;;' | sed 's;^00;;' | sed 's;^0;;' | sort | uniq >! jobnums.txt

set N=1

while ( $N <= $2 )
   set n=`cat jobnums.txt | grep '^'$N'$'`
   if ( .$n == . ) then
       echo -n $N","
   endif
   set N = `expr $N + 1`
end
echo

rm jobnums.txt

