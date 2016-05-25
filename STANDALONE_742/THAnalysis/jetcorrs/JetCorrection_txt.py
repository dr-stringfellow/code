import FWCore.ParameterSet.Config as cms

# USE WITH CMSRUN

# used tags should correspond to the right CMSSW version, for example 5_3,
# but apart from that you should use the most current ones

# GR_R_42_V14, START41_V0
#tag = 'START53_V14'
#tag = 'GR_R_52_V9'
tag = 'FT_53_V10_AN3'
#tag = 'GR_P_V42_AN2'
#tag = 'START53_V15'

tagall = tag + '::All'

process = cms.Process("myprocess")

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = tagall
process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(1) )

process.source = cms.Source("EmptySource")

process.demo = cms.EDAnalyzer('JetCorrectorDBReader', 
        payloadName    = cms.untracked.string('AK5PFchs'),
        globalTag      = cms.untracked.string(tag),
        printScreen    = cms.untracked.bool(False),
        createTextFile = cms.untracked.bool(True)
)

process.p = cms.Path(process.demo)

