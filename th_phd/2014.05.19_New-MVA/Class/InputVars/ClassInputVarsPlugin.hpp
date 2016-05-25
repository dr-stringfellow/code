/**
 * \file ClassInputVarsPlugin.hpp
 * \author Andrey Popov
 * 
 * Defines a plugin to dump input variables to discriminate tHq from backgrounds.
 */

#pragma once

#include <Plugin.hpp>

#include <PECReaderPlugin.hpp>
#include <TopPtWeightPlugin.hpp>
#include <RecoTHPlugin.hpp>
#include <RecoTTbarPlugin.hpp>

#include <string>
#include <memory>


/**
 * \class ClassInputVarsPlugin
 * \brief Dumps possile input variables to discriminate thq from backgrounds
 * 
 * \warning Definition of a b-tagged jet is hard-coded.
 */
class ClassInputVarsPlugin: public Plugin
{
public:
    /**
     * \brief Constructor
     * 
     * Accepts the name of the directory to host the produced files.
     */
    ClassInputVarsPlugin(std::string const &outDirectory);
    
public:
    /**
     * \brief Constructs a newly-initialised copy
     * 
     * Consult documentation of the overriden method for details.
     */
    Plugin *Clone() const;
    
    /**
     * \brief Notifies this that a dataset has been opened
     * 
     * Creates the output tree. Consult documentation of the overriden method for details.
     */
    void BeginRun(Dataset const &dataset);
    
    /**
     * \brief Notifies this that a dataset has been closed
     * 
     * Writes down the current output tree. Consult documentation of the overriden method for
     * details.
     */
    void EndRun();
    
    /**
     * \brief Processes the current event
     * 
     * Calculates and stores the input variables. The method always returns true, despite the fact
     * that the input variables are written for perfectly matched events only. Consult documentation
     * of the overriden method for details.
     */
    bool ProcessEvent();
    
private:
    /// Pointer to PECReaderPlugin
    PECReaderPlugin const *reader;
    
    /// Pointer to a plugin to perform top-pt reweighting (optional)
    TopPtWeightPlugin const *topPtReweighter;
    
    /// Pointer to a plugin that performs reconstruction of an event under thq hypothesis
    RecoTHPlugin const *builderTopHiggs;
    
    /// Pointer to a plugin that performs reconstruction of an event under ttbar hypothersis
    RecoTTbarPlugin const *builderTTbar;
    
    /// Directory to store output files
    std::string outDirectory;
    
    /// Current output file
    std::unique_ptr<TFile> file;
    
    /// Current output tree
    std::unique_ptr<TTree> tree;
    
    
    // Output buffers
    ULong64_t bfEventNumber, bfRunNumber, bfLumiSection;
    
    Float_t bfWeight;
    
    
    // Observables that do not require event reconstruction
    Float_t glb_Charge_Lep;
    Float_t glb_Pt_J1, glb_Pt_J2, glb_Ht;
    Float_t glb_SqrtSHat;  // mass of sum of all objects
    Float_t glb_Sphericity, glb_MET;
    
    
    // Observables defined under thq hypothesis
    Float_t tHq_Mass_Top, tHq_Pt_Top, tHq_Eta_Top;
    Float_t tHq_Pt_BTop, tHq_Eta_BTop, tHq_PassBTag_BTop;
    Float_t tHq_Charge_BTop;  // multiplied by the charge of the lepton
    Float_t tHq_DeltaR_BTopW;
    Float_t tHq_DEta_TopLep;
    Float_t tHq_Mass_BTopLep;
    
    Float_t tHq_Mass_Higgs, tHq_Pt_Higgs, tHq_Eta_Higgs;
    Float_t tHq_NumBTag_Higgs;
    Float_t tHq_AbsCharge_Higgs;
    Float_t tHq_MinPt_BHiggs, tHq_MaxEta_BHiggs;
    
    Float_t tHq_Pt_Recoil, tHq_Eta_Recoil;
    
    Float_t tHq_DeltaR_TopHiggs, tHq_DeltaR_BJetsHiggs;
    
    Float_t tHq_Cos_LepRecoil_TH;
    //^ Angle between three-momenta of the lepton and the light-flavour jet in the rest frame of
    //t+h system
    
    Float_t tHq_Mass_TopHiggs, tHq_RelHt;
    
    
    // Observables defined unber ttbar hypothesis
    Float_t tt_Mass_TopLep, tt_Pt_TopLep, tt_Eta_TopLep;
    Float_t tt_Pt_BTopLep, tt_Eta_BTopLep, tt_PassBTag_BTopLep;
    Float_t tt_Charge_BTopLep;  // multiplied by the charge of the lepton
    Float_t tt_DeltaR_BTopLepWLep;
    Float_t tt_DEta_TopLepLep;
    Float_t tt_Mass_BTopLepLep;
    
    Float_t tt_Mass_TopHad, tt_Pt_TopHad, tt_Eta_TopHad;
    Float_t tt_Pt_BTopHad, tt_Eta_BTopHad, tt_PassBTag_BTopHad;
    Float_t tt_Charge_BTopHad;  // multiplied by the charge of the lepton
    Float_t tt_DeltaR_BTopHadWHad;
    
    Float_t tt_Mass_WHad, tt_Pt_WHad, tt_Eta_WHad;
    Float_t tt_DeltaR_Light, tt_MaxEta_Light;
    Float_t tt_NumPassBTag_Light;
    Float_t tt_SumCharge_Light;  // multiplied by the charge of the lepton
    
    Float_t tt_DMass_TopHadWHad;
    Float_t tt_RelHt;
    //^ Sum of pt of the two top-quarks devided by the total Ht of the event
    
    Float_t tt_MaxMass_BTopHadLight;
    //^ max m(b + q), where b is the b-jet from hadronically decaying top-quark and q is one of
    //light-flavour jets from the subsequent W-boson decay
    
};
