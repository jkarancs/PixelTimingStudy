import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.MessageLogger.cerr.threshold = 'INFO'


# ------------------- Output Report --------------------
#process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# ----------------- Number of Events -------------------
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))


# ------------------- Input Files ----------------------
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0005E496-3661-E111-B31E-003048F0E426.root'
    )
)
# GEN-SIM Input file with MinBias events
PileupInput = cms.untracked.string(
    'file:/data/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0005E496-3661-E111-B31E-003048F0E426.root'
)
# Input pileup distribution for MixingModule
# root file, that contains a TH1 histo named: pileup
PileupDistHistoInput = cms.untracked.string(
    'file:$CMSSW_BASE/src/DPGAnalysis/PixelTimingStudy/data/PileupHistogram_test.root'
)


# --------------------- GlobalTag ----------------------
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
# For MC
process.GlobalTag.globaltag = "START53_V7E::All"


# ---------------------- BeamSpot ----------------------
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")
             

# ---------------------- Geometry ----------------------
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
#process.load("Configuration.StandardSequences.GeometryIdeal_cff")


# ------------------- Magnetic Field -------------------
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")


# ------------------------- DIGI -----------------------

process.load("IOMC.RandomEngine.IOMC_cff") # RandomNumberGeneratorService
process.load('Configuration.StandardSequences.Digi_cff') # pdigi

# configuration to model pileup for initial physics phase
# mix
#process.load("SimGeneral.MixingModule.mixNoPU_cfi") # No Pileup
process.load('SimGeneral.MixingModule.mix_2012_201278_1_cfi') # Silvias pileup mixing

#process.mix.fileNames = process.source.fileNames
process.mix.fileNames = PileupInput
process.mix.input.nbPileupEvents.fileName = PileupDistHistoInput

process.simSiPixelDigis.AddPixelInefficiency = -2 # It loads the efficiency scale factors from the config file
process.simSiPixelDigis.thePixelColEfficiency_BPix1 = cms.double(1.0)
process.simSiPixelDigis.thePixelColEfficiency_BPix2 = cms.double(1.0)
process.simSiPixelDigis.thePixelColEfficiency_BPix3 = cms.double(1.0)

process.simSiPixelDigis.thePixelEfficiency_BPix1 = cms.double(1.0)
process.simSiPixelDigis.thePixelEfficiency_BPix2 = cms.double(1.0)
process.simSiPixelDigis.thePixelEfficiency_BPix3 = cms.double(1.0)

process.simSiPixelDigis.theBPix1LadderEfficiency = cms.vdouble(
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
process.simSiPixelDigis.theBPix2LadderEfficiency = cms.vdouble(
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
process.simSiPixelDigis.theBPix3LadderEfficiency = cms.vdouble(
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
#process.simSiPixelDigis.theBPix1ModuleEfficiency = cms.vdouble(
#    1.12513,
#    1.04972,
#    1.00695,
#    0.997164
#)
#process.simSiPixelDigis.theBPix2ModuleEfficiency = cms.vdouble(
#    1.08175,
#    1.02053,
#    1.00217,
#    0.999507
#)
#process.simSiPixelDigis.theBPix3ModuleEfficiency = cms.vdouble(
#    1.05657,
#    1.00846,
#    1.00187,
#    1.00032
#)


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
process.Digi = cms.Sequence(process.pdigi)
process.L1_Sim = cms.Sequence(process.SimL1Emulator)
process.Digi_to_Raw = cms.Sequence( #process.DigiToRaw
    process.l1GtPack +
    process.l1GtEvmPack +
    process.siPixelRawData +
    process.SiStripDigiToRaw +
    process.rawDataCollector
)
process.Raw_to_Digi = cms.Sequence( #process.RawToDigi
     process.siPixelDigis +
     process.siStripDigis +
     process.scalersRawToDigi +
     process.gtEvmDigis
)
process.Digi_to_Reco = cms.Sequence( #process.reconstruction
    process.L1Reco +
    ( process.offlineBeamSpot +
      process.trackerlocalreco ) *
    process.recopixelvertexing *
    process.ckftracks*
    process.vertexreco
)
process.Ntuplizer = cms.Sequence(
    process.TrackRefitter*
    process.TimingStudy
)

process.p1 = cms.Path(
    # GEN-SIM ->
    process.Digi*
    process.L1_Sim*
    process.Digi_to_Raw*
    # GEN-SIM-RAW ->
    #process.SiStripDigiToRaw
    # RAW ->
    process.Raw_to_Digi*
    process.Digi_to_Reco*
    # RECO ->
    #process.MeasurementTrackerEvent*
    process.Ntuplizer
)

#process.output = cms.OutputModule("PoolOutputModule",
#    fileName = cms.untracked.string("TestReco.root"),
#    outputCommands = cms.untracked.vstring('keep *_mix_*_*'),
#)
#process.outpath = cms.EndPath(process.output)
