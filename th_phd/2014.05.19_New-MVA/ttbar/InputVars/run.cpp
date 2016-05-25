/**
 * Program performs the baseline event selection and saves possible input variables for
 * reconstruction of a semileptonic ttbar event.
 */

#include <Dataset.hpp>
#include <BTagger.hpp>
#include <BTagEfficiencies.hpp>
#include <BTagScaleFactors.hpp>
#include <TriggerSelection.hpp>
#include <GenericEventSelection.hpp>
#include <WeightBTag.hpp>
#include <WeightPileUp.hpp>
#include <RunManager.hpp>

#include <JetPtFilterPlugin.hpp>
#include <MetFilterPlugin.hpp>

#include <TopPtWeightPlugin.hpp>
#include <RecoTTbarInputVarsPlugin.hpp>

#include <iostream>
#include <memory>


using namespace std;


// Registration channel
enum class RegChannel
{
    Electron,
    Muon
};


int main(int argc, char const **argv)
{
    // Check the arguments
    if (argc not_eq 2)
    {
        cout << "Usage: [executable] registrationChannel\n";
        return 0;
    }
    
    string const regChannelArg(argv[1]);
    RegChannel regChannel;
    
    if (regChannelArg.find("mu") not_eq string::npos)
        regChannel = RegChannel::Muon;
    else if (regChannelArg.find("e") not_eq string::npos)
        regChannel = RegChannel::Electron;
    else
    {
        cout << "Cannot recognise lepton channel \"" << regChannelArg << "\"\n";
        return 1;
    }
    
    
    // Define the b-tagging object
    shared_ptr<BTagger const> bTagger(
     new BTagger(BTagger::Algorithm::CSV, BTagger::WorkingPoint::Tight));
    
    
    // Define the event selection
    GenericEventSelection sel(20., bTagger);
    
    if (regChannel == RegChannel::Muon)
        sel.AddLeptonThreshold(Lepton::Flavour::Muon, 26.);
    else
        sel.AddLeptonThreshold(Lepton::Flavour::Electron, 30.);
    
    for (unsigned nJets = 4; nJets < 50; ++nJets)
        for (unsigned nTags = 3; nTags < nJets and nTags <= 4; ++nTags)
        //^ There should be at least one untagged jet, that is why nTags < nJets
            sel.AddJetTagBin(nJets, nTags);
    
    
    // Define datasets (consult [1] for a complete list)
    //[1] https://twiki.cern.ch/twiki/bin/view/CMS/PlainEventContentTuples
    list<Dataset> datasets;
    string const filePrefix("/afs/cern.ch/user/a/aapopov/workspace/data/2012Bravo/");
    
    // ttbar
    datasets.emplace_back(Dataset({Dataset::Process::ttbar, Dataset::Process::ttSemilep}));
    datasets.back().AddFile(filePrefix + "ttbar-semilep-mg-p1_53X.02.01_VNz_p1.root", 107.7,
     24953451 + 31004465 + 30856876);
    datasets.back().AddFile(filePrefix + "ttbar-semilep-mg-p1_53X.02.01_VNz_p2.root", 107.7,
     24953451 + 31004465 + 30856876);
    datasets.back().AddFile(filePrefix + "ttbar-semilep-mg-p1_53X.02.01_VNz_p3.root", 107.7,
     24953451 + 31004465 + 30856876);
    datasets.back().AddFile(filePrefix + "ttbar-semilep-mg-p1_53X.02.01_VNz_p4.root", 107.7,
     24953451 + 31004465 + 30856876);
    datasets.back().AddFile(filePrefix + "ttbar-semilep-mg-p2_53X.02.01_Ptw_p1.root", 107.7,
     24953451 + 31004465 + 30856876);
    datasets.back().AddFile(filePrefix + "ttbar-semilep-mg-p2_53X.02.01_Ptw_p2.root", 107.7,
     24953451 + 31004465 + 30856876);
    datasets.back().AddFile(filePrefix + "ttbar-semilep-mg-p2_53X.02.01_Ptw_p3.root", 107.7,
     24953451 + 31004465 + 30856876);
    datasets.back().AddFile(filePrefix + "ttbar-semilep-mg-p2_53X.02.01_Ptw_p4.root", 107.7,
     24953451 + 31004465 + 30856876);
    datasets.back().AddFile(filePrefix + "ttbar-semilep-mg-p2_53X.02.01_Ptw_p5.root", 107.7,
     24953451 + 31004465 + 30856876);
    datasets.back().AddFile(filePrefix + "ttbar-semilep-mg-p3_53X.02.01_MXn_p1.root", 107.7,
     24953451 + 31004465 + 30856876);
    datasets.back().AddFile(filePrefix + "ttbar-semilep-mg-p3_53X.02.01_MXn_p2.root", 107.7,
     24953451 + 31004465 + 30856876);
    datasets.back().AddFile(filePrefix + "ttbar-semilep-mg-p3_53X.02.01_MXn_p3.root", 107.7,
     24953451 + 31004465 + 30856876);
    datasets.back().AddFile(filePrefix + "ttbar-semilep-mg-p3_53X.02.01_MXn_p4.root", 107.7,
     24953451 + 31004465 + 30856876);
    datasets.back().AddFile(filePrefix + "ttbar-semilep-mg-p3_53X.02.01_MXn_p5.root ", 107.7,
     24953451 + 31004465 + 30856876);
    
    
    // Define triggers
    list<TriggerRange> triggerRanges;
    
    if (regChannel == RegChannel::Muon)
        triggerRanges.emplace_back(0, -1, "IsoMu24_eta2p1", 19705., "IsoMu24_eta2p1");
    else
        triggerRanges.emplace_back(0, -1, "Ele27_WP80", 19690., "Ele27_WP80");
    
    TriggerSelection triggerSel(triggerRanges);
    
    
    // Define reweighting for b-tagging
    BTagEfficiencies bTagEff("BTagEff_2012Bravo_v1.0.root", "in4_jPt30/");
    
    // Set a mapping from process codes to names of histograms with b-tagging efficiencies
    bTagEff.SetProcessLabel(Dataset::Process::tHq, "tHq-nc");
    bTagEff.SetProcessLabel(Dataset::Process::ttSemilep, "ttbar-semilep");
    bTagEff.SetProcessLabel(Dataset::Process::ttDilep, "ttbar-dilep");
    bTagEff.SetProcessLabel(Dataset::Process::ttchan, "t-tchan");
    bTagEff.SetProcessLabel(Dataset::Process::ttWchan, "t-tWchan");
    bTagEff.SetProcessLabel(Dataset::Process::tschan, "t-schan");
    bTagEff.SetProcessLabel(Dataset::Process::ttH, "ttH");
    bTagEff.SetDefaultProcessLabel("ttbar-inc");
    
    BTagScaleFactors bTagSF(bTagger->GetAlgorithm());
    WeightBTag bTagReweighter(bTagger, bTagEff, bTagSF);
    
    
    // An object to reweight for pile-up
    WeightPileUp puReweighter("SingleMu2012ABCD_Bravo_v1.0_pixelLumi.pileupTruth_finebin.root",
     0.06);
    
    
    // Construct the run manager
    RunManager manager(datasets.begin(), datasets.end());
    
    // Set the configuration for PECReader
    auto &config = manager.GetPECReaderConfig();
    config.SetModule(&triggerSel);
    config.SetModule(&sel);
    config.SetModule(bTagger);
    config.SetModule(&bTagReweighter);
    config.SetModule(&puReweighter);
    config.SetReadHardInteraction(true);
    
    
    // Define the output directory
    string outDirectory("tuples/");
    
    if (regChannel == RegChannel::Muon)
        outDirectory += "mu/";
    else
        outDirectory += "e/";
    
    
    // Register plugins
    manager.RegisterPlugin(new JetPtFilterPlugin(4, 30.));
    
    if (regChannel == RegChannel::Muon)
        manager.RegisterPlugin(new MetFilterPlugin(35.));
    else
        manager.RegisterPlugin(new MetFilterPlugin(45.));
    
    manager.RegisterPlugin(new TopPtWeightPlugin);
    manager.RegisterPlugin(new RecoTTbarInputVarsPlugin(outDirectory));
    
    
    // Process the datasets
    manager.Process(10);
    
    
    return 0;
}