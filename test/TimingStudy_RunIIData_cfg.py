# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: ReReco -s RAW2DIGI,L1Reco,RECO --data --scenario pp --conditions auto:run2_data --process NTUPLE --eventcontent RECO --datatier RECO --customise Configuration/DataProcessing/RecoTLR.customiseDataRun2Common_25ns --filein /store/data/Run2015D/ZeroBias/RAW/v1/000/260/627/00000/8EC1242C-7D82-E511-AD25-02163E0141E9.root --python_filename=test/TimingStudy_RunIIData_cfg.py -n 100 --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('NTUPLE')

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
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/data/Run2015D/ZeroBias/RAW/v1/000/260/627/00000/8EC1242C-7D82-E511-AD25-02163E0141E9.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('ReReco nevts:100'),
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
    fileName = cms.untracked.string('ReReco_RAW2DIGI_L1Reco_RECO.root'),
    outputCommands = process.RECOEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step)

# customisation of the process.

# Automatic addition of the customisation function from Configuration.DataProcessing.RecoTLR
from Configuration.DataProcessing.RecoTLR import customiseDataRun2Common_25ns 

#call to customisation function customiseDataRun2Common_25ns imported from Configuration.DataProcessing.RecoTLR
process = customiseDataRun2Common_25ns(process)

# End of customisation functions



#--------------- Added for TimingStudy ---------------

#---------------------------
#  Settings
#---------------------------

# RAW or RECO
runOnRAW = False   # True: RAW, False: RECO
setAutoGT = True

if setAutoGT:
    print "GlobalTag (auto:run2_data): "+str(process.GlobalTag.globaltag)
else:
    process.GlobalTag.globaltag = '76X_dataRun2_v16'
    print "GlobalTag (auto:run2_data): "+str(process.GlobalTag.globaltag)

# TimingStudy Settings
nthClusterToSave =   100 # Default: 100   (New option, default chosen is shown to offer enough stats (17% of on-trk clu with pt>1.0))
minNStripHits    =     0 # Default: 0     (New - old value was 11, for efficiency, but affected eta dsitribution)
minTrkPt         =   0.6 # Default: 0.6   (New option, default chosen to be below eff cut and common cluster cuts)
useClosestVtx    = False # Default: False (New option, default is using best vertex, closest vtx needs more study)

# Input file
if runOnRAW:
    process.source.fileNames = cms.untracked.vstring('file:/data/store/data/Run2015D/ZeroBias/RAW/v1/000/260/627/00000/8EC1242C-7D82-E511-AD25-02163E0141E9.root')
else:
    process.source.fileNames = cms.untracked.vstring('file:/data/store/data/Run2015D/ZeroBias/RECO/16Dec2015-v1/60000/00CBB0FD-26AA-E511-AF86-002590A2CD68.root')

# Number of events
process.maxEvents.input = 100

# MessageLogger
if runOnRAW:
    process.MessageLogger.cerr.FwkReport.reportEvery = 1
else:
    process.MessageLogger.cerr.FwkReport.reportEvery = 100

#---------------------------
#  Track Refitter
#---------------------------
if runOnRAW:
    trajInput = cms.string('generalTracks')
else:
    process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
    process.TrackRefitterP5.src = 'generalTracks'
    process.TrackRefitterP5.TrajectoryInEvent = True
    trajInput = cms.string('TrackRefitterP5')

#---------------------------
#  TimingStudy
#---------------------------
process.TimingStudy = cms.EDAnalyzer("TimingStudy",
    trajectoryInput = trajInput,
    fileName = cms.string("Ntuple.root"),
    extrapolateFrom = cms.int32(2),
    extrapolateTo = cms.int32(1),
    keepOriginalMissingHit = cms.bool(False),
    usePixelCPE= cms.bool(True),
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
        "HLT_MinBias"),
    #dataPileupFile = cms.string("PileupHistogram_201278_flatpileupMC.root"),
    #mcPileupFile   = cms.string("PileupHistogram_201278_flatpileupMC.root"),
    #dataPileupHistoName = cms.string("pileup"),
    #mcPileupHistoName = cms.string("mcpileup"),
    mcLumiScale = cms.double(0.384408), # 2012 (1368b): 0.222, 2015 (2232b): 0.3136, 2016 (2736b) pred: 0.384408
    instlumiTextFile = cms.untracked.string("run_ls_instlumi_pileup_2015.txt"),
    nthClusterToSave = cms.int32(nthClusterToSave),
    minNStripHits    = cms.int32(minNStripHits),
    minTrkPt         = cms.double(minTrkPt),
    useClosestVtx    = cms.bool(useClosestVtx),
)

#---------------------------
#  Path/Schedule
#---------------------------
process.schedule.remove(process.RECOoutput_step)
process.schedule.remove(process.endjob_step)

if runOnRAW:
    process.TimingStudy_step = cms.Path(process.TimingStudy)
else:
    process.TimingStudy_step = cms.Path(process.MeasurementTrackerEvent*process.TrackRefitterP5*process.TimingStudy)
    process.schedule.remove(process.reconstruction_step)
    process.schedule.remove(process.L1Reco_step)
    process.schedule.remove(process.raw2digi_step)

process.schedule.append(process.TimingStudy_step)
