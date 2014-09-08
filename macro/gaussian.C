#include <TMath.h>
#include <TF1.h>
#include <TF2.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TPad.h>
#include <TLegend.h>

TF2 *func;

double myFunc(double *x, double *params)
{
	// x[0] - theta
	// x[1] - phi
	double 
		th = x[0],
		phi = x[1],
		r = params[0],
		Rout = params[1],
		Rside = params[2],
		Rlong = params[3],
		x2 = TMath::Power(TMath::Sin(th)*TMath::Cos(phi),2),
		y2 = TMath::Power(TMath::Sin(th)*TMath::Sin(phi),2),
		z2 = TMath::Power(TMath::Cos(th),2);
	return TMath::Exp(-r*r*(
			x2/Rout/Rout
			+ y2/Rside/Rside
			+ z2/Rlong/Rlong
		))*r*r*TMath::Sin(th);	
}

double integral(double *x,double *params)
{
	double r = x[0];
	func->SetParameter(0,x[0]);
	if(r > 0)
		return func->Integral(0,TMath::Pi(),0,2*TMath::Pi())/12.7/r/r;
	else
		return 1.0;
}

int gaussian()
{
	TCanvas *canv = new TCanvas("window","3dgauss",500,300);
	gStyle->SetOptStat(0);
	gPad->SetTopMargin(0.01);
	gPad->SetRightMargin(0.01);
	gPad->SetLeftMargin(0.08);
	gPad->SetBottomMargin(0.08);
	func = new TF2("gauss3d",myFunc,-0.1,4,-0.1,7,4);
	func->SetParameter(1,400);
	func->SetParameter(2,0.4);
	func->SetParameter(3,40);
	TF1* intgrl = new TF1("intgrl", integral, 0,5,0);
	TF1* gauss2 = new TF1("gauss","TMath::Gaus(x,0,[0])",0,5);
	gauss2->SetParameter(0,2);
	gauss2->Draw();
	intgrl->Draw("P");
	gauss2->SetLineColor(1);
	gauss2->SetLineStyle(2);
	intgrl->SetLineColor(0);
	intgrl->SetLineStyle(1);
	intgrl->GetHistogram()->Fit(gauss2);
	intgrl->Draw("SAME");
	intgrl->SetLineColor(1);

	intgrl->GetXaxis()->SetTickLength(0);
	intgrl->GetXaxis()->SetNdivisions(10);
	intgrl->GetYaxis()->SetTickLength(0.02);
	intgrl->GetYaxis()->SetNdivisions(2);
	intgrl->GetXaxis()->SetLabelColor(0);
	// intgrl->GetYaxis()->SetLabelColor(0);
	intgrl->SetTitle(";r;S(r)");
	intgrl->GetXaxis()->SetTitleSize(0.08);
	intgrl->GetXaxis()->SetLabelSize(0.001);
	intgrl->GetXaxis()->SetTitleOffset(0.3);
	intgrl->GetYaxis()->SetTitleSize(0.08);
	intgrl->GetYaxis()->SetLabelSize(0.08);
	intgrl->GetYaxis()->SetTitleOffset(0.4);
	intgrl->GetYaxis()->SetRangeUser(0,1.05);

	TLegend *leg = new TLegend(0.5,0.8,0.99,0.99);
	leg->AddEntry(intgrl, "Averaged 3D Gaussian","L");
	leg->AddEntry(gauss2, "1D Gaussian fit","L");
	leg->Draw();
	canv->SaveAs("3dg21d.eps");
	return 0;
}
