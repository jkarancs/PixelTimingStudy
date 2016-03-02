import os
import glob

from WMCore.Configuration import Configuration
config = Configuration()

PTSdir = os.environ['CMSSW_BASE']+'/src/DPGAnalysis/PixelTimingStudy/'

config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'TS_MC2015Tune_2232b' #can be anything

config.section_('JobType')
#config.JobType.allowUndistributedCMSSW = True
config.JobType.pluginName = 'PrivateMC'
config.JobType.psetName = PTSdir+'test/TimingStudy_GenNu_DynIneffDB_cfg.py'
config.JobType.inputFiles = [ PTSdir+'portcardmap.dat', PTSdir+'run_ls_instlumi_pileup_2015.txt', PTSdir+'test/siPixelDynamicInefficiency_13TeV_v2.db' ]
config.JobType.outputFiles = ['Ntuple.root']
config.JobType.disableAutomaticOutputCollection = True
#config.JobType.priority = -1
config.JobType.maxMemoryMB = 2500
config.JobType.eventsPerLumi = 500

config.section_('Data')
config.Data.outLFNDirBase = '/store/user/jkarancs/TimingStudy/2015'
config.Data.outputDatasetTag = 'v3836_INC_SPL2_def_763p2_76X_mcRun2_asymptotic_v14_DynIneffDBv2'
config.Data.splitting = 'EventBased'
config.Data.unitsPerJob = 250
config.Data.totalUnits = 500000
config.Data.publication = False

config.section_('Site')
config.Site.storageSite = 'T2_HU_Budapest'
