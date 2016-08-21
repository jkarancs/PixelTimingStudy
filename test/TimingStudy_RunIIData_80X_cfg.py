# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: RECO -s RAW2DIGI,RECO --data --scenario pp --conditions auto:run2_data --era Run2_25ns --process NTUPLE --eventcontent RECO --datatier RECO --customise Configuration/DataProcessing/RecoTLR.customiseDataRun2Common_25ns --filein /store/data/Run2016D/ZeroBias/AOD/PromptReco-v2/000/276/317/00000/12A3F60B-1145-E611-83B1-02163E01431C.root --secondfilein /store/data/Run2016D/ZeroBias/RAW/v2/000/276/317/00000/46CDE349-0842-E611-A1F4-02163E012067.root --python_filename=test/TimingStudy_RunIIData_80X_AODplusRAW_cfg.py --runUnscheduled -n 100 --no_exec
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('NTUPLE',eras.Run2_25ns)

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
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/data/Run2016D/ZeroBias/AOD/PromptReco-v2/000/276/317/00000/12A3F60B-1145-E611-83B1-02163E01431C.root'),
    secondaryFileNames = cms.untracked.vstring('/store/data/Run2016D/ZeroBias/RAW/v2/000/276/317/00000/46CDE349-0842-E611-A1F4-02163E012067.root')
)

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('RECO nevts:100'),
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
    fileName = cms.untracked.string('RECO_RAW2DIGI_RECO.root'),
    outputCommands = process.RECOEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step)

# customisation of the process.

# Automatic addition of the customisation function from Configuration.DataProcessing.RecoTLR
from Configuration.DataProcessing.RecoTLR import customiseDataRun2Common_25ns 

#call to customisation function customiseDataRun2Common_25ns imported from Configuration.DataProcessing.RecoTLR
process = customiseDataRun2Common_25ns(process)



# <--

#--------------- Added for TimingStudy ---------------

#---------------------------
#  Options
#---------------------------
import FWCore.ParameterSet.VarParsing as opts

options = opts.VarParsing ('analysis')

options.register('globalTag',        '',
                 opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
                 'Global Tag, Default="" which uses auto:run2_data')

options.register('runOnRAW',         False,
                 opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
                 'True: RAW, False: RECO/Express')

options.register('filterZeroBias',   False,
                 opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
                 'Filter only events with trigger HLT_ZeroBias_v*')

options.register('useClosestVtx',    False,
                 opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.bool,
                 'True: use best Vtx for d0/dz, True: use closest, Default=False')

options.register('nthClusterToSave', 100,
                 opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.int,
                 'Only save each Nth of inclusive clusters, Default=100 (shown to offer enough stats (17% of on-trk clu with pt>1.0))')

options.register('minNStripHits',    0,
                 opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.int,
                 'Save only hits with a track that has at least N stip detector hits (Default: 0, old was 11 for efficiency)')

options.register('minTrkPt',         0.6,
                 opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.float,
                 'Default=0.6, chosen to be below eff cut and common cluster cutsGlobal Tag')

options.register('dataTier',         'RECO',
                 opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
                 'Name of the AOD input root file')

options.register('inputFileName',   '',
                 opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
                 'Name of the input root file')

options.register('secondaryInputFileName',   '',
                 opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
                 'Name of the RAW (parent) input root file')

options.register('outputFileName',   'Ntuple.root',
                 opts.VarParsing.multiplicity.singleton, opts.VarParsing.varType.string,
                 'Name of the output root file')

### Events to process: 'maxEvents' is already registered by the framework
options.setDefault('maxEvents', 100)

options.parseArguments()

print "TimingStudy Options: "
print "  globalTag               = "+str(options.globalTag)
print "  runOnRAW                = "+str(options.runOnRAW)
print "  useClosestVtx           = "+str(options.useClosestVtx)
print "  nthClusterToSave        = "+str(options.nthClusterToSave)
print "  minNStripHits           = "+str(options.minNStripHits)
print "  minTrkPt                = "+str(options.minTrkPt)
print "  dataTier                = "+str(options.dataTier)
print "  inputFileName           = "+str(options.inputFileName)
print "  secondaryInputFileName  = "+str(options.secondaryInputFileName)
print "  outputFileName          = "+str(options.outputFileName)

if options.globalTag == '':
    print "GlobalTag (auto:run2_data): "+str(process.GlobalTag.globaltag)
else:
    process.GlobalTag.globaltag = options.globalTag
    print "GlobalTag: "+str(process.GlobalTag.globaltag)

# Input file
if options.inputFileName == '':
    if options.dataTier == 'RAW':
        process.source = cms.Source("PoolSource",
                                    fileNames = cms.untracked.vstring(
                                        'file:/data/store/data/Run2016B/ZeroBias/RAW/v2/000/273/158/00000/C62669DA-7418-E611-A8FB-02163E01377A.root' #273158 RAW
                                        )
                                    )
    elif options.dataTier == 'AOD':
        process.source = cms.Source("PoolSource",
                                    fileNames = cms.untracked.vstring('/store/data/Run2016D/ZeroBias/AOD/PromptReco-v2/000/276/317/00000/12A3F60B-1145-E611-83B1-02163E01431C.root'),
                                    secondaryFileNames = cms.untracked.vstring('/store/data/Run2016D/ZeroBias/RAW/v2/000/276/317/00000/46CDE349-0842-E611-A1F4-02163E012067.root')
                                    )
    else:
        process.source = cms.Source("PoolSource",
                                    fileNames = cms.untracked.vstring(
                                        'file:/data/store/data/Run2016B/ZeroBias/RECO/PromptReco-v2/000/273/158/00000/0C460BA1-EB19-E611-A6ED-02163E0120AE.root' #273158 RECO (same LS)
                                        #'/store/express/Run2016B/ExpressPhysics/FEVT/Express-v2/000/275/309/00000/008E9B65-B334-E611-BBA8-02163E013641.root' #275309 Express
                                        )
                                    )
else:
    if options.secondaryInputFileName == '':
        process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring(options.inputFileName))
    else:
        process.source = cms.Source("PoolSource",
                                    fileNames = cms.untracked.vstring(options.inputFileName),
                                    secondaryFileNames = cms.untracked.vstring(options.secondaryInputFileName)
                                    )

# Number of events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

# MessageLogger
process.MessageLogger.cerr.FwkReport.reportEvery = (1 if options.dataTier == 'RAW' else 100)

#---------------------------
#  HLT Filter
#---------------------------
process.zerobiasTriggerFilter = cms.EDFilter( "TriggerResultsFilter",
    triggerConditions = cms.vstring( 'HLT_ZeroBias_v* / 1' ),
    hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),
    l1tResults = cms.InputTag( "" ),
    l1tIgnoreMask = cms.bool( False ),
    l1techIgnorePrescales = cms.bool( True ),
    daqPartitions = cms.uint32( 1 ),
    throw = cms.bool( True )
)

#---------------------------
#  Track Refitter
#---------------------------
if options.dataTier == 'RECO' or options.dataTier == 'FEVT':
    process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
    trajInput = 'TrackRefitter'
else:
    trajInput = 'generalTracks'

#---------------------------
#  TimingStudy
#---------------------------
process.TimingStudy = cms.EDAnalyzer("TimingStudy",
    trajectoryInput = cms.string(trajInput),
    fileName = cms.string(options.outputFileName),
    extrapolateFrom = cms.int32(2),
    extrapolateTo = cms.int32(1),
    keepOriginalMissingHit = cms.bool(False),
    usePixelCPE= cms.bool(True),
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
        "HLT_MinBias"),
    #dataPileupFile = cms.string("PileupHistogram_201278_flatpileupMC.root"),
    #mcPileupFile   = cms.string("PileupHistogram_201278_flatpileupMC.root"),
    #dataPileupHistoName = cms.string("pileup"),
    #mcPileupHistoName = cms.string("mcpileup"),
    mcLumiScale = cms.double(0.384408), # 2012 (1368b): 0.222, 2015 (2232b): 0.313596, 2016 (2736b) pred: 0.384408
    instlumiTextFile = cms.untracked.string("run_ls_instlumi_pileup_2015.txt"),
    nthClusterToSave = cms.int32(options.nthClusterToSave),
    minNStripHits    = cms.int32(options.minNStripHits),
    minTrkPt         = cms.double(options.minTrkPt),
    useClosestVtx    = cms.bool(options.useClosestVtx),
)

#---------------------------
#  Path/Schedule
#---------------------------
process.schedule.remove(process.RECOoutput_step)
process.schedule.remove(process.endjob_step)

if options.dataTier == 'RAW' or options.dataTier == 'AOD':
    if options.filterZeroBias:
        process.TimingStudy_step = cms.Path(process.zerobiasTriggerFilter*process.TimingStudy)
    else:
        process.TimingStudy_step = cms.Path(process.TimingStudy)
else:
    if options.filterZeroBias:
        process.TimingStudy_step = cms.Path(process.zerobiasTriggerFilter*process.MeasurementTrackerEvent*process.TrackRefitter*process.TimingStudy)
    else:
        process.TimingStudy_step = cms.Path(process.MeasurementTrackerEvent*process.TrackRefitter*process.TimingStudy)
    process.schedule.remove(process.reconstruction_step)
    process.schedule.remove(process.raw2digi_step)

process.schedule.append(process.TimingStudy_step)

# disbales an error message
if options.dataTier == 'AOD': process.ecalRecHit.laserCorrection = False

# --> Rest from the original script


# End of customisation functions
#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)
from FWCore.ParameterSet.Utilities import cleanUnscheduled
process=cleanUnscheduled(process)
