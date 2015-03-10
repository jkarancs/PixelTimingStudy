# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step1 --conditions GR_P_V49 -s RECO --scenario cosmics --process RECO --data --eventcontent RECO -n 100 --customise Configuration/DataProcessing/RecoTLR.customiseCosmicDataRun2 --dasquery=file dataset=/ExpressCosmics/Commissioning2015-Express-v1/FEVT run=234029 --fileout file:ExpressCosmics.root --no_exec --python_filename=ExpressCosmics.py
import FWCore.ParameterSet.Config as cms

process = cms.Process('TimingStudy')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContentCosmics_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.ReconstructionCosmics_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2000)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/00097CE4-A2B2-E411-8FEB-02163E011D8A.root', 
        '/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/0049B70A-8EB2-E411-8979-02163E011D1C.root', 
        '/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/004DEB4A-9DB2-E411-AB38-02163E011D06.root', 
        '/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/00D04605-A5B2-E411-A496-02163E0129F4.root', 
        '/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/00D21ADC-A2B2-E411-8861-02163E011D57.root', 
        '/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/02035AA7-A6B2-E411-B294-02163E011885.root', 
        '/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/021D699B-9DB2-E411-8154-02163E011DB4.root', 
        '/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/027D5566-A4B2-E411-B4F1-02163E0121AD.root', 
        '/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/029BFD10-8EB2-E411-8415-02163E012593.root', 
        '/store/express/Commissioning2015/ExpressCosmics/FEVT/Express-v1/000/234/029/00000/02E76BF1-A2B2-E411-B2E3-02163E0129F4.root')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.19 $'),
    annotation = cms.untracked.string('step1 nevts:100'),
    name = cms.untracked.string('Applications')
)

# Output definition

process.RECOoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RECOEventContent.outputCommands,
    fileName = cms.untracked.string('file:ExpressCosmics.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    )
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_P_V49', '')

# Path and EndPath definitions
process.reconstruction_step = cms.Path(process.reconstructionCosmics)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)

# customisation of the process.

# Automatic addition of the customisation function from Configuration.DataProcessing.RecoTLR
from Configuration.DataProcessing.RecoTLR import customiseCosmicDataRun2 

#call to customisation function customiseCosmicDataRun2 imported from Configuration.DataProcessing.RecoTLR
process = customiseCosmicDataRun2(process)

# End of customisation functions



process.MessageLogger.cerr.FwkReport.reportEvery = 1000

#-------------------------
#  Track Refitter
#-------------------------

process.load("RecoTracker.TrackProducer.TrackRefitters_cff")

process.Refitter = process.TrackRefitterP5.clone()
#process.Refitter.src = 'cosmictrainderP5'
process.Refitter.src = 'ctfWithMaterialTracksP5'
process.Refitter.TrajectoryInEvent = True

# TimingStudy
process.TimingStudy = cms.EDAnalyzer("TimingStudy",
    trajectoryInput = cms.string('Refitter'),
    fileName = cms.string("Ntuple.root"),
    extrapolateFrom = cms.int32(2),
    extrapolateTo = cms.int32(1),
    keepOriginalMissingHit = cms.bool(False),
    usePixelCPE= cms.bool(True),
    minNStripHits = cms.int32(0),
    triggerNames=cms.vstring(
        "HLT_L1SingleMuOpen_DT",
        "HLT_L1SingleMuOpen",
        "HLT_L1Tech_HBHEHO_totalOR",
        "HLT_L1TrackerCosmics",
        "HLT_Random"),
)

process.TimingStudy_step = cms.Path(process.MeasurementTrackerEvent*process.Refitter*process.TimingStudy)

# Schedule definition
process.schedule = cms.Schedule(
    process.TimingStudy_step
    )
