# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: --data -s RAW2DIGI,RECO --conditions auto:run2_data --datatier RECO --eventcontent RECO --process TimingStudy --customise RecoTracker/Configuration/customiseForRunI.customiseForRunI --filein file:/data/store/data/Commissioning2015/MinimumBias/RAW/v1/000/245/194/00000/782163E3-B6FF-E411-B946-02163E0133E3.root --fileout file:RECO.root --python_filename=test/TimingStudy_RunIIZeroTeslaRawData_CMSSW7X_cfg.py -n 10 --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('TimingStudy')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        # Run1 - 0T
        'file:Run200991_ZeroTesla_RAW.root'
        # Run2 - 0T (No Pixels)
        #'file:/data/store/data/Commissioning2015/MinimumBias/RAW/v1/000/245/194/00000/782163E3-B6FF-E411-B946-02163E0133E3.root'
        ),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('--data nevts:10'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RECOoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RECO'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('file:RECO.root'),
    outputCommands = process.RECOEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)

# Schedule definition
#process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step)

# customisation of the process.

# Automatic addition of the customisation function from RecoTracker.Configuration.customiseForRunI
from RecoTracker.Configuration.customiseForRunI import customiseForRunI 

#call to customisation function customiseForRunI imported from RecoTracker.Configuration.customiseForRunI
process = customiseForRunI(process)

# End of customisation functions



# Added for TimingStudy
# Comment out line 69 (schedule)
# Create simplified Run I like tracking configuration up to Tracker steps (exclude muon and electron steps)
# configure simple Vertexing to use earlyGeneralTracks
process.unsortedOfflinePrimaryVertices.TrackLabel = "earlyGeneralTracks"

## Simplified Tracker only tracks (exclude muon seeded etc. steps)
process.raw2digi_Tracker_only_step = cms.Path(process.siPixelDigis * process.siStripDigis)

process.reconstruction_Tracker_only_step = cms.Path(
    process.trackerlocalreco*
    # Simplified globalreco
    process.offlineBeamSpot*
    process.MeasurementTrackerEvent*
    process.siPixelClusterShapeCache*
    ## Simplified trackingGlobalReco
    process.recopixelvertexing*
    ##   iterTracking
    process.InitialStep*
    process.LowPtTripletStep*
    process.PixelPairStep*
    process.DetachedTripletStep*
    process.MixedTripletStep*
    process.PixelLessStep*
    process.TobTecStep*
    process.earlyGeneralTracks*
    # Vertexreco, configured to use earlyGeneralTracks
    process.unsortedOfflinePrimaryVertices
    )

# TimingStudy
process.TimingStudy = cms.EDAnalyzer("TimingStudy",
    trajectoryInput = cms.string('earlyGeneralTracks'),
    fileName = cms.string("Ntuple.root"),
    extrapolateFrom = cms.int32(2),
    extrapolateTo = cms.int32(1),
    keepOriginalMissingHit = cms.bool(False),
    usePixelCPE= cms.bool(True),
    minNStripHits = cms.int32(0),
    triggerNames=cms.vstring(
        "HLT_ZeroBias",
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
        "HLT_MinBias"
        ),
    #dataPileupFile = cms.string("data/PileupHistogram_test.root"),
    #mcPileupFile   = cms.string("data/PileupHistogram_test.root"),
    #dataPileupHistoName = cms.string("pileup"),
    #mcPileupHistoName = cms.string("mcpileup"),
    mcLumiScale = cms.double(0.37935), # 2012 (1368b): 0.222, 2015 (2232b): 0.3136, 2016 (2700b) pred: 0.37935
    instlumiTextFile = cms.untracked.string("run_ls_instlumi_pileup_2015.txt"),
    )
process.TimingStudy_step = cms.Path(process.TimingStudy)


#process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.TimingStudy_step)
process.schedule = cms.Schedule(process.raw2digi_Tracker_only_step,process.reconstruction_Tracker_only_step,process.TimingStudy_step)
