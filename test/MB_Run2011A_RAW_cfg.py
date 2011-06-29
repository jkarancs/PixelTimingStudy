import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

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

process.load("Configuration.StandardSequences.Geometry_cff")


#-------------------------------------------------
# GLOBALTAG
#-------------------------------------------------

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = "GR_R_35X_V8B::All"
#process.GlobalTag.globaltag = "GR_R_38X_V13A::All"
#process.GlobalTag.globaltag = "FT_R_38X_V14A::All"
#process.GlobalTag.globaltag = "GR_R_39X_V5::All"
#process.GlobalTag.globaltag = "GR_R_39X_V6::All"
#process.GlobalTag.globaltag = "GR_R_38X_V14::All"
#process.GlobalTag.globaltag = "GR_R_311_V2::All"
#process.GlobalTag.globaltag = "GR_P_V14:All"
#process.GlobalTag.globaltag = "GR_P_V17::All"
#process.GlobalTag.globaltag = "GR_R_41_V0::All""
#process.GlobalTag.globaltag = "GR_R_38X_V15::All"
process.GlobalTag.globaltag = "GR_R_42_V14::All"


#-------------------------
#  Reconstruction Modules
#-------------------------

process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
##process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
#process.MeasurementTracker.inactivePixelDetectorLabels = cms.VInputTag(cms.InputTag('siPixelDigis'))

#process.DBService=cms.Service("DBService",
#           authPath=cms.untracked.string('/afs/cern.ch/cms/DB/lumi')
#)
#process.lumiProducer=cms.EDProducer("LumiProducer",
#   #connect=cms.string('frontier://cmsfrontier.cern.ch:8000/LumiPrep/CMS_LUMI_DEV_OFFLINE'),
#   #connect=cms.string('oracle://cms_orcoff_prep/cms_lumi_dev_offline'),
#   connect=cms.string('frontier://LumiProd/CMS_LUMI_PROD'),
#   lumiversion=cms.untracked.string('0001')
#)

# Conditions in edm producer
process.load("EventFilter.L1GlobalTriggerRawToDigi.conditionDumperInEdm_cfi")
process.conditionsInEdm = cms.EDProducer("ConditionDumperInEdm",
    gtEvmDigisLabel = cms.InputTag("gtEvmDigis")
)

print process.MeasurementTracker.inactiveStripDetectorLabels

process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")

# Real data raw to digi
# process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
# process.siPixelDigis.InputLabel = 'source'
process.siPixelDigis.IncludeErrors = True

# Local Reconstruction
process.load("RecoLocalTracker.SiPixelRecHits.PixelCPEESProducers_cff")
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
process.KFFittingSmootherWithOutliersRejectionAndRK.EstimateCut = 1000
process.KFFittingSmootherWithOutliersRejectionAndRK.NoInvalidHitsBeginEnd = False
process.KFFittingSmootherWithOutliersRejectionAndRK.LogPixelProbabilityCut =  -16.0


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
## #  HLT Trigger
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

# Trigger filter
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
#process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('40 OR 41')
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('(40 OR 41) AND ( NOT 36) AND ( NOT 37) AND ( NOT 38) AND ( NOT 39) AND 0 AND ( ( 42 AND 43) OR ( NOT 42 AND ( NOT 43) ) )')


#-------------------------
#  Ntuplizer code
#-------------------------

##process.load("DPGAnalysis.PixelTimingStudy.TimingStudy_cfi")
process.TimingStudy = cms.EDAnalyzer("TimingStudy",
                                     trajectoryInput = cms.string('TrackRefitter'),
                                     fileName = cms.string("MB_Run2011A_RAW.root"),
                                     extrapolateFrom = cms.int32(2),
                                     extrapolateTo = cms.int32(1),
                                     keepOriginalMissingHit = cms.bool(False),
                                     usePixelCPE= cms.bool(True),
                                     triggerNames=cms.vstring("HLT_IsoTrackHB_8E29",
                                                              "HLT_IsoTrackHE_8E29",
                                                              "HLT_IsoTrackHE"
                                                              "HLT_L1Tech_BSC_HighMultiplicity",
                                                              "HLT_L1Tech_BSC_halo_forPhysicsBackground",
                                                              "HLT_L1Tech_HCAL_HF",
                                                              "HLT_L1Tech_RPC_TTU_RBst1_collisions",
                                                              "HLT_L1_BscMinBiasOR_BptxPlusORMinus",
                                                              "HLT_MinBiasBSC",
                                                              "HLT_MinBiasBSC_NoBPTX",
                                                              "HLT_L1Tech_BSC_minBias",
                                                              "HLT_L1Tech_BSC_minBias_OR",
                                                              "HLT_MinBiasPixel_DoubleIsoTrack5",
                                                              "HLT_MinBiasPixel_DoubleTrack",
                                                              "HLT_MinBiasPixel_SingleTrack",
                                                              "HLT_PixelTracks_Multiplicity40",
                                                              "HLT_PixelTracks_Multiplicity70",
                                                              "HLT_PixelTracks_Multiplicity85",
                                                              "HLT_PixelTracks_Multiplicity100",
                                                              "HLT_StoppedHSCP_8E29",
                                                              "HLT_StoppedHSCP",
                                                              "HLT_StoppedHSCP20_v3",
                                                              "HLT_StoppedHSCP35_v3",
                                                              "HLT_ZeroBiasPixel_SingleTrack",
                                                              "HLT_ZeroBias",
                                                              "HLT_L1Tech_BSC_minBias",
                                                              "HLT_MultiVertex6",
                                                              "HLT_MultiVertex8_L1ETT60",
                                                              "HLT_L1_BPTX",
                                                              "HLT_L1_BPTX_MinusOnly",
                                                              "HLT_L1_BPTX_PlusOnly",
                                                              "HLT_Random")
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
#'file:/shared/data/CMSSW_4_1_2/MinimumBias__Run2011A-v1__RAW/DA580914-CE54-E011-AFB9-003048F024FE.root'
#'file:/data/jkarancs/RAW/MinimumBias_Run2011A-v1_RAW/BC11BA35-B492-E011-AF17-003048F118E0.root'
#'file:/data/jkarancs/RAW/MinimumBias_Run2011A-v1_RAW/10020C19-4353-E011-8BAE-001617E30D40.root'
#'file:/data/jkarancs/RAW/MinimumBias_Run2011A-v1_RAW/DA580914-CE54-E011-AFB9-003048F024FE.root'
#'file:/data/jkarancs/RAW/MinimumBias_Run2011A-v1_RAW/72DAFE04-F84D-E011-A8EE-0030487C2B86.root'
#'/store/data/Run2010A/MinimumBias/RAW/v1/000/136/035/54C7B44E-8B65-DF11-A80A-0030487CD840.root'
#'/store/data/Run2010B/MinimumBias/RAW/v1/000/149/181/6EE133DC-3BE2-DF11-841D-003048F01E88.root'
'file:/data/jkarancs/RAW/MinimumBias_Run2010B-v1_RAW/6EE133DC-3BE2-DF11-841D-003048F01E88.root'
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
    input = cms.untracked.int32(10) )

#-------------------------------------------------
# Path
#-------------------------------------------------
#process.pl = cms.Path(
#        # Lum
#    process.lumiProducer*
#    process.conditionsInEdm
#)

process.p = cms.Path(
    # Trigger selection:
    # process.hltLevel1GTSeed*
    #process.hltPhysicsDeclared*
    #process.hltfilter*
    # Reco:
    process.siPixelDigis*process.siStripDigis*
    #//process.RawToDigi*process.reconstructionCosmics*
    # Beamspot:
    process.offlineBeamSpot*
    # Track reco:
    process.trackerlocalreco*process.recopixelvertexing*process.ckftracks_plus_pixelless*
    # Vertex reco:
    process.vertexreco*
    # Lumi blocks:
    process.lumiProducer*process.conditionsInEdm*
    # Refitters:
#    process.ctfRefitter*
    process.ckfRefitter*
#    process.pixlessRefitter*
    # Ntuplizers
#    process.ctfNtuple
    process.ckfNtuple
#    process.pixlessNtuple
    )

#process.output = cms.OutputModule("PoolOutputModule",
#fileName = cms.untracked.string("/data/jkarancs/RAW/MinimumBias_Run2011A-v1_RAW/RECOout/TESTRECO.root")
#)
#process.outpath = cms.EndPath(process.output)

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.threshold = 'INFO'
process.TrackerDigiGeometryESModule.applyAlignment = True
