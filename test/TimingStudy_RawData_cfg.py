import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.MessageLogger.cerr.threshold = 'INFO'


# ------------------- Output Report --------------------
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )


# ----------------- Number of Events -------------------
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))


# ------------------- Input Files ----------------------
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data/store/data/Run2012C/MinimumBias/RAW/v1/000/203/002/7CE72DDD-3400-E211-A6ED-003048D374CA.root'
    )
)
## # GEN-SIM Input file with MinBias events
## PileupInput = cms.untracked.string(
##     'file:/data/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0005E496-3661-E111-B31E-003048F0E426.root'
## )
## # Input pileup distribution for MixingModule
## # root file, that contains a TH1 histo named: pileup
## PileupDistHistoInput = cms.untracked.string(
##     'file:$CMSSW_BASE/src/DPGAnalysis/PixelTimingStudy/data/PileupHistogram_test.root'
## )


# --------------------- GlobalTag ----------------------
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "FT_R_53_V18::All"
# For MC
#process.GlobalTag.globaltag = "START53_V26::All"


# ---------------------- BeamSpot ----------------------
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")
             

# ---------------------- Geometry ----------------------
#process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.Geometry.GeometryIdeal_cff")


# ------------------- Magnetic Field -------------------
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")


## # ------------------------- DIGI -----------------------
## 
## process.load("IOMC.RandomEngine.IOMC_cff") # RandomNumberGeneratorService
## process.load('Configuration.StandardSequences.Digi_cff') # pdigi
## 
## # configuration to model pileup for initial physics phase
## # mix
## #process.load("SimGeneral.MixingModule.mixNoPU_cfi") # No Pileup
## process.load('SimGeneral.MixingModule.mix_2012_201278_1_cfi') # Silvias pileup mixing
## 
## #process.mix.fileNames = process.source.fileNames
## process.mix.fileNames = PileupInput
## process.mix.input.nbPileupEvents.fileName = PileupDistHistoInput
## 
## # L1 Digis
## process.load('Configuration.StandardSequences.SimL1Emulator_cff') # SimL1Emulator


## # --------------------- DIGI to RAW --------------------
## process.load('Configuration.StandardSequences.DigiToRaw_cff')
## #-> process.load("EventFilter.SiPixelRawToDigi.SiPixelDigiToRaw_cfi") # SiPixelDigiToRaw
## #-> process.load("EventFilter.SiStripRawToDigi.SiStripDigiToRaw_cfi") # SiStripDigiToRaw


# --------------------- RAW to DIGI --------------------
process.load('Configuration.StandardSequences.RawToDigi_cff')
#-> process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi") # siPixelDigis
#-> process.load("EventFilter.SiStripRawToDigi.SiStripRawToDigis_standard_cff") # siStripDigis


# --------------------- DIGI to RECO -------------------
process.load("Configuration.StandardSequences.Reconstruction_cff")
#-> process.load("RecoTracker.MeasurementDet.MeasurementTrackerEventProducer_cfi") # MeasurementTrackerEvent
#-> process.load("RecoPixelVertexing.Configuration.RecoPixelVertexing_cff") # recopixelvertexing
#-> process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff") # trackerlocalreco
###-> process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")
###-> process.load("RecoLocalTracker.SiPixelClusterizer.SiPixelClusterizer_cfi")

# L1 Reco
process.load('Configuration.StandardSequences.L1Reco_cff') # L1Reco
#-> process.load("EventFilter.L1GlobalTriggerRawToDigi.conditionDumperInEdm_cfi") # conditionsInEdm


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
## process.Digi = cms.Sequence(process.pdigi)
## process.L1_Sim = cms.Sequence(process.SimL1Emulator)
## process.Digi_to_Raw = cms.Sequence( #process.DigiToRaw
##     process.l1GtPack +
##     process.l1GtEvmPack +
##     process.siPixelRawData +
##     process.SiStripDigiToRaw +
##     process.rawDataCollector
## )
process.Raw_to_Digi = cms.Sequence( #process.RawToDigi
     process.siPixelDigis +
     process.siStripDigis +
     process.scalersRawToDigi +
     process.gtEvmDigis
)
process.Digi_to_Reco = cms.Sequence( #process.reconstruction
    #process.L1Reco +
    process.conditionsInEdm +
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

process.p = cms.Path(
    ## # GEN-SIM ->
    ## process.Digi*
    ## process.L1_Sim*
    ## process.Digi_to_Raw*
    ## # GEN-SIM-RAW ->
    ## #process.SiStripDigiToRaw
    # RAW ->
    process.Raw_to_Digi*
    process.Digi_to_Reco*
    # RECO ->
    #process.MeasurementTrackerEvent*
    process.Ntuplizer
)

#process.output = cms.OutputModule("PoolOutputModule",
#    fileName = cms.untracked.string("TestReco.root"),
#    #outputCommands = cms.untracked.vstring('keep *_mix_*_*'),
#    outputCommands = cms.untracked.vstring('keep *'),
#)
#process.outpath = cms.EndPath(process.output)
