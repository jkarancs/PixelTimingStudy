import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.threshold = 'INFO'

## Options and Output Report
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

#-------------
# DQM services
#-------------

process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

#-----------------------------
# Magnetic Field
#-----------------------------

process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')

#-------------------------------------------------
# GEOMETRY
#-------------------------------------------------

process.load("Configuration.Geometry.GeometryIdeal_cff")

#-------------------------------------------------
# GLOBALTAG
#-------------------------------------------------

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = "GR_R_53_V14::All"
#process.GlobalTag.globaltag = "FT_R_53_V18::All"

# For MC
#process.GlobalTag.globaltag = "START53_V27::All"
process.GlobalTag.globaltag = "MC_70_V1::All"

#-------------------------
#  Reconstruction Modules
#-------------------------

process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")

# Local Reconstruction
process.load("RecoLocalTracker.SiPixelRecHits.PixelCPEESProducers_cff")

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

#--------------------
#  HLT Trigger filter
#--------------------

# import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
# # accept if 'path_1' succeeds
# process.hltfilter = hlt.hltHighLevel.clone(
# # Min-Bias
# #    HLTPaths = ['HLT_L1Tech_BSC_minBias'],
# #    HLTPaths = ['HLT_L1Tech_BSC_minBias_OR'],
# #    HLTPaths = ['HLT_L1Tech_BSC_halo_forPhysicsBackground'],
# #    HLTPaths = ['HLT_L1Tech_BSC_HighMultiplicity'],
# #    HLTPaths = ['HLT_L1Tech_BSC_minBias','HLT_L1Tech_BSC_minBias_OR','HLT_L1Tech_BSC_HighMultiplicity'],
# # Zero-Bias
# #    HLTPaths = ['HLT_L1_BPTX','HLT_ZeroBias','HLT_L1_BPTX_MinusOnly','HLT_L1_BPTX_PlusOnly'],
# #    HLTPaths = ['HLT_L1_BPTX'],
#     HLTPaths = ['HLT_ZeroBias'],
# #    HLTPaths = ['HLT_L1_BPTX_MinusOnly'],
# #    HLTPaths = ['HLT_L1_BPTX_PlusOnly'],
# # Commissioning: HLT_L1_BptxXOR_BscMinBiasOR
# #    HLTPaths = ['HLT_L1_BptxXOR_BscMinBiasOR'],
# #    HLTPaths = ['p*'],
# #    HLTPaths = ['path_?'],
#    andOr = True,  # False = and, True=or
#    throw = False
#    )
# 
# # to select PhysicsBit
# process.load('HLTrigger.special.hltPhysicsDeclared_cfi')
# process.hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'


#-------------------------
#  Ntuplizer code
#-------------------------

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
                                     mcPileupFile   = cms.string("data/PileupHistogram_test.root"),
                                     dataPileupFile = cms.string("data/PileupHistogram_test.root"),
                                     mcPileupHistoName = cms.string("mcpileup"),
                                     dataPileupHistoName = cms.string("pileup")
)

# For the three cases:
process.ctfNtuple = process.TimingStudy.clone()
process.ctfNtuple.trajectoryInput = 'ctfRefitter'

process.ckfNtuple = process.TimingStudy.clone()
process.ckfNtuple.trajectoryInput = 'ckfRefitter'

process.pixlessNtuple = process.TimingStudy.clone()
process.pixlessNtuple.trajectoryInput = 'pixlessRefitter'

#-------------------------------------------------
# Input files:
#-------------------------------------------------

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
# CMSSW_7_X:
'file:/data/store/relval/CMSSW_7_1_0_pre1/RelValProdMinBias/GEN-SIM-RECO/START70_V5-v1/00000/0869C91A-1C86-E311-B4C7-02163E00EB49.root'

# CMSSW_5_X RECO Files
#'/store/data/Run2012C/MinimumBias/RECO/22Jan2013-v1/20006/04E396DF-9172-E211-B250-003048FEAF90.root'
#'/store/data/Run2012C/MinimumBias/RECO/PromptReco-v2/000/201/278/A0EEE587-55ED-E111-B860-003048F118DE.root'

# Silvia's MC sample - 10k events
#'/store/user/taroni/Pixel/PUComparisonE/STEP2_normalized.root'
)
)

#-------------------------------------------------
# Number of events
#-------------------------------------------------

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

#-------------------------------------------------
# Path
#-------------------------------------------------

process.p = cms.Path(
    # Trigger selection:
    #process.hltPhysicsDeclared*
    #process.hltfilter*
    # Beamspot:
    #process.offlineBeamSpot*
    # New in 7X
    process.MeasurementTrackerEvent*    
    # Refitters:
#    process.ctfRefitter*
    process.ckfRefitter*
#    process.pixlessRefitter*
    # Ntuplizers
#    process.ctfNtuple
    process.ckfNtuple
#    process.pixlessNtuple
    )
