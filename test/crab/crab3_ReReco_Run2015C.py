import os
import glob

from WMCore.Configuration import Configuration
config = Configuration()

config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'TS_16Dec2015_2015C_25ns' #can be anything

config.section_('JobType')
#config.JobType.allowUndistributedCMSSW = True
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = os.environ['CMSSW_BASE']+'/src/DPGAnalysis/PixelTimingStudy/test/TimingStudy_RunIIRecoData_CMSSW7X_cfg.py'
config.JobType.inputFiles = [
    os.environ['CMSSW_BASE']+'/src/DPGAnalysis/PixelTimingStudy/portcardmap.dat',
    os.environ['CMSSW_BASE']+'/src/DPGAnalysis/PixelTimingStudy/run_ls_instlumi_pileup_2015.txt'
    ]
config.JobType.outputFiles = ['Ntuple.root']
config.JobType.disableAutomaticOutputCollection = True
#config.JobType.priority = -1

config.section_('Data')
config.Data.inputDataset = '/ZeroBias/Run2015C_25ns-16Dec2015-v1/RECO'
config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Reprocessing/Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON_MuonPhys.txt'
config.Data.outLFNDirBase = '/store/user/jkarancs/TimingStudy/2015'
config.Data.outputDatasetTag = 'v3836_INC_SPL0_ns0_pt0p6_100thclu_bestVtx_763p2_76X_dataRun2_v16_Feb05MuonJSON'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 5
#config.Data.totalUnits = 2
#config.Data.runRange = '260373-260426'

config.section_('Site')
config.Site.storageSite = 'T2_HU_Budapest'