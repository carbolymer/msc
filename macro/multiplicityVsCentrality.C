#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <string>
#include <TChain.h>
#include <TH1F.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>


class ParticleCoor {
  public:
    Float_t mass;
    Float_t t, x,  y,  z;
    Float_t e, px, py, pz;
    Int_t   decayed;
    Int_t   pid;
    Int_t   fatherpid;
    Int_t   rootpid;
    Int_t   eid;
    Int_t   fathereid;
    UInt_t  eventid;
};

using namespace std;

void addFiles(TChain* chain, string eventsDir, string dir) {
  chain->Add((eventsDir+string("/")+dir+"/event001.root").c_str());
  chain->Add((eventsDir+string("/")+dir+"/event002.root").c_str());
  chain->Add((eventsDir+string("/")+dir+"/event003.root").c_str());
  chain->Add((eventsDir+string("/")+dir+"/event004.root").c_str());
  chain->Add((eventsDir+string("/")+dir+"/event005.root").c_str());
  chain->Add((eventsDir+string("/")+dir+"/event006.root").c_str());
  chain->Add((eventsDir+string("/")+dir+"/event007.root").c_str());
  chain->Add((eventsDir+string("/")+dir+"/event008.root").c_str());
  chain->Add((eventsDir+string("/")+dir+"/event009.root").c_str());
  chain->Add((eventsDir+string("/")+dir+"/event010.root").c_str());
  chain->Add((eventsDir+string("/")+dir+"/event011.root").c_str());
  chain->Add((eventsDir+string("/")+dir+"/event012.root").c_str());
  chain->Add((eventsDir+string("/")+dir+"/event013.root").c_str());
  chain->Add((eventsDir+string("/")+dir+"/event014.root").c_str());
  chain->Add((eventsDir+string("/")+dir+"/event015.root").c_str());
  chain->Add((eventsDir+string("/")+dir+"/event016.root").c_str());
  chain->Add((eventsDir+string("/")+dir+"/event017.root").c_str());
  chain->Add((eventsDir+string("/")+dir+"/event018.root").c_str());
  chain->Add((eventsDir+string("/")+dir+"/event019.root").c_str());
  chain->Add((eventsDir+string("/")+dir+"/event020.root").c_str());
}

void processChain(TChain *chain, Long64_t &pions, Long64_t &kaons, Long64_t &protons) {
  ParticleCoor buffer;
  Long64_t npart = chain->GetEntries();
  chain->SetBranchAddress("particle",&buffer);
  for (Long64_t iter=0; iter<npart; ++iter) {
    chain->GetEntry(iter);
    switch((int)abs(buffer.pid)) {
      case 211:
        ++pions;
        break;
      case 321:
        ++kaons;
        break;
      case 2212:
        ++protons;
        break;
    }
  }
  cout << "pions:\t\t" << pions << endl;
  cout << "kaons:\t\t" << kaons << endl;
  cout << "protons:\t" << protons << endl;
  cout << "pi/k:\t\t" << 1.*pions/kaons << endl;
  cout << "pi/p:\t\t" << 1.*pions/protons << endl << endl;
}

// int multiplicityVsCentrality() {
int main() {
  const int nCentralities = 6;
  string dirs[] = {
    "data/Models/lhyqid3v_LHCPbPb_2760_b2",
    "data/Models/lhyqid3v_LHCPbPb_2760_b3",
    // "home/mgalazyn/b5",
    "data/Models/lhyqid3v_LHCPbPb_2760_b8.7",
    "data/Models/lhyqid3v_LHCPbPb_2760_b9.9",
    "data/Models/lhyqid3v_LHCPbPb_2760_b10.9",
    "data/Models/lhyqid3v_LHCPbPb_2760_b11.9"
  };

  double centralities[] = {
    2.3,
    3.1,
    // 5.7,
    8.7,
    9.9 ,
    10.9,
    11.9
  };


  string eventsDir = "/.";
  TChain *chain[nCentralities];

  Long64_t pions[nCentralities];
  Long64_t kaons[nCentralities];
  Long64_t protons[nCentralities];

  const char formula[] = "[0]*x^2+[1]*x+[2]";

  TCanvas * c1 = new TCanvas("c", "c", 800, 600);
  
  TH1F *thpions = new TH1F("pions", "pions", 130, 0, 13);
  TF1 *tfpions = new TF1("tfpions", formula,2,12);
  TH1F *thkaons = new TH1F("kaons", "kaons", 130, 0, 13);
  TF1 *tfkaons = new TF1("tfkaons", formula,2,12);
  TH1F *thprotons = new TH1F("protons", "protons", 130, 0, 13);
  TF1 *tfprotons = new TF1("tfprotons", formula,1,12);

  for(int i = 0 ; i < nCentralities ; ++i) {
    pions[i] = 0;
    kaons[i] = 0;
    protons[i] = 0;
    chain[i] = new TChain("particles");
    addFiles(chain[i], eventsDir, dirs[i]);
    processChain(chain[i], pions[i], kaons[i], protons[i]);
    thpions->Fill(centralities[i], pions[i]);
    thkaons->Fill(centralities[i], kaons[i]);
    thprotons->Fill(centralities[i], protons[i]);
  }

  thpions->Fit(tfpions);
  thkaons->Fit(tfkaons);
  thprotons->Fit(tfprotons);

  thpions->Draw();
  thkaons->Draw("SAME");
  thprotons->Draw("SAME");

  thpions->Draw("SAME");
  thkaons->Draw("SAME");
  thprotons->Draw("SAME");

  cout << "pions:\t\t" << "y = " << tfpions->GetParameter(0) << "x^2 + " 
      << tfpions->GetParameter(1) << "x + " << tfpions->GetParameter(2) << endl;
  cout << "kaons:\t\t" << "y = " << tfkaons->GetParameter(0) << "x^2 + " 
      << tfpions->GetParameter(1) << "x + " << tfkaons->GetParameter(2) << endl;
  cout << "protons:\t" << "y = " << tfprotons->GetParameter(0) << "x^2 + " 
      << tfpions->GetParameter(1) << "x + " << tfprotons->GetParameter(2) << endl;



  thpions->SetLineColor(1);
  tfpions->SetLineColor(1);
  thkaons->SetLineColor(2);
  tfkaons->SetLineColor(2);
  thprotons->SetLineColor(4);
  tfprotons->SetLineColor(4);

  TLegend *leg = new TLegend(0.8,0.65,0.9,0.75);
  leg->AddEntry(thpions,"pions","l");
  leg->AddEntry(thkaons,"kaons","l");
  leg->AddEntry(thprotons,"protons","l");
  leg->Draw();

  // c1->SaveAs("multiplicity.png");
  c1->SaveAs("multiplicity.root");

  return 0;
}
