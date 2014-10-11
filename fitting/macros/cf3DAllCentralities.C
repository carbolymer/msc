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
const float latexTextSizeBig = 0.08;

const int colors[9] = { kRed, kBlue, kOrange-1, kGreen+2, kViolet+1, kRed+3, kCyan+1, kCyan+2, kOrange-3 };
const int markers[9] = { 21, 24, 33, 25, 20, 27, 34, 30, 34 };

typedef struct
{
  TGraph* c00;
  TGraph* c20;
  TGraph* c22;
} CF3D;

void preparePad()
{
  gPad->SetFillColor(0);
  gPad->SetFillStyle(4000);
  gPad->SetTopMargin(0.05);
  gPad->SetRightMargin(0.005);
  gPad->SetBottomMargin(0.13);
  gPad->SetLeftMargin(0.17);
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
  histogram->GetYaxis()->SetTitleOffset(1.7);
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
  TCanvas *canvas = new TCanvas("sphpi","sphpi",1000, 500);
  canvas->Divide(3,1);

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
  TLegend *legend = new TLegend(0.7,0.6, 0.9,0.9);
  for(ushort i = 0; i < cf.size(); ++i) {
    /* c00 */
    canvas->cd(1);
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
    canvas->cd(2);
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
    canvas->cd(3);
    preparePad();
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
  canvas->cd(1);
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->SetTextSize(latexTextSizeSmall);
  legend->Draw();
  canvas->cd(2);
  latex.SetTextSize(latexTextSizeBig);
  latex.DrawLatex(0.06,0.002,"#pi-#pi");
  canvas->SaveAs("cf3dpi.eps");
}

void drawKaons() {
  TCanvas *canvas = new TCanvas("sphk","sphk",1000, 500);
  canvas->Divide(3,1);

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
  TLegend *legend = new TLegend(0.7,0.6, 0.9,0.9);
  for(ushort i = 0; i < cf.size(); ++i) {
    /* c00 */
    canvas->cd(1);
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
    canvas->cd(2);
    preparePad();
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
    canvas->cd(3);
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
  canvas->cd(1);
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->SetTextSize(latexTextSizeSmall);
  legend->Draw();
  canvas->cd(2);
  latex.SetTextSize(latexTextSizeBig);
  latex.DrawLatex(0.06,0.011,"K-K");
  canvas->SaveAs("cf3dk.eps");
}


void drawProtons() {
  TCanvas *canvas = new TCanvas("sphk","sphk",1000, 500);
  canvas->Divide(3,1);

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
    canvas->cd(1);
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
    canvas->cd(2);
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
    canvas->cd(3);
    preparePad();
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
  canvas->cd(1);
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->SetTextSize(latexTextSizeSmall);
  legend->Draw();
  canvas->cd(2);
  latex.SetTextSize(latexTextSizeBig);
  latex.DrawLatex(0.06,0.083,"p-p");
  canvas->SaveAs("cf3dp.eps");
}

void plotSphericalHarmonics() {

  // drawPions();
  // drawKaons();
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