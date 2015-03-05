# Auto generated configuration file
# using: 
# Revision: 1.381.2.28 
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: step1 --conditions GR_R_53_V21A::All -s RAW2DIGI,RECO --scenario cosmics --process RawToDigiToReco --data --dasquery=file dataset=/Cosmics/Commissioning12-26Mar2013-v1/RECO run=186160 --eventcontent RECO --fileout file:Cosmics.root --no_exec --python_filename=Cosmics.py
import FWCore.ParameterSet.Config as cms

process = cms.Process('RawToDigiToReco')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContentCosmics_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.ReconstructionCosmics_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring(
        # 2012 CRUZET RAW (186160, Peak mode)
        '/store/data/Commissioning12/Cosmics/RAW/v1/000/186/160/0224F8A5-6A61-E111-BCC5-BCAEC5329708.root',
        '/store/data/Commissioning12/Cosmics/RAW/v1/000/186/160/0624CCF2-7C61-E111-96CB-BCAEC532970F.root',
        '/store/data/Commissioning12/Cosmics/RAW/v1/000/186/160/0A3084B6-7861-E111-8FFA-BCAEC518FF7A.root',
        '/store/data/Commissioning12/Cosmics/RAW/v1/000/186/160/2432DB77-8061-E111-9057-5404A63886AF.root',
        '/store/data/Commissioning12/Cosmics/RAW/v1/000/186/160/2A9654CA-8661-E111-AECF-BCAEC5364C62.root',
        # 2012 CRUZET RECO (186160, Peak mode)
        #'/store/data/Commissioning12/Cosmics/RECO/26Mar2013-v1/10000/0015D3DE-5496-E211-9EA8-002618943947.root',
        #'/store/data/Commissioning12/Cosmics/RECO/26Mar2013-v1/10000/022FC48F-5A96-E211-A62C-002618FDA26D.root',
        #'/store/data/Commissioning12/Cosmics/RECO/26Mar2013-v1/10000/02ECF7FA-5896-E211-BC13-002618943972.root',
        #'/store/data/Commissioning12/Cosmics/RECO/26Mar2013-v1/10000/0462EB8D-5A96-E211-A41D-002618943985.root',
        #'/store/data/Commissioning12/Cosmics/RECO/26Mar2013-v1/10000/049F25FF-5896-E211-B5AC-003048678FE4.root',
        # 2012 CRAFT during Run2012D
        #'/store/data/Run2012D/Cosmics/RECO/22Jan2013-v1/10000/0001A139-608A-E211-82D0-0025B3E05CAA.root',
        #'/store/data/Run2012D/Cosmics/RECO/22Jan2013-v1/10000/00023A61-2F8B-E211-A435-003048D46136.root',
        #'/store/data/Run2012D/Cosmics/RECO/22Jan2013-v1/10000/002B21D7-268B-E211-84B3-0025B3E06396.root',
        #'/store/data/Run2012D/Cosmics/RECO/22Jan2013-v1/10000/005BA0CD-268B-E211-BAFE-002590200AB8.root',
        #'/store/data/Run2012D/Cosmics/RECO/22Jan2013-v1/10000/0078F9A8-258B-E211-81F7-001E67396BA3.root',
    )
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.381.2.28 $'),
    annotation = cms.untracked.string('step1 nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

process.RECOoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RECOEventContent.outputCommands,
    fileName = cms.untracked.string('file:Cosmics.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    )
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_R_53_V21A::All', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstructionCosmics)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)

# Schedule definition
## process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step)






#--------------- Added for TimingStudy ---------------



process.MessageLogger.cerr.FwkReport.reportEvery = 10

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
	"HLT_BeamHalo",
        "HLT_L1SingleMuOpen_AntiBPTX",
        "HLT_L1TrackerCosmics"),
)

process.TimingStudy_step = cms.Path(process.Refitter*process.TimingStudy)

# Schedule definition
process.schedule = cms.Schedule(
    process.raw2digi_step,
    process.reconstruction_step,
    process.TimingStudy_step
    )
