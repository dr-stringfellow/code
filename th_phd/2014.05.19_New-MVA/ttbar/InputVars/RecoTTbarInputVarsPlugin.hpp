/**
 * \file RecoTTbarInputVarsPlugin.hpp
 * \author Andrey Popov
 * 
 * Defines a plugin to dump input variables for reconstruction of a ttbar event.
 */

#pragma once

#include <Plugin.hpp>

#include <PECReaderPlugin.hpp>
#include <TopPtWeightPlugin.hpp>

#include <TRandom3.h>

#include <string>
#include <memory>


/**
 * \class RecoTTbarInputVarsPlugin
 * \brief Dumps input variables for reconstruction of a ttbar event
 * 
 * All partons must be matched to reconstructed jets with maximal distance dR = 0.3, and the jets
 * matched to b quarks must be central and b-tagged. Otherwise the event is skipped.
 */
class RecoTTbarInputVarsPlugin: public Plugin
{
public:
    /**
     * \brief Constructor
     * 
     * Accepts the name of the directory to host the produced files.
     */
    RecoTTbarInputVarsPlugin(std::string const &outDirectory);
    
    /**
     * \brief Destructor
     * 
     * Prints out matching efficiency.
     */
    ~RecoTTbarInputVarsPlugin();

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
    
    /**
     * \brief Calculates input variables
     * 
     * 
     */
    void CalculateVariables(Jet const &bTopLep, Jet const &bTopHad, Jet const &q1TopHad,
     Jet const &q2TopHad);

private:
    /// Pointer to PECReaderPlugin
    PECReaderPlugin const *reader;
    
    /// Pointer to top-pt reweighter
    TopPtWeightPlugin const *topPtReweighter;
    
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
    
    // Observables related to the top quark that decays semileptonically
    Float_t bfMass_TopLep, bfPt_TopLep, bfEta_TopLep;
    Float_t bfPt_BTopLep, bfEta_BTopLep, bfCharge_BTopLep;  // charge is multiplied by the lepton's charge
    Float_t bfMass_BTopLepLep;  // m(b+l)
    Float_t bfDeltaR_BTopLepWLep, bfDEta_TopLepLep;
    Float_t bfCos_LepBTopLep_WLep;
    //^ Cosine of an angle between lepton and top quark in the rest frame of W
    
    // Observables related to the top quark that decays hadronically
    Float_t bfMass_TopHad, bfPt_TopHad, bfEta_TopHad;
    Float_t bfMass_WHad, bfPt_WHad, bfEta_WHad;
    Float_t bfDeltaR_BTopHadWHad;
    Float_t bfPt_BTopHad, bfEta_BTopHad;
    Float_t bfCharge_BTopHad;  // Qb * Ql
    Float_t bfMinPt_Light;
    Float_t bfMaxEta_Light;  // max(|eta(q1)|, |eta(q2)|)
    Float_t bfNumBTag_Light;
    Float_t bfSumCharge_Light;  // (Qq1 + Qq2) * Ql
    Float_t bfDeltaR_Light;  // dR(q1, q2)
    Float_t bfMaxMass_BTopHadLight;  // max(m(b + q1), m(b + q2))
    
    // Observables that encode correlations between different objets
    Float_t bfRelHt, bfDeltaR_TopLepTopHad;
    Float_t bfMass_TopLepTopHad, bfPt_TopLepTopHad, bfEta_TopLepTopHad;
};
