#include <iostream>
#include <string>
#include <vector>
#include <TCanvas.h>
#include <TH1D.h>
#include <TGraphErrors.h>
#include <TPad.h>
#include <TStyle.h>
#include <TFile.h>
#include <TMarker.h>
#include <TLatex.h>
#include <TLegend.h>

using namespace std;

typedef unsigned short int ushort;

const string rootPathForCFs = "../../../tpi_output/";

const float latexTextSizeSmall = 0.04;
const float latexTextSizeBig = 0.06;

const int colors[9] = { kRed, kBlue, kOrange-1, kGreen+2, kViolet+1, kRed+3, kCyan+1, kCyan+2, kOrange-3 };
const int markers[9] = { 21, 24, 33, 25, 20, 27, 34, 30, 34 };

typedef struct
{
  TGraph* c00;
  TGraph* c20;
  TGraph* c22;
} CF3D;

void preparePad(bool isUpper = true)
{
  gPad->SetFillColor(0);
  gPad->SetFillStyle(4000);
  if(isUpper)
  {
    gPad->SetTopMargin(0.016);
    gPad->SetBottomMargin(0.17);
  } else {
    gPad->SetTopMargin(0.019);
    gPad->SetBottomMargin(0.13);
  }
  gPad->SetRightMargin(0.04);
  gPad->SetLeftMargin(0.18);
  gPad->SetTickx(1);
  gPad->SetTicky(1);
  gStyle->SetOptStat(1000000000);
  gStyle->SetStatBorderSize(0);
  gStyle->SetTextFont(42);
}

void changeHistogramStyle(TGraph *histogram,  ushort ht, ushort ct) {
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
  histogram->GetYaxis()->SetTitleOffset(1.8);
  histogram->SetLineWidth(2);
}

CF3D extractCorrelationFuncton(string fileName) {
  CF3D correlationFunction;
  TFile *file = new TFile((rootPathForCFs + fileName).c_str());
  correlationFunction.c00 = new TGraph((TH1D*) file->Get("CfnReYlm00IdLCYlm"));
  correlationFunction.c20 = new TGraph((TH1D*) file->Get("CfnReYlm20IdLCYlm"));
  correlationFunction.c22 = new TGraph((TH1D*) file->Get("CfnReYlm22IdLCYlm"));

  return correlationFunction;
}


void drawPions() {
  TCanvas *canvas = new TCanvas("sphpi","sphpi",800, 800);
  TPad 
    *pad1 = new TPad("pipipad1","foo",0.0,0.5,0.5,1.0),
    *pad2 = new TPad("pipipad2","foo",0.5,0.5,1,1),
    *pad3 = new TPad("pipipad3","foo",0.25,0.0,0.75,0.5);
  pad1->Draw();
  pad2->Draw();
  pad3->Draw();

  vector<CF3D> cf;
  cf.push_back(extractCorrelationFuncton("lhyquid3vb3/outfilecf52a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb5.7/outfilecf52a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb7.4/outfilecf52a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb8.7/outfilecf52a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb9.9/outfilecf52a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb10.9/outfilecf52a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfilecf52a.root"));

  vector<string> labels;
  labels.push_back(string("0-10%"));
  // labels.push_back(string("10-20%"));
  // labels.push_back(string("20-30%"));
  labels.push_back(string("30-40%"));
  // labels.push_back(string("40-50%"));
  labels.push_back(string("50-60%"));
  labels.push_back(string("60-70%"));

  TLatex latex;
  latex.SetTextSize(latexTextSizeSmall);
  TLegend *legend = new TLegend(0.7,0.55, 0.9,0.85);
  for(ushort i = 0; i < cf.size(); ++i) {
    /* c00 */
    pad1->cd();
    preparePad();
    changeHistogramStyle(cf[i].c00,i,0);
    cf[i].c00->GetXaxis()->SetLimits(0,0.15);
    cf[i].c00->GetYaxis()->SetRangeUser(1,2);
    cf[i].c00->SetTitle(";q_{LCMS} [GeV / c];#Rgothic C_{0}^{0}(q)");
    if( i == 0) {
      cf[i].c00->Draw("AL");
    } else {
      cf[i].c00->Draw("SAME");
    }
    legend->AddEntry(cf[i].c00, labels[i].c_str(),"l");

    /* c20 */
    pad2->cd();
    preparePad();
    changeHistogramStyle(cf[i].c20,i,0);
    cf[i].c20->GetXaxis()->SetLimits(0,0.14);
    cf[i].c20->GetYaxis()->SetRangeUser(-0.075,0);    
    cf[i].c20->SetTitle(";q_{LCMS} [GeV / c];#Rgothic C_{2}^{0}(q)");
    if( i == 0) {
      cf[i].c20->Draw("AL");
    } else {
      cf[i].c20->Draw("SAME");
    }

    /* c22 */
    pad3->cd();
    preparePad(false);
    changeHistogramStyle(cf[i].c22,i,0);
    cf[i].c22->GetXaxis()->SetLimits(0,0.14);
    cf[i].c22->GetYaxis()->SetRangeUser(-0.03,0.005);
    cf[i].c22->SetTitle(";q_{LCMS} [GeV / c];#Rgothic C_{2}^{2}(q)");
    if( i == 0) {
      cf[i].c22->Draw("AL");
    } else {
      cf[i].c22->Draw("SAME");
    }
  }
  pad1->cd();
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->SetTextSize(latexTextSizeSmall);
  legend->Draw();
  pad1->cd();
  latex.SetTextSize(latexTextSizeBig);
  latex.DrawLatex(0.105,1.85,"#pi-#pi");
  canvas->SaveAs("cf3dpi.eps");
}

void drawKaons() {
  TCanvas *canvas = new TCanvas("sphk","sphk",800, 800);
  TPad 
    *pad1 = new TPad("kkpad1","foo",0.0,0.5,0.5,1.0),
    *pad2 = new TPad("kkpad2","foo",0.5,0.5,1,1),
    *pad3 = new TPad("kkpad3","foo",0.25,0.0,0.75,0.5);
  pad1->Draw();
  pad2->Draw();
  pad3->Draw();

  vector<CF3D> cf;
  cf.push_back(extractCorrelationFuncton("lhyquid3vb3/outfilekkcf53a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb5.7/outfilekkcf53a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb7.4/outfilekkcf53a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb8.7/outfilekkcf53a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb9.9/outfilekkcf53a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb10.9/outfilekkcf53a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfilekkcf53a.root"));

  vector<string> labels;
  labels.push_back(string("0-10%"));
  // labels.push_back(string("10-20%"));
  labels.push_back(string("20-30%"));
  // labels.push_back(string("30-40%"));
  // labels.push_back(string("40-50%"));
  // labels.push_back(string("50-60%"));
  labels.push_back(string("60-70%"));

  TLatex latex;
  latex.SetTextSize(latexTextSizeSmall);
  TLegend *legend = new TLegend(0.7,0.55, 0.9,0.85);
  for(ushort i = 0; i < cf.size(); ++i) {
    /* c00 */
    pad1->cd();
    preparePad();
    changeHistogramStyle(cf[i].c00,i,0);
    cf[i].c00->GetXaxis()->SetLimits(0,0.15);
    cf[i].c00->GetYaxis()->SetRangeUser(1,2);
    cf[i].c00->SetTitle(";q_{LCMS} [GeV / c];#Rgothic C_{0}^{0}(q)");
    if( i == 0) {
      cf[i].c00->Draw("AL");
    } else {
      cf[i].c00->Draw("SAME");
    }
    legend->AddEntry(cf[i].c00, labels[i].c_str(),"l");

    /* c20 */
    pad2->cd();
    preparePad(false);
    changeHistogramStyle(cf[i].c20,i,0);
    cf[i].c20->GetXaxis()->SetLimits(0,0.2);
    cf[i].c20->GetYaxis()->SetRangeUser(-0.08,0.01);    
    cf[i].c20->SetTitle(";q_{LCMS} [GeV / c];#Rgothic C_{2}^{0}(q)");
    if( i == 0) {
      cf[i].c20->Draw("AL");
    } else {
      cf[i].c20->Draw("SAME");
    }

    /* c22 */
    pad3->cd();
    preparePad();
    changeHistogramStyle(cf[i].c22,i,0);
    cf[i].c22->GetXaxis()->SetLimits(0,0.2);
    cf[i].c22->GetYaxis()->SetRangeUser(-0.015,0.015);
    cf[i].c22->SetTitle(";q_{LCMS} [GeV / c];#Rgothic C_{2}^{2}(q)");
    if( i == 0) {
      cf[i].c22->Draw("AL");
    } else {
      cf[i].c22->Draw("SAME");
    }
  }
  pad1->cd();
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->SetTextSize(latexTextSizeSmall);
  legend->Draw();
  pad1->cd();
  latex.SetTextSize(latexTextSizeBig);
  latex.DrawLatex(0.105,1.85,"K-K");
  canvas->SaveAs("cf3dk.eps");
}


void drawProtons() {
  TCanvas *canvas = new TCanvas("sphp","sphp",800, 800);
  TPad 
    *pad1 = new TPad("pppad1","foo",0.0,0.5,0.5,1.0),
    *pad2 = new TPad("pppad2","foo",0.5,0.5,1,1),
    *pad3 = new TPad("pppad3","foo",0.25,0.0,0.75,0.5);
  pad1->Draw();
  pad2->Draw();
  pad3->Draw();

  vector<CF3D> cf;
  cf.push_back(extractCorrelationFuncton("lhyquid3vb3/outfileppcf55a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb5.7/outfileppcf55a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb7.4/outfileppcf55a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb8.7/outfileppcf55a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb9.9/outfileppcf55a.root"));
  // cf.push_back(extractCorrelationFuncton("lhyquid3vb10.9/outfileppcf55a.root"));
  cf.push_back(extractCorrelationFuncton("lhyquid3vb11.9/outfileppcf55a.root"));

  vector<string> labels;
  labels.push_back(string("0-10%"));
  labels.push_back(string("10-20%"));
  // labels.push_back(string("20-30%"));
  // labels.push_back(string("30-40%"));
  // labels.push_back(string("40-50%"));
  // labels.push_back(string("50-60%"));
  labels.push_back(string("60-70%"));

  TLatex latex;
  latex.SetTextSize(latexTextSizeSmall);
  TLegend *legend = new TLegend(0.7,0.2, 0.9,0.5);
  for(ushort i = 0; i < cf.size(); ++i) {
    /* c00 */
    pad1->cd();
    preparePad();
    changeHistogramStyle(cf[i].c00,i,0);
    cf[i].c00->GetXaxis()->SetLimits(0,0.15);
    cf[i].c00->GetYaxis()->SetRangeUser(0,1);
    cf[i].c00->SetTitle(";q_{LCMS} [GeV / c];#Rgothic C_{0}^{0}(q)");
    if( i == 0) {
      cf[i].c00->Draw("AL");
    } else {
      cf[i].c00->Draw("SAME");
    }
    legend->AddEntry(cf[i].c00, labels[i].c_str(),"l");

    /* c20 */
    pad2->cd();
    preparePad();
    changeHistogramStyle(cf[i].c20,i,0);
    cf[i].c20->GetXaxis()->SetLimits(0,0.15);
    cf[i].c20->GetYaxis()->SetRangeUser(-0.08,0.08);    
    cf[i].c20->SetTitle(";q_{LCMS} [GeV / c];#Rgothic C_{2}^{0}(q)");
    if( i == 0) {
      cf[i].c20->Draw("AL");
    } else {
      cf[i].c20->Draw("SAME");
    }

    /* c22 */
    pad3->cd();
    preparePad(false);
    changeHistogramStyle(cf[i].c22,i,0);
    cf[i].c22->GetXaxis()->SetLimits(0,0.15);
    cf[i].c22->GetYaxis()->SetRangeUser(-0.06,0.06);
    cf[i].c22->SetTitle(";q_{LCMS} [GeV / c];#Rgothic C_{2}^{2}(q)");
    if( i == 0) {
      cf[i].c22->Draw("AL");
    } else {
      cf[i].c22->Draw("SAME");
    }
  }
  pad1->cd();
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->SetTextSize(latexTextSizeSmall);
  legend->Draw();
  pad1->cd();
  latex.SetTextSize(latexTextSizeBig);
  latex.DrawLatex(0.105,0.45,"p-p");
  canvas->SaveAs("cf3dp.eps");
}

void plotSphericalHarmonics() {

  drawPions();
  drawKaons();
  drawProtons();
}

int cf3DAllCentralities() {
  plotSphericalHarmonics();
  return 0;
}

int main() {
  cf3DAllCentralities();
  return 0;
}