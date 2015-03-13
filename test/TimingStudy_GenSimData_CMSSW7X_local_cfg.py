# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: -s DIGI,DIGI2RAW,RAW2DIGI,RECO --mc --conditions auto:run2_mc --pileup=NoPileUp --beamspot NominalCollision2015 --magField 38T_PostLS1 --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1 --process GenSimToReco --filein=file:/data/store/user/hazia/minbias_13tev/ahazi/MinBias_13TeV_GEN_SIM_7_1_0/MinBias_13TeV_GEN_SIM_7_1_0/9cb32faabd78efe327c9c841fa706583/MinBias_13TeV_GENSIM_100_1_bOe.root -n 200 --no_exec --python_filename=test/TimingStudy_GenSimData_CMSSW7X_cfg.py
import FWCore.ParameterSet.Config as cms

process = cms.Process('GenSimToReco')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('file:/data/store/user/hazia/minbias_13tev/ahazi/MinBias_13TeV_GEN_SIM_7_1_0/MinBias_13TeV_GEN_SIM_7_1_0/9cb32faabd78efe327c9c841fa706583/MinBias_13TeV_GENSIM_100_1_bOe.root')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.19 $'),
    annotation = cms.untracked.string('-s nevts:200'),
    name = cms.untracked.string('Applications')
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('-s_DIGI_DIGI2RAW_RAW2DIGI_RECO.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    )
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

# Path and EndPath definitions
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.digitisation_step,process.digi2raw_step,process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.RECOSIMoutput_step)

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.postLS1Customs
from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1 

#call to customisation function customisePostLS1 imported from SLHCUpgradeSimulations.Configuration.postLS1Customs
process = customisePostLS1(process)

# End of customisation functions





#--------------- Added for TimingStudy ---------------

process.MessageLogger.cerr.FwkReport.reportEvery = 10

#-------------------------
#  MixingModule
#-------------------------

# GEN-SIM Input files with MinBias events
#PileupInput = process.source.fileNames
from DPGAnalysis.PixelTimingStudy.PoolSource_13Tev_Andras_local import *
PileupInput = FileNames

# Input pileup distribution for MixingModule
# Can use the same file in TimingStudy for pileup reweighting
# Note: the desired mc_input distribution has to be shifted by -1 wrt mcpileup
PileupHistoInput = cms.untracked.string(
    'file:PileupHistogram_201278_flatpileupMC.root' # Flat Pileup
    #'file:PileupHistogram_201278.root' # 201278 Pileup
)
PileupHistoName=cms.untracked.string('mc_input')

# Change MinBias input file and Input Pileup Distribution
process.mix.input = cms.SecSource("PoolSource",
    type = cms.string('histo'),
    nbPileupEvents  = cms.PSet(
        fileName = PileupHistoInput,
        histoName = PileupHistoName,
    ),
    sequential = cms.untracked.bool(False),
    manage_OOT = cms.untracked.bool(True),
    OOT_type = cms.untracked.string('Poisson'),
    fileNames = PileupInput
)

#-------------------------
#  Track Refitter
#-------------------------
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.Refitter = process.TrackRefitterP5.clone()
process.Refitter.src = 'generalTracks'
process.Refitter.TrajectoryInEvent = True

#-------------------------
#  TimingStudy
#-------------------------
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
        "HLT_MinBias"),
    dataPileupFile = cms.string("PileupHistogram_201278_flatpileupMC.root"),
    mcPileupFile   = cms.string("PileupHistogram_201278_flatpileupMC.root"),
    dataPileupHistoName = cms.string("pileup"),
    mcPileupHistoName = cms.string("mcpileup"),
    mcLumiScale = cms.double(200.0)
)

#-------------------------
#  Path/Schedule
#-------------------------
process.TimingStudy_step = cms.Path(process.Refitter*process.TimingStudy)

process.schedule = cms.Schedule(
    process.digitisation_step,
    process.L1simulation_step,
    process.digi2raw_step,
    process.raw2digi_step,
    process.reconstruction_step,
    process.TimingStudy_step
    )

