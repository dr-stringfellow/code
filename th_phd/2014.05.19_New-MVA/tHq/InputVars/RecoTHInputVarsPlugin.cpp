#include <RecoTHInputVarsPlugin.hpp>

#include <Processor.hpp>
#include <ROOTLock.hpp>
#include <Logger.hpp>

#include <boost/filesystem.hpp>

#include <limits>
#include <array>


using namespace std;


RecoTHInputVarsPlugin::RecoTHInputVarsPlugin(string const &outDirectory_):
    Plugin("RecoTHInputVars"),
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


RecoTHInputVarsPlugin::~RecoTHInputVarsPlugin()
{
    logging::logger << "Overall matching efficiency measured by plugin \"" << name <<
     "\" is " << sumWeightMatched / sumWeightTotal << logging::eom;
}


Plugin *RecoTHInputVarsPlugin::Clone() const
{
    return new RecoTHInputVarsPlugin(outDirectory);
}


void RecoTHInputVarsPlugin::BeginRun(Dataset const &dataset)
{
    // Save pointer to the reader plugin
    reader = dynamic_cast<PECReaderPlugin const *>(processor->GetPluginBefore("Reader", name));
    
    
    // Creation of ROOT objects is not thread-safe and must be protected
    ROOTLock::Lock();
    
    // Create the output file
    string fileName(dataset.GetFiles().front().GetBaseName());
    fileName = fileName.substr(0, fileName.find_first_of('_'));
    
    file.reset(new TFile((outDirectory + fileName + ".root").c_str(), "recreate"));
    
    // Create the tree
    tree.reset(new TTree("Vars", "Observables for reconstruction of a tHq event"));
    
    // End of critical block
    ROOTLock::Unlock();
    
    
    // Assign branch addresses
    tree->Branch("EventNumber", &bfEventNumber);
    tree->Branch("RunNumber", &bfRunNumber);
    tree->Branch("LumiSection", &bfLumiSection);
    
    tree->Branch("IsSignal", &bfIsSignal);
    tree->Branch("Weight", &bfWeight);
    
    tree->Branch("Mass_Higgs", &bfMass_Higgs);
    tree->Branch("Pt_Higgs", &bfPt_Higgs);
    tree->Branch("Eta_Higgs", &bfEta_Higgs);
    tree->Branch("DeltaR_BJetsHiggs", &bfDeltaR_BJetsHiggs);
    tree->Branch("NumBTag_Higgs", &bfNumBTag_Higgs);
    tree->Branch("SumCharge_Higgs", &bfSumCharge_Higgs);
    tree->Branch("MinPt_BHiggs", &bfMinPt_BHiggs);
    tree->Branch("MaxEta_BHiggs", &bfMaxEta_BHiggs);
    
    tree->Branch("Mass_Top", &bfMass_Top);
    tree->Branch("Pt_Top", &bfPt_Top);
    tree->Branch("Eta_Top", &bfEta_Top);
    tree->Branch("Pt_BTop", &bfPt_BTop);
    tree->Branch("Eta_BTop", &bfEta_BTop);
    tree->Branch("PassBTag_BTop", &bfPassBTag_BTop);
    tree->Branch("Charge_BTop", &bfCharge_BTop);
    tree->Branch("Mass_BTopLep", &bfMass_BTopLep);
    tree->Branch("DeltaR_BTopW", &bfDeltaR_BTopW);
    tree->Branch("DEta_TopLep", &bfDEta_TopLep);
    tree->Branch("Cos_LepBTop_W", &bfCos_LepBTop_W);
    
    tree->Branch("Pt_Recoil", &bfPt_Recoil);
    tree->Branch("Eta_Recoil", &bfEta_Recoil);
    tree->Branch("Charge_Recoil", &bfCharge_Recoil);
    
    tree->Branch("RelHt", &bfRelHt);
    tree->Branch("DeltaR_TopHiggs", &bfDeltaR_TopHiggs);
    tree->Branch("Cos_LepRecoil_TH", &bfCos_LepRecoil_TH);
}


void RecoTHInputVarsPlugin::EndRun()
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


bool RecoTHInputVarsPlugin::ProcessEvent()
{
    // Find the b quarks from decay of a higgs
    auto const &genParticles = (*reader)->GetHardGenParticles();
    GenParticle const *b1HiggsParton(nullptr), *b2HiggsParton(nullptr);
    
    for (GenParticle const &p: genParticles)
    {
        // A Higgs boson
        if (p.GetPdgId() != 25)
            continue;
        
        // It should have two daughters
        if (p.GetDaughters().size() != 2)
            continue;
        
        
        b1HiggsParton = p.GetDaughters().front();
        b2HiggsParton = p.GetDaughters().back();
        break;
    }
    
    
    // A sanity check
    if (not b1HiggsParton or not b2HiggsParton or b1HiggsParton == b2HiggsParton or
     abs(b1HiggsParton->GetPdgId()) != 5 or abs(b2HiggsParton->GetPdgId()) != 5)
        throw logic_error("RecoTHInputVarsPlugin::ProcessEvent: Failed to find the b quarks from "
         "decay of a higgs boson.");
    
    
    // Find the b quark from semileptonic decay of a top quark
    GenParticle const *bTopParton = nullptr;
    
    for (GenParticle const &p: genParticles)
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
                    bTopParton = &p;
                    break;
                }
            }
            
            if (bTopParton)
                break;
        }
        else
        {
            if (w->GetDaughters().size() == 0)
                continue;
            
            int const absLeptonPdgId = abs(w->GetDaughters().front()->GetPdgId());
            
            if (absLeptonPdgId >= 11 and absLeptonPdgId <= 18)  // charged and neutral leptons
            {
                bTopParton = &p;
                break;
            }
        }
    }
    
    
    // Make sure the b quark from decay of a top quark was found
    if (not bTopParton)
        throw logic_error("RecoTHInputVarsPlugin::ProcessEvent: Failed to find the b quark from "
         "semileptonic decay of a top quark.");
    
    
    // Find the recoil quark
    GenParticle const *recoilParton(nullptr);
    
    for (unsigned i = 4; i < genParticles.size(); ++i)  // skip the initial section
    {
        // The recoil quark is the only light-flavour (though, charm included) quark in the final
        //state
        if (abs(genParticles.at(i).GetPdgId()) > 4)
            continue;
        
        recoilParton = &genParticles.at(i);
        break;
    }
    
    
    // A sanity check
    if (not recoilParton)
        throw logic_error("RecoTHInputVarsPlugin::ProcessEvent: Failed to find the recoil quark.");
    
    
    // Get event weight
    bfWeight = (*reader)->GetCentralWeight();
    sumWeightTotal += bfWeight;
    
    
    // Match the b quarks from decay of the higgs to reconstructed jet
    auto const &jets = (*reader)->GetJets();
    
    auto matchRes = MatchJet(*b1HiggsParton, jets);
    unsigned b1HiggsJetIndex = matchRes.first;
    
    if (matchRes.second > 0.3)  // the matched jet is not close enough to the parton
        return true;
    
    
    matchRes = MatchJet(*b2HiggsParton, jets);
    unsigned b2HiggsJetIndex = matchRes.first;
    
    if (matchRes.second > 0.3)  // the matched jet is not close enough to the parton
        return true;
    
    
    // Check these are two different jets (rejects 0.1-0.2% of events)
    if (b1HiggsJetIndex == b2HiggsJetIndex)
        return true;
    
    // Make sure the matched jet is central (rejects ~1.0% of events)
    if (fabs(jets.at(b1HiggsJetIndex).Eta()) > 2.4 or fabs(jets.at(b2HiggsJetIndex).Eta()) > 2.4)
        return true;
    
    // Order the jets from decay of a higgs in pt
    if (jets.at(b1HiggsJetIndex).Pt() < jets.at(b2HiggsJetIndex).Pt())
        swap(b1HiggsJetIndex, b2HiggsJetIndex);
    
    
    // Match the b quark from decay of the top to a reconstucted jet
    matchRes = MatchJet(*bTopParton, jets);
    unsigned bTopJetIndex = matchRes.first;
    
    if (matchRes.second > 0.3)  // the matched jet is not close enough to the parton
        return true;
    
    // Make sure the matched jet is central (rejects 0.1-0.2% of events)
    if (fabs(jets.at(bTopJetIndex).Eta()) > 2.4)
        return true;
    
    
    // Match the recoil quark to a reconstructed jet
    matchRes = MatchJet(*recoilParton, jets);
    unsigned recoilJetIndex = matchRes.first;
    
    if (matchRes.second > 0.3)  // the matched jet is not close enough to the parton
        return true;
    
    // Make sure the jet is untagged. Although the recoil jet might be a charm and hence might be
    //tagged, in practice it happens almost never
    if (fabs(jets.at(recoilJetIndex).Eta() < 2.4) and jets.at(recoilJetIndex).CSV() > 0.898)
        return true;
    
    
    // Make sure that all jets are matched in a unique way. For the higgs jets it has already been
    //checked
    if (bTopJetIndex == b1HiggsJetIndex or bTopJetIndex == b2HiggsJetIndex or
     bTopJetIndex == recoilJetIndex or b1HiggsJetIndex == recoilJetIndex or
     b2HiggsJetIndex == recoilJetIndex)
        return true;
    
    
    // Increment sum of weights of matched events
    sumWeightMatched += bfWeight;
    
    
    // Update event ID
    auto const &eventID = (*reader)->GetEventID();
    bfRunNumber = eventID.Run();
    bfEventNumber = eventID.Event();
    bfLumiSection = eventID.LumiBlock();
    
    
    // Fill the output tree with variables calculated for the correct jet assignment
    bfIsSignal = true;
    CalculateVariables(jets.at(bTopJetIndex), jets.at(b1HiggsJetIndex), jets.at(b2HiggsJetIndex),
     jets.at(recoilJetIndex));
    
    tree->Fill();
    
    
    // Build a wrong match. Still only central jets can be (fakely) matched to b quarks
    unsigned bTopJetIndexFake, b1HiggsJetIndexFake, b2HiggsJetIndexFake, recoilJetIndexFake;
    vector<unsigned> centralJetIndices;
    
    for (unsigned i = 0; i < jets.size(); ++i)
    {
        if (fabs(jets.at(i).Eta()) > 2.4)
            continue;
        
        centralJetIndices.push_back(i);
    }
    
    
    // Check it is possible to build a wrong match
    if (centralJetIndices.size() < 3)
        throw logic_error("RecoTHInputVarsPlugin::ProcessEvent: Unexpected number of central jets "
         "is observed. There is a mistake in the logic of the plugin.");
    
    
    // Sample random combinations until one different from the correct match is found
    vector<unsigned> allowedIndicesRecoil;
    
    while (true)
    {
        // First choose the b-jets
        vector<unsigned> const randomCentralJetIndices = ChooseElements(centralJetIndices, 3);
        bTopJetIndexFake = randomCentralJetIndices.at(0);
        b1HiggsJetIndexFake = randomCentralJetIndices.at(1);
        b2HiggsJetIndexFake = randomCentralJetIndices.at(2);
        
        if (jets.at(b1HiggsJetIndexFake).Pt() < jets.at(b2HiggsJetIndexFake).Pt())
            swap(b1HiggsJetIndexFake, b2HiggsJetIndexFake);
        
        
        // Now choose the recoil jet
        allowedIndicesRecoil.clear();
        
        for (unsigned i = 0; i < jets.size(); ++i)
        {
            if (i == bTopJetIndexFake or i == b1HiggsJetIndexFake or i == b2HiggsJetIndexFake)
                continue;
            
            if (fabs(jets.at(i).Eta()) < 2.4 and jets.at(i).CSV() > 0.898)
                continue;
            
            allowedIndicesRecoil.push_back(i);
        }
        
        // It might happen that all untagged jets are assigned to b-quarks
        if (allowedIndicesRecoil.size() == 0)
            continue;
        //^ It would be more optimal to do the other way: first choose the recoil jet, then choose
        //jets for b-quarks. But I was lazy to rewrite the code
        
        
        recoilJetIndexFake = allowedIndicesRecoil.at(rGen.Integer(allowedIndicesRecoil.size()));
        
        
        // The fake match is ready. Check if it is different from the correct one
        if (bTopJetIndexFake != bTopJetIndex or b1HiggsJetIndexFake != b1HiggsJetIndex or
         b2HiggsJetIndexFake != b2HiggsJetIndexFake or recoilJetIndexFake != recoilJetIndex)
            break;
    }
    
    
    // Fill the output tree with variables calculated for the chosen wrong jet assignment
    bfIsSignal = false;
    CalculateVariables(jets.at(bTopJetIndexFake), jets.at(b1HiggsJetIndexFake),
     jets.at(b2HiggsJetIndexFake), jets.at(recoilJetIndexFake));
    
    tree->Fill();
    
    
    return true;
}


pair<unsigned, double> RecoTHInputVarsPlugin::MatchJet(Candidate const &parton,
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


vector<unsigned> RecoTHInputVarsPlugin::ChooseElements(vector<unsigned> srcVector, unsigned nChoose)
{
    // http://stackoverflow.com/a/9345144/966461
    
    if (nChoose > srcVector.size())
        nChoose = srcVector.size();
    
    for (unsigned i = 0; i < nChoose; ++i)
    {
        // Choose a random index in the source vector starting from position i
        unsigned const j = i + rGen.Integer(srcVector.size() - i);
        
        if (i != j)  // an element at given position is allowed to stay the same
            swap(srcVector.at(i), srcVector.at(j));
    }
    
    
    // The first nChoose elements in the srcVector are now a random subset of the whole vector
    return srcVector;
}


void RecoTHInputVarsPlugin::CalculateVariables(Jet const &bTop, Jet const &b1Higgs,
 Jet const &b2Higgs, Jet const &recoil)
{
    // Calculate variables related to the Higgs boson
    Candidate const higgs(b1Higgs.P4() + b2Higgs.P4());
    
    bfMass_Higgs = higgs.M();
    bfPt_Higgs = higgs.Pt();
    bfEta_Higgs = higgs.Eta();
    
    bfDeltaR_BJetsHiggs = b1Higgs.P4().DeltaR(b2Higgs.P4());
    bfSumCharge_Higgs = fabs(b1Higgs.Charge() + b2Higgs.Charge());
    
    bfNumBTag_Higgs = 0 + (b1Higgs.CSV() > 0.898) + (b2Higgs.CSV() > 0.898);
    
    bfMinPt_BHiggs = min(b1Higgs.Pt(), b2Higgs.Pt());
    bfMaxEta_BHiggs = max(fabs(b1Higgs.Eta()), fabs(b2Higgs.Eta()));
    
    
    // Calculate variables related to the top quark
    auto const &lepton = (*reader)->GetLeptons().front();
    Candidate const wBoson(lepton.P4() + (*reader)->GetNeutrino().P4());
    Candidate const top(wBoson.P4() + bTop.P4());
    
    bfMass_Top = top.M();
    bfPt_Top = top.Pt();
    bfEta_Top = top.Eta();
    
    bfPt_BTop = bTop.Pt();
    bfEta_BTop = bTop.Eta();
    bfPassBTag_BTop = 0. + (bTop.CSV() > 0.898);
    bfCharge_BTop = bTop.Charge() * lepton.Charge();
    
    bfMass_BTopLep = (bTop.P4() + lepton.P4()).M();
    bfDeltaR_BTopW = bTop.P4().DeltaR(wBoson.P4());
    bfDEta_TopLep = fabs(top.Eta() - lepton.Eta());
    
    // Calculate the cosine
    TVector3 b((wBoson.P4()).BoostVector());
    
    TLorentzVector boostedLepton(lepton.P4());
    boostedLepton.Boost(-b);
    TVector3 p3Lepton(boostedLepton.Vect());
    
    TLorentzVector boostedBJet(bTop.P4());
    boostedBJet.Boost(-b);
    TVector3 const p3BJet(boostedBJet.Vect());
    
    bfCos_LepBTop_W = p3Lepton.Dot(p3BJet) / (p3Lepton.Mag() * p3BJet.Mag());
    
    
    // Variables related to the recoil quark
    bfPt_Recoil = recoil.Pt();
    bfEta_Recoil = recoil.Eta();
    bfCharge_Recoil = recoil.Charge();
    
    
    // Calcualate variables with correlations between different objects
    double Ht = lepton.Pt() + (*reader)->GetMET().Pt();
    
    for (auto const &j: (*reader)->GetJets())
        Ht += j.Pt();
    
    bfRelHt = (top.Pt() + higgs.Pt() + recoil.Pt()) / Ht;
    
    bfDeltaR_TopHiggs = top.P4().DeltaR(higgs.P4());
    
    // Calculate the cosine in the rest frame of (t+h)
    b = (higgs.P4() + top.P4()).BoostVector();
    
    boostedLepton = lepton.P4();
    boostedLepton.Boost(-b);
    p3Lepton = boostedLepton.Vect();
    
    TLorentzVector boostedRecoil(recoil.P4());
    boostedRecoil.Boost(-b);
    TVector3 const p3Recoil(boostedRecoil.Vect());
    
    bfCos_LepRecoil_TH = p3Lepton.Dot(p3Recoil) / (p3Lepton.Mag() * p3Recoil.Mag());
}
