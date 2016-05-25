////////////////////////////////////////////////////////////////////////////////
//
// th_nbteacher_x
// ------------
//
// tool to train NB neural networks - requires a valid phi-t licence
// this version has been trimmed to only work with MCs
// 
//
//                                          02/13/2014 Benedikt Maier <benedikt.maier@cern.ch>
//                                          originally based on Hauke Held
////////////////////////////////////////////////////////////////////////////////


#include "../interface/NBRoot.h"
#include "../interface/CommandLine.h"
#include "../interface/MCSample.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cassert>

#include "TFile.h"
#include "TSystem.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TEventList.h"

#include "NeuroBayesTeacher.hh"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// helper
////////////////////////////////////////////////////////////////////////////////

/// decompose variables and prepro configurations
bool decompose_nbvariables(vector<string>& vvin,
			   vector<string>& vars,vector<int>& prepros);

/// write decomposed variables into pseudocodegen file
bool write_codegen(const vector<string>& vars,const vector<int>& prepros);

/// check if a file is to be considered signal sample
/// if no signals specified returns true (assuming signal cuts later on)
bool check_signal(const string& mcname,const vector<string>& signals);

string get_cut(const vector<string>&cuts,unsigned icut,const string& mode="expression");

/// slightly awkward function to retrieve and decode the mc event weights
void setWeights(const vector<string>& mcweights,const string& name,
		vector<string>& sweights,vector<float>&  fweights);

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
int main(int argc,char**argv)
{
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;

  vector<string> data      = cl.getVector<string>  ("data",                "");
  vector<string> mcs       = cl.getVector<string>  ("mcs",                 "");
  string         dirname   = cl.getValue <string>  ("dirname",          "utm");
  string         filesuffix= cl.getValue <string>  ("filesuffix",     ".root");
  string         treename  = cl.getValue <string>  ("treename",           "t");
  string         datapath  = cl.getValue <string>  ("datapath",            "");
  string         mcpath    = cl.getValue <string>  ("mcpath",              "");
  string         opath     = cl.getValue <string>  ("opath",     "./nboutput");

  vector<string> mccuts    = cl.getVector<string>  ("mccuts",              "");
  vector<string> mcweights = cl.getVector<string>  ("mcweights",           "");

  vector<string> cuts      = cl.getVector<string>  ("cuts",                "");

  vector<string> signals   = cl.getVector<string>  ("signals",             "");
  vector<string> variables = cl.getVector<string>  ("variables",           "");

  bool           doanalysis= cl.getValue<bool>     ("doanalysis",        true);

  int            niter     = cl.getValue<int>      ("niter",              100);
  int            globalprepro= cl.getValue<int>    ("globalprepro",       612);
  float          sigweight = cl.getValue<float>    ("sigweight",          1.0);
  float          bkgweight = cl.getValue<float>    ("bkgweight",          1.0);
  float          nbevtfraction= cl.getValue<float> ("nbevtfraction",      1.0);
  int            nbweightupdate=cl.getValue<int>   ("nbweightupdate",     100);
  int            addlayer  = cl.getValue <int>     ("addlayer",             1);

  string         expertfile= cl.getValue<string>   ("expertfile","expertise.nb");

  vector<string> mclabels  = cl.getVector<string>  ("mclabels",     "");//dummy
  vector<string> mccolors  = cl.getVector<string>  ("mccolors",     "");//dummy
  vector<string> labels    = cl.getVector<string>  ("labels",       "");//dummy



  if(!cl.check()) return 0;
  cl.print();

  // you can read in data and MC files on equal footing
  // and then specify what exactly is supposed to be treated as *signal*
  // density training is not yet supported

  if (signals.size()==0) {
    cout<<"Error: No target defined!"<<endl; return 0;
  }

  // consider also individual MC weights (eg QCDFlat samples)
  
  mcweights.push_back("1"); //temporarily used instead of mcweight (see below) since frameworkswitcher is not ready for it
  
  //mcweights.push_back("mcweight");



  // move all output to a NB working directory
  string cmdmkdir = "mkdir -p " + opath;
  gSystem->Exec(cmdmkdir.c_str());
  string execdir = gSystem->WorkingDirectory();
  if (!gSystem->ChangeDirectory(opath.c_str())) {
    cout<<"Could not open opath: "<<opath<<endl; return 0;
  }

  //
  // OPEN MC FILES / RETRIEVE TREES, WEIGHTS, AND SCALES 
  //
  vector<MCSample> mcsamples=
    MCSample::loadSamples(mcs,dirname,treename,mcpath,filesuffix);
  if (mcsamples.size()>0) {
    cout<<endl; MCSample::printSampleInfos(mcsamples); cout<<endl;
  }

  cout<<"======================"<<endl
      <<"Start NeuroBayes Setup"<<endl
      <<"======================"<<endl
      <<endl<<endl;

  cout<<"All outputs are written to: "<<gSystem->WorkingDirectory()<<endl;

  vector<string> vars;
  vector<int> prepros;

  if (!decompose_nbvariables(variables,vars,prepros)) {
    cout<<"Error: decomposition of nb variables faulty"<<endl; return 0;
  }

  write_codegen(vars,prepros);

  //  if (!write_codegen(vars,prepros)) {
  //    cout<<"Error: could not write pseudocodegen file"<<endl; return 0;
  //  }

  const unsigned nvar = vars.size();

  //
  //create NeuroBayes Teacher instance
  //

  NeuroBayesTeacher* nb = NeuroBayesTeacher::Instance();

  nb->NB_DEF_NODE1((int)(nvar+1));   // nodes in input layer
  nb->NB_DEF_NODE2((int)(nvar+addlayer));   // nodes in hidden layer
  nb->NB_DEF_NODE3(1);       	     // nodes in output layer

  nb->NB_DEF_TASK("CLASSIFICATION"); // binominal classification (default)

  nb->NB_DEF_PRE(globalprepro);
  nb->NB_DEF_REG("REG");             // 'OFF','REG' (def) ,'ARD','ASR','ALL' 
  nb->NB_DEF_SHAPE("DIAG");          // 'OFF', 'INCL', 'TOTL'
  nb->NB_DEF_LOSS("ENTROPY");        // 'ENTROPY'(def),'QUADRATIC'
  //nb->NB_DEF_LEARNDIAG( 1 );	     // BFGS

  nb->NB_DEF_RTRAIN(nbevtfraction);  // use all events for training
  nb->NB_DEF_EPOCH(nbweightupdate);  // weight update after n events

  //nb->NB_DEF_SPEED(2.0);           // multiplicative factor to enhance 
                                     //global learning speed
  //nb->NB_DEF_MAXLEARN(1.0);        // multiplicative factor to limit 
                                     // the global learning speed 
  //in any direction, this number should be smaller than NB_DEF_SPEED

  if (0==niter) {
    nb->NB_DEF_ITER(0);              // number of training iteration
    nb->NB_DEF_SHAPE("DIA2");        // 'OFF', 'INCL', 'TOTL'
    //    nb->NB_DEF_PRE(322);
  }
  else {
    nb->NB_DEF_ITER(niter);            // number of training iteration
    nb->NB_DEF_METHOD("BFGS");	       // bricht automatisch ab, wenn austrainiert
    //    nb->NB_DEF_PRE(312);
  }

  nb->SetOutputFile(expertfile.c_str());

  int irandom= 4701;  
  int jrandom= 21;  	 
  nb->NB_RANVIN(irandom,jrandom,2);  

  // set prepros flags for the input variables
  for (unsigned ivar=0;ivar<nvar;ivar++) 
    nb->SetIndividualPreproFlag(ivar,prepros[ivar],vars[ivar].c_str());

  // setup counters for training statistics

  unsigned isig(0);
  unsigned ibkg(0);

  float    fsig(0.0);
  float    fbkg(0.0);
  
  //
  // DEFINE SELECTION
  //

  string selection = "";
  for (unsigned icuts=0;icuts<mccuts.size();icuts++) {
    if (!selection.empty()) selection += "&&";
    selection += get_cut(mccuts,icuts,"expression");
  }
  for (unsigned icuts=0;icuts<cuts.size();icuts++) {
    if (!selection.empty()) selection += "&&";
    selection += get_cut(cuts,icuts,"expression");
  }

  //
  // LOOP AND FEED
  //

  cout<<"Loading Samples:"<<endl;
  for (unsigned imc=0;imc<mcsamples.size();imc++) {

    MCSample mcsample  = mcsamples[imc];
    mcsample.setWeights(mcweights);

    const float target = (check_signal(mcsample.name(),signals)) ? 1.0 : -1.0;

    for (unsigned itree=0;itree<mcsample.ntrees();itree++) {

      cout<<mcsample.filename(itree)<<endl;

      // retrieve tree
      TTree* tree = mcsample.tree(itree);

      // define weights

      vector<string> sweights;
      vector<float>  fweights;

      setWeights(mcweights,mcsample.name(),sweights,fweights);

      double sampleweight(1.0); // sampleweight is event independant!!!
      MCSample::getWeight(tree,sampleweight);
      for (unsigned i=0;i<fweights.size();i++) sampleweight *= fweights[i];

      // make selection

      TEventList* elsig = new TEventList("elsig","elsig");
      tree->Draw(">>elsig",selection.c_str());
 
      if (target>0.) cout<<"SIG:";
      else           cout<<"BKG:";
      cout<<mcsample.name()<<":"
	  <<mcsample.filename(itree)<<":"
	  <<elsig->GetN()<<" (selected) ... "<<flush;

      // setup interface to read out tree

      const unsigned nweights = sweights.size();

      // 1. interface for variables
      NBRoot root2nb(tree,vars);
      float* nbinput = new float[nvar];
      // 2. interface for evt weights - need to be of type var1[var2[1]] or so...
      NBRoot root2ww(tree,sweights);
      float* nbevtw  = new float[nweights];

      vector<string> helper; helper.push_back("evt");
      NBRoot root2evt(tree,helper);
      float* nbevt   = new float[1];


      const unsigned nevts    = elsig->GetN();
      for (unsigned ievt=0;ievt<nevts;ievt++) {
	const unsigned ientry = elsig->GetEntry(ievt);

	// set entry for variable array
	root2nb.setArray(ientry,nbinput);

	// set entry for evtweight array
	root2ww.setArray(ientry,nbevtw);

	root2evt.setArray(ientry,nbevt);

	int evt = (int) nbevt[0];

	/*
	cout<<"ievt: "<<ievt;
	for (unsigned i=0;i<nvar;i++) cout<<"input "<<i<<": "<<nbinput[i];
	cout<<endl;
	*/


	// calculate the actual weight as weight = w_sample * w_evt * w_sig/bkg
	double weight = sampleweight;
	for (unsigned i=0;i<nweights;i++) weight *= nbevtw[i];
	weight *= (target>0.0) ? sigweight : bkgweight;
	nb->SetWeight(weight);
	nb->SetTarget(target);


	/*if(ievt<10)
	  {
	    cout<<"evt: "<<evt<<endl;
	    //cout <<evt <<":"<< endl;
	    for(int i=0;i<nvar;i++)
	      cout <<  "input " <<i<< " = " <<nbinput[i]<<endl;
	    cout <<"target: " << target<<endl;
	  }
	*/
	  
	nb->SetNextInput(nvar,nbinput);
	if (target>0) {isig++; fsig += weight;}
	else          {ibkg++; fbkg += weight;}
      } //ievt<entries in event list
      delete nbinput;
      delete elsig;
      cout<<"done."<<endl;
    } //itree<ntrees
  } //imc<mcsamples

  //
  // PERFORM THE TRAINING
  //
  
  cout<<"***NB Input status:"<<endl
      <<"* NSig      : "<<isig<<endl
      <<"* NBkg      : "<<ibkg<<endl
      <<"* FSig      : "<<fsig<<endl
      <<"* FBkg      : "<<fbkg<<endl
      <<"* FSIG/FTOT : "<<fsig/(fsig+fbkg)<<endl
      <<"***"<<endl;
  

  //perform training
  cout<<"->To see NeuroBayes output have a look at \"nb_teacher.log\""<<endl;
  int original = dup(fileno(stdout));
  fflush(stdout);
  freopen("nb_teacher.log", "w", stdout);
  
  nb->TrainNet();
  
  fflush(stdout);
  dup2(original, fileno(stdout));
  close(original);

  if (doanalysis) {
    cout<<"Evaluating the training"<<endl;
    //    string cmd = "root -l "+execdir+
    //  "/macros/analysis.C'(\""+gSystem->WorkingDirectory()+ "/ahist.txt\")'";

    nb->nb_correl_signi("./correl_signi.txt","./correl_signi.html");

    string cmdCreateHistos = 
      "root -b ${STANDALONE_DIR}/THAnalysis/macros/asciiToRoot.C";
    gSystem->Exec(cmdCreateHistos.c_str());

    string cmdCreateAnalysis = 
      "root -b ${STANDALONE_DIR}/THAnalysis/macros/analysis.C";

    cout<<"Starting network analysis... "<<endl;
    gSystem->Exec(cmdCreateAnalysis.c_str());
    cout<<"Summary available in: "<<endl;
    gSystem->Exec("echo \"$(pwd)/analysis.ps\"");
  }

  cout<<"DONE."<<endl;

  gSystem->ChangeDirectory(execdir.c_str());
  return 0;
}





////////////////////////////////////////////////////////////////////////////////
// help implementations
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
bool check_signal(const string& mcname,const vector<string>& signals)
{
  for (unsigned i=0;i<signals.size();i++) 
    if (signals[i]==mcname) return true;
  return false;
}


//______________________________________________________________________________
bool decompose_nbvariables(vector<string>& vvin,
			   vector<string>& vars,vector<int>& prepros)
{
  if (0==vvin.size()) return false;

  for (unsigned itin=0;itin<vvin.size();itin++) {

    string tmp(vvin[itin]);
    size_t pos = tmp.find(":");
    string varname = tmp.substr(0,pos);
    vars.push_back(varname);
    if (pos==string::npos) {
      prepros.push_back(34);
      continue;
    }
    
    tmp = tmp.substr(pos+1);

    if (tmp.find_first_not_of("0123456789")!=string::npos) return false;
    stringstream sstmp; sstmp<<tmp; int iprepro; sstmp>>iprepro;
    prepros.push_back(iprepro);

  }
  return true;
}



//______________________________________________________________________________
bool write_codegen(const vector<string>& vars,const vector<int>& prepros)
{
  
  if (vars.size()!=prepros.size()) return false;
  if (vars.size()==0) return false;

  stringstream ss;
  ss<<"VARSET 1"<<endl;
  for (unsigned i=0;i<vars.size();i++) ss<<vars[i]<<" "<<prepros[i]<<endl;
  ss<<"ENDVARSET"<<endl
    <<"NETWORK classify 1"<<endl;

  ofstream file;
  file.open("pseudocodegen",ofstream::trunc);
  if (!file.is_open()) return false;

  file<<ss.str().c_str();
  file.close();

  cout<<"Created NB file: pseudocodegen"<<endl;

  return true;
}

//______________________________________________________________________________
string get_cut(const vector<string>& cuts,unsigned icut,const string& mode)
{
  if (icut>=cuts.size()) return string("");
  string tmp(cuts[icut]);
  size_t pos;
  pos = tmp.find(':');
  if (pos==string::npos) return tmp;
  string cutlabel = tmp.substr(0,pos);
  string cutexpression= tmp.substr(pos+1);
  return (mode.find("label")==string::npos) ? cutexpression : cutlabel;
}

//______________________________________________________________________________
void setWeights(const vector<string>& mcweights,
		const string& name,
		vector<string>& sweights,
		vector<float>&  fweights)
{
  for (vector<string>::const_iterator it=mcweights.begin();it!=mcweights.end();++it) {
    string weight(*it);
    size_t pos = weight.find(":");
    if (pos==string::npos) {
      float ff; stringstream ss; ss<<weight;
      if (ss>>ff) {assert(!isnan(ff));fweights.push_back(ff);}
      else        {sweights.push_back(weight);}
    }
    else if (weight.substr(0,pos)==name) {
      weight = weight.substr(pos+1);
      float ff; stringstream ss; ss<<weight;
      if (ss>>ff) {assert(!isnan(ff));fweights.push_back(ff);}
      else        {sweights.push_back(weight);}
    }
  }
  return;
}
