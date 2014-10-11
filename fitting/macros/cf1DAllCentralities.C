#include <iostream>
#include <string>
#include <vector>
#include <TH1D.h>

typedef unsigned short int ushort;

const string rootPathForCFs = "../../../tpi_output/";

const float latexTextSizeSmall = 0.04;
const float latexTextSizeBig = 0.06;

const int colors[9] = { kRed, kBlue, kOrange-1, kGreen+2, kViolet+1, kRed+3, kCyan+1, kCyan+2, kOrange-3 };
const int markers[9] = { 21, 24, 33, 25, 20, 27, 34, 30, 34 };

void preparePad()
{
  gPad->SetFillColor(0);
  gPad->SetFillStyle(4000);
  gPad->SetTopMargin(0.013);
  gPad->SetRightMargin(0.005);
  gPad->SetBottomMargin(0.13);
  gPad->SetLeftMargin(0.15);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gStyle->SetOptStat(1000000000);
  gStyle->SetStatBorderSize(0);
  gStyle->SetTextFont(42);
}

void changeHistogramStyle(TH1D *histogram,  ushort ht, ushort ct) {
  histogram->SetLineColor(colors[ht]);
  histogram->SetMarkerColor(colors[ht]);
  histogram->SetMarkerStyle(markers[ht + 3*ct]);
  histogram->SetMarkerSize(1.2);
  histogram->GetXaxis()->SetTitleSize(0.05);
  histogram->GetXaxis()->SetLabelSize(0.05);
  histogram->GetYaxis()->SetTitleSize(0.05);
  histogram->GetYaxis()->SetLabelSize(0.05);
  histogram->GetXaxis()->CenterTitle();
  histogram->GetYaxis()->CenterTitle();
  histogram->GetXaxis()->SetTitleOffset(1.2);
  histogram->GetYaxis()->SetTitleOffset(1.5);
}

TH1D* extractCorrelationFuncton(string fileName) {
  TFile *file = new TFile((rootPathForCFs + fileName).c_str());
  TH1D *numerator = (TH1D*) file->Get("cnumn1da");
  TH1D *denominator = (TH1D*) file->Get("cdenn1da");

  // numerator->Divide(numerator,denominator,1.,1.,"B");
  numerator->Divide(denominator);
  return numerator;
}


pipiVskt() {
  preparePad();

  vector<TH1D*> cf;
  cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfilecf52a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfilecf53a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfilecf54a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfilecf55a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfilecf56a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfilecf57a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfilecf58a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfilecf59a.root"));

  vector<string> labels;
  labels.push_back(string("k_{T} #in [ 0.2 ; 0.3 ] GeV / c"));
  labels.push_back(string("k_{T} #in ( 0.3 ; 0.4 ] GeV / c"));
  // labels.push_back(string("k_{T} #in ( 0.4 ; 0.5 ] GeV / c"));
  labels.push_back(string("k_{T} #in ( 0.5 ; 0.6 ] GeV / c"));
  // labels.push_back(string("k_{T} #in ( 0.6 ; 0.7 ] GeV / c"));
  labels.push_back(string("k_{T} #in ( 0.7 ; 0.8 ] GeV / c"));
  // labels.push_back(string("k_{T} #in ( 0.8 ; 1.0 ] GeV / c"));
  // labels.push_back(string("k_{T} #in ( 1.0 ; 1.2 ] GeV / c"));

  TMarker *m1;
  TLatex latex;
  latex.SetTextSize(latexTextSizeSmall);
  for(ushort i = 0; i < cf.size(); ++i) {
    changeHistogramStyle(cf[i],i,0);
    cf[i]->SetAxisRange(0.04,0.15,"X");
    cf[i]->SetAxisRange(1,1.2,"Y");
    cf[i]->SetTitle(";q_{inv} [GeV / c];C(q)");
    cf[i]->Draw("SAME");
    if( i == 0)
      latex.Draw();
    // label
    float xpos = 0.088;
    float ypos = 1.17-i*0.012;
    m1 = new TMarker(xpos, ypos, markers[i]);
    m1->SetMarkerSize(1.2);
    m1->SetMarkerColor(colors[i]);
    m1->Draw();
    latex.DrawLatex(xpos + 0.003, ypos - 0.002, labels[i].c_str());
  }
  latex.SetTextSize(latexTextSizeBig);
  latex.DrawLatex(0.11,1.182,"#pi-#pi");
  return 0;
}

kkVskt() {
  preparePad();

  vector<TH1D*> cf;
  cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfilekkcf53a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfilekkcf54a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfilekkcf55a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfilekkcf56a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfilekkcf57a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfilekkcf58a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfilekkcf59a.root"));

  vector<string> labels;
  labels.push_back(string("k_{T} #in [ 0.3 ; 0.4 ] GeV / c"));
  // labels.push_back(string("k_{T} #in ( 0.4 ; 0.5 ] GeV / c"));
  // labels.push_back(string("k_{T} #in ( 0.5 ; 0.6 ] GeV / c"));
  labels.push_back(string("k_{T} #in ( 0.6 ; 0.7 ] GeV / c"));
  // labels.push_back(string("k_{T} #in ( 0.7 ; 0.8 ] GeV / c"));
  labels.push_back(string("k_{T} #in ( 0.8 ; 1.0 ] GeV / c"));
  // labels.push_back(string("k_{T} #in ( 1.0 ; 1.2 ] GeV / c"));

  TMarker *m1;
  TLatex latex;
  latex.SetTextSize(latexTextSizeSmall);
  for(ushort i = 0; i < cf.size(); ++i) {
    changeHistogramStyle(cf[i],i,0);
    cf[i]->SetAxisRange(0.06,0.22,"X");
    cf[i]->SetAxisRange(0.99,1.4,"Y");
    cf[i]->SetTitle(";q_{inv} [GeV / c];C(q)");
    cf[i]->Draw("SAME");
    if( i == 0)
      latex.Draw();
    // label
    float xpos = 0.135;
    float ypos = 1.34-i*0.021;
    m1 = new TMarker(xpos, ypos, markers[i]);
    m1->SetMarkerSize(1.2);
    m1->SetMarkerColor(colors[i]);
    m1->Draw();
    latex.DrawLatex(xpos + 0.005, ypos - 0.003, labels[i].c_str());
  }
  latex.SetTextSize(latexTextSizeBig);
  latex.DrawLatex(0.16,1.36,"K-K");
  return 0;
}

ppVskt() {
  preparePad();

  vector<TH1D*> cf;
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfileppcf54a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfileppcf55a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfileppcf56a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfileppcf57a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfileppcf58a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfileppcf59a.root"));

  vector<string> labels;
  // labels.push_back(string("k_{T} #in ( 0.4 ; 0.5 ] GeV / c"));
  labels.push_back(string("k_{T} #in ( 0.5 ; 0.6 ] GeV / c"));
  labels.push_back(string("k_{T} #in ( 0.6 ; 0.7 ] GeV / c"));
  // labels.push_back(string("k_{T} #in ( 0.7 ; 0.8 ] GeV / c"));
  labels.push_back(string("k_{T} #in ( 0.8 ; 1.0 ] GeV / c"));
  labels.push_back(string("k_{T} #in ( 1.0 ; 1.2 ] GeV / c"));

  TMarker *m1;
  TLatex latex;
  latex.SetTextSize(latexTextSizeSmall);
  for(ushort i = 0; i < cf.size(); ++i) {
    changeHistogramStyle(cf[i],i,0);
    cf[i]->SetAxisRange(0.07,0.22,"X");
    cf[i]->SetAxisRange(0.7,1,"Y");
    cf[i]->SetTitle(";q_{inv} [GeV / c];C(q)");
    cf[i]->Draw("SAME");
    if( i == 0)
      latex.Draw();
    // label
    float xpos = 0.14;
    float ypos = 0.8-i*0.016;
    m1 = new TMarker(xpos, ypos, markers[i]);
    m1->SetMarkerSize(1.2);
    m1->SetMarkerColor(colors[i]);
    m1->Draw();
    latex.DrawLatex(xpos + 0.005, ypos - 0.003, labels[i].c_str());
  }
  latex.SetTextSize(latexTextSizeBig);
  latex.DrawLatex(0.17,0.82,"p-p");
  return 0;
}

/*** CENTRALITY DEPENDENCE ***/

pipiVsctr() {
  preparePad();

  vector<TH1D*> cf;
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb2/outfilecf52a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb3/outfilecf52a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb5.7/outfilecf52a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb7.4/outfilecf52a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb8.7/outfilecf52a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb9.9/outfilecf52a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb10.9/outfilecf52a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfilecf52a.root"));

  vector<string> labels;
  // labels.push_back(string("0-5%"));
  labels.push_back(string("0-10%"));
  labels.push_back(string("10-20%"));
  labels.push_back(string("20-30%"));
  labels.push_back(string("30-40%"));
  labels.push_back(string("40-50%"));
  labels.push_back(string("50-60%"));
  labels.push_back(string("60-70%"));

  TMarker *m1;
  TLatex latex;
  latex.SetTextSize(latexTextSizeSmall);
  for(ushort i = 0; i < cf.size(); ++i) {
    changeHistogramStyle(cf[i],i,0);
    cf[i]->SetAxisRange(0,0.1,"X");
    cf[i]->SetAxisRange(1,1.9,"Y");
    cf[i]->SetTitle(";q_{inv} [GeV / c];C(q)");
    cf[i]->Draw("SAME");
    if( i == 0)
      latex.Draw();
    // label
    float xpos = 0.06;
    float ypos = 1.7-i*0.05;
    m1 = new TMarker(xpos, ypos, markers[i]);
    m1->SetMarkerSize(1.2);
    m1->SetMarkerColor(colors[i]);
    m1->Draw();
    latex.DrawLatex(xpos + 0.003, ypos - 0.01, labels[i].c_str());
  }
  latex.SetTextSize(latexTextSizeBig);
  latex.DrawLatex(0.065,1.75,"#pi-#pi");
  return 0;
}

kkVsctr() {
  preparePad();

  vector<TH1D*> cf;
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb2/outfilekkcf53a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb3/outfilekkcf53a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb5.7/outfilekkcf53a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb7.4/outfilekkcf53a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb8.7/outfilekkcf53a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb9.9/outfilekkcf53a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb10.9/outfilekkcf53a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfilekkcf53a.root"));

  vector<string> labels;
  // labels.push_back(string("0-5%"));
  labels.push_back(string("0-10%"));
  labels.push_back(string("10-20%"));
  labels.push_back(string("20-30%"));
  labels.push_back(string("30-40%"));
  // labels.push_back(string("40-50%"));
  labels.push_back(string("50-60%"));
  labels.push_back(string("60-70%"));

  TMarker *m1;
  TLatex latex;
  latex.SetTextSize(latexTextSizeSmall);
  for(ushort i = 0; i < cf.size(); ++i) {
    changeHistogramStyle(cf[i],i,0);
    cf[i]->SetAxisRange(0,0.17,"X");
    cf[i]->SetAxisRange(1,2,"Y");
    cf[i]->SetTitle(";q_{inv} [GeV / c];C(q)");
    cf[i]->SetNdivisions(505);
    cf[i]->Draw("SAME");
    if( i == 0)
      latex.Draw();
    // label
    float xpos = 0.12;
    float ypos = 1.7-i*0.05;
    m1 = new TMarker(xpos, ypos, markers[i]);
    m1->SetMarkerSize(1.2);
    m1->SetMarkerColor(colors[i]);
    m1->Draw();
    latex.DrawLatex(xpos + 0.003, ypos - 0.01, labels[i].c_str());
  }
  latex.SetTextSize(latexTextSizeBig);
  latex.DrawLatex(0.125,1.75,"K-K");
  return 0;
}

ppVsctr() {
  preparePad();

  vector<TH1D*> cf;
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb2/outfileppcf55a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb3/outfileppcf55a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb5.7/outfileppcf55a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb7.4/outfileppcf55a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb8.7/outfileppcf55a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb9.9/outfileppcf55a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb10.9/outfileppcf55a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfileppcf55a.root"));

  vector<string> labels;
  // labels.push_back(string("0-5%"));
  labels.push_back(string("0-10%"));
  labels.push_back(string("10-20%"));
  labels.push_back(string("20-30%"));
  labels.push_back(string("30-40%"));
  labels.push_back(string("40-50%"));
  labels.push_back(string("50-60%"));
  labels.push_back(string("60-70%"));

  TMarker *m1;
  TLatex latex;
  latex.SetTextSize(latexTextSizeSmall);
  for(ushort i = 0; i < cf.size(); ++i) {
    changeHistogramStyle(cf[i],i,0);
    cf[i]->SetAxisRange(0,0.2,"X");
    cf[i]->SetAxisRange(0.5,1,"Y");
    cf[i]->SetTitle(";q_{inv} [GeV / c];C(q)");
    cf[i]->SetNdivisions(505);
    cf[i]->Draw("SAME");
    if( i == 0)
      latex.Draw();
    // label
    float xpos = 0.14;
    float ypos = 0.72-i*0.03;
    m1 = new TMarker(xpos, ypos, markers[i]);
    m1->SetMarkerSize(1.2);
    m1->SetMarkerColor(colors[i]);
    m1->Draw();
    latex.DrawLatex(xpos + 0.005, ypos - 0.01, labels[i].c_str());
  }
  latex.SetTextSize(latexTextSizeBig);
  latex.DrawLatex(0.145,0.75,"p-p");
  return 0;
}

/******************/
/******************/
/******************/


void ktDependence() {
  TCanvas *canvas = new TCanvas("vskt","vskt",1000, 400);
  canvas->Divide(3,1);
  canvas->cd(1);
  pipiVskt();
  canvas->cd(2);
  kkVskt();
  canvas->cd(3);
  ppVskt();
  canvas->SaveAs("cfvskt.eps");
}

centralityDependence() {
  TCanvas *canvas = new TCanvas("vsctr","vsctr",1000, 400);
  canvas->Divide(3,1);
  canvas->cd(1);
  pipiVsctr();
  canvas->cd(2);
  kkVsctr();
  canvas->cd(3);
  ppVsctr();
  canvas->SaveAs("cfvsctr.eps");
}

int cf1DAllCentralities() {
  // ktDependence();
  centralityDependence();
  return 0;
}