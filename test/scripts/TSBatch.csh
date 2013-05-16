#!/bin/tcsh
##########################################################################
# Creator: Janos Karancsi - 2012/04/11
#   This is a Script that creates jobs to run TimingStudy on the Lxbatch.
#   it uses various other scripts in the test/scripts directory.
#   Make sure to use this on the lxplus in a directory accessible outside!
#   Jobs are submitted via the bsub command - some experience helps
#   The script is foolproof, it doesn't submit anything but
#   produces other scripts in the task directory that you can source
#   Feel free to try it!
#
# Job info: See test/scripts/TSBatch_Job_RECO.csh file
#   The Job Script moves output files to EOS:
#   /store/caf/user/<username>/...
#   So make sure that you edit USERDIR first! (see BSub  tutorial)
#   I don't know yet, how to delegate proxy on lxbatch to save to other SE,
#   but there is an option to copy output from EOS to T2_HU_Budapest
#   You can try a test task with Create and then
#   run a test job on one file only (see BSub tutorial below)
#
# BSub tutorial:
#   - if a submit/resubmit script (see Usage) suggest sourcing a file view it first!
#   - make a test job out of the first line like in the example
#     - edit test/scripts/TSBatch_Job_RECO.csh
#       replace the USERDIR string ("jkarancs/crab") with your username
#     - do previous step for test/scrips/eosmis.csh and copy_to_kfki.csh as well,
#     - Create a task by doing Step 0,1 of Usage below (it is a working example)
#     - you can replace the cmscaf1nd queue (Step 1) if you don't have access
#       with 8nm or just delete "-q <queue>" equivalently
#       Available queues for user: bqueues -u <username>
#     - specify a maximum number of events, eg 10
#     eg:
#     cp test/scripts/TSBatch_Job_RECO.csh test.csh
#     chmod 777 test.csh     #This is needed so lxbatch job can access it
#     bsub -L tcsh test.csh CMSSW_5_2_3_patch1 GR_R_52_V7 0001_test /store/data/Commissioning12/ZeroBias1/RECO/PromptReco-v1/000/190/411/FE4CD0E8-DD80-E111-88F1-5404A638869B.root 10
#    - The above steps are done in the test.csh script of the submit option (Step 2)
#
# Usage - In Steps 0-6:
#
# Step 0 - Check out PixelTimingStudy (or just the test directory like below)
#   eg:
#   cmsenv      #in any CMSSW release area
#   cvs co -d test UserCode/Debrecen/PixelTimingStudy/test
#
#   - This script uses other scripts in the test/scripts directory so stay in the current directory
#   - Create a list of input files that are accessible within CERN (T2/T1/T0_CERN_CH, T2_CH_CAF etc)
#   - Put it inside a file like input.txt
#   eg:
#   dbs search --query "find file where dataset=/ZeroBias1/Commissioning12-PromptReco-v1/RECO and run = 190411" | grep .root > input.txt; 
#   dbs search --query "find file where dataset=/ZeroBias1/Run2012A-PromptReco-v1/RECO and run = 190456" | grep .root >> input.txt; 
#   dbs search --query "find file where dataset=/ZeroBias1/Run2012A-PromptReco-v1/RECO and run = 190538" | grep .root >> input.txt; 
#   dbs search --query "find file where dataset=/ZeroBias1/Run2012A-PromptReco-v1/RECO and run = 190539" | grep .root >> input.txt; 
#   dbs search --query "find file where dataset=/ZeroBias1/Run2012A-PromptReco-v1/RECO and run = 190591" | grep .root >> input.txt; 
#   dbs search --query "find file where dataset=/ZeroBias1/Run2012A-PromptReco-v1/RECO and run = 190592" | grep .root >> input.txt; 
#   dbs search --query "find file where dataset=/ZeroBias1/Run2012A-PromptReco-v1/RECO and run = 190593" | grep .root >> input.txt; 
#   dbs search --query "find file where dataset=/ZeroBias1/Run2012A-PromptReco-v1/RECO and run = 190595" | grep .root >> input.txt
#
#   A trick to order input files by run and lumisections and use a list of runs from more datasets:
#   dbs search --query "find run, lumi, file where dataset=/ZeroBias*/Run2012A-v1/RAW and (run=190538 or run=190539 or run=190591 or run=190592 or run=190593 or run=190595)" | grep .root | awk '{ printf "%d %.4d %s\n", $1, $2, $3 }' | sort -k3,3 -u | sort | awk '{ print $3 }' > input.txt
#
# Step 1 - Create - TSBatch.csh create [TaskDir] [InputFile] [BatchQueue] [JobScript] [CMSSW_Version] [GlobalTag]
# 
#   source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.csh
#   voms-proxy-init --voms cms -valid 120:00
#   <Grid pass phrase>
#   source test/scripts/TSBatch.csh create v3029_INC_SPL0_442p2_MB0TN_Run2011A_PR_RECO_HV5 input.txt cmscaf1nd test/scripts/TSBatch_Job_RECO.csh CMSSW_4_4_2_patch2 GR_R_44_V15
#
# 
# Step 2 - Submit - TSBatch.csh submit [TaskDir]
#
#   source test/scripts/TSBatch.csh submit v3029_523p1_ZB1_PR_v1_RECO_INC_SPL2
#   > source v3029_523p1_ZB1_PR_v1_RECO_INC_SPL2/submit.csh
#
#
# Step 3 - Missing * - TSBatch.csh missing [TaskDir]
#
#   source test/scripts/TSBatch.csh missing v3029_523p1_ZB1_PR_v1_RECO_INC_SPL2
#   > 21,105,165,345,524,
#
#
# Step 4 - Resubmit * - TSBatch.csh resubmit [TaskDir] [list]
#
#   source test/scripts/TSBatch.csh resubmit v3029_523p1_ZB1_PR_v1_RECO_INC_SPL2 21,105,165,345,524
#   > source v3029_523p1_ZB1_PR_v1_RECO_INC_SPL2/resub.csh
# 
#
# Step 5 - Resubmit Missing - TSBatch.csh resubmit_missing [TaskDir]
#   * Combines the above two automatically
#
#   source test/scripts/TSBatch.csh resubmit_missing v3029_523p1_ZB1_PR_v1_RECO_INC_SPL2
#   > source v3029_523p1_ZB1_PR_v1_RECO_INC_SPL2/resub.csh
#
#
# Step 6 - Copy to KFKI - TSBatch.csh copy_to_kfki [TaskDir]
#
#   source test/scripts/TSBatch.csh copy_to_kfki v3029_523p1_ZB1_PR_v1_RECO_INC_SPL2
#   > source v3029_523p1_ZB1_PR_v1_RECO_INC_SPL2/copy_to_kfki.csh
#
#
# Step 7 - Delete - TSBatch.csh delete [TaskDir]
#   - Or you can simply delete the directory now
#
#   source test/scripts/TSBatch.csh delete v3029_523p1_ZB1_PR_v1_RECO_INC_SPL2
#
##########################################################################

set USERDIR_EOS = "jkarancs/crab"
set USERDIR_KFKI = "jkarancs"


echo "Usage: ">! Usage
echo "TSBatch.csh [TaskDir] -create [InputFile] [BatchQueue] [JobScript] [CMSSW_Version] [GlobalTag]">> Usage
echo "TSBatch.csh [TaskDir] -submit">> Usage
echo "TSBatch.csh [TaskDir] -status">> Usage
echo "TSBatch.csh [TaskDir] -missing">> Usage
echo "TSBatch.csh [TaskDir] -resubmit [list]">> Usage
echo "TSBatch.csh [TaskDir] -resubmit_missing">> Usage
echo "TSBatch.csh [TaskDir] -copy_to_kfki">> Usage
echo "TSBatch.csh [TaskDir] -delete">> Usage


if ( "$1" == "" ) then
    echo "No Task Directory Specified"
    cat Usage
    rm Usage
    exit
else
    set TASKDIR = $1
endif

if ( "$2" == "" ) then
    echo "No Option specified"
    cat Usage
    rm Usage
    exit
else
    set OPT = $2
endif

if ( ! -d $TASKDIR ) then
    if ( "$OPT" != "-create" ) then
	echo "Task Directory does not exist"
	rm Usage
	exit
    endif
else
    if ( "$OPT" == "-create" ) then
	echo "Task "$TASKDIR" already exists"
	rm Usage
	exit
    else
	set NJOBS = `wc -l $TASKDIR/alljobs.csh | awk '{ print $1}'`
    endif
endif

if ( "$OPT" == "-create" ) then
    if ( ! -e $3 ) then
	echo "Input file does not exist"
    else
        echo "--------------------------------------------------------------------------------"
        mkdir $TASKDIR
        chmod 777 $TASKDIR
        echo "Task Name:       "$TASKDIR
        cp $3 $TASKDIR/input.txt
        echo "Input file:      "$3
        set NJOBS = `wc -l $TASKDIR/input.txt | awk '{ print $1}'`
        echo "Number of Jobs:  "$NJOBS
        echo "Lxbatch queue:   "$4
        sed "s;outdir;"$TASKDIR";" $5 > $TASKDIR/jobscript.csh
        chmod 777 $TASKDIR/jobscript.csh
        echo "Job Script file: "$5
        echo "CMSSW version:   "$6
        echo "Global Tag:      "$7
        echo
        cd $TASKDIR
        mkdir STDOUT
        chmod 666 STDOUT
        sed = input.txt | sed 'N;s;\n; ;' | awk '{ printf "%.4d %s\n", $1, $2 }' | awk '{ print "bsub -J '$TASKDIR'_JOB"$1" -oo STDOUT/JOB_"$1".log -q '$4' -L tcsh jobscript.csh '$6' '$7' "$1" "$2 }' > alljobs.csh
        cd -
        echo "Task Created"
        echo "--------------------------------------------------------------------------------"
    endif
else if ( "$OPT" == "-submit" ) then
    echo "cd "$TASKDIR >! $TASKDIR/submit.csh
    cat $TASKDIR/alljobs.csh >> $TASKDIR/submit.csh
    echo "cd -" >> $TASKDIR/submit.csh
    echo "cd "$TASKDIR >! $TASKDIR/test.csh
    head -1 $TASKDIR/alljobs.csh | sed "s;-q cmscaf1nd ;;" | sed 's;$; 10;' | sed "s;0001;test;;s;0001;test;" >> $TASKDIR/test.csh
    echo "cd -" >> $TASKDIR/test.csh
    echo "source "$TASKDIR"/submit.csh"
    echo "Or you can try this test script:"
    echo "source "$TASKDIR"/test.csh"
else if ( "$OPT" == "-status" ) then
    bjobs -J "$TASKDIR*" >! taskjobs
    grep PEND taskjobs >! Pending
    grep RUN taskjobs >! Running
    set PEND=`wc -l Pending | awk '{ print $1}'`
    set RUN=`wc -l Running | awk '{ print $1}'`
    eos ls eos/cms/store/caf/user/$USERDIR_EOS/$1 | grep .root >! Completed
    lcg-ls -b -D srmv2 --vo cms srm://grid143.kfki.hu:8446/srm/managerv2\?SFN=/dpm/kfki.hu/home/cms/phedex/store/user/$USERDIR_KFKI/$1 | grep .root | sed "s;/; ;g" | awk '{ print $NF }' >> Completed
    sort -u Completed >! Comp
    mv Comp Completed
    set COMP=`wc -l Completed | awk '{ print $1}'`
    ls -l $1/STDOUT | grep -v test | grep .log >! Done
    set DONE=`wc -l Done | awk '{ print $1}'`
    rm taskjobs Pending Running Completed Done
    echo "Status of Task "$1" ("$NJOBS" Jobs):"
    echo "Jobs - Pending                : "$PEND
    echo "     - Running                : "$RUN
    echo "     - Done (with STDOUT)     : "$DONE
    echo "------------------------------------"
    echo "     - Completed (has output) : "$COMP
    if ( $DONE > $COMP ) then
        ls -l $TASKDIR/STDOUT | grep -v test | grep .log | awk '{ print "'$TASKDIR'/STDOUT/"$NF }' >! STDOUT_list
        echo -n "" >! quota_list
        set QUOTA = 0
        foreach a ( `cat STDOUT_list` )
            if ( `grep "Unable to access quota space" $a` != "" ) then
                echo $a | sed "s;_; ;g;s;\.; ;" | awk '{ printf "%d,", $(NF-1) }' >> quota_list
        	set QUOTA = `expr $QUOTA + 1`
            endif
        end
        set RESUB_LIST = `sed 's;,$;;' quota_list`
        if ( $QUOTA > 0 ) then
            echo "     - Disk quota problem     : "$QUOTA
            echo "       Free up space on EOS using -move_to_kfki"
            echo "       Then issue -resubmit "$RESUB_LIST
        endif
        rm STDOUT_list quota_list
    endif
else if ( "$OPT" == "-missing" ) then
    lcg-ls -b -D srmv2 --vo cms srm://grid143.kfki.hu:8446/srm/managerv2\?SFN=/dpm/kfki.hu/home/cms/phedex/store/user/$USERDIR_KFKI/$TASKDIR | grep .root | sed "s;/; ;g" | awk '{ print $NF }' > ! output
    eos ls eos/cms/store/caf/user/$USERDIR_EOS/$TASKDIR | grep .root >> output
    cat output | sort -u | sed 's;_; ;g;s;\.; ;g' | awk '{ printf "%d\n", $(NF-1) }' >! jobnums
    seq 1 $NJOBS >! Seq
    diff Seq jobnums | grep "<" | awk '{ printf "%d,", $2 }' | sed 's;,$;\n;'
    rm Seq jobnums output
else if ( "$OPT" == "-resubmit" ) then
    if ( $3 == "" ) then
	echo "No Jobs specified"
    else
	echo "cd "$TASKDIR >! $TASKDIR/resub.csh
	echo $3 | tr ',' '\n' >! list
	echo "" >! list2
	foreach a ( `cat list` )
	    echo $a | grep -v "-" >> list2
	    eval `echo $a | grep - | sed 's;^;seq ;;s;-; ;'` >> list2
	end
	foreach a ( `cat list2` )
	    sed -n $a'p' $TASKDIR/alljobs.csh >> $TASKDIR/resub.csh
	end
	rm list list2
	echo "cd -" >> $TASKDIR/resub.csh
	echo "source "$TASKDIR"/resub.csh"
    endif
else if ( "$OPT" == "-resubmit_missing" ) then
    lcg-ls -b -D srmv2 --vo cms srm://grid143.kfki.hu:8446/srm/managerv2\?SFN=/dpm/kfki.hu/home/cms/phedex/store/user/$USERDIR_KFKI/$1 | grep .root > ! output
    eos ls eos/cms/store/caf/user/$USERDIR_EOS/$1 | grep .root >> output
    cat output | sort -u | sed 's;_; ;g;s;\.; ;g' | awk '{ printf "%d\n", $(NF-1) }' >! jobnums
    seq 1 $NJOBS >! Seq
    diff Seq jobnums | grep "<" | awk '{ print $2 }' >! Missing
    echo "cd "$TASKDIR >! $TASKDIR/resub.csh
    foreach a ( `cat Missing` )
	cat $TASKDIR/alljobs.csh | head -$a | tail -1 >> $TASKDIR/resub.csh
    end
    rm Seq jobnums output Missing
    echo "cd -" >> $TASKDIR/resub.csh
    echo "source "$TASKDIR"/resub.csh"
else if ( "$OPT" == "-move_to_kfki" ) then
    eos ls -l eos/cms/store/caf/user/$USERDIR_EOS/$TASKDIR | grep .root | awk '{ print $5" "$NF }' >! eos_list
    lcg-ls -l -b -D srmv2 --vo cms srm://grid143.kfki.hu:8446/srm/managerv2\?SFN=/dpm/kfki.hu/home/cms/phedex/store/user/$USERDIR_KFKI/$TASKDIR | grep .root | sed "s;/; ;g" | awk '{ print $5" "$NF }' | sort -k 2 >! kfki_list
    diff eos_list kfki_list | grep "<" | awk '{ print $3 }' | sort -k 2 >! to_move_list
    echo -n "" >! safe_to_delete_from_eos
    foreach a ( `cat eos_list | awk '{ print $2 }'` )
	grep $a kfki_list | awk '{ print "eos rm eos/cms/store/caf/user/'$USERDIR_EOS'/'$TASKDIR'/"$2 }' >> safe_to_delete_from_eos
    end
    echo -n "" >! kfki_filesize_mismatch
    foreach a ( `cat to_move_list` )
	grep $a kfki_list | awk '{ print "lcg-del -l -D srmv2 --vo cms srm://grid143.kfki.hu:8446/srm/managerv2\\?SFN=/dpm/kfki.hu/home/cms/phedex/store/user/'$USERDIR_KFKI'/'$TASKDIR'/"$2 }' >> kfki_filesize_mismatch
    end
    cat safe_to_delete_from_eos >! $TASKDIR/move_to_kfki.csh
    cat kfki_filesize_mismatch >> $TASKDIR/move_to_kfki.csh
    awk '{ print "lcg-cp -v -b -D srmv2 --vo cms srm://srm-eoscms.cern.ch:8443/srm/v2/server\\?SFN=/eos/cms/store/caf/user/'$USERDIR_EOS'/'$TASKDIR'/"$1" srm://grid143.kfki.hu:8446/srm/managerv2\\?SFN=/dpm/kfki.hu/home/cms/phedex/store/user/'$USERDIR_KFKI'/'$TASKDIR'/"$1 }' to_move_list >> $TASKDIR/move_to_kfki.csh
    echo "source "$TASKDIR"/move_to_kfki.csh"
    rm eos_list kfki_list to_move_list safe_to_delete_from_eos kfki_filesize_mismatch
else if ( "$OPT" == "-delete" ) then
    rm -r $TASKDIR
else
    echo "Wrong Option"
    cat Usage
    rm Usage
    exit
endif

rm Usage
eval `ls -l | grep RECV.log | awk '{ print "rm "$NF }'`
eval `ls -l | grep TEST.log | awk '{ print "rm "$NF }'`
eval `ls -l | grep SENT.log | awk '{ print "rm "$NF }'`
