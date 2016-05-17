import os
import glob

GT='80X_dataRun2_Prompt_v8'

from WMCore.Configuration import Configuration
config = Configuration()

PTSdir = os.environ['CMSSW_BASE']+'/src/DPGAnalysis/PixelTimingStudy/'

config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'TimingScan16_38T_Ext_RECO_ZB3' #can be anything

config.section_('JobType')
config.JobType.allowUndistributedCMSSW = True
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = PTSdir+'test/TimingStudy_RunIIData_80X_cfg.py'
config.JobType.pyCfgParams = ['globalTag='+GT]
config.JobType.inputFiles = [ PTSdir+'portcardmap.dat', PTSdir+'run_ls_instlumi_pileup_2015.txt' ]
config.JobType.outputFiles = ['Ntuple.root']
config.JobType.disableAutomaticOutputCollection = True
#config.JobType.priority = -1

config.section_('Data')
config.Data.inputDataset = '/ZeroBias3/Run2016B-PromptReco-v1/RECO'
#config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Reprocessing/Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON_MuonPhys.txt'
config.Data.outLFNDirBase = '/store/user/jkarancs/TimingStudy/Scans'
config.Data.outputDatasetTag = 'v3936_INC_SPL1_def_808_'+GT+'_TimingScan16_38T_Ext_RECO'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 2
#config.Data.totalUnits = 2
config.Data.runRange = '272761,272762'

config.section_('Site')
config.Site.storageSite = 'T2_HU_Budapest'
