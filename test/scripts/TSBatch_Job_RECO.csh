#!/bin/tcsh
##########################################################################
# Script to run TimingStudy on RECO data on an lxbatch node.
# It runs a cmsRun on a specified file and moves the output to EOS:
# /store/caf/user/<username>/...
# Make sure to modify USERDIR (and OUTDIR, TSBatch.csh does it automatically)
# on the bottom of the script
#
# Default settings:
# INCOMPLETE, SPLIT 1, Nstrip > 0
#
# Usage:
# source TS_BatchJob_RECO.csh [CMSSW version] [GlobalTag] [job number] [/store/...] [Nevent = -1]
# example:
# source TS_BatchJob_RECO.csh CMSSW_5_2_3_patch1 GR_R_52_V7 0001 /store/...
##########################################################################
echo
echo "--------------------------------------------------------------------------------"
echo "--------------------------------------------------------------------------------"
echo "                          Creating JOB ["$3"]"
echo

if ( $?VO_CMS_SW_DIR ) then
    source ${VO_CMS_SW_DIR}/cmsset_default.csh
endif
setenv SCRAM_ARCH slc6_amd64_gcc530
cmsrel $1
cd $1/src
cmsenv
git clone https://github.com/jkarancs/PixelTimingStudy DPGAnalysis/PixelTimingStudy
cd DPGAnalysis/PixelTimingStudy

# Set SPLIT mode (current: INCOMPLETE, SPLIT 1)
#sed -i "s;#define SPLIT 1;#define SPLIT 2;" plugins/TimingStudy.cc

# output file
set output="Ntuple_"$3".root"

echo
echo "--------------------------------------------------------------------------------"
echo "                                JOB ["$3"] ready"
echo "                                  Compiling..."
echo

scram b
cmsenv
echo
echo "--------------------------------------------------------------------------------"
echo "                                 Compiling ready"
echo "                               Starting JOB ["$3"]"
echo

if ( $#argv > 4 ) then
    echo "cmsRun test/TimingStudy_RunIIData_80X_cfg.py globalTag=$2 outputFileName=$output inputFileName=$4 maxEvents=$5\n"
    cmsRun test/TimingStudy_RunIIData_80X_cfg.py globalTag=$2 outputFileName=$output inputFileName=$4 maxEvents=$5
else
    echo "cmsRun test/TimingStudy_RunIIData_80X_cfg.py globalTag=$2 outputFileName=$output inputFileName=$4 maxEvents=-1\n"
    cmsRun test/TimingStudy_RunIIData_80X_cfg.py globalTag=$2 outputFileName=$output inputFileName=$4 maxEvents=-1
endif

echo
echo "--------------------------------------------------------------------------------"
echo "                               JOB ["$3"] Finished"
echo "                            Writing output to EOS..."
echo

# Copy to Eos
set USERDIR = "userdir"
set OUTDIR = "outdir"
eos mkdir -p eos/cms/store/caf/user/$USERDIR/$OUTDIR
cmsStage $output /store/caf/user/$USERDIR/$OUTDIR/$output

echo
echo "Output: "
eos ls -l eos/cms/store/caf/user/$USERDIR/$OUTDIR/$output

cd ../../../..
rm -rf $1

echo
echo "--------------------------------------------------------------------------------"
echo "                                 JOB ["$3"] DONE"
echo "--------------------------------------------------------------------------------"
echo "--------------------------------------------------------------------------------"



