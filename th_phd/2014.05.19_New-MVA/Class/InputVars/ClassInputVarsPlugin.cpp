#include <ClassInputVarsPlugin.hpp>

#include <Processor.hpp>
#include <ROOTLock.hpp>
#include <Logger.hpp>

#include <TVector3.h>
#include <TMatrixDSym.h>
#include <TMatrixDSymEigen.h>

#include <boost/filesystem.hpp>


using namespace std;


ClassInputVarsPlugin::ClassInputVarsPlugin(string const &outDirectory_):
    Plugin("ClassInputVars"),
    outDirectory(outDirectory_)
{
    // Make sure the directory path ends with a slash
    if (outDirectory.back() != '/')
        outDirectory += '/';
    
    // Create the output directory if it does not exist
    boost::filesystem::create_directories(outDirectory);
}


Plugin *ClassInputVarsPlugin::Clone() const
{
    return new ClassInputVarsPlugin(outDirectory);
}


void ClassInputVarsPlugin::BeginRun(Dataset const &dataset)
{
    // Save pointers to required plugins for convenience
    reader = dynamic_cast<PECReaderPlugin const *>(processor->GetPluginBefore("Reader", name));
    builderTopHiggs =
     dynamic_cast<RecoTHPlugin const *>(processor->GetPluginBefore("RecoTH", name));
    builderTTbar =
     dynamic_cast<RecoTTbarPlugin const *>(processor->GetPluginBefore("RecoTTbar", name));
    
    // Save pointer to top-pt reweighter
    topPtReweighter = dynamic_cast<TopPtWeightPlugin const *>(
     processor->GetPluginBeforeQuiet("TopPtWeight", name));
    
    
    // Creation of ROOT objects is not thread-safe and must be protected
    ROOTLock::Lock();
    
    // Create the output file
    string fileName(dataset.GetFiles().front().GetBaseName());
    
    file.reset(new TFile((outDirectory + fileName + ".root").c_str(), "recreate"));
    
    // Create the tree
    tree.reset(new TTree("Vars", "Observables for to discriminate tHq from backgrounds"));
    
    // End of critical block
    ROOTLock::Unlock();
    
    
    // Assign branch addresses
    tree->Branch("EventNumber", &bfEventNumber);
    tree->Branch("RunNumber", &bfRunNumber);
    tree->Branch("LumiSection", &bfLumiSection);
    
    tree->Branch("Weight", &bfWeight);
    
    tree->Branch("glb_Charge_Lep", &glb_Charge_Lep);
    tree->Branch("glb_Pt_J1", &glb_Pt_J1);
    tree->Branch("glb_Pt_J2", &glb_Pt_J2);
    tree->Branch("glb_Ht", &glb_Ht);
    tree->Branch("glb_SqrtSHat", &glb_SqrtSHat);
    tree->Branch("glb_Sphericity", &glb_Sphericity);
    tree->Branch("glb_MET", &glb_MET);
    
    tree->Branch("tHq_Mass_Top", &tHq_Mass_Top);
    tree->Branch("tHq_Pt_Top", &tHq_Pt_Top);
    tree->Branch("tHq_Eta_Top", &tHq_Eta_Top);
    tree->Branch("tHq_Pt_BTop", &tHq_Pt_BTop);
    tree->Branch("tHq_Eta_BTop", &tHq_Eta_BTop);
    tree->Branch("tHq_PassBTag_BTop", &tHq_PassBTag_BTop);
    tree->Branch("tHq_Charge_BTop", &tHq_Charge_BTop);
    tree->Branch("tHq_DeltaR_BTopW", &tHq_DeltaR_BTopW);
    tree->Branch("tHq_DEta_TopLep", &tHq_DEta_TopLep);
    tree->Branch("tHq_Mass_BTopLep", &tHq_Mass_BTopLep);
        
    tree->Branch("tHq_Mass_Higgs", &tHq_Mass_Higgs);
    tree->Branch("tHq_Pt_Higgs", &tHq_Pt_Higgs);
    tree->Branch("tHq_Eta_Higgs", &tHq_Eta_Higgs);
    tree->Branch("tHq_NumBTag_Higgs", &tHq_NumBTag_Higgs);
    tree->Branch("tHq_DeltaR_BJetsHiggs", &tHq_DeltaR_BJetsHiggs);
    tree->Branch("tHq_AbsCharge_Higgs", &tHq_AbsCharge_Higgs);
    tree->Branch("tHq_MaxEta_BHiggs", &tHq_MaxEta_BHiggs);
    tree->Branch("tHq_MinPt_BHiggs", &tHq_MinPt_BHiggs);
    
    tree->Branch("tHq_Pt_Recoil", &tHq_Pt_Recoil);
    tree->Branch("tHq_Eta_Recoil", &tHq_Eta_Recoil);
    tree->Branch("tHq_DeltaR_TopHiggs", &tHq_DeltaR_TopHiggs);
    tree->Branch("tHq_Cos_LepRecoil_TH", &tHq_Cos_LepRecoil_TH);
    tree->Branch("tHq_Mass_TopHiggs", &tHq_Mass_TopHiggs);
    tree->Branch("tHq_RelHt", &tHq_RelHt);
    
    tree->Branch("tt_Mass_TopLep", &tt_Mass_TopLep);
    tree->Branch("tt_Pt_TopLep", &tt_Pt_TopLep);
    tree->Branch("tt_Eta_TopLep", &tt_Eta_TopLep);
    tree->Branch("tt_Pt_BTopLep", &tt_Pt_BTopLep);
    tree->Branch("tt_Eta_BTopLep", &tt_Eta_BTopLep);
    tree->Branch("tt_PassBTag_BTopLep", &tt_PassBTag_BTopLep);
    tree->Branch("tt_Charge_BTopLep", &tt_Charge_BTopLep);
    tree->Branch("tt_DeltaR_BTopLepWLep", &tt_DeltaR_BTopLepWLep);
    tree->Branch("tt_DEta_TopLepLep", &tt_DEta_TopLepLep);
    tree->Branch("tt_Mass_BTopLepLep", &tt_Mass_BTopLepLep);
    
    tree->Branch("tt_Mass_TopHad", &tt_Mass_TopHad);
    tree->Branch("tt_Pt_TopHad", &tt_Pt_TopHad);
    tree->Branch("tt_Eta_TopHad", &tt_Eta_TopHad);
    tree->Branch("tt_Pt_BTopHad", &tt_Pt_BTopHad);
    tree->Branch("tt_Eta_BTopHad", &tt_Eta_BTopHad);
    tree->Branch("tt_PassBTag_BTopHad", &tt_PassBTag_BTopHad);
    tree->Branch("tt_Charge_BTopHad", &tt_Charge_BTopHad);
    tree->Branch("tt_DeltaR_BTopHadWHad", &tt_DeltaR_BTopHadWHad);
    
    tree->Branch("tt_Mass_WHad", &tt_Mass_WHad);
    tree->Branch("tt_Pt_WHad", &tt_Pt_WHad);
    tree->Branch("tt_Eta_WHad", &tt_Eta_WHad);
    tree->Branch("tt_DeltaR_Light", &tt_DeltaR_Light);
    tree->Branch("tt_NumPassBTag_Light", &tt_NumPassBTag_Light);
    tree->Branch("tt_MaxEta_Light", &tt_MaxEta_Light);
    tree->Branch("tt_SumCharge_Light", &tt_SumCharge_Light);
    
    tree->Branch("tt_DMass_TopHadWHad", &tt_DMass_TopHadWHad);
    tree->Branch("tt_RelHt", &tt_RelHt);
    tree->Branch("tt_MaxMass_BTopHadLight", &tt_MaxMass_BTopHadLight);
    
    // O(60) variables in total
}


void ClassInputVarsPlugin::EndRun()
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


bool ClassInputVarsPlugin::ProcessEvent()
{
    // Update event ID and weight
    auto const &eventID = (*reader)->GetEventID();
    
    bfRunNumber = eventID.Run();
    bfEventNumber = eventID.Event();
    bfLumiSection = eventID.LumiBlock();
    
    bfWeight = (*reader)->GetCentralWeight();
    
    if (topPtReweighter)
        bfWeight *= topPtReweighter->GetWeight();
    
    
    // Define some shortcuts and reconstruct W boson
    auto const &jets = (*reader)->GetJets();
    auto const &lepton = (*reader)->GetLeptons().front();
    
    Candidate const wLep(lepton.P4() + (*reader)->GetNeutrino().P4());
    
    
    // Calculate observables that do not require event reconstruction
    glb_Charge_Lep = lepton.Charge();
    glb_Pt_J1 = jets.at(0).Pt();
    glb_Pt_J2 = jets.at(1).Pt();
    
    TLorentzVector p4AllJets;
    glb_Ht = 0.;
    
    for (auto const &j: jets)
    {
        p4AllJets += j.P4();
        glb_Ht += j.Pt();
    }
    
    glb_MET = (*reader)->GetMET().Pt();
    glb_Ht += lepton.Pt() + glb_MET;
    glb_SqrtSHat = (p4AllJets + wLep.P4()).M();
    
    
    // Calculate sphericity
    TMatrixDSym sphericityTensor(3);
    double norm = 0.;
    
    for (auto const &p: {lepton.P4(), (*reader)->GetNeutrino().P4()})
    {
        TVector3 p3(p.Vect());
        norm += p3.Mag2();
        
        for (unsigned i = 0; i < 3; ++i)
            for (unsigned j = 0; j < 3; ++j)
                sphericityTensor(i, j) += p3[i] * p3[j];
    }
    
    for (auto const &jet: jets)
    {
        TVector3 p3(jet.P4().Vect());
        norm += p3.Mag2();
        
        for (unsigned i = 0; i < 3; ++i)
            for (unsigned j = 0; j < 3; ++j)
                sphericityTensor(i, j) += p3[i] * p3[j];
    }
    
    sphericityTensor *= 1. / norm;
    
    TMatrixDSymEigen eigenValCalc(sphericityTensor);
    TVectorD eigenVals(eigenValCalc.GetEigenValues());
    
    glb_Sphericity = 1.5 * (eigenVals[1] + eigenVals[2]);
    
    
    // Calculate variables reconstructed under thq hypothesis
    Jet const &bTop = builderTopHiggs->GetBTop();
    Jet const &b1Higgs = builderTopHiggs->GetBHiggs().first;
    Jet const &b2Higgs = builderTopHiggs->GetBHiggs().second;
    
    Candidate const top(bTop.P4() + wLep.P4());
    Candidate const higgs(b1Higgs.P4() + b2Higgs.P4());
    Jet const &recoilQuark = builderTopHiggs->GetRecoilJet();
    
    tHq_Mass_Top = top.M();
    tHq_Pt_Top = top.Pt();
    tHq_Eta_Top = top.Eta();
    
    tHq_Pt_BTop = bTop.Pt();
    tHq_Eta_BTop = bTop.Eta();
    tHq_PassBTag_BTop = 0. + (bTop.CSV() > 0.898);
    tHq_Charge_BTop = bTop.Charge() * lepton.Charge();
    
    tHq_DeltaR_BTopW = bTop.P4().DeltaR(wLep.P4());
    tHq_DEta_TopLep = fabs(top.Eta() - lepton.Eta());
    tHq_Mass_BTopLep = (bTop.P4() + lepton.P4()).M();
    
    tHq_Mass_Higgs = higgs.M();
    tHq_Pt_Higgs = higgs.Pt();
    tHq_Eta_Higgs = higgs.Eta();
    
    tHq_NumBTag_Higgs = 0. + (b1Higgs.CSV() > 0.898) + (b2Higgs.CSV() > 0.898);
    tHq_AbsCharge_Higgs = fabs(b1Higgs.Charge() + b2Higgs.Charge());
    tHq_MinPt_BHiggs = min(b1Higgs.Pt(), b2Higgs.Pt());
    tHq_MaxEta_BHiggs = max(fabs(b1Higgs.Eta()), fabs(b2Higgs.Eta()));

    
    tHq_Pt_Recoil = recoilQuark.Pt();
    tHq_Eta_Recoil = recoilQuark.Eta();
    
    tHq_DeltaR_TopHiggs = higgs.P4().DeltaR(top.P4());
    tHq_DeltaR_BJetsHiggs = b1Higgs.P4().DeltaR(b2Higgs.P4());
    
    tHq_Mass_TopHiggs = (higgs.P4() + top.P4()).M();
    tHq_RelHt = (higgs.Pt() + top.Pt() + recoilQuark.Pt()) / glb_Ht;
    
    
    // Calculate tHq_Cos_LepRecoil_TH
    TVector3 b((higgs.P4() + top.P4()).BoostVector());
    
    TLorentzVector boostedLepton(lepton.P4());
    boostedLepton.Boost(-b);
    TVector3 const p3Lepton(boostedLepton.Vect());
    
    TLorentzVector boostedRecoil(recoilQuark.P4());
    boostedRecoil.Boost(-b);
    TVector3 const p3Recoil(boostedRecoil.Vect());
    
    tHq_Cos_LepRecoil_TH = p3Lepton.Dot(p3Recoil) / (p3Lepton.Mag() * p3Recoil.Mag());
    
    
    // Calculate observables constructed under ttbar hypothesis
    Jet const &bTopLep = builderTTbar->GetBTopLep();
    Jet const &bTopHad = builderTTbar->GetBTopHad();
    Jet const &q1TopHad = builderTTbar->GetLightTopHad().first;
    Jet const &q2TopHad = builderTTbar->GetLightTopHad().second;
    
    Candidate const topLep(bTopLep.P4() + wLep.P4());
    Candidate const wHad(q1TopHad.P4() + q2TopHad.P4());
    Candidate const topHad(bTopHad.P4() + wHad.P4());
    
    tt_Mass_TopLep = topLep.M();
    tt_Pt_TopLep = topLep.Pt();
    tt_Eta_TopLep = topLep.Eta();
    
    tt_Pt_BTopLep = bTopLep.Pt();
    tt_Eta_BTopLep = bTopLep.Eta();
    tt_PassBTag_BTopLep = 0. + (bTopLep.CSV() > 0.898);
    tt_Charge_BTopLep = bTopLep.Charge() * lepton.Charge();
    
    tt_DeltaR_BTopLepWLep = bTopLep.P4().DeltaR(wLep.P4());
    tt_DEta_TopLepLep = fabs(topLep.Eta() - lepton.Eta());
    tt_Mass_BTopLepLep = (bTopLep.P4() + lepton.P4()).M();
    
    tt_Mass_TopHad = topHad.M();
    tt_Pt_TopHad = topHad.Pt();
    tt_Eta_TopHad = topHad.Eta();
    
    tt_Pt_BTopHad = bTopHad.Pt();
    tt_Eta_BTopHad = bTopHad.Eta();
    tt_PassBTag_BTopHad = 0. + (bTopHad.CSV() > 0.898);
    tt_Charge_BTopHad = bTopHad.Charge() * lepton.Charge();
    
    tt_DeltaR_BTopHadWHad = bTopHad.P4().DeltaR(wHad.P4());
    
    tt_Mass_WHad = wHad.M();
    tt_Pt_WHad = wHad.Pt();
    tt_Eta_WHad = wHad.Eta();
    
    tt_DMass_TopHadWHad = tt_Mass_TopHad - tt_Mass_WHad;
    tt_RelHt = (topHad.Pt() + topLep.Pt()) / glb_Ht;
    
    tt_DeltaR_Light = q1TopHad.P4().DeltaR(q2TopHad.P4());
    tt_NumPassBTag_Light = 0. + (q1TopHad.CSV() > 0.898) + (q2TopHad.CSV() > 0.898);
    tt_MaxEta_Light = max(fabs(q1TopHad.Eta()), fabs(q2TopHad.Eta()));
    tt_SumCharge_Light = (q1TopHad.Charge() + q2TopHad.Charge()) * lepton.Charge();
    
    tt_MaxMass_BTopHadLight =
     max((bTopHad.P4() + q1TopHad.P4()).M(), (bTopHad.P4() + q2TopHad.P4()).M());
    
    
    // Finally, fill the tree
    tree->Fill();
    
    return true;
}
