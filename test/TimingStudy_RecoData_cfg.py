import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

#-------------
# DQM services
#-------------

# process.load("DQMServices.Core.DQM_cfg")
# process.load("CondCore.DBCommon.CondDBCommon_cfi")                ???
# process.load("CondCore.DBCommon.CondDBSetup_cfi")

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
process.GlobalTag.globaltag = "GR09_P_V4::All"

#-------------------------
#  Reconstruction Modules
#-------------------------

process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
##process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")

# Real data raw to digi
# process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
# process.siPixelDigis.InputLabel = 'source'
# process.siPixelDigis.IncludeErrors = True

# Local Reconstruction
# process.load("RecoLocalTracker.SiPixelClusterizer.SiPixelClusterizer_cfi")
# process.load("EventFilter.SiStripRawToDigi.SiStripRawToDigis_standard_cff")
# process.siStripDigis.ProductLabel = 'source'

# process.load("RecoLocalTracker.SiStripClusterizer.SiStripClusterizer_cfi")
# process.load("RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitConverter_cfi")
# process.load("RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitMatcher_cfi")
# process.load("RecoLocalTracker.SiStripRecHitConverter.StripCPEfromTrackAngle_cfi")
# process.load("RecoLocalTracker.SiStripZeroSuppression.SiStripZeroSuppression_cfi")

# change strip cluster threshold to reduce noise effects
process.siStripClusters.Clusterizer.ClusterThreshold = 9
process.siStripClusters.Clusterizer.SeedThreshold = 6
process.siStripClusters.Clusterizer.ChannelThreshold = 4



#-------------------------
#  Track Refitter
#-------------------------

process.load("RecoTracker.TrackProducer.TrackRefitters_cff")

process.ctfRefitter = process.TrackRefitterP5.clone()
process.ctfRefitter.src = 'ctfWithMaterialTracksP5'
process.ctfRefitter.TrajectoryInEvent = True

process.ckfRefitter = process.TrackRefitter.clone()
process.ckfRefitter.src = 'globalTracks'
process.ckfRefitter.TrajectoryInEvent = True

process.pixlessRefitter = process.TrackRefitter.clone()
process.pixlessRefitter.src = 'ctfPixelLess'
process.pixlessRefitter.TrajectoryInEvent = True

process.load("RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilderWithoutRefit_cfi")

#--------------------------------------------------
#  Load and Configure Offline Validation - needed?
#--------------------------------------------------

process.load("Alignment.OfflineValidation.TrackerOfflineValidation_cfi")

#-------------------------
#  Ntuplizer code
#-------------------------

process.TimingStudy = cms.EDFilter("TimingStudy",
                                   trajectoryInput = cms.string('TrackRefitter'),
                                   OutputFile = cms.string('TTreeFile.root')
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

process.TFileService = cms.Service("TFileService", 
                                   fileName = cms.string("test.root"),
                                   closeFileFast = cms.untracked.bool(True)
                                   )

#-------------------------------------------------
# Input files:
#-------------------------------------------------

process.source = cms.Source("PoolSource",
                            # replace with your files
                            #lastRun = cms.untracked.uint32(64789),
                            #timetype = cms.string('runnumber'),
                            #firstRun = cms.untracked.uint32(64108),
                            #interval = cms.uint32(1),
    fileNames = cms.untracked.vstring(
    #'/store/data/CRAFT09/Cosmics/RAW-RECO/SuperPointing-CRAFT09_R_V4_CosmicsSeq_v1/0009/F8D1CF5D-DCB9-DE11-9C5D-00261894393A.root'
    #'file:/home/veszpv/data/CMSSW_3_2_7/CRAFT09-SuperPointing-CRAFT09_R_V4_CosmicsSeq_v1/763782DB-DCB9-DE11-A238-003048678B30.root'
    #'file:/home/veszpv/data/CMSSW_3_3_3/MinimumBias.BeamCommissioning09-v1.RAW/E6F8D1B6-B1D8-DE11-83B6-001D09F2AD7F.root'
    'file:/home/veszpv/data/CMSSW_3_3_4/MinimumBias.BeamCommissioning09-PromptReco-v2.RECO/30BB156B-8FDD-DE11-92FD-003048D37456.root'

) )

# these drop commands are necessary to get rid of all HLT problems and DQM bulk
process.source.inputCommand = cms.untracked.vstring("drop *_*_*_FU"
                                                    ,"drop *_*_*_HLT",
                                                    "drop *_MEtoEDMConverter_*_*","drop *_lumiProducer_*_REPACKER"
                                                    )


#-------------------------------------------------
# Number of events
#-------------------------------------------------

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1) )


#-------------------------------------------------
# Path
#-------------------------------------------------

process.p = cms.Path(
    # Reco:
    #process.siPixelDigis*process.siStripDigis*
    #//process.RawToDigi*process.reconstructionCosmics*
    # Beamspot:
    process.offlineBeamSpot*
    # Track reco:
    #process.trackerlocalreco*process.recopixelvertexing*process.ckftracks_plus_pixelless*
    # Refitters:
#    process.ctfRefitter*
    process.ckfRefitter*
    process.pixlessRefitter*
    # Ntuplizers
#    process.ctfNtuple
    process.ckfNtuple*
    process.pixlessNtuple
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.MessageLogger.cerr.threshold = 'INFO'
process.TrackerDigiGeometryESModule.applyAlignment = True



