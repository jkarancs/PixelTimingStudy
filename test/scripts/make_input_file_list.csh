#!/bin/tcsh

set nonomatch
if ( $1 != "" ) then
    if ( $2 != "" ) then
	dbs search --query "find run, lumi, file where dataset="$1" and (run=`echo $2 | sed 's;,; or run=;g'`)" | grep .root | awk '{ printf "%d %.4d %s\n", $1, $2, $3 }' | sort -k3,3 -u | sort | awk '{ print $3 }'
    else
	dbs search --query "find run, lumi, file where dataset="$1 | grep .root | awk '{ printf "%d %.4d %s\n", $1, $2, $3 }' | sort -k3,3 -u | sort | awk '{ print $3 }'
    endif
else
    echo "Dataset argument missing"
endif
#cvs commit -m "source make_input_file_list.csh 'Dataset(accepts wildcards only in quotes)' runs(comma separated list)" test/scripts/make_input_file_list.csh 
