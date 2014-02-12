#include <iostream>
#include <TFile.h>
#include <TBranch.h>
#include <TTree.h>
#include <stdio.h>
#include <libgen.h>
#include <errno.h>

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
  Model_t_Lhyquid3V parameters;
  Float_t CenMin[2];

  bool doNotMove = false;
  string targetDirectory;
  string command;
  string directories[9] = {
    "/mnt/data2/Models/sorted/2/",
    "/mnt/data2/Models/sorted/3/",
    "/mnt/data2/Models/sorted/5/",
    "/mnt/data2/Models/sorted/5.7/", // new
    "/mnt/data2/Models/sorted/7/",
    "/mnt/data2/Models/sorted/8/",
    "/mnt/data2/Models/sorted/9/",
    "/mnt/data2/Models/sorted/10/",
    "/mnt/data2/Models/sorted/11/"
  };

  if(argc < 2) {
    cout << "Please provide file name." << endl;
    return -1;
  }
  string filePath = argv[1];
  string dirName = dirname(argv[1]);
  string fileName = basename((char*)filePath.c_str());
  // cout << "File name: " << fileName << endl;
  // string filePath = "sample/" + string(argv[1]) +"/event.root";


  TFile file(filePath.c_str());
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
  if(bParameters->GetEntries() != 2) {
    cout << "Too few entries in parameters tree." << endl;
    return -1;
  }
  bParameters ->GetEntry(0);
  CenMin[0] = parameters.CentralityMin;
  bParameters ->GetEntry(1);
  CenMin[1] = parameters.CentralityMin;
  
  if(CenMin[0] < 1) {
    doNotMove = true; // b2 & b3
  } else if(CenMin[0] == 10 && CenMin[1] == 10) {
    targetDirectory = directories[2]; // b5
  } else if(CenMin[0] == 10 && CenMin[1] < 1) {
    targetDirectory = directories[3]; // b5_new
  } else if(CenMin[0] == 20) {
    targetDirectory = directories[4]; // b7
  } else if(CenMin[0] == 30) {
    targetDirectory = directories[5]; // b8
  } else if(CenMin[0] == 40) {
    targetDirectory = directories[6]; // b9
  } else if(CenMin[0] == 50) {
    targetDirectory = directories[7]; // b10
  } else if(CenMin[0] == 60) {
    targetDirectory = directories[8]; // b11
  } else {
    cerr << "!!> " << filePath << "\t" << CenMin[0] << " | " << CenMin[1] << endl;
  }
  if(!doNotMove) {
    cout << filePath << "\t\t-> " << targetDirectory+fileName << endl;
    if(rename(filePath.c_str(), (targetDirectory + fileName).c_str()) != 0) {
      cout << "rename failed! errno: " << errno << endl;
    }
  } else {
    cout << "b2 / b3" << endl;
  }

  return 0;
}