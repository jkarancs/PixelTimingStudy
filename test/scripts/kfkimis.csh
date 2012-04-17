#!/bin/tcsh

set USERDIR = "jkarancs"

lcg-ls -b -D srmv2 --vo cms srm://grid143.kfki.hu:8446/srm/managerv2\?SFN=/dpm/kfki.hu/home/cms/phedex/store/user/$USERDIR/$1 | grep .root | sed 's;_; ;g' | sed 's;\.; ;g'| awk '{ print $(NF-1) }' | sed 's;^000;;' | sed 's;^00;;' | sed 's;^0;;' | sort | uniq >! jobnums.txt

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

