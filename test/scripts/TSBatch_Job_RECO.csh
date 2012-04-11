#!/bin/tcsh
##########################################################################
# Script to run TimingStudy on RECO data on an lxbatch node.
# It runs a cmsRun on a specified file and saves the output to Castor on:
# /castor/cern.ch/user/j/jkarancs/lxplus_run/
#
# Default settings:
# INCOMPLETE, SPLIT 2, Nstrip > 0
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

source ${VO_CMS_SW_DIR}/cmsset_default.csh
if ( $1 =~ "CMSSW_5_"* ) then
    setenv SCRAM_ARCH slc5_amd64_gcc462
else
    setenv SCRAM_ARCH slc5_amd64_gcc434
endif
cmsrel $1
cd $1/src
cmsenv
mkdir DPGAnalysis
cd DPGAnalysis
cvs co -d PixelTimingStudy UserCode/Debrecen/PixelTimingStudy
cd PixelTimingStudy/
sed -i "s;#define SPLIT 0;#define SPLIT 2;" plugins/TimingStudy.cc
sed -i "s;MB_RECO.root;MB_RECO_"$3".root;" test/MB_RECO_cfg.py
sed -i "s;#minNStripHits = cms.int32(0),;minNStripHits = cms.int32(0),;" test/MB_RECO_cfg.py
sed -i "s;GR_R_52_V7;"$2";" test/MB_RECO_cfg.py
sed -i "s;fileNames = cms.untracked.vstring(;fileNames = cms.untracked.vstring(\'"$4"\';" test/MB_RECO_cfg.py
if ( $5 ) then
    sed -i "s;input = cms.untracked.int32(-1);input = cms.untracked.int32("$5");" test/MB_RECO_cfg.py
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

cmsRun test/MB_RECO_cfg.py | tee JOB_$3.log

echo
echo "--------------------------------------------------------------------------------"
echo "                               JOB ["$3"] Finished"
echo "                            Writing output to Castor..."
echo

# Copy to Castor
rfcp MB_RECO_$3.root /castor/cern.ch/user/j/jkarancs/lxplus_run/
rfcp JOB_$3.log /castor/cern.ch/user/j/jkarancs/lxplus_run/LOG/
rfdir /castor/cern.ch/user/j/jkarancs/lxplus_run/ | grep MB_RECO_$3.root
rfdir /castor/cern.ch/user/j/jkarancs/lxplus_run/LOG/ | grep JOB_$3.log

cd ../../../..
rm -r $1

echo
echo "--------------------------------------------------------------------------------"
echo "                                 JOB ["$3"] DONE"
echo "--------------------------------------------------------------------------------"
echo "--------------------------------------------------------------------------------"



