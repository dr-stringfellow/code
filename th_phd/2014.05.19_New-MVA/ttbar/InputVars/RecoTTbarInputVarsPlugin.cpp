#include <RecoTTbarInputVarsPlugin.hpp>

#include <Processor.hpp>
#include <ROOTLock.hpp>
#include <Logger.hpp>

#include <boost/filesystem.hpp>

#include <limits>
#include <array>
#include <algorithm>


using namespace std;


RecoTTbarInputVarsPlugin::RecoTTbarInputVarsPlugin(string const &outDirectory_):
    Plugin("RecoTTbarInputVars"),
    outDirectory(outDirectory_),
    sumWeightTotal(0.), sumWeightMatched(0.),
    rGen(0)
{
    // Make sure the directory path ends with a slash
    if (outDirectory.back() != '/')
        outDirectory += '/';
    
    // Create the output directory if it does not exist
    boost::filesystem::create_directories(outDirectory);
}


RecoTTbarInputVarsPlugin::~RecoTTbarInputVarsPlugin()
{
    logging::logger << "Overall matching efficiency measured by plugin \"" << name <<
     "\" is " << sumWeightMatched / sumWeightTotal << logging::eom;
}


Plugin *RecoTTbarInputVarsPlugin::Clone() const
{
    return new RecoTTbarInputVarsPlugin(outDirectory);
}


void RecoTTbarInputVarsPlugin::BeginRun(Dataset const &dataset)
{
    // Save pointer to the reader plugin
    reader = dynamic_cast<PECReaderPlugin const *>(processor->GetPluginBefore("Reader", name));
    
    // Save pointer to top-pt reweighter
    topPtReweighter = dynamic_cast<TopPtWeightPlugin const *>(
     processor->GetPluginBefore("TopPtWeight", name));
    
    
    // Creation of ROOT objects is not thread-safe and must be protected
    ROOTLock::Lock();
    
    // Create the output file
    string fileName(dataset.GetFiles().front().GetBaseName());
    
    file.reset(new TFile((outDirectory + fileName + ".root").c_str(), "recreate"));
    
    // Create the tree
    tree.reset(new TTree("Vars", "Observables for reconstruction of a ttbar event"));
    
    // End of critical block
    ROOTLock::Unlock();
    
    
    // Assign branch addresses
    tree->Branch("EventNumber", &bfEventNumber);
    tree->Branch("RunNumber", &bfRunNumber);
    tree->Branch("LumiSection", &bfLumiSection);
    
    tree->Branch("IsSignal", &bfIsSignal);
    tree->Branch("Weight", &bfWeight);
    
    tree->Branch("Mass_TopLep", &bfMass_TopLep);
    tree->Branch("Pt_TopLep", &bfPt_TopLep);
    tree->Branch("Eta_TopLep", &bfEta_TopLep);
    tree->Branch("Pt_BTopLep", &bfPt_BTopLep);
    tree->Branch("Eta_BTopLep", &bfEta_BTopLep);
    tree->Branch("Charge_BTopLep", &bfCharge_BTopLep);
    tree->Branch("Mass_BTopLepLep", &bfMass_BTopLepLep);
    tree->Branch("DeltaR_BTopLepWLep", &bfDeltaR_BTopLepWLep);
    tree->Branch("DEta_TopLepLep", &bfDEta_TopLepLep);
    tree->Branch("Cos_LepBTopLep_WLep", &bfCos_LepBTopLep_WLep);
    
    tree->Branch("Mass_TopHad", &bfMass_TopHad);
    tree->Branch("Pt_TopHad", &bfPt_TopHad);
    tree->Branch("Eta_TopHad", &bfEta_TopHad);
    tree->Branch("Mass_WHad", &bfMass_WHad);
    tree->Branch("Pt_WHad", &bfPt_WHad);
    tree->Branch("Eta_WHad", &bfEta_WHad);
    tree->Branch("DeltaR_BTopHadWHad", &bfDeltaR_BTopHadWHad);
    tree->Branch("Pt_BTopHad", &bfPt_BTopHad);
    tree->Branch("Eta_BTopHad", &bfEta_BTopHad);
    tree->Branch("Charge_BTopHad", &bfCharge_BTopHad);
    tree->Branch("MinPt_Light", &bfMinPt_Light);
    tree->Branch("MaxEta_Light", &bfMaxEta_Light);
    tree->Branch("NumBTag_Light", &bfNumBTag_Light);
    tree->Branch("SumCharge_Light", &bfSumCharge_Light);
    tree->Branch("DeltaR_Light", &bfDeltaR_Light);
    tree->Branch("MaxMass_BTopHadLight", &bfMaxMass_BTopHadLight);
    
    tree->Branch("RelHt", &bfRelHt);
    tree->Branch("Mass_TopLepTopHad", &bfMass_TopLepTopHad);
    tree->Branch("Pt_TopLepTopHad", &bfPt_TopLepTopHad);
    tree->Branch("Eta_TopLepTopHad", &bfEta_TopLepTopHad);
    tree->Branch("DeltaR_TopLepTopHad", &bfDeltaR_TopLepTopHad);
}


void RecoTTbarInputVarsPlugin::EndRun()
{
    // Operations with ROOT objects performed here are not thread-safe and must be guarded
    ROOTLock::Lock();
    
    // Write the tree and close the file
    file->cd();
    tree->Write("", TObject::kOverwrite);
    
    // Delete the objects
    tree.reset();
    file.reset();
    
    ROOTLock::Unlock();
}


bool RecoTTbarInputVarsPlugin::ProcessEvent()
{
    auto const &jets = (*reader)->GetJets();
    
    // Find the b quark from semileptonic decay of a top quark
    GenParticle const *bTopLepParton = nullptr;
    
    for (GenParticle const &p: (*reader)->GetHardGenParticles())
    {
        // A b quark
        if (abs(p.GetPdgId()) != 5)
            continue;
        
        
        // From decay of a top quark
        if (abs(p.GetFirstMotherPdgId()) != 6)
            continue;
        
        
        // The W boson from decay of the top quark must decay to leptons
        GenParticle const *w = p.GetFirstMother()->FindFirstDaughter({-24, 24});  // a W boson
        
        if (not w)
        //^ An off-shell W. Then leptons are immediate dauthers of the top quark
        {
            for (auto const &d: p.GetFirstMother()->GetDaughters())
            {
                int const daughterPdgId = abs(d->GetPdgId());
                
                if (daughterPdgId >= 11 and daughterPdgId <= 18)  // charged and neutral leptons
                {
                    bTopLepParton = &p;
                    break;
                }
            }
            
            if (bTopLepParton)
                break;
        }
        else
        {
            if (w->GetDaughters().size() == 0)
                continue;
            
            int const absLeptonPdgId = abs(w->GetDaughters().front()->GetPdgId());
            
            if (absLeptonPdgId >= 11 and absLeptonPdgId <= 18)  // charged and neutral leptons
            {
                bTopLepParton = &p;
                break;
            }
        }
    }
    
    
    // Make sure the b quark from decay of a top quark was found
    if (not bTopLepParton)
        throw logic_error("RecoTTbarInputVarsPlugin::ProcessEvent: Failed to find the b quark from "
         "semileptonic decay of a top quark.");
    
    
    // Find quarks from hadronic decay of a top quark
    GenParticle const *bTopHadParton(nullptr), *q1TopHadParton(nullptr), *q2TopHadParton(nullptr);
    
    for (GenParticle const &p: (*reader)->GetHardGenParticles())
    {
        // Find a top quark
        if (abs(p.GetPdgId()) != 6)
            continue;
        
        
        // The W boson from decay of the top quark must decay to hadrons
        GenParticle const *w = p.FindFirstDaughter({-24, 24});  // a W boson
        
        if (not w or w->GetDaughters().size() != 2)
            continue;
        
        if (abs(w->GetDaughters().front()->GetPdgId()) > 4)
            continue;
        
        
        // Thus, the current particle is a top quark that decays hadronically
        bTopHadParton = p.FindFirstDaughter({-5, 5});
        q1TopHadParton = w->GetDaughters().front();
        q2TopHadParton = w->GetDaughters().back();
    }
    
    
    // Make sure all quarks were found
    if (not bTopHadParton or not q1TopHadParton or not q2TopHadParton)
        throw logic_error("RecoTTbarInputVarsPlugin::ProcessEvent: Failed to find some quarks from "
         "hadronic decay of a top quark.");
    
    
    // Get event weight
    bfWeight = (*reader)->GetCentralWeight() * topPtReweighter->GetWeight();
    sumWeightTotal += bfWeight;
    
    
    // Match the b quark from semileptonic decay of the top to a reconstucted jet
    auto matchRes = MatchJet(*bTopLepParton, jets);
    unsigned bTopLepJetIndex = matchRes.first;
    
    if (matchRes.second > 0.3)  // the matched jet is not close enough to the parton
        return true;
    
    // Make sure the matched jet is central (rejects 0.1-0.2% of events)
    if (fabs(jets.at(bTopLepJetIndex).Eta()) > 2.4)
        return true;
    
    // Make sure the matched jet is b-tagged (rejects ~3% of events)
    if (jets.at(bTopLepJetIndex).CSV() < 0.898)
        return true;
    
    
    // Match jets from hadronic decay of a top quark
    matchRes = MatchJet(*bTopHadParton, jets);
    unsigned const bTopHadJetIndex = matchRes.first;
    
    if (matchRes.second > 0.3)  // the matched jet is not close enough to the parton
        return true;
    
    if (fabs(jets.at(bTopHadJetIndex).Eta()) > 2.4)  // the b-jet must be central
        return true;
    
    if (jets.at(bTopHadJetIndex).CSV() < 0.898)  // the b-jet from top must be b-tagged
        return true;
    
    
    matchRes = MatchJet(*q1TopHadParton, jets);
    unsigned q1TopHadJetIndex = matchRes.first;
    
    if (matchRes.second > 0.3)  // the matched jet is not close enough to the parton
        return true;
    
    
    matchRes = MatchJet(*q2TopHadParton, jets);
    unsigned q2TopHadJetIndex = matchRes.first;
    
    if (matchRes.second > 0.3)  // the matched jet is not close enough to the parton
        return true;
    
    
    // Make sure that no jet is matched to more than one parton
    if (bTopLepJetIndex == bTopHadJetIndex or bTopLepJetIndex == q1TopHadJetIndex or
     bTopLepJetIndex == q2TopHadJetIndex or bTopHadJetIndex == q1TopHadJetIndex or
     bTopHadJetIndex == q2TopHadJetIndex or q1TopHadJetIndex == q2TopHadJetIndex)
        return true;
    
    
    // Order light-flavour jets in pt
    if (jets.at(q1TopHadJetIndex).Pt() < jets.at(q2TopHadJetIndex).Pt())
        swap(q1TopHadJetIndex, q2TopHadJetIndex);
    
    
    // Increment sum of weights of matched events
    sumWeightMatched += bfWeight;
    
    
    // Update event ID
    auto const &eventID = (*reader)->GetEventID();
    bfRunNumber = eventID.Run();
    bfEventNumber = eventID.Event();
    bfLumiSection = eventID.LumiBlock();
    
    
    // Fill the output tree with variables calculated for the correct jet assignment
    bfIsSignal = true;
    CalculateVariables(jets.at(bTopLepJetIndex), jets.at(bTopHadJetIndex),
     jets.at(q1TopHadJetIndex), jets.at(q2TopHadJetIndex));
    
    tree->Fill();
    
    
    // Build a wrong match. The b-jets are chosen from the central ones
    unsigned bTopLepJetIndexFake, bTopHadJetIndexFake, q1TopHadJetIndexFake, q2TopHadJetIndexFake;
    vector<unsigned> bTaggedJetIndices;
    
    for (unsigned i = 0; i < jets.size(); ++i)
    {
        if (fabs(jets.at(i).Eta()) > 2.4 or jets.at(i).CSV() < 0.898)
            continue;
        
        bTaggedJetIndices.push_back(i);
    }
    
    
    if (bTaggedJetIndices.size() < 2)
        throw logic_error("RecoTTbarInputVarsPlugin::ProcessEvent: Unexpected number of b-tagged "
         "jets is found. There is something wrong with the logic of this plugin.");
    
    
    // Sample random combinations until one different from the correct match is found
    vector<unsigned> occupiedIndices;
    
    while (true)
    {
        // First, choose the b-jets
        unsigned ic = rGen.Integer(bTaggedJetIndices.size());
        bTopLepJetIndexFake = bTaggedJetIndices.at(ic);
        bTopHadJetIndexFake = bTaggedJetIndices.at((ic + 1 +
         rGen.Integer(bTaggedJetIndices.size() - 1)) % bTaggedJetIndices.size());
        
        
        // Then select the light-flavour jets out of the remaining ones
        occupiedIndices.clear();
        occupiedIndices.push_back(bTopLepJetIndexFake);
        occupiedIndices.push_back(bTopHadJetIndexFake);
        sort(occupiedIndices.begin(), occupiedIndices.end());
        
        q1TopHadJetIndexFake = rGen.Integer(jets.size() - 2);
        
        for (unsigned const &index: occupiedIndices)
            if (q1TopHadJetIndexFake >= index)
                ++q1TopHadJetIndexFake;
        
        occupiedIndices.push_back(q1TopHadJetIndexFake);
        sort(occupiedIndices.begin(), occupiedIndices.end());
        
        q2TopHadJetIndexFake = rGen.Integer(jets.size() - 3);
        
        for (unsigned const &index: occupiedIndices)
            if (q2TopHadJetIndexFake >= index)
                ++q2TopHadJetIndexFake;
        
        
        // Order the light-flavour jets
        if (jets.at(q1TopHadJetIndexFake).Pt() < jets.at(q2TopHadJetIndexFake).Pt())
            swap(q1TopHadJetIndexFake, q2TopHadJetIndexFake);
        
        
        // If the constructed match is different from the correct one, break the loop
        if (bTopLepJetIndexFake != bTopLepJetIndex or bTopHadJetIndexFake != bTopHadJetIndex or
         q1TopHadJetIndexFake != q1TopHadJetIndex or q2TopHadJetIndexFake != q2TopHadJetIndex)
            break;
    }
    
    
    // Fill the output tree with variables calculated for the chosen wrong jet assignment
    bfIsSignal = false;
    CalculateVariables(jets.at(bTopLepJetIndexFake), jets.at(bTopHadJetIndexFake),
     jets.at(q1TopHadJetIndexFake), jets.at(q2TopHadJetIndexFake));
    
    tree->Fill();
    
    
    return true;
}


pair<unsigned, double> RecoTTbarInputVarsPlugin::MatchJet(Candidate const &parton,
 vector<Jet> const &jets)
{
    double minDR = numeric_limits<double>::infinity();
    unsigned matchedJetIndex = -1;
    
    for (unsigned i = 0; i < jets.size(); ++i)
    {
        double const dR = jets.at(i).P4().DeltaR(parton.P4());
        
        if (dR < minDR)
        {
            matchedJetIndex = i;
            minDR = dR;
        }
    }
    
    return {matchedJetIndex, minDR};
}


void RecoTTbarInputVarsPlugin::CalculateVariables(Jet const &bTopLep, Jet const &bTopHad,
 Jet const &q1TopHad, Jet const &q2TopHad)
{
    // Calculate variables related to the top quark decaying semileptonically
    auto const &lepton = (*reader)->GetLeptons().front();
    Candidate const wLep(lepton.P4() + (*reader)->GetNeutrino().P4());
    Candidate const topLep(wLep.P4() + bTopLep.P4());
    
    bfMass_TopLep = topLep.M();
    bfPt_TopLep = topLep.Pt();
    bfEta_TopLep = topLep.Eta();
    
    bfPt_BTopLep = bTopLep.Pt();
    bfEta_BTopLep = bTopLep.Eta();
    bfCharge_BTopLep = bTopLep.Charge() * lepton.Charge();
    
    bfMass_BTopLepLep = (bTopLep.P4() + lepton.P4()).M();
    bfDeltaR_BTopLepWLep = bTopLep.P4().DeltaR(wLep.P4());
    bfDEta_TopLepLep = fabs(topLep.Eta() - lepton.Eta());
    
    // Calculate the cosine
    TVector3 b((wLep.P4()).BoostVector());
    
    TLorentzVector boostedLepton(lepton.P4());
    boostedLepton.Boost(-b);
    TVector3 p3Lepton(boostedLepton.Vect());
    
    TLorentzVector boostedBJet(bTopLep.P4());
    boostedBJet.Boost(-b);
    TVector3 const p3BJet(boostedBJet.Vect());
    
    bfCos_LepBTopLep_WLep = p3Lepton.Dot(p3BJet) / (p3Lepton.Mag() * p3BJet.Mag());
    
    
    // Calculate variables related to the top quark decaying semileptonically
    Candidate const wHad(q1TopHad.P4() + q2TopHad.P4());
    Candidate const topHad(wHad.P4() + bTopHad.P4());
    
    bfMass_TopHad = topHad.M();
    bfPt_TopHad = topHad.Pt();
    bfEta_TopHad = topHad.Eta();
    
    bfMass_WHad = wHad.M();
    bfPt_WHad = wHad.Pt();
    bfEta_WHad= wHad.Eta();
    
    bfDeltaR_BTopHadWHad = bTopHad.P4().DeltaR(wHad.P4());
    
    bfPt_BTopHad = bTopHad.Pt();
    bfEta_BTopHad = bTopHad.Eta();
    bfCharge_BTopHad = bTopHad.Charge() * lepton.Charge();
    
    bfMinPt_Light = min(fabs(q1TopHad.Pt()), fabs(q2TopHad.Pt()));
    bfMaxEta_Light = max(fabs(q1TopHad.Eta()), fabs(q2TopHad.Eta()));
    bfSumCharge_Light = (q1TopHad.Charge() + q2TopHad.Charge()) * lepton.Charge();
    bfNumBTag_Light = 0. + (q1TopHad.CSV() > 0.898) + (q2TopHad.CSV() > 0.898);
    
    bfDeltaR_Light = q1TopHad.P4().DeltaR(q2TopHad.P4());
    bfMaxMass_BTopHadLight =
     max((bTopHad.P4() + q1TopHad.P4()).M(), (bTopHad.P4() + q2TopHad.P4()).M());
    
    
    // Calcualate variables with correlations between different objects
    double Ht = lepton.Pt() + (*reader)->GetMET().Pt();
    
    for (auto const &j: (*reader)->GetJets())
        Ht += j.Pt();
    
    bfRelHt = (topLep.Pt() + topHad.Pt()) / Ht;
    bfMass_TopLepTopHad = (topLep.P4() + topHad.P4()).M();
    bfPt_TopLepTopHad = (topLep.P4() + topHad.P4()).Pt();
    bfEta_TopLepTopHad = (topLep.P4() + topHad.P4()).Eta();
    bfDeltaR_TopLepTopHad = topLep.P4().DeltaR(topHad.P4());
}
