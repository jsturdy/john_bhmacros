#include <stdio.h>
#include <iostream>
#include "Riostream.h"
#include "TH2.h"
#include "TF1.h"
#include "TF2.h"
#include "TCanvas.h"
#include "TPad.h"
#include <fstream>
#include <string>
#include <vector>

#include <TROOT.h>

void SThist_mc(void);
float dR(float eta1, float phi1, float eta2, float phi2);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

void SThist_mc(std::string filename) {
  // define output file and output histogram
  TFile *outfile = new TFile("outfile_mc.root","RECREATE");
  TH1F stHist = TH1F("stHist", "ST", 100, 700, 9700);

  // variables calculated in the loop
	int NJets         = 0    ;
	float ST          = 0.   ;
	bool passIso      = true ;
	int nPassedEvents = 0    ;

  // variables accessed from the tree
	//Bool_t firedHLT_PFHT800_v2       ;
	float    *JetPt   = new float[25];
	float    *JetEta  = new float[25];
	float    *JetPhi  = new float[25];
	float    *ElePt   = new float[25];
	float    *EleEta  = new float[25];
	float    *ElePhi  = new float[25];
	float    *PhPt    = new float[25];
	float    *PhEta   = new float[25];
	float    *PhPhi   = new float[25];
	float    *MuPt    = new float[25];
	float    *MuEta   = new float[25];
	float    *MuPhi   = new float[25];

  // tree branches
	TBranch  *b_JetPt  ;
	TBranch  *b_JetEta ;
	TBranch  *b_JetPhi ;
	TBranch  *b_ElePt  ;
	TBranch  *b_EleEta ;
	TBranch  *b_ElePhi ;
	TBranch  *b_PhPt   ;
	TBranch  *b_PhEta  ;
	TBranch  *b_PhPhi  ;
	TBranch  *b_MuPt   ;
	TBranch  *b_MuEta  ;
	TBranch  *b_MuPhi  ;

  std::vector<std::vector<std::string> > samplesWeightsMap;
  std::vector<std::string> sampleWithWeight;
  ifstream infile;
  infile.open(filename.c_str()); 
  std::string buffer;
  while (std::getline(infile, buffer)) {
    sampleWithWeight = split(buffer, ',');
    cout << "For sample with name " << sampleWithWeight[0] << " the weight is " << sampleWithWeight[1] << std::endl;
    samplesWeightsMap.push_back(sampleWithWeight);
  }


  
  
  
  for (int iSample = 0; iSample<samplesWeightsMap.size(); ++iSample) { 
    //create a one-element chain by looping over the input filename
	  TChain chain("bhana/t");
    const char *eosURL = "root://eoscms.cern.ch/";
    std::string ntupleURL = eosURL + samplesWeightsMap[iSample][0]; 
    cout << "The ntuple URL is: " << ntupleURL << endl;
	  chain.Add(ntupleURL.c_str());
    
	  cout << "Opened chain: " << chain.GetName() << endl;

    // set all branch addresses
	  //chain.SetBranchAddress( "firedHLT_PFHT800_v2"       , &firedHLT_PFHT800_v2       );
	  chain.SetBranchAddress( "JetPt",  JetPt,  &b_JetPt  );
	  chain.SetBranchAddress( "JetEta", JetEta, &b_JetEta );
	  chain.SetBranchAddress( "JetPhi", JetPhi, &b_JetPhi );
	  chain.SetBranchAddress( "ElePt",  ElePt,  &b_ElePt  );
	  chain.SetBranchAddress( "EleEta", EleEta, &b_EleEta );
	  chain.SetBranchAddress( "ElePhi", ElePhi, &b_ElePhi );
	  chain.SetBranchAddress( "PhPt",   PhPt,   &b_PhPt   );
	  chain.SetBranchAddress( "PhEta",  PhEta,  &b_PhEta  );
	  chain.SetBranchAddress( "PhPhi",  PhPhi,  &b_PhPhi  );
	  chain.SetBranchAddress( "MuPt",   MuPt,   &b_MuPt   );
	  chain.SetBranchAddress( "MuEta",  MuEta,  &b_MuEta  );
	  chain.SetBranchAddress( "MuPhi",  MuPhi,  &b_MuPhi  );

	  const int nEvents = chain.GetEntries();
	  cout << "Number of events in this sample is: " << nEvents << endl;

    // loop over all events
	  for (int iEvent = 0; iEvent < nEvents; ++iEvent) {
      if (iEvent % 25000 == 0) cout << "Processed " << iEvent << " events." << endl; //, of which " << nPassedEvents << " have passed the trigger and filter requirements." << endl;

      // reset variables
	  	ST      = 0.   ;
	  	NJets   = 0    ;
	  	passIso = true ;
 
	  	chain.GetEntry(iEvent);
      // don't apply trigger and filter requirements for MC
	  	// if (    !firedHLT_PFHT800_v2 || !passed_CSCTightHaloFilter 
      //     || !passed_goodVertices || !passed_eeBadScFilter      ) continue;
      
      // apply isolation requirement and calculate ST.
	  	//cout << "For event number " << iEvent << endl;
	  	for (int iJet = 0; iJet < 25; ++iJet) {
	  		if (JetPt[iJet]>20.) {
	  			for (int iElectron = 0; iElectron < 25; ++iElectron ) {
	  				if (dR(JetEta[iJet],JetPhi[iJet], EleEta[iElectron], ElePhi[iElectron]) < 0.3) {
	  					passIso = false;
	  					break;
	  				}
	  			}

	  			if (!passIso) continue;
	  			for (int iPhoton = 0; iPhoton < 25; ++iPhoton ) {
	  				if (dR(JetEta[iJet],JetPhi[iJet], PhEta[iPhoton], PhPhi[iPhoton]) < 0.3) {
	  					passIso = false;
	  					break;
	  				}
	  			}
	  			if (!passIso) continue;

	  			for (int iMuon = 0; iMuon < 25; ++iMuon ) {
	  				if (dR(JetEta[iJet],JetPhi[iJet], MuEta[iMuon], MuPhi[iMuon]) < 0.3) {
	  					passIso = false;
	  					break;
	  				}
	  			}
	  			if (!passIso) continue;

	  			//cout << "    JetPt for jet number " << iJet << " is: " << JetPt[iJet] << endl;
	  			++NJets;
	  			ST += JetPt[iJet];
	  		}
	  		else break;
	  	}
	  	for (int iElectron = 0; iElectron < 25; ++iElectron) {
	  		if (ElePt[iElectron]>20.) {
	  			for (int iJet = 0; iJet < 25; ++iJet ) {
	  				if (dR(EleEta[iElectron],ElePhi[iElectron], JetEta[iJet], JetPhi[iJet]) < 0.3) {
	  					passIso = false;
	  					break;
	  				}
	  			}
	  			if (!passIso) continue;

	  			for (int iPhoton = 0; iPhoton < 25; ++iPhoton ) {
	  				if (dR(EleEta[iElectron],ElePhi[iElectron], PhEta[iPhoton], PhPhi[iPhoton]) < 0.3) {
	  					passIso = false;
	  					break;
	  				}
	  			}
	  			if (!passIso) continue;

	  			for (int iMuon = 0; iMuon < 25; ++iMuon ) {
	  				if (dR(EleEta[iElectron],ElePhi[iElectron], MuEta[iMuon], MuPhi[iMuon]) < 0.3) {
	  					passIso = false;
	  					break;
	  				}
	  			}
	  			if (!passIso) continue;

	  			//cout << "    ElePt for electron number " << iElectron << " is: " << ElePt[iElectron] << endl;
	  			ST += ElePt[iElectron];
	  		}
	  		else break;
	  	}
	  	for (int iPhoton = 0; iPhoton < 25; ++iPhoton) {
	  		if (PhPt[iPhoton]>20.) {
	  			for (int iJet = 0; iJet < 25; ++iJet ) {
	  				if (dR(PhEta[iPhoton],PhPhi[iPhoton], JetEta[iJet], JetPhi[iJet]) < 0.3) {
	  					passIso = false;
	  					break;
	  				}
	  			}
	  			if (!passIso) continue;

	  			for (int iElectron = 0; iElectron < 25; ++iElectron ) {
	  				if (dR(PhEta[iPhoton], PhPhi[iPhoton], EleEta[iElectron],ElePhi[iElectron]) < 0.3) {
	  					passIso = false;
	  					break;
	  				}
	  			}
	  			if (!passIso) continue;

	  			for (int iMuon = 0; iMuon < 25; ++iMuon ) {
	  				if (dR(PhEta[iPhoton], PhPhi[iPhoton], MuEta[iMuon], MuPhi[iMuon]) < 0.3) {
	  					passIso = false;
	  					break;
	  				}
	  			}
	  			if (!passIso) continue;

	  			//cout << "    PhPt for photon number " << iPhoton << " is: " << PhPt[iPhoton] << endl;
	  			ST += PhPt[iPhoton];
	  		}
	  		else break;
	  	}
	  	for (int iMuon = 0; iMuon < 25; ++iMuon) {
	  		if (MuPt[iMuon]>20.) {
	  			for (int iJet = 0; iJet < 25; ++iJet ) {
	  				if (dR(MuEta[iMuon],MuPhi[iMuon], JetEta[iJet], JetPhi[iJet]) < 0.3) {
	  					passIso = false;
	  					break;
	  				}
	  			}
	  			if (!passIso) continue;

	  			for (int iElectron = 0; iElectron < 25; ++iElectron ) {
	  				if (dR(MuEta[iMuon], MuPhi[iMuon], EleEta[iElectron],ElePhi[iElectron]) < 0.3) {
	  					passIso = false;
	  					break;
	  				}
	  			}
	  			if (!passIso) continue;

	  			for (int iPhoton = 0; iPhoton < 25; ++iPhoton ) {
	  				if (dR( MuEta[iMuon], MuPhi[iMuon], PhEta[iPhoton], PhPhi[iPhoton]) < 0.3) {
	  					passIso = false;
	  					break;
	  				}
	  			}
	  			if (!passIso) continue;

	  			//cout << "    MuPt for muon number " << iMuon << " is: " << MuPt[iMuon] << endl;
	  			ST += MuPt[iMuon];
	  		}
	  		else break;
	  	}
	  	//
	  	//cout << "    ST is: " << ST << endl;
	  	stHist.Fill(ST,std::atof(samplesWeightsMap[iSample][1].c_str()));
	  	nPassedEvents+=1;
	  	//if (nPassedEvents==5) break;
	  }
  }
  // write the histogram and the output file
  outfile->cd();
  stHist.Write();
  //
  // clean up
	delete[] JetPt;
	delete[] JetEta;
	delete[] JetPhi;
	delete[] ElePt;
	delete[] EleEta;
	delete[] ElePhi;
	delete[] PhPt;
	delete[] PhEta;
	delete[] PhPhi;
	delete[] MuPt;
	delete[] MuEta;
	delete[] MuPhi;
}

// function to calculate dR between two objects
float dR(float eta1, float phi1, float eta2, float phi2) {
	return std::sqrt( ( eta1 - eta2 )*( eta1 - eta2 ) + ( phi1 - phi2 )*( phi1 - phi2 ) );
}

// String splitting functions. Why does c++ not have this in the stl?
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

