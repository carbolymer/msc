#include <iostream>
#include <TFile.h>
#include <TBranch.h>
#include <TTree.h>


using namespace std;

#define _MODEL_T_FORMAT_LHYQUID3D_  "RapPRange/F:Lambda:TauI:TempF:MuB:MuI:MuS:MuC:CollidingEnergy:CentralityMin:CentralityMax:ImpactParameter:TempI:DeviceName[30]/C:CollidingSystem[30]"

struct Model_t_Lhyquid3D
{
  Float_t RapPRange, Lambda, TauI;
  Float_t TempF;
  Float_t MuB, MuI, MuS, MuC;
  Float_t CollidingEnergy, CentralityMin, CentralityMax, ImpactParameter, TempI;
  Char_t  DeviceName[30], CollidingSystem[30];
};

struct Model_t_Lhyquid3V
{
  Float_t RapPRange, Lambda, TauI;
  Float_t TempF;
  Float_t MuB, MuI, MuS, MuC;
  Float_t cs2freeze,etas,zetas;
  Float_t CollidingEnergy, CentralityMin, CentralityMax, ImpactParameter, TempI;
  Char_t  DeviceName[30], CollidingSystem[30];
};

/*
Klasifykator:

b2      CenMin[0] = 0 ; CenMin[1] = ~3e-14 / 3e-22 ???
b3      CenMin[0] = 0 ; CenMin[1] = ~2e-28 ???
b5      CenMin[0] = CenMin[1] = 10
b5_new  CenMin[0] = 10 ; CenMin[1] ~ 3e-13 - 4e-13 ???
b7      CenMin[0] = 20 ; CenMin[1] ~ e-27
b8      CenMin[0] = CenMin[1] = 30
b9      CenMin[0] = CenMin[1] = 40
b10     CenMin[0] = CenMin[1] = 50
b11     CenMin[0] = CenMin[1] = 60
*/


// int sortEvents() {
int main(int argc, char *argv[]) {
  if(argc < 1) {
    cout << "Please provide file name." << endl;
    return -1;
  }
  string fileName = argv[1];
  cout << "File name: " << fileName << endl;
  // string fileName = "sample/" + string(argv[1]) +"/event.root";

  Model_t_Lhyquid3V parameters;

  TFile file(fileName.c_str());
  if(file.IsZombie()) {
    cout << "Error opening file!!" << endl;
    return -1;
  }
  TTree *parametersTree = (TTree*) file.Get("parameters");
  if(parametersTree == 0) {
    cout << "Could not find parameters tree!" << endl;
    return -1;
  }

  TBranch *bParameters = parametersTree->GetBranch("ModelParameters");
  parameters.CollidingEnergy = 0;
  // parameters.CollidingSystem = "";
  parameters.ImpactParameter = 0;
  parameters.CentralityMin = 0;
  parameters.CentralityMax = 0;
  // parameters.DeviceName = "";
  bParameters->SetAddress(&parameters);
  for(unsigned short int i = 0; i < bParameters->GetEntries(); ++i) {
    bParameters ->GetEntry(i);
    cout << "=== Entry: " << i << " ===" << endl;
    cout << "CollEn:\t" << parameters.CollidingEnergy << endl;
    // cout << "CollSys:\t" << parameters.CollidingSystem << endl;
    cout << "b:\t" << parameters.ImpactParameter << endl;
    cout << "CenMin:\t" << parameters.CentralityMin << endl;
    cout << "CenMax:\t" << parameters.CentralityMax << endl;
    // cout << "DeviceName:\t" << parameters.DeviceName << endl;
  }
  
}