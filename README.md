# PhaseIPixelNtuplizer
"PixelTimingStudy" plugin to create all purpose Ntuples for detector performance analyses.

### &#x1F539; Recipe to create Monte-Carlo with dynamic innefficiencies (CMSSW_8_0_X):

```bash
export SCRAM_ARCH=slc6_amd64_gcc530
cmsrel CMSSW_8_0_15
cd CMSSW_8_0_15/src
cmsenv
git-cms-addpkg CondTools/SiPixel
git clone https://github.com/jkarancs/PixelTimingStudy DPGAnalysis/PixelTimingStudy
scram b -j 20
mkdir Simulations
cd Simulations
ln -s ../DPGAnalysis/PixelTimingStudy/TimingStudy_GenNu_DynIneffDB_cfg.py .
```

### &#x1F539; Split Cluster Merger
This plugin can be used to find clusters split by double column inefficiencies and restore them. The plugin runs on digis. Examples for using this plugin can be found in test/SplitClusterMerger.

### &#x1F539; Phase-I Pixel Detector
If you are want to generate Ntuples for the Phase-I Pixel Detector, check out this repository (under development...): 
https://github.com/jkarancs/PhaseIPixelNtuplizer

### &#x1F539;
Contact me: adam.hunyadi@cern.ch
