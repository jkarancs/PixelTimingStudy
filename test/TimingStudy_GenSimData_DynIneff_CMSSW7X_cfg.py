import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.threshold = 'INFO'


# ------------------- Output Report --------------------
#process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )


# ----------------- Number of Events -------------------
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))


# ------------------- Input Files ----------------------
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:/data/store/relval/CMSSW_7_0_0_pre8/GEN-SIM/00E8DCE5-3459-E311-A9FB-0025905A608A.root'
        #'file:/data/store/relval/CMSSW_7_1_0_pre1/GEN-SIM/88CDC2A6-1186-E311-A9F5-02163E00E5C7.root'
        #'/store/caf/user/jkarancs/MinBias_TuneZ2star_8TeV_GENSIM_CMSSW_7_1_0pre1/MinBias_8TeV_GEN_SIM_2000k_1_1_FQV.root'
        'file:/data/store/relval/CMSSW_7_1_0_pre1/GEN-SIM/A4846C0D-0B86-E311-8B2E-003048FEB9EE.root'
    )
)
# GEN-SIM Input file with MinBias events
#PileupInput = process.source.fileNames
PileupInput = cms.untracked.vstring(
    #'/store/caf/user/jkarancs/MinBias_TuneZ2star_8TeV_GENSIM_CMSSW_7_1_0pre1/MinBias_8TeV_GEN_SIM_2000k_1_1_FQV.root'
    'file:/data/store/relval/CMSSW_7_1_0_pre1/GEN-SIM/88CDC2A6-1186-E311-A9F5-02163E00E5C7.root'
)
# Input pileup distribution for MixingModule
# Can use the same file in TimingStudy for pileup reweighting
# Note: the desired mc_input distribution has to be shifted by -1 wrt mcpileup
PileupHistoInput = cms.untracked.string(
    'file:PileupHistogram_201278_flatpileupMC.root' # Flat Pileup
    #'file:PileupHistogram_201278.root' # 201278 Pileup
)
PileupHistoName=cms.untracked.string('mc_input')


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


# ------------------------- DIGI -----------------------

process.load("IOMC.RandomEngine.IOMC_cff") # RandomNumberGeneratorService

# configuration to model pileup for initial physics phase
# mix
#process.load("SimGeneral.MixingModule.mixNoPU_cfi") # No Pileup
process.load('SimGeneral.MixingModule.mix_2012_201278_1_cfi') # Silvias pileup mixing

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

process.mix.digitizers.pixel.AddPixelInefficiencyFromPython = True # default: true
process.mix.digitizers.pixel.thePixelColEfficiency_BPix1 = cms.double(1.0)
process.mix.digitizers.pixel.thePixelColEfficiency_BPix2 = cms.double(1.0)
process.mix.digitizers.pixel.thePixelColEfficiency_BPix3 = cms.double(1.0)
process.mix.digitizers.pixel.thePixelColEfficiency_FPix1 = cms.double(0.999)
process.mix.digitizers.pixel.thePixelColEfficiency_FPix2 = cms.double(0.999)

process.mix.digitizers.pixel.thePixelEfficiency_BPix1 = cms.double(1.0)
process.mix.digitizers.pixel.thePixelEfficiency_BPix2 = cms.double(1.0)
process.mix.digitizers.pixel.thePixelEfficiency_BPix3 = cms.double(1.0)
process.mix.digitizers.pixel.thePixelEfficiency_FPix1 = cms.double(0.999)
process.mix.digitizers.pixel.thePixelEfficiency_FPix2 = cms.double(0.999)

process.mix.digitizers.pixel.thePixelChipEfficiency_BPix1 = cms.double(1.0)
process.mix.digitizers.pixel.thePixelChipEfficiency_BPix2 = cms.double(1.0)
process.mix.digitizers.pixel.thePixelChipEfficiency_BPix3 = cms.double(1.0)
process.mix.digitizers.pixel.thePixelChipEfficiency_FPix1 = cms.double(0.999)
process.mix.digitizers.pixel.thePixelChipEfficiency_FPix2 = cms.double(0.999)

#process.mix.digitizers.pixel.theLadderEfficiency_BPix1 = cms.vdouble(
#    0.978351,
#    0.971877,
#    0.974283,
#    0.969328,
#    0.972922,
#    0.970964,
#    0.975762,
#    0.974786,
#    0.980244,
#    0.978452,
#    0.982129,
#    0.979737,
#    0.984381,
#    0.983971,
#    0.98186,
#    0.983283,
#    0.981485,
#    0.979753,
#    0.980287,
#    0.975195,
#)
#process.mix.digitizers.pixel.theModuleEfficiency_BPix1 = cms.vdouble(
#    1.00361,
#    0.999371,
#    0.961242,
#    0.766481,
#)

#process.mix.digitizers.pixel.theLadderEfficiency_BPix2 = cms.vdouble(
#    0.996276,
#    0.993354,
#    0.993752,
#    0.992948,
#    0.993871,
#    0.992317,
#    0.997733,
#    0.992516,
#    0.992649,
#    0.993425,
#    0.994065,
#    0.993481,
#    0.993169,
#    0.994223,
#    0.992397,
#    0.99509,
#    0.995177,
#    0.995319,
#    0.994925,
#    0.992933,
#    0.994111,
#    0.9948,
#    0.994711,
#    0.994294,
#    0.995392,
#    0.994229,
#    0.994414,
#    0.995271,
#    0.993585,
#    0.995264,
#    0.992977,
#    0.993642,
#)
#process.mix.digitizers.pixel.theModuleEfficiency_BPix2 = cms.vdouble(
#    1.00069,
#    0.999792,
#    0.99562,
#    0.955627,
#)

#process.mix.digitizers.pixel.theLadderEfficiency_BPix3 = cms.vdouble(
#    0.996206,
#    0.998039,
#    0.995801,
#    0.99665,
#    0.996414,
#    0.995755,
#    0.996518,
#    0.995584,
#    0.997171,
#    0.998056,
#    0.99595,
#    0.997473,
#    0.996858,
#    0.996486,
#    0.997442,
#    0.998002,
#    0.995429,
#    0.997939,
#    0.996896,
#    0.997434,
#    0.996616,
#    0.996439,
#    0.996546,
#    0.997597,
#    0.995435,
#    0.996396,
#    0.99621,
#    0.998316,
#    0.998431,
#    0.99598,
#    0.997063,
#    0.996245,
#    0.997502,
#    0.996254,
#    0.997545,
#    0.997553,
#    0.996722,
#    0.996107,
#    0.996588,
#    0.996277,
#    0.99785,
#    0.997087,
#    0.998139,
#    0.997139,
#)
#process.mix.digitizers.pixel.theModuleEfficiency_BPix3 = cms.vdouble(
#    1.00006,
#    0.999744,
#    0.998147,
#    1.00314,
#)

process.load('Configuration.StandardSequences.Digi_cff') # pdigi

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

process.siPixelRawData.InputLabel = 'mix'


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
                                     dataPileupFile = cms.string("PileupHistogram_201278_flatpileupMC.root"),
                                     mcPileupFile   = cms.string("PileupHistogram_201278_flatpileupMC.root"),
                                     #dataPileupFile = cms.string("PileupHistogram_201278.root"),
                                     #mcPileupFile   = cms.string("PileupHistogram_201278.root"),
                                     dataPileupHistoName = cms.string("pileup"),
                                     mcPileupHistoName = cms.string("mcpileup"),
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
    process.MeasurementTrackerEvent *
    #process.ckftracks*
    process.ckftracks_woMS*
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
