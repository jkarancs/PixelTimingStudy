#!/bin/tcsh

castorls $1 | grep .root | sed 's;/; ;g' | awk '{ print $NF }' | sed 's;_; ;g' | sed 's;\.; ;g'| awk '{ print $(NF-1) }' | sed 's;^000;;' | sed 's;^00;;' | sed 's;^0;;' | sort | uniq >! jobnums.txt
rm SENT.log RECV.log TEST.log

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
