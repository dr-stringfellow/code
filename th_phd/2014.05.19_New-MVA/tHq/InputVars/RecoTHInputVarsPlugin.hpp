/**
 * \file RecoTHInputVarsPlugin.hpp
 * \author Andrey Popov
 * 
 * Defines a plugin to dump input variables for reconstruction of a thq event.
 */

#pragma once

#include <Plugin.hpp>

#include <PECReaderPlugin.hpp>

#include <TRandom3.h>

#include <string>
#include <memory>


/**
 * \class RecoTHInputVarsPlugin
 * \brief Dumps input variables for reconstruction of a thq event
 * 
 * All partons must be matched to reconstructed jets with maximal distance dR = 0.3, and the jets
 * matched to b quarks must be central. Otherwise the event is skipped.
 */
class RecoTHInputVarsPlugin: public Plugin
{
public:
    /**
     * \brief Constructor
     * 
     * Accepts the name of the directory to host the produced files.
     */
    RecoTHInputVarsPlugin(std::string const &outDirectory);
    
    /**
     * \brief Destructor
     * 
     * Prints out matching efficiency.
     */
    ~RecoTHInputVarsPlugin();

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
    /**
     * \brief Matches a parton to a reconstructed jet
     * 
     * Returns the index of the jet with smallest dR and the value of dR.
     */
    static std::pair<unsigned, double> MatchJet(Candidate const &parton,
     std::vector<Jet> const &jets);
    
    /// Returns a vector whose first nChoose elements are a random subset of srcVector
    std::vector<unsigned> ChooseElements(std::vector<unsigned> srcVector, unsigned nChoose);
    
    /**
     * \brief Calculates input variables
     * 
     * 
     */
    void CalculateVariables(Jet const &bTop, Jet const &b1Higgs, Jet const &b2Higgs,
     Jet const &recoil);

private:
    /// Pointer to PECReaderPlugin
    PECReaderPlugin const *reader;
    
    /// Directory to store output files
    std::string outDirectory;
    
    /// Sums of event weights to calculate matching efficiency
    double sumWeightTotal, sumWeightMatched;
    
    /// Random-number generator
    TRandom3 rGen;
    
    /// Current output file
    std::unique_ptr<TFile> file;
    
    /// Current output tree
    std::unique_ptr<TTree> tree;
    
    
    // Output buffers
    ULong64_t bfEventNumber, bfRunNumber, bfLumiSection;
    
    Bool_t bfIsSignal;
    Float_t bfWeight;
    
    // Observables related to the Higgs boson
    Float_t bfMass_Higgs, bfPt_Higgs, bfEta_Higgs;
    Float_t bfDeltaR_BJetsHiggs;
    Float_t bfNumBTag_Higgs;  // number of b-tagged jets among decay products of the higgs
    Float_t bfSumCharge_Higgs;  // |Q(b1) + Q(b2)|
    Float_t bfMinPt_BHiggs;
    Float_t bfMaxEta_BHiggs;
    
    // Observables related to the top quark
    Float_t bfMass_Top, bfPt_Top, bfEta_Top;
    Float_t bfPt_BTop, bfEta_BTop, bfCharge_BTop;  // charge is multiplied by the lepton's charge
    Float_t bfPassBTag_BTop;
    Float_t bfMass_BTopLep;  // m(b+l)
    Float_t bfDeltaR_BTopW, bfDEta_TopLep;
    Float_t bfCos_LepBTop_W;
    //^ Cosine of an angle between lepton and top quark in the rest frame of W
    
    // Observables related to the recoil quark
    Float_t bfPt_Recoil, bfEta_Recoil, bfCharge_Recoil;
    
    
    // Observables that encode correlations between different objets
    Float_t bfRelHt, bfDeltaR_TopHiggs;
    Float_t bfCos_LepRecoil_TH;
};
