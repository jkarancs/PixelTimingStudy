from WMCore.Configuration import Configuration
config = Configuration()

config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'v3836_COMP_SPL2_ns0_5326_GR_P_V49'
config.General.workArea = 'crab_projects'

config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'test/TimingStudy_Cosmics_RawData_CMSSW53X_cfg.py'
config.JobType.inputFiles = ['portcardmap.dat']
config.JobType.outputFiles = ['Ntuple.root']

config.section_('Data')
config.Data.outLFN = '/store/user/jkarancs/TimingStudy/2015/'
config.Data.inputDataset = '/Cosmics/Commissioning12-v1/RAW'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1 # 50 - for RECO
config.Data.runRange = '185878-185894,186147-186264'
#config.Data.totalUnits = 1
#config.Data.publication = True
config.Data.publishDataName = 'v3836_COMP_SPL2_ns0_5326_GR_P_V49_CRUZET_Peak_runs'
#config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions12/8TeV/Prompt/Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON.txt'

config.section_('Site')
config.Site.storageSite = 'T2_HU_Budapest'
