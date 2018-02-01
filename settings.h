// Purpose: Analysis settings for jet physics package
// Author:  mikko.voutilainen@cern.ch
// Co-author: hannu.siikonen@cern.ch
// Created: June 1, 2015

// All the settings variables are in global name space
// To avoid conflicts, and to make them easier to find
// global variables all start with _jp_ (for JetPhysics)

#ifndef __settings_h__
#define __settings_h__
#include <map>
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <algorithm>
#include <cassert>

using std::map;
using std::vector;
using std::string;
using std::cout;
using std::endl;


constexpr bool strings_equal(char const * a, char const * b) {
  return *a == *b and (*a == '\0' or strings_equal(a + 1, b + 1));
}


// Debugging info
constexpr bool _jp_debug = false;
// Will assert be used in the programs?
//#define USEASSERT // Expert option: comment this if certain that no problems occur. More than 1/3 off the run time.

//{ BEGIN fundamental file settings
// Do we use CHS jets? ("CHS" for yes, "", for legacy/no)
const constexpr char _jp_chs[] = "CHS";
// Algorithm to use ("AK4PF" or "AK8PF" + "chs" for chs jets)
const constexpr char _jp_algo[] = "AK4PFchs";
// Data type ("DATA", "MC", or "HW")
const constexpr char _jp_type[] = "DATA";
// In case of DATA, choose run ("RunB/C/D/E/Fearly/Flate/G/H")
const constexpr char _jp_run[] = "RunG16";
// Simple helper
constexpr bool _jp_isdt = strings_equal(_jp_type,"DATA");
constexpr bool _jp_ismc = !_jp_isdt;
constexpr bool _jp_ispy = strings_equal(_jp_type,"MC");
constexpr bool _jp_ishw = strings_equal(_jp_type,"HW");
//} END fundamental settings


//{ BEGIN run settings
// Number of events to process (-1 for all)
constexpr Long64_t _jp_nentries =
-1; // all
//10; // debug
//10000; // shorter test run
//100000; // short test run
//1000000; // shortish test run
// Number of events to skip from the beginning (for debugging)
constexpr Long64_t _jp_nskip = 0;
// Step between events
constexpr Long64_t _jp_skim = 0; // "prescale", off if zero
// Only load selected branches (large speedup, but be careful!)
constexpr bool _jp_quick = true;
// Save the infos at times. Setting this to true might help with long runs. (Not recommended
constexpr bool _jp_save = false;
//} END run settings


//{ BEGIN A group of constexpr switches and their settings to turn some parts of the code on/off
// If the output files grow too much, processing becomes more difficult.

// Produce run-level histograms
constexpr bool _jp_doRunHistos = false; // Not needed very often
// Produce basic set of histograms
constexpr bool _jp_doBasicHistos = true;
// Produce full-eta TH3 histograms
constexpr bool _jp_doEtaHistos = true;
// Special reco/gen histos in mc
constexpr bool _jp_doEtaHistosMcResponse = false; // Special studies
// Toggle 3D asymmetry and mpf histos off, if too much space is consumed
constexpr bool _jp_do3dHistos = true;

// QGL studies by Ozlem (use an external file for qgl studies)
constexpr bool _jp_doqglfile = false;
const constexpr char _jp_qglfile[] = "output-DATA_RunGfullpart-1.root";

// At some point of time Kostas stored UNCORRECTED four-vector. Current status: CORRECTED
// NOTE: this is a source of constant anxiety, should be rechecked from time to time
constexpr bool _jp_undojes = true;
// We can choose also not to apply the new jes onto a four-vector
constexpr bool _jp_redojes = true;
// For debugging
constexpr bool _jp_skipl2l3res = false;

// Reapply json selection based on the latest one (check lumicalc if false!).
constexpr bool _jp_dojson = true;
const constexpr char _jp_json[] = "lumicalc/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt";
//const constexpr char _jp_json[] = "lumicalc/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt";

// Calculate luminosity on the fly based on .csv file and take only events with non-zero luminosity.
constexpr bool _jp_dolumi = true;
const constexpr char _jp_lumifile[] = "lumicalc/brilcalc_lumibyls16.csv";
//const constexpr char _jp_lumifile[] = "lumicalc/brilcalc_lumibyls17.csv";

// Add prescale information from a file
constexpr bool _jp_doprescale = false; // CAUTION, this shouldn't be nowadays necessary
const constexpr char _jp_prescalefile[] = "pileup/prescales74x.txt";

constexpr bool _jp_dotrpufile = false; // CAUTION, the histo file in the repo is 2015
const constexpr char _jp_trpufile[] = "pileup/MUperLSvsRUN_MB.root";

// Decide whether or not to simulate triggers from MC (this is slow)
constexpr bool _jp_domctrigsim = true;
// Use "mc" trigger for whole pT range instead of stiching triggers together in histosCombine (false requires trigsim)
constexpr bool _jp_usemctrig = false;
// This is the 13 TeV 25 ns list of triggers (Run2016BCDEFG)
// Check the recommended settings from https://twiki.cern.ch/twiki/bin/view/CMS/InclusiveJetsLegacy
constexpr int _jp_ntrigs = 9; // CAUTION: In 2016 jt450 is unprescaled but in 2017 it is not (jt500!)
const constexpr char* _jp_triggers[_jp_ntrigs] =
{"jt40",    "jt60",    "jt80",   "jt140",   "jt200",   "jt260",   "jt320",   "jt400",  "jt450"}; // "jt500"};
// reference trigger for PU profile in the mc folder and for trigger lumi weighting
const constexpr char _jp_reftrig[] = "jt450"; // "jt500";
// Thresholds for the corresponding triggers (same as in trigger name)
const constexpr double _jp_trigthr[_jp_ntrigs] =
{40,        60,        80,       140,       200,       260,       320,       400,      450}; // 500};
// Trigger ranges (differ from thresholds)
const constexpr double _jp_trigranges[_jp_ntrigs][2] =
{ {0,84}, {84,114}, {114,196}, {196,272}, {272,330}, {330,395}, {395,468}, {468,548}, {548,6500} }; // V[5,6], AK4

// Trigger lumis for weighting
constexpr bool _jp_usetriglumi = true; // use luminosity numbers below, in /ub
const constexpr double _jp_triglumi[_jp_ntrigs] = // in /ub
// 2016 final: brilcalc lumi -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt --byls --normtag=$PWD/../../../Normtags/normtag_DATACERT.json --minBiasXsec 80000 --hltpath="HLT_PFJet*" -o brilcalc_lumibyls16.csv
{264821.835,718829.826,2733420.74,23966019.286,102854094.409,587728815.19,1753996573.885,5141160409.826,35860066023.522};
// 2017 final: brilcalc lumi -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/Final/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt --byls --normtag=/eos/user/h/hsiikone/Normtags/normtag_BRIL.json --minBiasXsec 80000 --hltpath="HLT_PFJet*" -o brilcalc_lumibyls17.csv
// {480907.381,1135408.863,4300901.657,39784735.913,218494113.161,553531633.649,1402704877.698,4201460774.775,10434425834.42,41366526620.215};
// Unprescaled luminosity for plots
constexpr double _jp_lumi = _jp_triglumi[_jp_ntrigs-1]/1000000000.0; // in /fb
//} END Switches and their settings

//{ JEC and IOV settings. In the modern world we have a group of IOV's for which different corrections are applied.
// https://github.com/cms-jet/JECDatabase/tree/master/tarballs
// Summer16_03Feb2017G_V7_DATA.tar.gz [BCD, EF, G, H]
const constexpr char _jp_jecgt[] = "Summer16_03Feb2017";//"Summer16_07Aug2017";////BCD_";//"Summer15_50ns";// "Summer16_23Sep2016";
const constexpr char _jp_jecvers[] = "_V9";//"_V4";////"V4"; // Summer16_03Feb // "V6"; // Summer16_23Sep // "V2" ; // Spring16

// Use Intervals-Of-Validity for JEC
constexpr const bool _jp_useIOV = true ;//false
constexpr const unsigned int _jp_nIOV = 4; //3;
const constexpr char* _jp_IOVnames[_jp_nIOV] =
  {"BCD",    "EF",    "G",   "H"};//{"BCD",    "EF",    "GH"};
// Trigger IOVs: the 1 for -inf and 400000 for inf (currently)
const constexpr int _jp_IOVranges[_jp_nIOV][2] =
//  { {1,276811}, {276831,278801}, {278802,400000} }; // Summer16_23Sep2016
  { {1,276811}, {276831,278801}, {278802,280385}, {280919,400000} }; // Spring/Summer16_23Sep2016
//} END JES and JEC


//{ BEGIN For MC reweighting purposes, PU profiles for data and MC
constexpr bool _jp_reweighPU = true;
const constexpr char _jp_pudata[] = "pileup/pileup_DT.root";
const constexpr char _jp_pumc[]   = "pileup/pu.root";
//} END PU profiles


//{ BEGIN MC: Process pThatbins instead of flat sample
// All the following values need to be updated, if other source tuples are used.
// These can be obtained using getSliceEvts.C
constexpr bool _jp_pthatbins = true;//false;
// Number of pthat bins
constexpr unsigned int _jp_npthatbins = 14;
// The corresponding ranges, the last number is ~inf
const vector<double> _jp_pthatranges =
  {30,50,80,120,170,300,470,600,800,1000,1400,1800,2400,3200,20000};
// The corresponding lumis in an arbitrary scale
const vector<double> _jp_pthatsigmas =
  {140932000,19204300,2762530,471100,117276,7823,648.2,186.9,32.293,9.4183,0.84265,0.114943,0.00682981,0.000165445};
  //{140932000,19204300,2762530,471100,117276,7823,648.2,186.9,32.293,9.4183,0.84265,0.114943,0.00682981,0.000165445};
const vector<double> _jp_pthatnevts =
  {9699558,9948791,7742665,5748730,7838066,11701816,3959986,9628335,11915305,6992746,2477018,1584378,596904,391735};
// The filenames need to be given here and in mk_histosFill, since ROOT is exceedingly stupid
const vector<string> _jp_pthatfiles = {
    "QCD_Pt_30to50_TuneCUETP8M_13TeV_pythia8.root",
    "QCD_Pt_50to80_TuneCUETP8M_13TeV_pythia8.root",
    "QCD_Pt_80to120_TuneCUETP8M_13TeV_pythia8.root",
    "QCD_Pt_120to170_TuneCUETP8M_13TeV_pythia8.root",
    "QCD_Pt_170to300_TuneCUETP8M_13TeV_pythia8.root",
    "QCD_Pt_300to470_TuneCUETP8M_13TeV_pythia8.root",
    "QCD_Pt_470to600_TuneCUETP8M_13TeV_pythia8.root",
    "QCD_Pt_600to800_TuneCUETP8M_13TeV_pythia8.root",
    "QCD_Pt_800to1000_TuneCUETP8M_13TeV_pythia8.root",
    "QCD_Pt_1000to1400_TuneCUETP8M_13TeV_pythia8.root",
    "QCD_Pt_1400to1800_TuneCUETP8M_13TeV_pythia8.root",
    "QCD_Pt_1800to2400_TuneCUETP8M_13TeV_pythia8.root",
    "QCD_Pt_2400to3200_TuneCUETP8M_13TeV_pythia8.root",
    "QCD_Pt_3200toInf_TuneCUETP8M_13TeV_pythia8.root",
};
//} END pthatbins stuff


//{ BEGIN RUN2 params
constexpr double _jp_sqrts = 13000.; // GeV
constexpr double _jp_emax = _jp_sqrts/2.; // Max possible jet pt
constexpr double _jp_recopt = 15; // Min observed jet pt
constexpr double _jp_xsecMinBias = 7.126E+10;
//} END Run2

//{ BEGIN Special histosFill analyses that are not typically needed
// Eta phi exclusion due to ecal problems
constexpr bool _jp_doetaphiexcl = false;
const constexpr char _jp_etaphitype[] = "h2hotr"; // h2hotr (Robert) or h2hotm (Mikko)

// Veto jets near ECAL boundaries in JetID
constexpr bool _jp_doECALveto = false;//true;
const constexpr char _jp_ecalveto[] = "lumicalc/ecalveto.root";

// Check for duplicates (warning: takes a lot of memory!)
constexpr bool _jp_checkduplicates = false;
//} END Special analyses that are not typically needed

//{ BEGIN histosNormalize (leave these off if not interested on details)
// Correct for trigger efficiency based on MC
constexpr bool _jp_dotrigeff = false; // CAUTION: Requires output-MC-1.root file
// Correct pT<114 GeV only, if above _jp_dotrigeff=true
constexpr bool _jp_dotrigefflowptonly = false; // CAUTION: Not needed
// Correct for time-dependence (prescales) in data
constexpr bool _jp_dotimedep = false; // CAUTION: Unknown consequences
//} END histosNormalize

//{ BEGIN drawSummary
// Center uncertainties around ansatz (true) or data (false)
constexpr bool _jp_centerOnAnsatz = false;
constexpr bool _jp_centerOnTheory = false;
// Plot Pythia for final PRL results
constexpr bool _jp_plotPythia = false;
// Draw againts HERAPDF1.7 instead of PDF4LHC (drawSummary)
constexpr bool _jp_herapdf = false;
// Produce plots (also drawRunHistos)
constexpr bool _jp_pdf = true;
//} END drawSummary

//{ BEGIN limits for histosNormalize, dagostini, drawSummary, drawRunHistos
// Minimum and maximum pT range to be plotted and fitted
constexpr double _jp_fitptmin = 114;//43;
// Changed on 2013-05-020: analysis from 49 GeV to 56 GeV
constexpr double _jp_xmin57 = 114;//56;
constexpr double _jp_xminpas = 114;//56;
constexpr double _jp_xmin = 114;//24.;//20.;
constexpr double _jp_xmax = 1497;//TEMP PATCH for partial data //1999.;
//} END limits

//{ BEGIN Binnings
// Binning agreed within JTF: pT>100 GeV from CaloJet resolutions,
// pT<100 GeV to optimize bin widths for PFJets and b-tagging
// (little higher than resolution, but fairly flat relative width)
// http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/QCDAnalysis/HighPtJetAnalysis/interface/DefaultPtBins.h?revision=1.2&view=markup
const constexpr double _jp_ptrange[] =
  {1, 5, 6, 8, 10, 12, 15, 18, 21, 24, 28, 32, 37, 43, 49, 56, 64, 74, 84,
   97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468,
   507, 548, 592, 638, 686, 737, 790, 846, 905, 967,
   1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1684, 1784, 1890, 2000,
   2116, 2238, 2366, 2500, 2640, 2787, 2941, 3103, 3273, 3450, 3637, 3832,
   4037, 4252, 4477, 4713, 4961, 5220, 5492, 5777, 6076, 6389, 6717, 7000};
const constexpr unsigned int _jp_npts = sizeof(_jp_ptrange)/sizeof(_jp_ptrange[0])-1;

const constexpr double _jp_wptrange[] =
  {1, 15, 21, 28, 37, 49, 64, 84, 114, 153, 196, 245, 300, 395, 468, 548,
   686, 846, 1032, 1248, 1588, 2000, 2500, 3103, 3450, 3637, 3832,
   4037, 4252, 4477, 4713, 4961, 5220, 5492, 5777, 6076, 6389, 6717, 7000};
const constexpr int _jp_nwpts = sizeof(_jp_wptrange)/sizeof(_jp_wptrange[0])-1;

// Wider version of the binning for less statistical scatter for b-jets
const constexpr double _jp_wwptrange[] =
  {1, 5, 15, 24, 37, 56, 84, 114, 153, 196, 245, 330, 430, 548, 686, 846,
   1032, 1248, 1497, 1784, 2116, 2500, 2941, 3450, 3637,
   4252, 4961, 5777, 6717, 7000};
const constexpr int _jp_nwwpts = sizeof(_jp_wwptrange)/sizeof(_jp_wwptrange[0])-1;

// Optimized binning created by optimizeBins.C ("MC"; lumi 1000/pb, eff 1e+10%)
// Using NLOxNP theory fit as input when available
const constexpr unsigned int _jp_nptranges = 8;
const constexpr unsigned int _jp_npts_eta = 65;
const constexpr double _jp_ptrangevseta[_jp_nptranges][_jp_npts_eta] =
  {{10, 12, 15, 18, 21, 24, 28, 32, 37, 43, 49, 56, 64, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1684, 1784, 1890, 2000, 2116, 2238, 2366, 2500, 2640, 2787, 2941, 3103, 3273, 3450, 3832, 6076, 6389}, // Eta_0.0-0.5
   {10, 12, 15, 18, 21, 24, 28, 32, 37, 43, 49, 56, 64, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1684, 1784, 1890, 2000, 2116, 2238, 2366, 2500, 2640, 2787, 2941, 3103, 3273, 3637, 5220, 5492, 0}, // Eta_0.5-1.0
   {10, 12, 15, 18, 21, 24, 28, 32, 37, 43, 49, 56, 64, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1684, 1784, 1890, 2000, 2116, 2238, 2366, 2500, 2640, 2941, 3832, 4037, 0, 0, 0, 0, 0}, // Eta_1.0-1.5
   {10, 12, 15, 18, 21, 24, 28, 32, 37, 43, 49, 56, 64, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1684, 1784, 1890, 2000, 2116, 2500, 2640, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Eta_1.5-2.0
   {10, 12, 15, 18, 21, 24, 28, 32, 37, 43, 49, 56, 64, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1684, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Eta_2.0-2.5
   {10, 12, 15, 18, 21, 24, 28, 32, 37, 43, 49, 56, 64, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Eta_2.5-3.0
   {10, 12, 15, 18, 21, 24, 28, 32, 37, 43, 49, 56, 64, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Eta_3.0-3.5
   {10, 12, 15, 18, 21, 24, 28, 32, 37, 43, 49, 56, 64, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}; // Eta_3.5-4.0

const constexpr double _jp_wetarange[] =
  {-5.191, -3.839, -3.489, -3.139, -2.964, -2.853, -2.650, -2.500, -2.322,
   -2.172, -1.930, -1.653, -1.479, -1.305, -1.044, -0.783, -0.522, -0.261,
    0.000,  0.261,  0.522,  0.783,  1.044,  1.305,  1.479,  1.653,  1.930,
    2.172,  2.322,  2.500,  2.650,  2.853,  2.964,  3.139,  3.489,  3.839, 5.191};
const constexpr unsigned int _jp_nwetas = sizeof(_jp_wetarange)/sizeof(_jp_wetarange[0])-1;

const constexpr double _jp_posetarange[] =
  {0, 0.261, 0.522, 0.783, 0.957, 1.131, 1.305, 1.479, 1.93, 2.322, 2.411, 2.5, 2.853, 2.964, 5.191};
const constexpr unsigned int _jp_nposetas = sizeof(_jp_posetarange)/sizeof(_jp_posetarange[0]);

const constexpr double _jp_etarange[] =
  {-5.191,-4.889,-4.716,-4.538,-4.363,-4.191,-4.013,-3.839,-3.664,-3.489,-3.314,
   -3.139,-2.964,-2.853,-2.65,-2.5,-2.322,-2.172,-2.043,-1.93,-1.83,-1.74,-1.653,
   -1.566,-1.479,-1.392,-1.305,-1.218,-1.131,-1.044,-0.957,-0.879,-0.783,-0.696,
   -0.609,-0.522,-0.435,-0.348,-0.261,-0.174,-0.087, 0.000, 0.087, 0.174, 0.261,
    0.348, 0.435, 0.522, 0.609, 0.696, 0.783, 0.879, 0.957, 1.044, 1.131, 1.218,
    1.305, 1.392, 1.479, 1.566, 1.653, 1.74, 1.83, 1.93, 2.043, 2.172, 2.322, 2.5,
    2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.664, 3.839, 4.013, 4.191, 4.363,
    4.538, 4.716, 4.889, 5.191};
const constexpr unsigned int _jp_netas = sizeof(_jp_etarange)/sizeof(_jp_etarange[0])-1;
//} END Binnings

#endif // __settings_h__
