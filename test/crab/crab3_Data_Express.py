import os
import glob

GT='80X_dataRun2_Express_v7'

from WMCore.Configuration import Configuration
config = Configuration()

PTSdir = os.environ['CMSSW_BASE']+'/src/DPGAnalysis/PixelTimingStudy/'

config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'Run2016Bv1' #can be anything

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
config.Data.inputDataset = '/ExpressPhysics/Run2016B-Express-v1/FEVT'
config.Data.outLFNDirBase = '/store/user/jkarancs/TimingStudy/2016/'
config.Data.outputDatasetTag = 'v3936_INC_SPL1_def_808_'+GT+'_Run2016Bv1'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 2
config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/DCSOnly/json_DCSONLY.txt'
#config.Data.totalUnits = 2
#config.Data.runRange = '273554,273725'

config.section_('Site')
config.Site.storageSite = 'T2_HU_Budapest'
