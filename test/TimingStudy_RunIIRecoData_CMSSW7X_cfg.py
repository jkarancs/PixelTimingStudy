# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: --data -s RAW2DIGI,RECO --conditions auto:run2_data --datatier RECO --eventcontent RECO --process TimingStudy --fileout file:RECO.root --python_filename=test/TimingStudy_RunIIRecoData_CMSSW7X_cfg.py -n 100 --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('TimingStudy')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'/store/data/Run2015A/ZeroBias1/RECO/PromptReco-v1/000/247/398/00000/18DEC26E-1E0F-E511-84CB-02163E0125CE.root'
        #'file:/data/store/data/Run2015A/ZeroBias1/RECO/PromptReco-v1/000/247/398/00000/18DEC26E-1E0F-E511-84CB-02163E0125CE.root'
        # 3.8T data
        'file:/data/store/data/Run2015B/ZeroBias1/RECO/PromptReco-v1/000/251/028/00000/5E2082E5-1B26-E511-B368-02163E0129A2.root'
        ),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('--data nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RECOoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RECO'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('file:RECO.root'),
    outputCommands = process.RECOEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
process.GlobalTag = GlobalTag(process.GlobalTag, '74X_dataRun2_Prompt_v0', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)

# Schedule definition
#process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step)



# Added for TimingStudy
# Comment out line 69 (schedule)
# Add a Refitter for tracks
print process.GlobalTag.globaltag
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.threshold = 'INFO'

process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.Refitter = process.TrackRefitterP5.clone()
process.Refitter.src = 'generalTracks'
process.Refitter.TrajectoryInEvent = True

# TimingStudy
process.TimingStudy = cms.EDAnalyzer("TimingStudy",
    trajectoryInput = cms.string('Refitter'),
    fileName = cms.string("Ntuple.root"),
    extrapolateFrom = cms.int32(2),
    extrapolateTo = cms.int32(1),
    keepOriginalMissingHit = cms.bool(False),
    usePixelCPE= cms.bool(True),
    minNStripHits = cms.int32(0),
    triggerNames=cms.vstring(
        "HLT_ZeroBias",
        "HLT_Physics",
        "HLT_Random",
        "HLT_PixelTracks_Multiplicity100",
        "HLT_PixelTracks_Multiplicity80",
        "HLT_PixelTracks_Multiplicity125",
        "HLT_PixelTracks_Multiplicity110",
        "HLT_PixelTracks_Multiplicity85",
        "HLT_PixelTracks_Multiplicity70",
        "HLT_PixelTracks_Multiplicity40",
        "HLT_L1Tech_BSC_HighMultiplicity",
        "HLT_JetE30_NoBPTX",
        "HLT_JetE30_NoBPTX_NoHalo",
        "HLT_JetE30_NoBPTX3BX_NoHalo",
        "HLT_JetE50_NoBPTX3BX_NoHalo",
        "HLT_60Jet10",
        "HLT_70Jet10",
        "HLT_70Jet13",
        "HLT_L1Tech_BSC_minBias",
        "HLT_MinBias"
        ),
    #dataPileupFile = cms.string("data/PileupHistogram_test.root"),
    #mcPileupFile   = cms.string("data/PileupHistogram_test.root"),
    #dataPileupHistoName = cms.string("pileup"),
    #mcPileupHistoName = cms.string("mcpileup"),
    mcLumiScale = cms.double(0.37935), # 2012 (1368b): 0.222, 2015 (2232b): 0.3136, 2016 (2700b) pred: 0.37935
    instlumiTextFile = cms.untracked.string("run_ls_instlumi_pileup_2015.txt"),
    )
process.TimingStudy_step = cms.Path(process.MeasurementTrackerEvent*process.Refitter*process.TimingStudy)

process.schedule = cms.Schedule(process.TimingStudy_step)
