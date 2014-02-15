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
set cfg_file = "test/dcol_scan_l1/dcol_0970.py"

echo
echo "--------------------------------------------------------------------------------"
echo "--------------------------------------------------------------------------------"
echo "                          Creating JOB ["$3"]"
echo

echo "source "${VO_CMS_SW_DIR}"/cmsset_default.csh"
source ${VO_CMS_SW_DIR}/cmsset_default.csh
if ( $1 =~ "CMSSW_7_"* ) then
    echo "setenv SCRAM_ARCH slc6_amd64_gcc481"
    setenv SCRAM_ARCH slc6_amd64_gcc481
endif
echo "cmsrel "$1
cmsrel $1
echo "cd "$1"/src"
cd $1/src
echo cmsenv
cmsenv
echo "cp -r /afs/cern.ch/user/j/jkarancs/public/BATCH/CMSSW_7_1_0_pre1/src/* ."
cp -r /afs/cern.ch/user/j/jkarancs/public/BATCH/CMSSW_7_1_0_pre1/src/* .

# git is slow on lxbatch, so just copy from afs
#git cms-addpkg PhysicsTools/Utilities
#git cms-addpkg SimGeneral/MixingModule
#git cms-addpkg SimTracker/SiPixelDigitizer
#git clone https://github.com/jkarancs/PixelTimingStudy DPGAnalysis/PixelTimingStudy
#cp /afs/cern.ch/user/j/jkarancs/public/BATCH/CMSSW_7_1_0_pre1/src/SimTracker/SiPixelDigitizer/plugins/SiPixelDigitizerAlgorithm.h SimTracker/SiPixelDigitizer/plugins/
#cp /afs/cern.ch/user/j/jkarancs/public/BATCH/CMSSW_7_1_0_pre1/src/SimTracker/SiPixelDigitizer/plugins/SiPixelDigitizerAlgorithm.cc SimTracker/SiPixelDigitizer/plugins/
#cp /afs/cern.ch/user/j/jkarancs/public/BATCH/CMSSW_7_1_0_pre1/src/SimTracker/SiPixelDigitizer/plugins/SiPixelDigitizer.h SimTracker/SiPixelDigitizer/plugins/
#cp /afs/cern.ch/user/j/jkarancs/public/BATCH/CMSSW_7_1_0_pre1/src/SimTracker/SiPixelDigitizer/plugins/SiPixelDigitizer.cc SimTracker/SiPixelDigitizer/plugins/
#cp /afs/cern.ch/user/j/jkarancs/public/BATCH/CMSSW_7_1_0_pre1/src/SimGeneral/MixingModule/python/mix_2012_201278_1_cfi.py SimGeneral/MixingModule/python/
#cp /afs/cern.ch/user/j/jkarancs/public/BATCH/CMSSW_7_1_0_pre1/src/SimGeneral/MixingModule/python/mixPoolSource_cfi.py SimGeneral/MixingModule/python/

echo "cd DPGAnalysis/PixelTimingStudy"
cd DPGAnalysis/PixelTimingStudy

echo "setting config file"

# Set Job Number for output
sed -i "s;Ntuple.root;Ntuple_"$3".root;" $cfg_file

# Set SPLIT mode, number of strip hits stored (current: INCOMPLETE, SPLIT 1, Nstrip>0)
#sed -i "s;#define SPLIT 1;#define SPLIT 2;" plugins/TimingStudy.cc

# Set to store all missing hits
#sed -i "s;#minNStripHits = cms.int32(0),;minNStripHits = cms.int32(0),;" $cfg_file

# GlobalTag
sed -i "s;MC_70_V1;"$2";" $cfg_file

# Input file:
sed -i "s;file:/data/store/relval/CMSSW_7_1_0_pre1/GEN-SIM/A4846C0D-0B86-E311-8B2E-003048FEB9EE.root;"$4";" $cfg_file

# Pileup Input file:
sed -i "s;file:/data/store/relval/CMSSW_7_1_0_pre1/GEN-SIM/88CDC2A6-1186-E311-A9F5-02163E00E5C7.root;/store/caf/user/jkarancs/MinBias_TuneZ2star_8TeV_GENSIM_CMSSW_7_1_0pre1/MinBias_8TeV_GEN_SIM_2000k_1_1_FQV.root;" $cfg_file

# Set Number of Events if specified (all by default)
if ( $5 ) then
    sed -i "s;input = cms.untracked.int32(10);input = cms.untracked.int32("$5");" $cfg_file
else
    sed -i "s;input = cms.untracked.int32(10);input = cms.untracked.int32(-1);" $cfg_file
endif

echo
echo "--------------------------------------------------------------------------------"
echo "                                JOB ["$3"] ready"
echo "                                  Compiling..."
echo

echo "cmsenv"
cmsenv
echo "scram b"
scram b
echo
echo "--------------------------------------------------------------------------------"
echo "                                 Compiling ready"
echo "                               Starting JOB ["$3"]"
echo

echo "cmsRun "$cfg_file
echo "..."
echo
cmsRun $cfg_file > & ! out.log; 
sed -e '/TrackNaN:/,+2d' out.log | sed -e '/TrackProducer:/,+7d' | sed -e '/TrackRefitter:TrackRefitter/,+7d' | sed -e '/TrackListMerger:/,+2d'

echo
echo "--------------------------------------------------------------------------------"
echo "                               JOB ["$3"] Finished"
echo "                            Writing output to EOS..."
echo

# Copy to Eos
set USERDIR = "userdir"
set OUTDIR = "outdir"
echo "cmsLs /store/caf/user/"$USERDIR" | grep "$OUTDIR
cmsLs /store/caf/user/$USERDIR | grep $OUTDIR > ! checkdir.txt
cat checkdir.txt
if ( -z checkdir.txt ) then
    cmsMkdir /store/caf/user/$USERDIR/$OUTDIR
    echo "Created directory on EOS"
endif
rm checkdir.txt

echo "cmsStage Ntuple_"$3".root /store/caf/user/"$USERDIR"/"$OUTDIR
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



