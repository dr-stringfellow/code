/**
 * \author Andrey Popov
 * 
 * The script plots distributions over input variables normalised to unit area. The binning is
 * adapted in such a way that every bin contains similar number of events taken from 1:1 signal and
 * background mixture. Several criteria of similarity between signal and background PDFs are
 * evaluated. The code can be compiled with
 *   g++ -o plotInputVars plotInputVars.cpp --std=c++11 `root-config --cflags --libs` -lTreePlayer -I/afs/cern.ch/sw/lcg/external/Boost/1.50.0_python2.7/x86_64-slc5-gcc46-opt/include/boost-1_50/ -O2
 */

#include "/afs/cern.ch/user/a/aapopov/workspace/tHq/2012Alpha/2013.08.23_Full-analysis-CSVM/Step1_MVA-reco/plotInputVars/NamedFormula.hpp"
#include "/afs/cern.ch/user/a/aapopov/workspace/tHq/2012Alpha/2013.08.23_Full-analysis-CSVM/Step1_MVA-reco/plotInputVars/BinnedVariable.hpp"
#include "/afs/cern.ch/user/a/aapopov/workspace/PlotTools/Util/TreeReader.hpp"
#include "/afs/cern.ch/user/a/aapopov/workspace/PlotTools/Util/BinaryClassPerformance.hpp"

#include <THStack.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TColor.h>
#include <TROOT.h>
#include <TError.h>

#include <vector>
#include <list>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sys/stat.h>


using namespace std;



/// An aggregate to run BinaryClassPerformance for each variable
struct VarSeparation
{
    /// Constructor
    VarSeparation(NamedFormula const &var_, vector<double> const &edges):
        var(var_), perf(edges.begin(), edges.end())
    {}
    
    /// Constructor
    VarSeparation(NamedFormula const &var_, initializer_list<double> const &edges):
        var(var_), perf(edges)
    {}
    
    /// Associated object with TTreeFormula expression and title
    NamedFormula const &var;
    
    /// An object to evaluate the performance
    BinaryClassPerformance perf;
};


/**
 * Loops over the given files and fills AdaptiveBinning objects in the provided binnings. Only
 * nEntries entires passing the selection are read (splitted evenly among all the input files)
 */
void FillBinners(string const &filePrefix, list<string> const &fileNames, string const &treeName,
 string const &commonWeight, string const &selection, unsigned long nEntries,
 vector<AdaBinnedVariable> &vars)
{
    unsigned long const nEntriesToRead = nEntries / fileNames.size();
    
    for (auto const &fName: fileNames)
    {
        TreeReader reader(filePrefix + fName, treeName);
        reader.BookVariable(commonWeight, "weight");
        reader.BookVariable(selection, "sel");
        
        for (auto const &v: vars)
            reader.BookVariable(v.GetFormula());
        
        unsigned long const nEntriesAvailable = reader.GetEntries();
        
        for (unsigned long ev = 0, entriesPassed = 0;
         entriesPassed < nEntriesToRead and ev < nEntriesAvailable; ++ev)
        {
            reader.ReadEntry(ev);
            
            if (reader["sel"] == 0. or reader["weight"] == 0.)
                continue;
            
            for (auto &v: vars)
                v.Fill(reader[v.GetFormula()]);
            
            ++entriesPassed;
        }
    }
}


/**
 * Loops over the given files and fills BinaryClassPerformance objects in the provided separations
 */
void FillSeparations(string const &filePrefix, list<string> const &fileNames,
 string const &treeName, string const &commonWeight, string const &selection,
 vector<VarSeparation> &separations, bool isSignal)
{
    for (auto const &fName: fileNames)
    {
        TreeReader reader(filePrefix + fName, treeName);
        reader.BookVariable(commonWeight, "weight");
        reader.BookVariable(selection, "sel");
        
        for (auto const &sep: separations)
            reader.BookVariable(sep.var.GetFormula());
        
        while (reader.ReadNextEntry())
        {
            double const weight = reader["weight"];
            
            if (reader["sel"] == 0. or weight == 0.)
                continue;
            
            for (auto &sep: separations)
                sep.perf.Fill(isSignal, reader[sep.var.GetFormula()], weight);
        }
    }
}


/**
 * An aggregate to store evaluated criteria for a variable
 */
struct DiscrCriteria
{
    DiscrCriteria(string const &name_, string const &title_):
        name(name_), title(title_)
    {}
    
    bool operator<(DiscrCriteria const &rhs) const
    {
        return complArea < rhs.complArea;
    }
    
    void Print() const
    {
        cout << name << " &  $" << title << "$ &  " <<
         setprecision(3) << complArea << " &  " << setprecision(3) << ksStat << " &  " <<
         setprecision(3) << symChi2 << " &  " << setprecision(3) << jsDiv << " \\\\\n"; 
    }
    
    string name;
    string title;
    
    double complArea;
    double ksStat;
    double symChi2;
    double jsDiv;
};


void plotInputVars()
{
    // Set the desired number of bins (same for all the variables). There are some rules of thumb
    //how to do it [1], but here the value is just set manually
    //[1] http://en.wikipedia.org/wiki/Histogram#Number_of_bins_and_width
    unsigned const nBins = 15;
    //unsigned const nBins = 10;
    
    
    // Continuous variables to plot
    vector<AdaBinnedVariable> contVars;
    
    contVars.emplace_back("glb_Pt_J1", "p_{T}(j_{1})", "GeV/c");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("glb_Pt_J2", "p_{T}(j_{2})", "GeV/c");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("glb_Ht", "Total H_{T}", "GeV/c");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("glb_MET", "MET", "GeV");
    contVars.back().SetHistParams(nBins, 0.015, 0.04);
    
    contVars.emplace_back("glb_SqrtSHat", "#sqrt{#hat{s}}", "GeV/c^{2}");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("glb_Sphericity", "Sphericity");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("tHq_Mass_Top", "m(t) [tHq]", "GeV/c^{2}");
    contVars.back().SetHistParams(nBins, 0.025, 0.07);
    
    contVars.emplace_back("tHq_Pt_Top", "p_{T}(t) [tHq]", "GeV/c");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("abs(tHq_Eta_Top)", "|#eta(t)| [tHq]");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("tHq_Pt_BTop", "p_{T}(b_{t}) [tHq]", "GeV/c");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("abs(tHq_Eta_BTop)", "|#eta(b_{t})| [tHq]");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("tHq_Charge_BTop", "Q(b_{t}) #times Q(l) [tHq]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tHq_DeltaR_BTopW", "#DeltaR(b_{t},W) [tHq]");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("tHq_DEta_TopLep", "|#eta(t) - #eta(l)| [tHq]");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("tHq_Mass_BTopLep", "m(b_{t}+l) [tHq]", "GeV/c^{2}");
    contVars.back().SetHistParams(nBins, 0.025, 0.07);
    
    contVars.emplace_back("tHq_Mass_Higgs", "m(H) [tHq]", "GeV/c^{2}");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tHq_Pt_Higgs", "p_{T}(H) [tHq]", "GeV/c");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("abs(tHq_Eta_Higgs)", "|#eta(H)| [tHq]");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("tHq_DeltaR_BJetsHiggs", "#DeltaR(b_{H}, b_{H}) [tHq]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tHq_AbsCharge_Higgs", "|#SigmaQ(b_{H})| [tHq]");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("tHq_MaxEta_BHiggs", "max |#eta(b_{H})| [tHq]");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("tHq_MinPt_BHiggs", "min p_{T}(b_{H}) [tHq]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tHq_Pt_Recoil", "p_{T}(j') [tHq]", "GeV/c");
    contVars.back().SetHistParams(nBins, 0.01, 0.04);
    
    contVars.emplace_back("tHq_DeltaR_TopHiggs", "#DeltaR(t, H) [tHq]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tHq_Cos_LepRecoil_TH", "cos_{t+h}(l, j') [tHq]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tHq_Mass_TopHiggs", "m(t+H) [tHq]", "GeV/c^{2}");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tHq_RelHt", "(p_{T}(t) + p_{T}(H) + p_{T}(j')) / H_{T} [tHq]");
    contVars.back().SetHistParams(nBins, 0., 0.);
    
    
    contVars.emplace_back("tt_Mass_TopLep", "m(t_{lep}) [t#bar{t}]", "GeV/c^{2}");
    contVars.back().SetHistParams(nBins, 0.025, 0.07);
    
    contVars.emplace_back("tt_Pt_TopLep", "p_{T}(t_{lep}) [t#bar{t}]", "GeV/c");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("abs(tt_Eta_TopLep)", "|#eta(t_{lep})| [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("tt_Pt_BTopLep", "p_{T}(b_{tlep}) [t#bar{t}]", "GeV/c");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("abs(tt_Eta_BTopLep)", "|#eta(b_{tlep})| [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("tt_Charge_BTopLep", "Q(b_{tlep}) #times Q(l) [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tt_DeltaR_BTopLepWLep", "#DeltaR(b_{tlep},W_{lep}) [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0., 0.03);
    
    contVars.emplace_back("tt_Mass_BTopLepLep", "m(b_{tlep}+l) [t#bar{t}]", "GeV/c^{2}");
    contVars.back().SetHistParams(nBins, 0.025, 0.07);
    
    contVars.emplace_back("tt_Mass_TopHad", "m(t_{had}) [t#bar{t}]", "GeV/c^{2}");
    contVars.back().SetHistParams(nBins, 0.025, 0.12);
    
    contVars.emplace_back("tt_Pt_TopHad", "p_{T}(t_{had}) [t#bar{t}]", "GeV/c");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("abs(tt_Eta_TopHad)", "|#eta(t_{had})| [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tt_Pt_BTopHad", "p_{T}(b_{thad}) [t#bar{t}]", "GeV/c");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("abs(tt_Eta_BTopHad)", "|#eta(b_{thad})| [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tt_Charge_BTopHad", "Q(b_{thad}) #times Q(l) [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tt_DeltaR_BTopHadWHad", "#DeltaR(b_{thad},W_{had}) [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tt_Mass_WHad", "m(W_{had}) [t#bar{t}]", "GeV/c^{2}");
    contVars.back().SetHistParams(nBins, 0.025, 0.12);
    
    contVars.emplace_back("tt_Pt_WHad", "p_{T}(W_{had}) [t#bar{t}]", "GeV/c");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("abs(tt_Eta_WHad)", "|#eta(W_{had})| [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("tt_DeltaR_Light", "#DeltaR(q_{Whad}, q_{Whad}) [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tt_SumCharge_Light", "#SigmaQ(q_{Whad}) #times Q(l) [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("tt_DMass_TopHadWHad", "m(t_{had}) - m(W_{had}) [t#bar{t}]", "GeV/c^{2}");
    contVars.back().SetHistParams(nBins, 0.025, 0.10);
    
    contVars.emplace_back("tt_RelHt", "(p_{T}(t_{lep}) + p_{T}(t_{had})) / H_{T} [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0., 0.);
    
    contVars.emplace_back("tt_MaxMass_BTopHadLight", "max m(b_{thad}, q_{Whad}) [t#bar{t}]");
    contVars.back().SetHistParams(nBins, 0.025, 0.10);
    
    
    // Variables with manual binning
    vector<BinnedVariable> discrVars;
    
    discrVars.emplace_back("glb_Charge_Lep", "Q(l)");
    discrVars.back().SetBinning(initializer_list<double>({-1.5, -0.5, 0.5, 1.5}));
    
    discrVars.emplace_back("tHq_PassBTag_BTop", "tagged(b_{t}) [tHq]");
    discrVars.back().SetBinning(initializer_list<double>({-0.5, 0.5, 1.5}));
    
    discrVars.emplace_back("tHq_NumBTag_Higgs", "#tagged(b_{H}) [tHq]");
    discrVars.back().SetBinning(initializer_list<double>({-0.5, 0.5, 1.5, 2.5}));
    
    discrVars.emplace_back("tt_PassBTag_BTopLep", "tagged(b_{tlep}) [t#bar{t}]");
    discrVars.back().SetBinning(initializer_list<double>({-0.5, 0.5, 1.5}));
    
    discrVars.emplace_back("tt_PassBTag_BTopHad", "tagged(b_{thad}) [t#bar{t}]");
    discrVars.back().SetBinning(initializer_list<double>({-0.5, 0.5, 1.5}));
    
    discrVars.emplace_back("tt_NumPassBTag_Light", "#tagged(q_{Whad}) [t#bar{t}]");
    discrVars.back().SetBinning(initializer_list<double>({-0.5, 0.5, 1.5, 2.5}));
    
    
    vector<double> etaBins;
    
    for (double eta = 0.; eta < 2.401; eta += 0.2)
        etaBins.push_back(eta);
    
    etaBins.push_back(3.2);
    etaBins.push_back(4.7);
    
    discrVars.emplace_back("abs(tHq_Eta_Recoil)", "|#eta(j')| [tHq]");
    discrVars.back().SetBinning(etaBins.begin(), etaBins.end());
    
    discrVars.emplace_back("tt_MaxEta_Light", "max |#eta(q_{Whad})| [t#bar{t}]");
    discrVars.back().SetBinning(etaBins.begin(), etaBins.end());
    
    
    /*
    list<double> binsUnitDomain;
    
    for (int i = 0; i <= nBins; ++i)
        binsUnitDomain.push_back(-0.01 + 1.02 / nBins * i);
    
    discrVars.emplace_back("CSVBJetTop", "CSV(j_{t})");
    discrVars.back().SetBinning(binsUnitDomain.begin(), binsUnitDomain.end());
    
    discrVars.emplace_back("CSVLJet", "CSV(j')");
    discrVars.back().SetBinning(binsUnitDomain.begin(), binsUnitDomain.end());
    
    discrVars.emplace_back("MinCSVBJetsHiggs", "min CSV(j_{h})");
    discrVars.back().SetBinning(binsUnitDomain.begin(), binsUnitDomain.end());
    */
    
    
    // The common event weight and selection
    string const commonWeight("Weight");
    string const sgnWeight("1");
    string const bkgWeight("1");
    
    
    // Define and create directories to store the output
    string const outputDir("fig/");
    struct stat dirStat;
    
    if (stat(outputDir.c_str(), &dirStat) != 0)  // the directory does not exist
        mkdir(outputDir.c_str(), 0755);
    if (stat((outputDir + "png/").c_str(), &dirStat) != 0)
        mkdir((outputDir + "png/").c_str(), 0755);
    if (stat((outputDir + "pdf/").c_str(), &dirStat) != 0)
        mkdir((outputDir + "pdf/").c_str(), 0755);
    if (stat((outputDir + "C/").c_str(), &dirStat) != 0)
        mkdir((outputDir + "C/").c_str(), 0755);
    
    
    // Define the input files. For the sake of generality, files containing signal and background
    //are treated as if they are independent
    string const filePrefix("../Train/tuples/");
    list<string> const sgnFileNames = {"sgn_train.root", "sgn_exam.root"};
    list<string> const bkgFileNames = {"bkg_train.root", "bkg_exam.root"};
    string const treeName("Vars");
    
    
    // Construct binnings for the continuous variables
    cout << "Construction of adaptive binnings started.\n";
    FillBinners(filePrefix, sgnFileNames, treeName, commonWeight, sgnWeight, -1, contVars);
    cout << "Adaptive binnings have been filled with signal events.\n";
    FillBinners(filePrefix, bkgFileNames, treeName, commonWeight, bkgWeight, -1, contVars);
    cout << "Adaptive binnings have been filled with background events.\n";
    
    
    /*
    for (auto const &b: binnings)
    {
        cout << "Binning: ";
        
        for (auto &edge: b.binner.GetBinBoundaries())
            cout << edge << ", ";
        
        cout << '\n';
    }
    */
    
    // Constuct performance-evaluating objects for the continuous variables
    vector<VarSeparation> separations;
    
    for (auto const &v: contVars)
        separations.emplace_back(v, v.GetBinning());
    
    for (auto const &v: discrVars)
        separations.emplace_back(v, v.GetBinning());
    
    
    // Prepare for drawing
    gStyle->SetOptStat(0);
    gStyle->SetHistLineWidth(2);
    gStyle->SetStripDecimals(kFALSE);
    gErrorIgnoreLevel = kWarning;  // suppress info messages
    
    TCanvas canvas("canvas", "canvas", 1200, 800);
    TPad mainPad("mainPad", "", 0., 0., 0.9, 1., 0);
    mainPad.SetTicks(1, 1);
    mainPad.SetGrid();
    mainPad.Draw();
    
    
    // Fill performance-evaluating objects
    cout << "Filling of performance-evaluating objects started.\n";
    FillSeparations(filePrefix, sgnFileNames, treeName, commonWeight, sgnWeight,
     separations, true);
    cout << "Signal files have been processed.\n";
    FillSeparations(filePrefix, bkgFileNames, treeName, commonWeight, bkgWeight,
     separations, false);
    cout << "Background files have been processed.\n";
    
    
    // Draw the variables and fill discrimination criteria
    vector<DiscrCriteria> criteria;
    
    for (auto &sep: separations)
    {
        // Get and adjust the histograms
        THStack hStack("hStack",
         (sep.var.GetTitle() + ";" + sep.var.GetTitleDim() + ";Probability density").c_str());
        
        TH1D &sgnHist = sep.perf.GetSgnHist();
        sgnHist.Scale(1. / sgnHist.Integral(0, -1), "width");
        sgnHist.SetLineColor(kBlue);
        sgnHist.SetFillColor(38);
        sgnHist.SetFillStyle(1001);
        //sgnHist.SetMarkerStyle(20);
        //sgnHist.SetMarkerColor(kBlue);
        hStack.Add(&sgnHist, "hist");
        //hStack.Add(&sgnHist, "hist c");
        //hStack.Add(&sgnHist, "p0e1");
        
        TH1D &bkgHist = sep.perf.GetBkgHist();
        bkgHist.Scale(1. / bkgHist.Integral(0, -1), "width");
        bkgHist.SetLineColor(kRed);
        bkgHist.SetFillColor(kRed);
        bkgHist.SetFillStyle(3354);
        //bkgHist.SetMarkerStyle(20);
        //bkgHist.SetMarkerColor(kRed);
        hStack.Add(&bkgHist, "hist");
        //hStack.Add(&bkgHist, "hist c");
        //hStack.Add(&bkgHist, "p0e1");
        
        
        mainPad.cd();
        hStack.Draw("nostack");
        hStack.GetYaxis()->SetTitleOffset(1.5);
        
        
        // Make the legend
        TLegend legend(0.83, 0.7, 0.98, 0.85);
        legend.SetFillColor(kWhite);
        
        legend.SetHeader(" sgn vs bkg ");
        legend.AddEntry(&sgnHist, " tHq ", "f");
        legend.AddEntry(&bkgHist, " bkg ", "f");
        
        canvas.cd();
        legend.Draw();
        
        
        // Print to files
        string const outFileName(sep.var.GetEscapedName());
        canvas.Print((outputDir + "png/" + outFileName + ".png").c_str());
        canvas.Print((outputDir + "pdf/" + outFileName + ".pdf").c_str());
        canvas.Print((outputDir + "C/" + outFileName + ".C").c_str());
        
        
        // Fill discrimination criteria
        criteria.emplace_back(sep.var.GetFormula(), sep.var.GetTitle());
        criteria.back().complArea = 1. - sep.perf.GetCrossArea();
        criteria.back().ksStat = sep.perf.GetKSStatistics();
        criteria.back().symChi2 = sep.perf.GetSymmetricChi2();
        criteria.back().jsDiv = sep.perf.GetJensenShannonDivergence();
    }
    
    
    // Print discrimination criteria
    sort(criteria.rbegin(), criteria.rend());
    
    cout << '\n';
    cout << "Formula   Title   (1 - CrossArea)   KS statistis   Sym. chi2   Jensen-Shannon\n";
    cout << fixed;
    
    for (auto const &c: criteria)
        c.Print();
    cout << '\n';
    
}


int main()
{
    plotInputVars();
    
    return 0;
}
