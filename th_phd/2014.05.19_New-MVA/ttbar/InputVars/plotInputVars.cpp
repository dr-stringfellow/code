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
    
    contVars.emplace_back("Mass_TopLep", "m(t_{lep})", "GeV/c^{2}");
    contVars.back().SetHistParams(nBins, 0.025, 0.07);
    
    contVars.emplace_back("Pt_TopLep", "p_{T}(t_{lep})", "GeV/c");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("abs(Eta_TopLep)", "|#eta(t_{lep})|");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("Pt_BTopLep", "p_{T}(b_{tlep})", "GeV/c");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("abs(Eta_BTopLep)", "|#eta(b_{tlep})|");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("Charge_BTopLep", "Q(b_{tlep}) #times Q(l)");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("Mass_BTopLepLep", "m(b_{tlep}+l)", "GeV/c^{2}");
    contVars.back().SetHistParams(nBins, 0.025, 0.07);
    
    contVars.emplace_back("DeltaR_BTopLepWLep", "#DeltaR(b_{tlep},W_{lep})");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("DEta_TopLepLep", "|#eta(t_{lep}) - #eta(l)|");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("Cos_LepBTopLep_WLep", "cos#theta_{Wlep}(l, b_{tlep})");
    contVars.back().SetHistParams(nBins, 0., 0.);
    
    
    contVars.emplace_back("Mass_TopHad", "m(t_{had})", "GeV/c^{2}");
    contVars.back().SetHistParams(nBins, 0.025, 0.07);
    
    contVars.emplace_back("Pt_TopHad", "p_{T}(t_{had})", "GeV/c");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("abs(Eta_TopHad)", "|#eta(t_{had})|");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("Mass_WHad", "m(W_{had})", "GeV/c^{2}");
    contVars.back().SetHistParams(nBins, 0.025, 0.07);
    
    contVars.emplace_back("Pt_WHad", "p_{T}(W_{had})", "GeV/c");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("abs(Eta_WHad)", "|#eta(W_{had})|");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("Mass_TopHad - Mass_WHad", "m(t_{had}) - m(W_{had})", "GeV/c^{2}");
    contVars.back().SetHistParams(nBins, 0.02, 0.07);
    
    contVars.emplace_back("DeltaR_BTopHadWHad", "#DeltaR(b_{thad},W_{had})");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("Pt_BTopHad", "p_{T}(b_{thad})", "GeV/c");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("abs(Eta_BTopHad)", "|#eta(b_{thad})|");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("Charge_BTopHad", "Q(b_{thad})#times Q(l)");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("MinPt_Light", "min p_{T}(q_{thad})", "GeV/c");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("MaxEta_Light", "max |#eta(q_{thad})|");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("SumCharge_Light", "#SigmaQ(q_{thad}) #times Q(l)");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("DeltaR_Light", "#DeltaR(q1_{thad},q2_{thad})");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("MaxMass_BTopHadLight", "max m(b_{thad}+q_{thad})", "GeV/c^{2}");
    contVars.back().SetHistParams(nBins, 0.025, 0.07);
    
    
    contVars.emplace_back("Charge_BTopHad - Charge_BTopLep", "(Q(b_{thad}) - Q(b_{tlep}))#times Q(l)");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("RelHt", "(p_{T}(t_{lep}) + p_{T}(t_{had})) / H_{T}");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("Mass_TopLepTopHad", "m(t_{lep} + t_{had})", "GeV/c^{2}");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("Pt_TopLepTopHad", "p_{T}(t_{lep} + t_{had})", "GeV/c");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("abs(Eta_TopLepTopHad)", "|#eta(t_{lep} + t_{had})|");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("DeltaR_TopLepTopHad", "#DeltaR(t_{lep}, t_{had})");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    
    // Discrete variables to plot
    vector<BinnedVariable> discrVars;
    
    // discrVars.emplace_back("PassBTag_BTopLep", "tagged(b_{tlep})");
    // discrVars.back().SetBinning(initializer_list<double>({-0.5, 0.5, 1.5}));
    
    // discrVars.emplace_back("PassBTag_BTopHad", "tagged(b_{thad})");
    // discrVars.back().SetBinning(initializer_list<double>({-0.5, 0.5, 1.5}));
    
    discrVars.emplace_back("NumBTag_Light", "#tagged(q_{thad})");
    discrVars.back().SetBinning(initializer_list<double>({-0.5, 0.5, 1.5, 2.5}));
    
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
    string const sgnWeight("IsSignal == 1");
    string const bkgWeight("IsSignal == 0");
    
    
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
    string const filePrefix("tuples/");
    list<string> const sgnFileNames = {"mu/ttbar-semilep-mg-p1_53X.02.01_VNz_p1.root",
     "mu/ttbar-semilep-mg-p1_53X.02.01_VNz_p2.root", "mu/ttbar-semilep-mg-p1_53X.02.01_VNz_p3.root",
     "mu/ttbar-semilep-mg-p1_53X.02.01_VNz_p4.root", "mu/ttbar-semilep-mg-p2_53X.02.01_Ptw_p1.root",
     "mu/ttbar-semilep-mg-p2_53X.02.01_Ptw_p2.root", "mu/ttbar-semilep-mg-p2_53X.02.01_Ptw_p3.root",
     "mu/ttbar-semilep-mg-p2_53X.02.01_Ptw_p4.root", "mu/ttbar-semilep-mg-p2_53X.02.01_Ptw_p5.root",
     "mu/ttbar-semilep-mg-p3_53X.02.01_MXn_p1.root", "mu/ttbar-semilep-mg-p3_53X.02.01_MXn_p2.root",
     "mu/ttbar-semilep-mg-p3_53X.02.01_MXn_p3.root", "mu/ttbar-semilep-mg-p3_53X.02.01_MXn_p4.root",
     "mu/ttbar-semilep-mg-p3_53X.02.01_MXn_p5.root",
     "e/ttbar-semilep-mg-p1_53X.02.01_VNz_p1.root",
     "e/ttbar-semilep-mg-p1_53X.02.01_VNz_p2.root", "e/ttbar-semilep-mg-p1_53X.02.01_VNz_p3.root",
     "e/ttbar-semilep-mg-p1_53X.02.01_VNz_p4.root", "e/ttbar-semilep-mg-p2_53X.02.01_Ptw_p1.root",
     "e/ttbar-semilep-mg-p2_53X.02.01_Ptw_p2.root", "e/ttbar-semilep-mg-p2_53X.02.01_Ptw_p3.root",
     "e/ttbar-semilep-mg-p2_53X.02.01_Ptw_p4.root", "e/ttbar-semilep-mg-p2_53X.02.01_Ptw_p5.root",
     "e/ttbar-semilep-mg-p3_53X.02.01_MXn_p1.root", "e/ttbar-semilep-mg-p3_53X.02.01_MXn_p2.root",
     "e/ttbar-semilep-mg-p3_53X.02.01_MXn_p3.root", "e/ttbar-semilep-mg-p3_53X.02.01_MXn_p4.root",
     "e/ttbar-semilep-mg-p3_53X.02.01_MXn_p5.root"};
    list<string> const bkgFileNames(sgnFileNames);
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
        
        legend.SetHeader(" t#bar{t} reco ");
        legend.AddEntry(&sgnHist, " correct comb. ", "f");
        legend.AddEntry(&bkgHist, " wrong comb. ", "f");
        
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
