#------------------------------------------
#  Options - can be given from command line
#------------------------------------------
import FWCore.ParameterSet.VarParsing as opts

opt = opts.VarParsing ('analysis')

opt.register('era',                '',
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
	     'Choose from: 2011_53X_ReReco, 2012_53X_ReReco, 2012_76X_PrivateReco, 2015_76X_ReReco, 2016_80X_ReReco, 2016_80X_PromptReco')

opt.register('dataTier',           'RAW',
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
	     'Input data tier: RAW, RECO, FEVT or AOD')

opt.register('inputFile',          '',
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
	     'input file name')

opt.register('secondaryInputFile', '',
	     opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
	     'input file name')

### Events to process: 'maxEvents' is already registered by the framework
opt.setDefault('maxEvents', 200)

# Proceed with settings from command line
opt.parseArguments()

import FWCore.ParameterSet.Config as cms
import sys

if opt.era in ["2011_53X_ReReco", "2012_53X_ReReco", "2012_76X_PrivateReco"]:
    process = cms.Process('NTUPLE')
    
elif opt.era == "2015_76X_ReReco":
    from Configuration.StandardSequences.Eras import eras
    process = cms.Process('NTUPLE',eras.Run2_25ns)
    
elif opt.era in ["2016_80X_ReReco", "2016_80X_PromptReco"]:
    from Configuration.StandardSequences.Eras import eras
    process = cms.Process('NTUPLE',eras.Run2_25ns)
    
else:
    print "Wrong era given: "+str(opt.era)
    sys.exit()

# input tracks
trackInput = 'generalTracks'
if opt.dataTier == 'RECO' or opt.dataTier == 'FEVT':
    trackInput = 'TrackRefitter'

#------------------------------------------
#  Define your Analyzer here
#------------------------------------------

# Keep the name Analyzer, which is assumed later
process.Analyzer = cms.EDAnalyzer("TimingStudy",
    trajectoryInput = cms.string(trackInput),
    fileName = cms.string('Ntuple.root'),
    extrapolateFrom = cms.int32(2),
    extrapolateTo = cms.int32(1),
    keepOriginalMissingHit = cms.bool(False),
    usePixelCPE= cms.bool(True),
    nthClusterToSave = cms.int32(100),
    minNStripHits    = cms.int32(0),
    minTrkPt         = cms.double(0.6),
    useClosestVtx    = cms.bool(False),
    triggerNames=cms.vstring(
        "HLT_ZeroBias",
        "HLT_Physics",
        "HLT_Random",
        "HLT_MinBias"),
    #dataPileupFile = cms.string("PileupHistogram_201278_flatpileupMC.root"),
    #mcPileupFile   = cms.string("PileupHistogram_201278_flatpileupMC.root"),
    #dataPileupHistoName = cms.string("pileup"),
    #mcPileupHistoName = cms.string("mcpileup"),
    #mcLumiScale = cms.double(0.348),
    #instlumiTextFile = cms.untracked.string("run_ls_instlumi_pileup_2015.txt"),
)

#------------------------------------------
#  Configurations from cmsDriver.py
#------------------------------------------

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# number of events to run on
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(opt.maxEvents))

# MessageLogger
process.MessageLogger.cerr.FwkReport.reportEvery = (1 if opt.dataTier == 'RAW' else 100)

# Input files
# Set some defaults if not given
if opt.inputFile == '':
    if   opt.era == '2011_53X_ReReco':
        if   opt.dataTier == 'RAW':
            opt.inputFile = '/store/data/Run2011B/MinimumBias/RAW/v1/000/177/719/445783AA-E9EC-E011-BF51-00237DDBE0E2.root'
        elif opt.dataTier == 'RECO':
            opt.inputFile = '/store/data/Run2011B/MinimumBias/RECO/12Oct2013-v1/20000/A086EB53-6F45-E311-9837-002481E0E5EE.root'
    elif opt.era == '2012_53X_ReReco':
        if   opt.dataTier == 'RAW':
            opt.inputFile = '/store/data/Run2012A/MinimumBias/RAW/v1/000/190/456/0CD4EB5C-297F-E111-9FB9-0025901D627C.root'
        elif opt.dataTier == 'RECO':
            opt.inputFile = '/store/data/Run2012A/MinimumBias/RECO/22Jan2013-v1/20002/0E16C9E3-8367-E211-A916-00304867BFAA.root'
    elif opt.era == '2012_76X_PrivateReco':
        if   opt.dataTier == 'RAW':
            opt.inputFile = '/store/data/Run2012B/MinimumBias/RAW/v1/000/194/050/02836886-119C-E111-A05F-003048D2BF1C.root'
        elif opt.dataTier == 'RECO':
            opt.inputFile = '/store/user/jkarancs/data/MinimumBias/Run2012B-CMSSW_7_6_6_patch1-76X_dataRun1_v16-RECO/170310_133650/0000/RECO_RAW2DIGI_L1Reco_RECO_1.root'
    elif opt.era == '2015_76X_ReReco':
        if   opt.dataTier == 'RAW':
            opt.inputFile = '/store/data/Run2015D/ZeroBias/RAW/v1/000/259/388/00000/A0002C40-0075-E511-9805-02163E012957.root'
        elif opt.dataTier == 'AOD':
            opt.inputFile = '/store/data/Run2015D/ZeroBias/AOD/16Dec2015-v1/60000/0014CB4C-BCA8-E511-8819-0025905A6136.root'
            opt.secondaryInputFile = '/store/data/Run2015D/ZeroBias/RAW/v1/000/259/388/00000/A0002C40-0075-E511-9805-02163E012957.root'
    elif opt.era == '2016_80X_ReReco':
        if   opt.dataTier == 'RAW':
            opt.inputFile = '/store/data/Run2016G/ZeroBias/RAW/v1/000/278/820/00000/8EDB3E8A-5562-E611-97F2-FA163E893235.root'
        elif opt.dataTier == 'AOD':
            opt.inputFile = '/store/data/Run2016G/ZeroBias/AOD/23Sep2016-v1/50000/002540F8-0087-E611-B6C2-008CFA111164.root'
            opt.secondaryInputFile = '/store/data/Run2016G/ZeroBias/RAW/v1/000/278/820/00000/8EDB3E8A-5562-E611-97F2-FA163E893235.root'
    elif opt.era == '2016_80X_PromptReco':
        if   opt.dataTier == 'RAW':
            opt.inputFile = '/store/data/Run2016H/ZeroBias/RAW/v1/000/281/242/00000/688452AF-0D80-E611-B15E-02163E013939.root'
        elif opt.dataTier == 'AOD':
            opt.inputFile = '/store/data/Run2016H/ZeroBias/AOD/PromptReco-v2/000/281/242/00000/F409C103-6E82-E611-86B7-02163E01296C.root'
            opt.secondaryInputFile = '/store/data/Run2016H/ZeroBias/RAW/v1/000/281/242/00000/688452AF-0D80-E611-B15E-02163E013939.root'

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(opt.inputFile),
)
if opt.secondaryInputFile != '':
    process.source.secondaryFileNames = cms.untracked.vstring(opt.secondaryInputFile)

if opt.era in ["2011_53X_ReReco", "2012_53X_ReReco", "2012_76X_PrivateReco"]:
    process.options = cms.untracked.PSet()
else:
    process.options = cms.untracked.PSet(allowUnscheduled = cms.untracked.bool(True))

#if opt.era in ["2011_53X_ReReco", "2012_53X_ReReco"]:
#    # Production Info
#    process.configurationMetadata = cms.untracked.PSet(
#        version = cms.untracked.string('$Revision: 1.381.2.28 $'),
#        annotation = cms.untracked.string('RECO nevts:10'),
#        name = cms.untracked.string('PyReleaseValidation')
#    )
#else:
#    # Production Info
#    process.configurationMetadata = cms.untracked.PSet(
#        annotation = cms.untracked.string('RECO nevts:10'),
#        name = cms.untracked.string('Applications'),
#        version = cms.untracked.string('$Revision: 1.19 $')
#    )

# GlobalTag
if   opt.era == '2011_53X_ReReco':
    GT = 'FT_R_53_LV5::All'
elif opt.era == '2012_53X_ReReco':
    GT = 'FT_R_53_V18::All'
elif opt.era == '2012_76X_PrivateReco':
    GT = '76X_dataRun1_v16'
elif opt.era == '2015_76X_ReReco':
    GT = '76X_dataRun2_v15'
elif opt.era == '2016_80X_ReReco':
    GT = '80X_dataRun2_2016SeptRepro_v3'
elif opt.era == '2016_80X_PromptReco':
    GT = '80X_dataRun2_Prompt_v14'

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, GT, '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)

# Analyzer
# Add TrackRefitter if needed
if trackInput == 'TrackRefitter':
    process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
    if opt.era in ["2011_53X_ReReco", "2012_53X_ReReco"]:
        process.Analyzer_step = cms.Path(process.TrackRefitter*process.Analyzer)
    else:
        process.Analyzer_step = cms.Path(process.MeasurementTrackerEvent*process.TrackRefitter*process.Analyzer)
else:
    process.Analyzer_step = cms.Path(process.Analyzer)

# Schedule definition
if opt.dataTier == 'RAW' or opt.dataTier == 'AOD':
    process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.Analyzer_step)
else:
    process.schedule = cms.Schedule(process.Analyzer_step)

# Customisations if needed
if opt.era in ["2011_53X_ReReco", "2012_53X_ReReco"]:
    from Configuration.DataProcessing.RecoTLR import customisePrompt 
    process = customisePrompt(process)
elif opt.era in ["2015_76X_ReReco", "2016_80X_ReReco", "2016_80X_PromptReco"]:
    from Configuration.DataProcessing.RecoTLR import customiseDataRun2Common_25ns 
    process = customiseDataRun2Common_25ns(process)
    #do not add changes to your config after this point (unless you know what you are doing)
    from FWCore.ParameterSet.Utilities import convertToUnscheduled
    process=convertToUnscheduled(process)
    from FWCore.ParameterSet.Utilities import cleanUnscheduled
    process=cleanUnscheduled(process)

