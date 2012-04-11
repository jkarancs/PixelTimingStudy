#!/bin/tcsh

echo $2 | tr ',' '\n'| grep . > ! missing_jobs.txt

foreach a ( `cat missing_jobs.txt` )
    cat $1 | head -$a | tail -1
end

rm missing_jobs.txt

