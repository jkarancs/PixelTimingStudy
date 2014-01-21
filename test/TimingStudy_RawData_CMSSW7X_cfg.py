import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.threshold = 'INFO'


#-------------
# DQM services
#-------------

# process.load("DQMServices.Core.DQM_cfg")
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

#-----------------------------
# Magnetic Field
#-----------------------------

process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')

#-------------------------------------------------
# GEOMETRY
#-------------------------------------------------

process.load("Configuration.StandardSequences.GeometryIdeal_cff")

#-------------------------------------------------
# GLOBALTAG
#-------------------------------------------------

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = "GR_R_42_V25::All"
#process.GlobalTag.globaltag = "GR_R_44_V15::All"
#process.GlobalTag.globaltag = "GR_R_53_V9F::All"

process.GlobalTag.globaltag = "MC_70_V1::All"

#-------------------------
# Event Content
#-------------------------

process.load('Configuration.EventContent.EventContent_cff')

#-------------------------
#  Reconstruction Modules
#-------------------------

# clusterizer
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")

# for raw
#process.load("EventFilter.SiPixelRawToDigi.SiPixelDigiToRaw_cfi")
#process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.siPixelDigis.IncludeErrors = True # To create UserError DetIdCollection

# needed for pixel RecHits
process.load("Configuration.StandardSequences.Reconstruction_cff")

# Conditions in edm producer
process.load("EventFilter.L1GlobalTriggerRawToDigi.conditionDumperInEdm_cfi")

# Beamspot
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")

# New Producer needed for Trajectory Measurements
process.load("RecoTracker.MeasurementDet.MeasurementTrackerEventProducer_cfi")

# Local Reconstruction
# process.load("RecoLocalTracker.SiPixelRecHits.PixelCPEESProducers_cff") # for templates
# process.load("RecoLocalTracker.SiPixelClusterizer.SiPixelClusterizer_cfi")
# process.load("EventFilter.SiStripRawToDigi.SiStripRawToDigis_standard_cff")
# process.siStripDigis.ProductLabel = 'source'

# process.load("RecoLocalTracker.SiStripClusterizer.SiStripClusterizer_cfi")
# process.load("RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitConverter_cfi")
# process.load("RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitMatcher_cfi")
# process.load("RecoLocalTracker.SiStripRecHitConverter.StripCPEfromTrackAngle_cfi")
# process.load("RecoLocalTracker.SiStripZeroSuppression.SiStripZeroSuppression_cfi")

##
## Needed when running on RAW:
##
# change strip cluster threshold to reduce noise effects
##process.siStripClusters.Clusterizer.ClusterThreshold = 9
##process.siStripClusters.Clusterizer.SeedThreshold = 6
##process.siStripClusters.Clusterizer.ChannelThreshold = 4

##
## Needed when running on RECO:
##
## Fitter-smoother: loosen outlier rejection as for first data-taking with LHC "collisions"
#process.KFFittingSmootherWithOutliersRejectionAndRK.BreakTrajWith2ConsecutiveMissing = False
#process.KFFittingSmootherWithOutliersRejectionAndRK.EstimateCut = 1000
#process.KFFittingSmootherWithOutliersRejectionAndRK.NoInvalidHitsBeginEnd = False
#process.KFFittingSmootherWithOutliersRejectionAndRK.LogPixelProbabilityCut =  -16.0

#------------------------------------
# ckftracks alternative for CMSSW 70X
#------------------------------------

# trackingGlobalReco does not work, needs EarlyMuons for muon seeding.
# ckftracks & iterTracking does not work as well  (same problem).
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

#-------------------------
#  Track Refitter
#-------------------------

process.load("RecoTracker.TrackProducer.TrackRefitters_cff")

process.ctfRefitter = process.TrackRefitterP5.clone()
process.ctfRefitter.src = 'ctfWithMaterialTracksP5'
process.ctfRefitter.TrajectoryInEvent = True

process.ckfRefitter = process.TrackRefitter.clone()
process.ckfRefitter.src = 'generalTracks'
process.ckfRefitter.TrajectoryInEvent = True

process.pixlessRefitter = process.TrackRefitter.clone()
process.pixlessRefitter.src = 'ctfPixelLess'
process.pixlessRefitter.TrajectoryInEvent = True

process.load("RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilderWithoutRefit_cfi")

#--------------------------------------------------
#  Load and Configure Offline Validation - needed?
#--------------------------------------------------

process.load("Alignment.OfflineValidation.TrackerOfflineValidation_cfi")

## #-------------
## #  HLT Filter
## #-------------

## import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
## # accept if 'path_1' succeeds
## process.hltfilter = hlt.hltHighLevel.clone(
## # Min-Bias
## #   HLTPaths = ['HLT_L1Tech_BSC_minBias'],
# ---->    HLTPaths = ['HLT_L1Tech_BSC_minBias_OR'],
## #    HLTPaths = ['HLT_L1Tech_BSC_halo_forPhysicsBackground'],
## #    HLTPaths = ['HLT_L1Tech_BSC_HighMultiplicity'],
## #    HLTPaths = #['HLT_L1Tech_BSC_minBias','HLT_L1Tech_BSC_minBias_OR','HLT_L1Tech
## #_BSC_HighMultiplicity'],
## # Zero-Bias
## #    HLTPaths = #['HLT_L1_BPTX','HLT_ZeroBias','HLT_L1_BPTX_MinusOnly','HLT_L1_BP
## #TX_PlusOnly'],
## #    HLTPaths = ['HLT_L1_BPTX'],
## #    HLTPaths = ['HLT_ZeroBias'],
## #    HLTPaths = ['HLT_L1_BPTX_MinusOnly'],
## #    HLTPaths = ['HLT_L1_BPTX_PlusOnly'],
## # Commissioning: HLT_L1_BptxXOR_BscMinBiasOR
## #    HLTPaths = ['HLT_L1_BptxXOR_BscMinBiasOR'],
## #
## #    HLTPaths = ['p*'],
## #    HLTPaths = ['path_?'],
##    andOr = True,  # False = and, True=or
##    throw = False
##    )
## 
## # to select PhysicsBit
## process.load('HLTrigger.special.hltPhysicsDeclared_cfi')
## process.hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'


#-------------------------
#  Ntuplizer code
#-------------------------

##process.load("DPGAnalysis.PixelTimingStudy.TimingStudy_cfi")
process.TimingStudy = cms.EDAnalyzer("TimingStudy",
                                     trajectoryInput = cms.string('TrackRefitter'),
                                     fileName = cms.string("Raw_Ntuple.root"),
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
                                                              "HLT_MinBias")
)

# For the three cases:
process.ctfNtuple = process.TimingStudy.clone()
process.ctfNtuple.trajectoryInput = 'ctfRefitter'

process.ckfNtuple = process.TimingStudy.clone()
process.ckfNtuple.trajectoryInput = 'ckfRefitter'

process.pixlessNtuple = process.TimingStudy.clone()
process.pixlessNtuple.trajectoryInput = 'pixlessRefitter'


#-------------------------------------------------
# Configure output ntuple file using TFileService
#-------------------------------------------------

##process.TFileService = cms.Service("TFileService", 
##                                   ___file_Name___ = cms.string("test.root"),
##                                   closeFileFast = cms.untracked.bool(True)
##                                   )

#-------------------------------------------------
# Input files:
#-------------------------------------------------

process.source = cms.Source("PoolSource",
                            # replace with your files
                            #lastRun = cms.untracked.uint32(64789),
                            #timetype = cms.string('runnumber'),
                            #firstRun = cms.untracked.uint32(64108),
                            #interval = cms.uint32(1),
                            #firstLuminosityBlock = cms.untracked.uint32(44),
    fileNames = cms.untracked.vstring(
#CMSSW70X RAW test file
'/store/relval/CMSSW_7_0_0_pre11/RelValProdTTbar/GEN-SIM-RAW/START70_V4-v1/00000/36994557-496A-E311-A69D-002618943868.root'

# High Background Fill to test 42X
#'/store/data/Run2011B/MinimumBias/RAW/v1/000/178/421/9E3586B3-5BF5-E011-8F0B-BCAEC518FF6E.root'
#'file:/home/jkarancs/RAW/MinimumBias_Run2011B-v1_RAW/CMSSW_4_2_7/0CBB28E2-CCDD-E011-8F23-003048D2C020.root'
)
)

##process.source.inputCommand = cms.untracked.vstring("drop *_*_*_FU"
##                                                    ,"drop *_*_*_HLT",
##                                                    "drop *_MEtoEDMConverter_*_*","drop *_lumiProducer_*_REPACKER"
##                                                    )

#-------------------------------------------------
# Number of events
#-------------------------------------------------

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1))

#-------------------------------------------------
# Path
#-------------------------------------------------
#process.pl = cms.Path(
#        # Lum
#    process.lumiProducer*
#    process.conditionsInEdm
#)

#process.p = cms.Path(
#    # Trigger selection:
#    # process.hltLevel1GTSeed*
#    #process.hltPhysicsDeclared*
#    #process.hltfilter*
#    # Beamspot:
#    process.offlineBeamSpot*
#    # Digi
#    process.scalersRawToDigi*
#    process.gtEvmDigis*
#    process.siPixelDigis*
#    process.siStripDigis*
#    #//process.RawToDigi*process.reconstructionCosmics*
#    # Track reco:
#    process.trackerlocalreco*
#    process.MeasurementTrackerEvent*
#    process.recopixelvertexing*
#    process.ckftracks*
#    #process.ckftracks_plus_pixelless*
#    # Vertex reco:
#    process.vertexreco*
#    # Lumi blocks:
#    process.conditionDumperInEdm*
#    process.lumiProducer*
#    # Refitters:
##    process.ctfRefitter*
#    process.ckfRefitter*
##    process.pixlessRefitter*
#    # Ntuplizers
##    process.ctfNtuple
#    process.ckfNtuple
##    process.pixlessNtuple
#    )

process.p = cms.Path(
    # DigiToRaw (Needed for MC)
    process.SiStripDigiToRaw*
    # Digi
    process.scalersRawToDigi*
    process.gtEvmDigis*
    process.siPixelDigis*
    process.siStripDigis*
    #process.RawToDigi*              #Whole detector
    # Reco
    process.trackerlocalreco*
    #process.localreco*              #Whole detector
    process.offlineBeamSpot*
    process.recopixelvertexing*
    process.MeasurementTrackerEvent* #New in 70X
    #process.standalonemuontracking*
    #process.iterTracking 
    #process.ckftracks               #has iterTracking
    process.ckftracks_woMS*
    process.vertexreco*
    process.conditionDumperInEdm*
    process.lumiProducer*
    # Refitter
    process.ckfRefitter*
    # Ntuplizer
    process.ckfNtuple
    )

#process.output = cms.OutputModule("PoolOutputModule",
#    fileName = cms.untracked.string("TestReco.root")
#    )
#process.outpath = cms.EndPath(process.output)
