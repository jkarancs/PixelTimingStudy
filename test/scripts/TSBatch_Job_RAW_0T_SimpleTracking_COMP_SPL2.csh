#!/bin/tcsh
##########################################################################
# Script to run TimingStudy on RAW data on an lxbatch node.
# It runs a cmsRun on a specified file and moves the output to EOS:
# /store/caf/user/<username>/...
# Make sure to modify USERDIR (and OUTDIR, TSBatch.csh does it automatically)
# on the bottom of the script
#
# Default settings:
# INCOMPLETE, SPLIT 1, Nstrip > 0
#
# Usage:
# source TS_BatchJob_RAW.csh [CMSSW version] [GlobalTag] [job number] [/store/...] [Nevent = -1]
# example:
# source TS_BatchJob_RAW.csh CMSSW_5_2_3_patch1 GR_R_52_V7 0001 /store/...
##########################################################################
echo
echo "--------------------------------------------------------------------------------"
echo "--------------------------------------------------------------------------------"
echo "                          Creating JOB ["$3"]"
echo

source ${VO_CMS_SW_DIR}/cmsset_default.csh
setenv SCRAM_ARCH slc6_amd64_gcc491
cmsrel $1
cd $1/src
cmsenv
git clone https://github.com/jkarancs/PixelTimingStudy DPGAnalysis/PixelTimingStudy
scram b -j 20
cd DPGAnalysis/PixelTimingStudy

set script="test/TimingStudy_RunIIZeroTeslaRawData_CMSSW7X_cfg.py"

# Set Job Number for output
sed -i "s;Ntuple.root;Ntuple_"$3".root;" $script

# Set SPLIT mode, number of strip hits stored (current: INCOMPLETE, SPLIT 1, Nstrip>0)
sed -i "s;//#define COMPLETE;#define COMPLETE;" plugins/TimingStudy.h
sed -i "s;#define SPLIT 1;#define SPLIT 2;" plugins/TimingStudy.cc
sed -i "s;offlinePrimaryVertices;unsortedOfflinePrimaryVertices;" plugins/TimingStudy.cc
#sed -i "s;#minNStripHits = cms.int32(0),;minNStripHits = cms.int32(0),;" $script

# GlobalTag (auto)
##sed -i "s;GR_R_53_V9F;"$2";" $script

# Input file:
sed -i "s;file:Run200991_ZeroTesla_RAW.root;"$4";" $script

# Set Number of Events if specified (all by default)
if ( $5 ) then
    sed -i "s;input = cms.untracked.int32(100);input = cms.untracked.int32("$5");" $script
else
    sed -i "s;input = cms.untracked.int32(100);input = cms.untracked.int32(-1);" $script
endif

echo
echo "--------------------------------------------------------------------------------"
echo "                                JOB ["$3"] ready"
echo "                                  Compiling..."
echo

cmsenv
scram b
echo
echo "--------------------------------------------------------------------------------"
echo "                                 Compiling ready"
echo "                               Starting JOB ["$3"]"
echo

cmsRun $script

echo
echo "--------------------------------------------------------------------------------"
echo "                               JOB ["$3"] Finished"
echo "                            Writing output to EOS..."
echo

# Copy to Eos
set USERDIR = "userdir"
set OUTDIR = "outdir"
cmsLs /store/caf/user/$USERDIR | grep $OUTDIR > ! checkdir.txt
if ( -z checkdir.txt ) then
    cmsMkdir /store/caf/user/$USERDIR/$OUTDIR
    echo "Created directory on EOS"
endif
rm checkdir.txt

cmsStage Ntuple_$3.root /store/caf/user/$USERDIR/$OUTDIR

echo
echo "Output: "
cmsLs /store/caf/user/$USERDIR/$OUTDIR/Ntuple_$3.root

cd ../../../..
rm -r $1

echo
echo "--------------------------------------------------------------------------------"
echo "                                 JOB ["$3"] DONE"
echo "--------------------------------------------------------------------------------"
echo "--------------------------------------------------------------------------------"



