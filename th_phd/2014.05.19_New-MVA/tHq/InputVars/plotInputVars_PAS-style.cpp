/**
 * \author Andrey Popov
 * 
 * The script plots distributions over input variables normalised to unit area. The binning is
 * adapted in such a way that every bin contains similar number of events taken from 1:1 signal and
 * background mixture. Several criteria of similarity between signal and background PDFs are
 * evaluated. The code can be compiled with
 *   g++ -o plotInputVars_PAS-style plotInputVars_PAS-style.cpp --std=c++11 `root-config --cflags --libs` -lTreePlayer -I$BOOST_ROOT/include/boost-1_55/ -O2
 */

#include "/afs/cern.ch/user/a/aapopov/workspace/tHq/2012Alpha/2013.08.23_Full-analysis-CSVM/Step1_MVA-reco/plotInputVars/NamedFormula.hpp"
#include "/afs/cern.ch/user/a/aapopov/workspace/tHq/2012Alpha/2013.08.23_Full-analysis-CSVM/Step1_MVA-reco/plotInputVars/BinnedVariable.hpp"
#include "/afs/cern.ch/user/a/aapopov/workspace/PlotTools/Util/TreeReader.hpp"
#include "/afs/cern.ch/user/a/aapopov/workspace/PlotTools/Util/BinaryClassPerformance.hpp"
#include "/afs/cern.ch/user/a/aapopov/workspace/tHq/2012Bravo/2014.05.19_New-MVA/tHq/InputVars/convUserNDC.hpp"

#include <THStack.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TGaxis.h>
#include <TLegend.h>
#include <TLatex.h>
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
    // Set style to resemble TDR
    gStyle->SetOptStat(0);
    gStyle->SetStripDecimals(false);
    gStyle->SetErrorX(0.);
    TGaxis::SetMaxDigits(3);

    gStyle->SetTitleFont(42);
    gStyle->SetTitleFontSize(0.04);  // was 0.05
    gStyle->SetTitleFont(42, "XYZ");
    gStyle->SetTitleXOffset(0.9);
    gStyle->SetTitleYOffset(1.25);
    gStyle->SetTitleSize(0.05, "XYZ");  // was 0.06
    gStyle->SetLabelFont(42, "XYZ");
    gStyle->SetLabelOffset(0.007, "XYZ");
    gStyle->SetLabelSize(0.04, "XYZ");  // was 0.05
    gStyle->SetNdivisions(508, "XYZ");
    gStyle->SetLegendFont(42);

    gStyle->SetPadTopMargin(0.13);
    gStyle->SetPadBottomMargin(0.13);
    gStyle->SetPadLeftMargin(0.16);
    gStyle->SetPadRightMargin(0.05);
    
    
    // Set the desired number of bins (same for all the variables). There are some rules of thumb
    //how to do it [1], but here the value is just set manually
    //[1] http://en.wikipedia.org/wiki/Histogram#Number_of_bins_and_width
    unsigned const nBins = 15;
    //unsigned const nBins = 10;
    
    
    // Continuous variables to plot
    vector<AdaBinnedVariable> contVars;
    
    contVars.emplace_back("Mass_Higgs", "m(H)", "GeV");
    contVars.back().SetHistParams(nBins, 0.02, 0.07);
    
    contVars.emplace_back("Pt_Higgs", "p_{T}(H)", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("abs(Eta_Higgs)", "|#eta(H)|");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("DeltaR_BJetsHiggs", "#DeltaR(b_{H},b_{H})");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("SumCharge_Higgs", "|#SigmaQ(b_{H})|");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("MinPt_BHiggs", "min p_{T}(b_{H})", "GeV");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("MaxEta_BHiggs", "max |#eta(b_{H})|");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    
    contVars.emplace_back("Mass_Top", "m(t)", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.07);
    
    contVars.emplace_back("Pt_Top", "p_{T}(t)", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("abs(Eta_Top)", "|#eta(t)|");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("Pt_BTop", "p_{T}(b_{t})", "GeV");
    contVars.back().SetHistParams(nBins, 0., 0.025);
    
    contVars.emplace_back("abs(Eta_BTop)", "|#eta(b_{t})|");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("Charge_BTop", "Q(b_{t}) #times Q(l)");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("Mass_BTopLep", "m(b_{t}+l)", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.07);
    
    contVars.emplace_back("DeltaR_BTopW", "#DeltaR(b_{t},W)");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("DEta_TopLep", "|#eta(t) - #eta(l)|");
    contVars.back().SetHistParams(nBins, 0., 0.05);
    
    contVars.emplace_back("Cos_LepBTop_W", "cos #theta_{W}(l, b_{t})");
    contVars.back().SetHistParams(nBins, 0., 0.);
    
    
    contVars.emplace_back("Pt_Recoil", "p_{T}(j')", "GeV");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    
    contVars.emplace_back("RelHt", "(p_{T}(H) + p_{T}(t) + p_{T}(j')) / H_{T}");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("DeltaR_TopHiggs", "#DeltaR(t,H)");
    contVars.back().SetHistParams(nBins, 0.025, 0.025);
    
    contVars.emplace_back("Cos_LepRecoil_TH", "cos #theta_{t+H}(l, j')");
    contVars.back().SetHistParams(nBins, 0., 0.);
    
    
    // Variables with manual binning
    vector<BinnedVariable> discrVars;
    
    discrVars.emplace_back("NumBTag_Higgs", "#tagged(b_{H})");
    discrVars.back().SetBinning(initializer_list<double>({-0.5, 0.5, 1.5, 2.5}));
    
    discrVars.emplace_back("PassBTag_BTop", "tagged(b_{t})");
    discrVars.back().SetBinning(initializer_list<double>({-0.5, 0.5, 1.5}));
    
    discrVars.emplace_back("Charge_Recoil", "Q(j')");
    discrVars.back().SetBinning(nBins, -1.125, 1.125);
    
    
    vector<double> etaBins;
    
    for (double eta = 0.; eta < 2.401; eta += 0.2)
        etaBins.push_back(eta);
    
    etaBins.push_back(3.2);
    etaBins.push_back(4.7);
    
    discrVars.emplace_back("abs(Eta_Recoil)", "|#eta(j')|");
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
    string const sgnWeight("IsSignal == 1");
    string const bkgWeight("IsSignal == 0");
    
    
    // Define and create directories to store the output
    string const outputDir("fig_PAS-style/");
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
    list<string> const sgnFileNames = {"mu/thq-nc-mg.root", "e/thq-nc-mg.root"};
    list<string> const bkgFileNames = {"mu/thq-nc-mg.root", "e/thq-nc-mg.root"};
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
    gStyle->SetHistLineWidth(2);
    gErrorIgnoreLevel = kWarning;  // suppress info messages
    
    TCanvas canvas("canvas", "canvas", 1200, 1000);
    canvas.SetTicks();
    
    // TPad mainPad("mainPad", "", 0., 0., 0.9, 1., 0);
    // mainPad.SetTicks(1, 1);
    // mainPad.SetGrid();
    // mainPad.Draw();
    
    // Change position of the common factor for the y axis
    TGaxis::SetExponentOffset(-0.06, 0.02, "y");
    
    
    // CMS label
    TLatex cmsLabel(0.20, 0.89, "#scale[1.2]{#font[62]{CMS}} #font[52]{Simulation Preliminary}");
    cmsLabel.SetNDC();
    cmsLabel.SetTextFont(42);
    cmsLabel.SetTextSize(0.05);  // was 0.06
    cmsLabel.SetTextAlign(11);

    TLatex energyLabel(0.95, 0.89, "8 TeV");
    energyLabel.SetNDC();
    energyLabel.SetTextFont(42);
    energyLabel.SetTextSize(0.05);  // was 0.06
    energyLabel.SetTextAlign(31);
    
    
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
         (";" + sep.var.GetTitleDim('[') + ";Probability density [a. u.]").c_str());
        
        TH1D &sgnHist = sep.perf.GetSgnHist();
        sgnHist.Scale(1. / sgnHist.Integral(0, -1), "width");
        sgnHist.SetLineColor(kBlue);
        sgnHist.SetFillColor(38);
        sgnHist.SetFillStyle(1001);
        hStack.Add(&sgnHist, "hist");
        
        TH1D &bkgHist = sep.perf.GetBkgHist();
        bkgHist.Scale(1. / bkgHist.Integral(0, -1), "width");
        bkgHist.SetLineColor(kRed);
        bkgHist.SetFillColor(kRed);
        bkgHist.SetFillStyle(3354);
        hStack.Add(&bkgHist, "hist");
        
        
        // mainPad.cd();
        hStack.Draw("nostack");
        hStack.SetMinimum(0.);
        // hStack.GetYaxis()->SetTitleOffset(1.5);
        
        canvas.Update();  // needed to convert coordinates below
        
        
        // Find a suitable place for the legend
        double const legendWidth = 0.19;
        double const legendHeight = 0.17;
        double const legendInnerMargin = 0.03;  // distance between legend and frame
        
        bool legendRightSide = true;  // legend can be placed in upper left or right corner
        double yRescaleFactor = 1.;
        //^ It might be needed to reset maximum of hStack to give some place for the legend
        
        // First try the right corner
        double xEdgeSpace = undc::UtoX(canvas, 1. - canvas.GetRightMargin() - legendWidth -
         2 * legendInnerMargin);  // left edge of the region to host the legend
        double const yEdgeSpace = undc::VtoY(canvas, 1. - canvas.GetTopMargin() - legendHeight -
         2 * legendInnerMargin);
        double maxY = 0.;  // will be maximal y of a histogram bin that gets into the region
        
        for (auto const &h: {&sgnHist, &bkgHist})
            for (int bin = h->FindFixBin(xEdgeSpace); bin <= h->GetNbinsX(); ++bin)
            {
                double const y = h->GetBinContent(bin);
                
                if (y > maxY)
                    maxY = y;
            }
        
        if (maxY > yEdgeSpace)
        //^ If not, the legend can be hosted at the right corner without a problem
        {
            yRescaleFactor = maxY / yEdgeSpace;  // assuming hStack.GetMiminum() == 0.
            
            // Since would need to reset the maximum to place the legend in the right corner, check
            //the left corner. Maybe, there is enough place there
            xEdgeSpace = undc::UtoX(canvas, canvas.GetLeftMargin() + legendWidth +
             2 * legendInnerMargin);
            maxY = 0.;
            
            for (auto const &h: {&sgnHist, &bkgHist})
            {
                int maxBin = h->FindFixBin(xEdgeSpace);
                
                for (int bin = 1; bin <= maxBin; ++bin)
                {
                    double const y = h->GetBinContent(bin);
                    
                    if (y > maxY)
                        maxY = y;
                }
            }
            
            
            // Check if the left corner is better
            double const proposedRescaleFactor = max(maxY / yEdgeSpace, 1.);
            
            if (proposedRescaleFactor < yRescaleFactor)
            {
                legendRightSide = false;
                yRescaleFactor = proposedRescaleFactor;
            }
        }
        
        
        // Reset the maximum to host the legend. There is also an additional margin between the
        //histogram and the upper axis of the pad, which is accounted for. The statement below
        //exploits the fact that the histogram miminum is set to zero
        hStack.SetMaximum(hStack.GetMaximum("nostack") * (1. + gStyle->GetHistTopMargin()) *
         yRescaleFactor);
        
        
        // Finally, create the legend
        double const legendX = ((legendRightSide) ?
         1. - canvas.GetRightMargin() - legendWidth - legendInnerMargin :
         canvas.GetLeftMargin() + legendInnerMargin);
        
        TLegend legend(legendX, 1. - canvas.GetTopMargin() - legendInnerMargin - legendHeight,
         legendX + legendWidth, 1. - canvas.GetTopMargin() - legendInnerMargin);
        legend.SetFillColor(kWhite);
        legend.SetTextSize(0.04);
        
        legend.SetHeader("Interpretation");
        legend.AddEntry(&sgnHist, " correct ", "f");
        legend.AddEntry(&bkgHist, " wrong ", "f");
        
        // canvas.cd();
        
        cmsLabel.Draw();
        energyLabel.Draw();
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
