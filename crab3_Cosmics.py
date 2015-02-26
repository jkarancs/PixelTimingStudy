from WMCore.Configuration import Configuration
config = Configuration()

config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'v3836_COMP_SPL2_ns0_732_GR_P_V49_triggers_update4'
config.General.workArea = 'crab_projects'

config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'test/TimingStudy_Cosmics_RecoData_cfg.py'
config.JobType.inputFiles = ['portcardmap.dat']
config.JobType.outputFiles = ['Ntuple.root']

config.section_('Data')
config.Data.outLFN = '/store/user/jkarancs/TimingStudy/2015/'
config.Data.inputDataset = '/ExpressCosmics/Commissioning2015-Express-v1/FEVT'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 50
config.Data.runRange = '235586-235699' #'234798-235175','235177-235506', '235508-235585'
#config.Data.totalUnits = 1
#config.Data.publication = True
config.Data.publishDataName = 'v3836_COMP_SPL2_ns0_732_GR_P_V49_triggers_test'
#config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions12/8TeV/Prompt/Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON.txt'

config.section_('Site')
config.Site.storageSite = 'T2_HU_Budapest'
