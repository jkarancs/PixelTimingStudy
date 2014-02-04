import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.threshold = 'INFO'

## Options and Output Report
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# ----------------- Number of Events -------------------
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))


# ------------------- Input Files ----------------------
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'file:/data/store/relval/CMSSW_7_0_0_pre8/GEN-SIM/00E8DCE5-3459-E311-A9FB-0025905A608A.root'
)
)


# --------------------- GlobalTag ----------------------
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
# For MC
process.GlobalTag.globaltag = "MC_70_V1::All"


# ---------------------- BeamSpot ----------------------
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")
             

# ---------------------- Geometry ----------------------
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
#process.load("Configuration.StandardSequences.GeometryIdeal_cff")


# ------------------- Magnetic Field -------------------
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")


# ------------------- GEN-SIM Modules ------------------
#process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("IOMC.RandomEngine.IOMC_cff") # RandomNumberGeneratorService
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    simMuonDTDigis = cms.PSet(
        initialSeed = cms.untracked.uint32(1234567),
        engineName = cms.untracked.string('TRandom3')
    ),
    simMuonCSCDigis = cms.PSet(
        initialSeed = cms.untracked.uint32(1234567),
        engineName = cms.untracked.string('TRandom3')
    ),
    mix = cms.PSet(
        initialSeed = cms.untracked.uint32(1234567),
        engineName = cms.untracked.string('TRandom3')
    ),
    simMuonRPCDigis = cms.PSet(
        initialSeed = cms.untracked.uint32(1234567),
        engineName = cms.untracked.string('TRandom3')
    )
)

#process.load("SimG4Core.Configuration.SimG4Core_cff") # g4SimHits
process.load("SimGeneral.MixingModule.mixNoPU_cfi") # mix
#-> process.load("SimTracker.Configuration.SimTracker_cff") # stripDigitizer

process.mix.bunchspace = 50

process.mix.digitizers.pixel.AddPixelInefficiency = -100
process.mix.digitizers.pixel.thePixelColEfficiency_BPix1 = cms.double(0.999)
process.mix.digitizers.pixel.thePixelColEfficiency_BPix2 = cms.double(1.0)
process.mix.digitizers.pixel.thePixelColEfficiency_BPix3 = cms.double(1.0)

process.mix.digitizers.pixel.thePixelEfficiency_BPix1 = cms.double(1.0)
process.mix.digitizers.pixel.thePixelEfficiency_BPix2 = cms.double(1.0)
process.mix.digitizers.pixel.thePixelEfficiency_BPix3 = cms.double(1.0)

process.mix.digitizers.pixel.theBPix1LadderEfficiency = cms.vdouble(
0.978109,
0.971506,
0.973958,
0.968912,
0.972571,
0.970577,
0.975467,
0.974471,
0.980044,
0.978212,
0.981972,
0.979526,
0.984278,
0.983857,
0.981697,
0.983154,
0.981313,
0.979542,
0.980088,
0.974888
)
process.mix.digitizers.pixel.theBPix2LadderEfficiency = cms.vdouble(
0.996517,
0.993511,
0.993921,
0.993094,
0.994043,
0.992445,
0.998017,
0.99265,
0.992787,
0.993585,
0.994242,
0.993641,
0.993321,
0.994404,
0.992527,
0.995297,
0.995386,
0.995533,
0.995127,
0.993078,
0.994289,
0.994998,
0.994907,
0.994478,
0.995607,
0.994411,
0.994601,
0.995483,
0.993749,
0.995476,
0.993124,
0.993807
)
process.mix.digitizers.pixel.theBPix3LadderEfficiency = cms.vdouble(
0.996391,
0.998275,
0.995975,
0.996847,
0.996605,
0.995927,
0.996711,
0.995751,
0.997383,
0.998293,
0.996128,
0.997693,
0.99706,
0.996678,
0.997661,
0.998237,
0.995592,
0.998173,
0.997099,
0.997654,
0.996812,
0.99663,
0.996741,
0.99782,
0.995598,
0.996586,
0.996395,
0.99856,
0.998679,
0.996158,
0.997272,
0.99643,
0.997723,
0.99644,
0.997767,
0.997775,
0.996921,
0.996289,
0.996784,
0.996464,
0.998081,
0.997297,
0.998379,
0.99735
)
#process.mix.digitizers.pixel.theBPix1ModuleEfficiency = cms.vdouble(
#    1.00323,
#    1.00074,
#    0.977744,
#    0.783408
#)

#process.mix.digitizers.pixel.theBPix1PUEfficiency = cms.vdouble(
#    1.01997,
#    -4.03709e-07,
#    -1.26739e-09
#)


# ------------------- GEN-SIM to DIGI ------------------
process.load('Configuration.StandardSequences.Digi_cff') # pdigi
#  from SimMuon.Configuration.SimMuon_cff import *
#  from SimCalorimetry.Configuration.SimCalorimetry_cff import *
#  from SimGeneral.Configuration.SimGeneral_cff import *
#  doAllDigi = cms.Sequence(calDigi+muonDigi)
#  #pdigi = cms.Sequence(cms.SequencePlaceholder("randomEngineStateProducer")*cms.SequencePlaceholder("mix")*doAllDigi*addPileupInfo)
#  pdigi = cms.Sequence(cms.SequencePlaceholder("randomEngineStateProducer")*addPileupInfo*cms.SequencePlaceholder("mix")*doAllDigi)

#process.pdigi = cms.Sequence(cms.SequencePlaceholder("randomEngineStateProducer")*process.addPileupInfo*cms.SequencePlaceholder("mix")*process.doAllDigi)

# L1 Digis
process.load('Configuration.StandardSequences.SimL1Emulator_cff') # SimL1Emulator


# --------------------- DIGI to RAW --------------------
process.load('Configuration.StandardSequences.DigiToRaw_cff')
#-> process.load("EventFilter.SiPixelRawToDigi.SiPixelDigiToRaw_cfi") # SiPixelDigiToRaw
#-> process.load("EventFilter.SiStripRawToDigi.SiStripDigiToRaw_cfi") # SiStripDigiToRaw


# --------------------- RAW to DIGI --------------------
process.load('Configuration.StandardSequences.RawToDigi_cff')
#-> process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi") # siPixelDigis
#-> process.load("EventFilter.SiStripRawToDigi.SiStripRawToDigis_standard_cff") # siStripDigis

#process.siPixelDigis.IncludeErrors = True # To create UserError DetIdCollection
#process.siPixelDigis.InputLabel = cms.InputTag("siPixelRawData")


# --------------------- DIGI to RECO -------------------
process.load("Configuration.StandardSequences.Reconstruction_cff")
#-> process.load("RecoTracker.MeasurementDet.MeasurementTrackerEventProducer_cfi") # MeasurementTrackerEvent
#-> process.load("RecoPixelVertexing.Configuration.RecoPixelVertexing_cff") # recopixelvertexing
#-> process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff") # trackerlocalreco
###-> process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")
###-> process.load("RecoLocalTracker.SiPixelClusterizer.SiPixelClusterizer_cfi")

# L1 Reco
process.load('Configuration.StandardSequences.L1Reco_cff') # L1Reco
#-> process.load("EventFilter.L1GlobalTriggerRawToDigi.conditionDumperInEdm_cfi") # conditionDumperInEdm


# -------- ckf Tracks without early muons --------------
# ckftracks & iterTracking does not work (eeds EarlyMuons for muon seeding)
process.ckftracks_woMS = cms.Sequence(process.InitialStep
                                         *process.DetachedTripletStep
                                         *process.LowPtTripletStep
                                         *process.PixelPairStep
                                         *process.MixedTripletStep
                                         *process.PixelLessStep
                                         *process.TobTecStep
                                         *process.earlyGeneralTracks
                                        #*process.muonSeededStep
                                         *process.preDuplicateMergingGeneralTracks
                                         *process.generalTracksSequence
                                         *process.ConvStep
                                         *process.conversionStepTracks
                                         *process.electronSeedsSeq
                                         *process.doAlldEdXEstimators
)

# ------------------- Track Refitter -------------------
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.src = "generalTracks"
process.TrackRefitter.TrajectoryInEvent = True

# ---------------------- Ntuplizer ---------------------
process.TimingStudy = cms.EDAnalyzer("TimingStudy",
                                     trajectoryInput = cms.string('TrackRefitter'),
                                     fileName = cms.string("Ntuple.root"),
                                     extrapolateFrom = cms.int32(2),
                                     extrapolateTo = cms.int32(1),
                                     keepOriginalMissingHit = cms.bool(False),
                                     usePixelCPE= cms.bool(True),
                                     #minNStripHits = cms.int32(0),
                                     triggerNames=cms.vstring("HLT_ZeroBias",
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
                                     #dataPileupFile = cms.string("data/PileupHistogram_test.root"),
                                     #mcPileupFile   = cms.string("data/PileupHistogram_test.root"),
                                     #dataPileupHistoName = cms.string("pileup"),
                                     #mcPileupHistoName = cms.string("mcpileup"),
                                     mcLumiScale = cms.double(221.95)
)

# ------------------------ Path ------------------------
process.p = cms.Path(
    process.pdigi*
    process.SimL1Emulator*
    process.DigiToRaw*
    process.RawToDigi*
    
    # # DigiToRaw (Needed for MC)
    # process.SiStripDigiToRaw*
    # # Digi
    # process.scalersRawToDigi*
    # process.gtEvmDigis*
    # process.siPixelDigis*
    # process.siStripDigis*
    # #process.RawToDigi*              #Whole detector
    # LumiSummary
    process.lumiProducer*
    # L1Reco
    process.L1Reco*
    #process.conditionDumperInEdm* (L1Reco has it)    
    # Reco
    process.trackerlocalreco*
    #process.localreco*              #Whole detector
    process.offlineBeamSpot*
    process.recopixelvertexing*
    process.MeasurementTrackerEvent* #New in 70X
    #process.ckftracks*
    process.ckftracks_woMS*
    process.vertexreco*
    # Refitter
    process.TrackRefitter*
    process.TimingStudy
)
