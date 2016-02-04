# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: -s GEN,SIM,DIGI,DIGI2RAW,RAW2DIGI,RECO --mc --conditions auto:run2_mc --beamspot NominalCollision2015 --magField 38T_PostLS1 --evt_type SingleNuE10_cfi --pileup=Flat_0_50_25ns --filein=/store/mc/RunIISummer15GS/MinBias_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v2/10000/004CC894-4877-E511-A11E-0025905C3DF8.root --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1,Configuration/DataProcessing/Utils.addMonitoring --python_filename=test/TimingStudy_GenNu_DynIneffDB_cfg.py -n 10 --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mix_Flat_0_50_25ns_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.Geometry.GeometrySimDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedNominalCollision2015_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

# Input source
process.source = cms.Source("PoolSource",
    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring('/store/mc/RunIISummer15GS/MinBias_TuneCUETP8M1_13TeV-pythia8/GEN-SIM/MCRUN2_71_V1-v2/10000/004CC894-4877-E511-A11E-0025905C3DF8.root'),
    inputCommands = cms.untracked.vstring('keep *', 
        'drop LHEXMLStringProduct_*_*_*', 
        'keep *', 
        'drop *_genParticles_*_*', 
        'drop *_genParticlesForJets_*_*', 
        'drop *_kt4GenJets_*_*', 
        'drop *_kt6GenJets_*_*', 
        'drop *_iterativeCone5GenJets_*_*', 
        'drop *_ak4GenJets_*_*', 
        'drop *_ak7GenJets_*_*', 
        'drop *_ak8GenJets_*_*', 
        'drop *_ak4GenJetsNoNu_*_*', 
        'drop *_ak8GenJetsNoNu_*_*', 
        'drop *_genCandidatesForMET_*_*', 
        'drop *_genParticlesForMETAllVisible_*_*', 
        'drop *_genMetCalo_*_*', 
        'drop *_genMetCaloAndNonPrompt_*_*', 
        'drop *_genMetTrue_*_*', 
        'drop *_genMetIC5GenJs_*_*'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('SingleNuE10_cfi nevts:10'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    ),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string(''),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('SingleNuE10_cfi_GEN_SIM_DIGI_DIGI2RAW_RAW2DIGI_RECO_PU.root'),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

process.generator = cms.EDProducer("FlatRandomEGunProducer",
    AddAntiParticle = cms.bool(False),
    PGunParameters = cms.PSet(
        MaxE = cms.double(10.01),
        MaxEta = cms.double(2.5),
        MaxPhi = cms.double(3.14159265359),
        MinE = cms.double(9.99),
        MinEta = cms.double(-2.5),
        MinPhi = cms.double(-3.14159265359),
        PartID = cms.vint32(12)
    ),
    Verbosity = cms.untracked.int32(0),
    firstRun = cms.untracked.uint32(1),
    psethack = cms.string('single Nu E 10')
)


# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.digitisation_step = cms.Path(process.pdigi)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.digitisation_step,process.digi2raw_step,process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.RECOSIMoutput_step)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.postLS1Customs
from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1 

#call to customisation function customisePostLS1 imported from SLHCUpgradeSimulations.Configuration.postLS1Customs
process = customisePostLS1(process)

# Automatic addition of the customisation function from Configuration.DataProcessing.Utils
from Configuration.DataProcessing.Utils import addMonitoring 

#call to customisation function addMonitoring imported from Configuration.DataProcessing.Utils
process = addMonitoring(process)

# End of customisation functions


#--------------- Added for TimingStudy ---------------

#---------------------------
#  MessageLogger
#---------------------------
process.MessageLogger.cerr.FwkReport.reportEvery = 100

#---------------------------
#  Pile-up (RunIISummer15GS)
#---------------------------
from DPGAnalysis.PixelTimingStudy.PoolSource_13TeV_RunIISummer15GS import *
process.mix.input.fileNames = pileupFileNames

#---------------------------
#  DynIneff from DB
#---------------------------
useSqlite = True

if useSqlite:
    process.DynIneffDBSource = cms.ESSource("PoolDBESSource",
        DBParameters = cms.PSet(
            messageLevel = cms.untracked.int32(0),
            authenticationPath = cms.untracked.string('')
        ),
        connect = cms.string('sqlite_file:dcol80.db'),
        toGet = cms.VPSet(cms.PSet(
            record = cms.string("SiPixelDynamicInefficiencyRcd"),
            tag = cms.string("SiPixelDynamicInefficiency_v1")
        ))
    )
    process.es_prefer_DynIneffDBSource = cms.ESPrefer("PoolDBESSource","DynIneffDBSource")

#---------------------------
#  Track Refitter
#---------------------------
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitterP5.src = 'generalTracks'
process.TrackRefitterP5.TrajectoryInEvent = True

#---------------------------
#  TimingStudy
#---------------------------
process.TimingStudy = cms.EDAnalyzer("TimingStudy",
    trajectoryInput = cms.string('TrackRefitterP5'),
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
    #dataPileupFile = cms.string("PileupHistogram_201278_flatpileupMC.root"),
    #mcPileupFile   = cms.string("PileupHistogram_201278_flatpileupMC.root"),
    #dataPileupHistoName = cms.string("pileup"),
    #mcPileupHistoName = cms.string("mcpileup"),
    mcLumiScale = cms.double(0.37935) # 2016 prediction
)

#---------------------------
#  Path/Schedule
#---------------------------

process.TimingStudy_step = cms.Path(process.TrackRefitterP5*process.TimingStudy)
process.schedule.remove(process.RECOSIMoutput_step)
process.schedule.remove(process.endjob_step)
process.schedule.append(process.TimingStudy_step)

