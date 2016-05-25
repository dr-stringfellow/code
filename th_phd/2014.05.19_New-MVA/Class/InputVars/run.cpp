/**
 * Program performs the baseline event selection and saves possible input variables to distriminate
 * a tHq event from backgrounds.
 */

#include <TopPtWeightPlugin.hpp>
#include <GlobalRecoTHPlugin.hpp>
#include <GlobalRecoTTbarPlugin.hpp>
#include <ClassInputVarsPlugin.hpp>

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
#include <StdBTaggerPlugin.hpp>

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
    
    // t-channel single top plus higgs
    datasets.emplace_back(Dataset::Process::tHq);
    datasets.back().AddFile(filePrefix + "thq-nc-mg_53X.02.01_Wyg.root", 36.4e-3, 4847334);
    
    // ttH
    datasets.emplace_back(Dataset::Process::ttH);
    datasets.back().AddFile(filePrefix + "tth_53X.02.01_bVJ.root", 130.2e-3, 995697);
    
    // // Single top
    // datasets.emplace_back(Dataset({Dataset::Process::SingleTop, Dataset::Process::ttchan}));
    // datasets.back().AddFile(filePrefix + "t-tchan-pw_53X.02.01_PIN.root", 18.27, 3915598);
    // datasets.back().AddFile(filePrefix + "tbar-tchan-pw_53X.02.01_VcT.root", 9.95, 1711403);
    // datasets.emplace_back(Dataset({Dataset::Process::SingleTop, Dataset::Process::ttWchan}));
    // datasets.back().AddFile(filePrefix + "t-tWchan-pw_53X.02.01_LxG.root", 11.1, 497658);
    // datasets.back().AddFile(filePrefix + "tbar-tWchan-pw_53X.02.01_tge.root", 11.1, 493460);
    
    // Semileptonic ttbar
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
    
    // Dileptonic ttbar
    datasets.emplace_back(Dataset({Dataset::Process::ttbar, Dataset::Process::ttDilep}));
    datasets.back().AddFile(filePrefix + "ttbar-dilep-mg_53X.02.01_FFe_p1.root", 25.8, 11991428);
    datasets.back().AddFile(filePrefix + "ttbar-dilep-mg_53X.02.01_FFe_p2.root", 25.8, 11991428);
    
    // // W+jets
    // datasets.emplace_back(Dataset({Dataset::Process::EWK, Dataset::Process::Wjets}));
    // datasets.back().AddFile(filePrefix + "Wjets-2p-mg_53X.02.01_Dhm.root", 2116, 34044921);
    // datasets.back().AddFile(filePrefix + "Wjets-3p-mg_53X.02.01_YDn.root", 637, 15539503);
    // datasets.back().AddFile(filePrefix + "Wjets-4p-mg_53X.02.01_FxU.root", 262, 13382803);
    // //^ There is no need to consider W+0p, W+1p as they have no chance to survive the event
    // //selection
    
    
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
    
    manager.RegisterPlugin(new StdBTaggerPlugin("BTagger", bTagger, bTagger->GetWorkingPoint()));
    
    manager.RegisterPlugin(new TopPtWeightPlugin);
    manager.RegisterPlugin(new GlobalRecoTHPlugin("RecoTH", "BTagger"));
    manager.RegisterPlugin(new GlobalRecoTTbarPlugin("RecoTTbar", "BTagger"));
    
    manager.RegisterPlugin(new ClassInputVarsPlugin(outDirectory));
    
    
    // Process the datasets
    manager.Process(10);
    
    
    return 0;
}