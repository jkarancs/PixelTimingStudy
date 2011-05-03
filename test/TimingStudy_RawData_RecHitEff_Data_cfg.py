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
process.GlobalTag.globaltag = "GR_P_V14::All" #"GR_R_38X_V13A::All"


#-------------------------
#  Reconstruction Modules
#-------------------------

process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
##process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
#process.MeasurementTracker.inactivePixelDetectorLabels = cms.VInputTag(cms.InputTag('siPixelDigis'))

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
process.KFFittingSmootherWithOutliersRejectionAndRK.BreakTrajWith2ConsecutiveMissing =False
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
                                   fileName = cms.string("MinBiasRaw.root"),
                                   extrapolateFrom = cms.int32(2),
                                   extrapolateTo = cms.int32(1),
                                   keepOriginalMissingHit = cms.bool(False),
                                   usePixelCPE= cms.bool(True),
                                   triggerNames=cms.vstring("HLT_Mu11",
                                                            "HLT_L1Tech_BSC",
                                                            "HLT_L1Tech_BSC_minBias_OR")
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
                            firstLuminosityBlock = cms.untracked.uint32(44),
    fileNames = cms.untracked.vstring(
#'file:/data/common/CMSSW/CMSSW_4_1_2/MinimumBias__Run2011A-v1__RAW/DA580914-CE54-E011-AFB9-003048F024FE.root'
'file:/shared/data/CMSSW_4_1_2/MinimumBias__Run2011A-v1__RAW/DA580914-CE54-E011-AFB9-003048F024FE.root'

#'file:/data/common/CMSSW/CMSSW_4_1_2/MinimumBias__Run2011A-PromptReco-v1__RECO/FEB69734-4151-E011-9C69-003048F1C58C.root'
#'file:/home/veszpv/data/CMSSW_3_9_7/MinimumBias_Run2010B-Dec22ReReco_v1_RECO/8CB02934-9B1A-E011-BFE8-002618943959.root'
#'file:/home/veszpv/data/CMSSW_3_9_7/Mu__Run2010B-Dec22ReReco_v1__RECO/D6241B16-F60F-E011-82F0-90E6BA442F08.root'
                                
#'file:/home/veszpv/data/CMSSW_3_9_7/Mu__Run2010B-Dec22ReReco_v1__RECO/C648EE14-070F-E011-A1AA-E0CB4E29C4B9.root',
#'file:/home/veszpv/data/CMSSW_3_9_7/Mu__Run2010B-Dec22ReReco_v1__RECO/8ECD1724-550E-E011-AD3C-485B39800B9F.root',
#'file:/home/veszpv/data/CMSSW_3_9_7/Mu__Run2010B-Dec22ReReco_v1__RECO/E21CC56E-4B0E-E011-931A-E0CB4E1A1167.root',
#'file:/home/veszpv/data/CMSSW_3_9_7/Mu__Run2010B-Dec22ReReco_v1__RECO/BCEA5E1C-BB0E-E011-9A63-0030487CB568.root'
## 'file:/home/veszpv/data/CMSSW_3_9_7/MinimumBias_Run2010B-Dec22ReReco_v1_RECO/3ED0462F-F21A-E011-A96C-002354EF3BDA.root',
## 'file:/home/veszpv/data/CMSSW_3_9_7/MinimumBias_Run2010B-Dec22ReReco_v1_RECO/4A0EB41E-F11A-E011-9170-002618943906.root',
## 'file:/home/veszpv/data/CMSSW_3_9_7/MinimumBias_Run2010B-Dec22ReReco_v1_RECO/620163C0-081B-E011-B528-002618943948.root',
## 'file:/home/veszpv/data/CMSSW_3_9_7/MinimumBias_Run2010B-Dec22ReReco_v1_RECO/8294B9FF-0A1B-E011-8165-002618943934.root'

)
)

# these drop commands are necessary to get rid of all HLT problems and DQM bulk
##process.source.inputCommand = cms.untracked.vstring("drop *_*_*_FU"
##                                                    ,"drop *_*_*_HLT",
##                                                    "drop *_MEtoEDMConverter_*_*","drop *_lumiProducer_*_REPACKER"
##                                                    )


#-------------------------------------------------
# Number of events
#-------------------------------------------------

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100) )


#-------------------------------------------------
# Path
#-------------------------------------------------
process.pl = cms.Path(
        # Lum
    process.lumiProducer
)

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
    # Refitters:
#    process.ctfRefitter*
    process.ckfRefitter*
#    process.pixlessRefitter*
    # Ntuplizers
#    process.ctfNtuple
    process.ckfNtuple
#    process.pixlessNtuple
    )

process.output = cms.OutputModule("PoolOutputModule",
fileName = cms.untracked.string("/shared/data/CMSSW_4_1_2/TimingStudy/DA580914-CE54-E011-AFB9-003048F024FE_TESTRECO.root")
)
process.outpath = cms.EndPath(process.output)

process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.MessageLogger.cerr.threshold = 'INFO'
process.TrackerDigiGeometryESModule.applyAlignment = True



