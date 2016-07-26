import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process("SplitClusterMergerPluginTest", eras.Run2_2017)

# Message logger service
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger = cms.Service("MessageLogger",
	destinations = cms.untracked.vstring('cerr'),
	cerr = cms.untracked.PSet(threshold  = cms.untracked.string('DEBUG')),
	debugModules = cms.untracked.vstring('SplitClusterMergerPlugin'))

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))
process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring('file:/data/hunyadi/CMSSW/SplitClusterMerger/CMSSW_8_0_15/src/Data_for_testing/GENSIMRECO.root'),
	secondaryFileNames = cms.untracked.vstring()
)

process.SplitClusterMergerPlugin = cms.EDAnalyzer('SplitClusterMerger')
process.SplitClusterMerger_step = cms.Path(process.SplitClusterMergerPlugin)
