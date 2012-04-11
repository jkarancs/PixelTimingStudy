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
#   The Job Script saves files to /castor/... so make sure 
#   that you edit it first! (see BSub  tutorial)
#   Or if you have access, you can also save to EOS with the command below
#   eg:
#   cmsStage <file> /store/caf/user/<username>/<dir>
#   I don't know yet how to delegate proxy on batch to copy outside CERN
#   You can try a test task with Create and then
#   run a test job on one file only (see BSub tutorial below)
#
# BSub tutorial:
#   - if a submit/resubmit script (see Usage) suggest sourcing a file view it first!
#   - make a test job out of the first line like in the example
#     - edit test/scripts/TSBatch_Job_RECO.csh
#       replace /castor/cern.ch/user/j/jkarancs/lxplus_run/
#       with your directory, modify directory to be writeable from outside
#	eg:
#       rfmkdir /castor/cern.ch/user/<initial>/<username>/<dir>
#       rfchmod 777 /castor/cern.ch/user/<initial>/<username>/<dir>
#     - do previous step for other scripts in test/scripts
#     - Create a task by doing Step 0,1 of Usage below (it is a working example)
#     - you can replace the cmscaf1nd queue (Step 1) if you don't have access
#       with 8nm or just delete "-q <queue>"
#       Available queues for user: bqueues -u <username>
#     - specify a maximum number of events, eg 10
#     eg:
#     cp test/scripts/TSBatch_Job_RECO.csh test.csh
#     chmod 777 test.csh     #This is needed so lxbatch job can access it
#     bsub -L tcsh test.csh CMSSW_5_2_3_patch1 GR_R_52_V7 0001_test /store/data/Commissioning12/ZeroBias1/RECO/PromptReco-v1/000/190/411/FE4CD0E8-DD80-E111-88F1-5404A638869B.root 10
#
#
# Usage - In Steps 0-6:
#
# Step 0 - Check out PixelTimingStudy
#   eg:
#   setenv SCRAM_ARCH slc5_amd64_gcc462
#   cmsrel CMSSW_5_2_3_patch1
#   cd CMSSW_5_2_3_patch1/src
#   cmsenv
#   mkdir DPGAnalysis
#   cd DPGAnalysis
#   cvs co -d PixelTimingStudy UserCode/Debrecen/PixelTimingStudy
#   cd PixelTimingStudy/
#
#   - This script uses other scripts in the test/scripts directory so stay in the PixelTimingStudy directory
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
#
# Step 1 - Create - TSBatch.csh create [TaskDir] [InputFile] [BatchQueue] [JobScript] [CMSSW_Version] [GlobalTag]
# 
#   source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.csh
#   voms-proxy-init --voms cms -valid 120:00
#   source test/scripts/TSBatch.csh create v3029_523p1_ZB1_PR_v1_RECO_INC_SPL2 input.txt cmscaf1nd test/scripts/TSBatch_Job_RECO.csh CMSSW_5_2_3_patch1 GR_R_52_V7
#
# 
# Step 2 - Submit - TSBatch.csh submit [TaskDir]
#
#   source test/scripts/TSBatch.csh submit v3029_523p1_ZB1_PR_v1_RECO_INC_SPL2
#   > Source this file:
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
#   > Source this file:
#   > source v3029_523p1_ZB1_PR_v1_RECO_INC_SPL2/resub.csh
# 
#
# Step 5 - Resubmit Missing - TSBatch.csh resubmit_missing [TaskDir]
#   * Combines the above two automatically
#
#   source test/scripts/TSBatch.csh resubmit_missing v3029_523p1_ZB1_PR_v1_RECO_INC_SPL2
#   > Source this file:
#   > source v3029_523p1_ZB1_PR_v1_RECO_INC_SPL2/resub.csh
#
#
# Step 6 - Delete - TSBatch.csh delete [TaskDir]
#   - Or you can simply delete the directory now
#
#   source test/scripts/TSBatch.csh delete v3029_523p1_ZB1_PR_v1_RECO_INC_SPL2
#
##########################################################################

if ( $1 == "create" ) then
    if ( -d $2 ) then
	echo "Task "$2" already exists"
    else
	if ( ! -e $3 ) then
	    echo "Input file does not exist"
	else
	    mkdir $2
	    chmod 777 $2
	    echo "--------------------------------------------------------------------------------"
	    echo "Creating Task: "$2
	    echo "Input file: "$3
	    echo "Lxbatch queue: "$4
	    echo "Job Scrip file: "$5
	    echo "CMSSW version: "$6
	    echo "Global Tag: "$7
	    echo
	    cp $3 $2/input.txt
	    cp $5 $2/jobscript.csh
	    chmod 777 $2/jobscript.csh
	    cd $2
	    sed = input.txt | sed 'N;s;\n; ;' | sed '1,9s;^;000;' | sed '10,99s;^;00;' | sed '100,999s;^;0;' | awk '{ print "bsub -q '$4' -L tcsh jobscript.csh '$6' '$7' "$1" "$2 }' > alljobs.csh
	    cd -
	    echo "Task Created"
	    echo "--------------------------------------------------------------------------------"
	endif
    endif
else if ( $1 == "submit" ) then
    echo "Source this file:"
    echo "cd "$2 >! $2/submit.csh
    cat $2/alljobs.csh >> $2/submit.csh
    echo "cd -" >> $2/submit.csh
    echo "source "$2"/submit.csh"
else if ( $1 == "missing" ) then
    set NJOBS = `wc -l $2/alljobs.csh | awk '{ print $1}'`
    source test/scripts/castormis.csh ../lxplus_run $NJOBS
else if ( $1 == "resubmit" ) then
    echo "cd "$2 >! $2/resub.csh
    source test/scripts/resub.csh $2/alljobs.csh $3 >> $2/resub.csh
    echo "cd -" >> $2/resub.csh
    echo "Source this file:"
    echo "source "$2"/resub.csh"
else if ( $1 == "resubmit_missing" ) then
    set NJOBS = `wc -l $2/alljobs.csh | awk '{ print $1}'`
    set LIST = `source test/scripts/castormis.csh ../lxplus_run $NJOBS`
    echo "cd "$2 >! $2/resub.csh
    source test/scripts/resub.csh $2/alljobs.csh $LIST >> $2/resub.csh
    echo "cd -" >> $2/resub.csh
    echo "Source this file:"
    echo "source "$2"/resub.csh"
else if ( $1 == "delete" ) then
    if ( -d $2) then
	rm -r $2
    else
	echo "Task "$2" does not exist"
    endif
else
    echo "Wrong first argument: "
    echo "Usage "
    echo "TSBatch.csh create [TaskDir] [InputFile] [BatchQueue] [JobScript] [CMSSW_Version] [GlobalTag]"
    echo "TSBatch.csh submit [TaskDir]"
    echo "TSBatch.csh missing [TaskDir]"
    echo "TSBatch.csh resubmit [TaskDir] [list]"
    echo "TSBatch.csh resubmit_missing [TaskDir]"
    echo "TSBatch.csh delete [TaskDir]"
endif
