import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

# DQM services
process.load("DQMServices.Core.DQM_cfg")

# Database configuration
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

# conditions
# either:
#process.load("Configuration.StandardSequences.MagneticField_38T_cff")
# or:
#process.load("Configuration.StandardSequences.MagneticField_cff")
#process.load("Configuration.GlobalRuns.ForceZeroTeslaField_cff")
# or the supposedly general solution:
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.connect = "frontier://FrontierInt/CMS_COND_30X_GLOBALTAG"
#process.GlobalTag.globaltag = "CRAFT0831X_V1::All"
#process.GlobalTag.globaltag = "GR09_31X_V5P::All"
process.GlobalTag.globaltag = "CRAFT09_R_V4::All"

##
## Load and Configure track selection for alignment
##
#process.load("Alignment.CommonAlignmentProducer.AlignmentTrackSelector_cfi")
#process.AlignmentTrackSelector.ptMin = 3.0

# reconstruction sequence for Cosmics
# reconstruction sequence for Cosmics
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")


##
## Load and Configure TrackRefitter
##
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
# tracks available in cosmic reco: 'cosmictrackfinderP5', 'ctfWithMaterialTracksP5', 'rsWithMaterialTracksP5'
#process.TrackRefitterP5.src =  'cosmictrackfinderP5' #'ctfWithMaterialTracksP5'
# AlCa tracks:
#processlTrackRefitterP5.src = 'ALCARECOTkAlCosmicsCosmicTF0T'
#process.TrackRefitterP5.TrajectoryInEvent = True

# Instead, load all three track algorithms:
process.ctfRefitter = process.TrackRefitterP5.clone()
process.ctfRefitter.src = 'ctfWithMaterialTracksP5'
process.ctfRefitter.TrajectoryInEvent = True
process.rsRefitter = process.TrackRefitterP5.clone()
process.rsRefitter.src = 'rsWithMaterialTracksP5'
process.rsRefitter.TrajectoryInEvent = True
process.cosmRefitter = process.TrackRefitterP5.clone()
process.cosmRefitter.src = 'cosmictrackfinderP5'
process.cosmRefitter.TrajectoryInEvent = True


process.load("RecoTracker.TransientTrackingRecHit.TransientTrackingRecHitBuilderWithoutRefit_cfi")

##
## Load and Configure OfflineValidation
##
process.load("Alignment.OfflineValidation.TrackerOfflineValidation_cfi")

##
## Pixel event filters:
##

process.load("DPGAnalysis.SiPixelTools.muonTOF_cfi")
process.load("DPGAnalysis.SiPixelTools.FEDInRunFilter_cfi")
process.MuonTOFFilter_trackQuality.max_goodmuons = 2
process.MuonTOFFilter_trackQuality.max_timeError = 15
process.MuonTOFFilter_trackQuality.max_chi2_ndof = 15


##
##  Ntuplizer
##

process.load("DPGAnalysis.SiPixelTools.TimingStudy_cfi")

# also run 3 times:
process.ctfNtuple = process.TimingStudy.clone()
process.ctfNtuple.trajectoryInput = 'ctfRefitter'

process.rsNtuple = process.TimingStudy.clone()
process.rsNtuple.trajectoryInput = 'rsRefitter'

process.cosmtfNtuple = process.TimingStudy.clone()
process.cosmtfNtuple.trajectoryInput = 'cosmRefitter'

#process.load("DPGAnalysis.SiPixelTools.TimingStudy_cfi")
#process.TimingStudy.trajectoryInput = 'TrackRefitterP5'


##
## configure output ntuple file using TFileService
##

process.TFileService = cms.Service("TFileService", 
                                   fileName = cms.string("test.root"),
                                   closeFileFast = cms.untracked.bool(True)
                                   )

##
## Configure input file
##
process.source = cms.Source("PoolSource",
    # replace with your files
    #lastRun = cms.untracked.uint32(64789),
    #timetype = cms.string('runnumber'),
    #firstRun = cms.untracked.uint32(64108),
    #interval = cms.uint32(1),
    fileNames = cms.untracked.vstring(
    '/store/data/Commissioning09/Cosmics/RECO/v8/000/116/736/FAB2D8E0-9AB4-DE11-BEB6-001617DBCF6A.root'
    #'/store/data/Commissioning08/Cosmics/RAW-RECO/CRAFT_ALL_V9_SuperPointing_225-v3/0006/26B79166-7C00-DE11-AA81-0030486790A0.root'
    # Run 69997
  ##  'file:/home/veszpv/Project/Pixel/Run_69997/E8F05F92-FE73-DE11-925D-0018F3D09608.root',
##    'file:/home/veszpv/Project/Pixel/Run_69997/F8C7BEB7-F773-DE11-B816-0018F3D096EA.root',
    #
    # Run 68021
##    'file:/home/veszpv/Project/Pixel/Run_68021/02F99E95-FE73-DE11-AA48-001A92971B06.root',
##    'file:/home/veszpv/Project/Pixel/Run_68021/1049DBA7-F773-DE11-BB21-001731AF66BF.root',
##    'file:/home/veszpv/Project/Pixel/Run_68021/1CF19B92-F773-DE11-980F-0018F3D096CA.root',
##    'file:/home/veszpv/Project/Pixel/Run_68021/3E1CD2B9-F773-DE11-9652-0018F3D09636.root',
##    'file:/home/veszpv/Project/Pixel/Run_68021/5CCAA394-F773-DE11-AF2A-0018F3D09636.root',
##    'file:/home/veszpv/Project/Pixel/Run_68021/AE7D20BD-F773-DE11-936E-001731A28585.root',
##    'file:/home/veszpv/Project/Pixel/Run_68021/F8C522CD-9E73-DE11-A06C-001731AF684D.root',
    #
    # Run 68288 and Run 68279
##    'file:/home/veszpv/Project/Pixel/Run_68288/4A545F91-FE73-DE11-891E-001A92971AAA.root',
##    'file:/home/veszpv/Project/Pixel/Run_68288/6077F3B1-F773-DE11-9F50-0018F3D095FE.root',
##    'file:/home/veszpv/Project/Pixel/Run_68288/6231E3A1-F773-DE11-B853-0018F3D09642.root',
##    'file:/home/veszpv/Project/Pixel/Run_68288/723447BC-F773-DE11-92DB-001A92971B88.root',
##    'file:/home/veszpv/Project/Pixel/Run_68288/DCAE1C83-A973-DE11-994E-003048679030.root',
##    'file:/home/veszpv/Project/Pixel/Run_68288/FAE63098-F773-DE11-87FF-001A92971BBE.root',
##    'file:/home/veszpv/Project/Pixel/Run_68279/16529CA0-F773-DE11-A688-0018F3D096DA.root',
##    'file:/home/veszpv/Project/Pixel/Run_68279/202C4C9E-F773-DE11-A0D0-0018F3D096F0.root',
##    'file:/home/veszpv/Project/Pixel/Run_68279/962D69B4-F773-DE11-8E59-0018F3D0967E.root'

) )

# these drop commands are necessary to get rid of all HLT problems and DQM bulk
process.source.inputCommand = cms.untracked.vstring("drop *_*_*_FU"
                                                    ,"drop *_*_*_HLT",
                                                    "drop *_MEtoEDMConverter_*_*","drop *_lumiProducer_*_REPACKER"
                                                    )
##
## number of events
##
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1) )

##
## executionpath
##
process.p = cms.Path(
    # filters:
#process.fedInRunFilter*
    # standard reco sequence
#process.RawToDigi*process.reconstructionCosmics*
    # more filters:
#process.MuonTOFFilter_trackQuality *
    # create rechits
    # (rechits are transient data members so are not saved in CMSSW .root files)
    process.offlineBeamSpot*
#   refitters for all tracking algos (thse are what actually create the rechits)
    process.ctfRefitter*
#    process.rsRefitter*
#    process.cosmRefitter*
    # run ntuplizers
    process.ctfNtuple
#    process.rsNtuple*
#    process.cosmtfNtuple
    )
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.MessageLogger.cerr.threshold = 'INFO'
process.TrackerDigiGeometryESModule.applyAlignment = True


