#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <TCanvas.h>
#include <TF1.h>
#include <TMath.h>
#include <TStyle.h>
#include <TRandom.h>
#include "../src/MultiPlot.cxx"
#include "../src/MultiFitPlot.cxx"

using namespace std;

#define DEBUG false

const Double_t particleMasses[3] = { // in GeV / c^2
	0.493677,// kaon
	0.1349766, // pion
	0.938272029 // proton
};

const int numberOfCentralities = 8;

void createLcmsPlot(std::string *graphNames, std::string *prefixes, const char *fileName,
			MultiFitPlot &Rinv, MultiFitPlot &Rlcms, unsigned short int periphericity = 0);
void createPrfPlotAllInONe(MultiFitPlot Rinv[], MultiFitPlot Rlcms[], int nCentralities);
void createPrfPlots(MultiFitPlot Rinv[], MultiFitPlot Rlcms[], int nCentralities);
void fillGraph(std::string fileName, TGraphErrors *graph, unsigned int iParticle, bool isInvariant = kFALSE);

int main()
{
	gStyle->SetOptStat(0);
	gStyle->SetLabelSize(0.06, "xyz");
	gStyle->SetPadTopMargin(0.023);
	gStyle->SetPadBottomMargin(0.16);
	gStyle->SetPadLeftMargin(0.14);
	gStyle->SetPadRightMargin(0.001);

	MultiFitPlot::defaultFunction  = new TF1("fit","[0]*TMath::Power(x,-[1])", 0.1, 1.5);
	MultiFitPlot::defaultFunction->SetParameter(0,1);
	MultiFitPlot::defaultFunction->SetParameter(1,1);

	MultiFitPlot qqq;
	qqq.GetDispersionPlot(MultiFitPlot::Normalization);

	return 0;


	std::string prefixes[graphCount];
	std::string graphNames[graphCount];

	MultiFitPlot Rinv[numberOfCentralities];
	MultiFitPlot Rlcms[numberOfCentralities];

	prefixes[kaon] = "b2.3/kk";
	prefixes[pion] = "b2.3/pipi";
	prefixes[proton] = "b2.3/pp";
	graphNames[kaon] = "K-K b = 2.3 fm";
	graphNames[pion] = "#pi-#pi b = 2.3 fm";
	graphNames[proton] = "p-p b = 2.3 fm";
	createLcmsPlot(graphNames, prefixes, "b2.3", Rinv[0], Rlcms[0]);

	prefixes[kaon] = "b3.1/kk";
	prefixes[pion] = "b3.1/pipi";
	prefixes[proton] = "b3.1/pp";
	graphNames[kaon] = "K-K b = 3.1 fm";
	graphNames[pion] = "#pi-#pi b = 3.1 fm";
	graphNames[proton] = "p-p b = 3.1 fm";
	createLcmsPlot(graphNames, prefixes, "b3.1", Rinv[1], Rlcms[1]);

	prefixes[kaon] = "b5.7/kk";
	prefixes[pion] = "b5.7/pipi";
	prefixes[proton] = "b5.7/pp";
	graphNames[kaon] = "K-K b = 5.7 fm";
	graphNames[pion] = "#pi-#pi b = 5.7 fm";
	graphNames[proton] = "p-p b = 5.7 fm";
	createLcmsPlot(graphNames, prefixes, "b5.7", Rinv[2], Rlcms[2]);

	prefixes[kaon] = "b7.4/kk";
	prefixes[pion] = "b7.4/pipi";
	prefixes[proton] = "b7.4/pp";
	graphNames[kaon] = "K-K b=7.4 fm";
	graphNames[pion] = "#pi-#pi b=7.4 fm";
	graphNames[proton] = "p-p b=7.4 fm";
	createLcmsPlot(graphNames, prefixes, "b7.4", Rinv[3], Rlcms[3], 1);	

	prefixes[kaon] = "b8.7/kk";
	prefixes[pion] = "b8.7/pipi";
	prefixes[proton] = "b8.7/pp";
	graphNames[kaon] = "K-K b=8.7 fm";
	graphNames[pion] = "#pi-#pi b=8.7 fm";
	graphNames[proton] = "p-p b=8.7 fm";
	createLcmsPlot(graphNames, prefixes, "b8.7", Rinv[4], Rlcms[4], 1);

	prefixes[kaon] = "b9.9/kk";
	prefixes[pion] = "b9.9/pipi";
	prefixes[proton] = "b9.9/pp";
	graphNames[kaon] = "K-K b=9.9 fm";
	graphNames[pion] = "#pi-#pi b=9.9 fm";
	graphNames[proton] = "p-p b=9.9 fm";
	createLcmsPlot(graphNames, prefixes, "b9.9", Rinv[5], Rlcms[5], 1);

	prefixes[kaon] = "b10.9/kk";
	prefixes[pion] = "b10.9/pipi";
	prefixes[proton] = "b10.9/pp";
	graphNames[kaon] = "K-K b=10.9 fm";
	graphNames[pion] = "#pi-#pi b=10.9 fm";
	graphNames[proton] = "p-p b=10.9 fm";
	createLcmsPlot(graphNames, prefixes, "b10.9", Rinv[6], Rlcms[6], 2);

	prefixes[kaon] = "b11.9/kk";
	prefixes[pion] = "b11.9/pipi";
	prefixes[proton] = "b11.9/pp";
	graphNames[kaon] = "K-K b=11.9 fm";
	graphNames[pion] = "#pi-#pi b=11.9 fm";
	graphNames[proton] = "p-p b=11.9 fm";
	createLcmsPlot(graphNames, prefixes, "b11.9", Rinv[7], Rlcms[7], 2);

	createPrfPlots(Rinv, Rlcms, numberOfCentralities);
	return 0;
}

void fillGraph(std::string fileName, TGraphErrors *graph, unsigned int iParticle, bool isInvariant)
{
	char buffer[256];
	std::ifstream infile(fileName.c_str(), std::ifstream::in);
	double kT, kTmin, kTmax, R, dR, mT, gamma;
	Int_t i;
	if(DEBUG) std::cout << std::endl << fileName << std::endl << "mT\tR\tdR" << std::endl;
	while(infile.good())
	{
		for(i=0; i < 256; ++i)
			buffer[i] = '\0';
    infile >> buffer;
    if(buffer[0] == '\0')
    	continue;

    std::stringstream(buffer) >> kTmin;
    infile >> kTmax;
    infile >> R;
		for(i=0; i < 256; ++i)
			buffer[i] = '\0';
    infile >> buffer;
    if(buffer[1] == 'n') // nan hax
    	dR = 0;
    else
    	std::stringstream(buffer) >> dR;
    if( R != R )
    	R = 0;
    if( dR != dR)
    	dR = 0;
    // cout << "\t" << fileName << "\t******** R:" << R << endl;
    // cout << "\t" << fileName << "\t******** dR:" << dR << endl;
    kT = (kTmax + kTmin)/2;
		mT = TMath::Sqrt(
			TMath::Power(kT,2)
			+ TMath::Power(particleMasses[iParticle],2));
		if(isInvariant)
		{
			gamma = 1/TMath::Sqrt(1-(kT/mT));
			R /= TMath::Sqrt( (TMath::Sqrt(gamma) + 2) / 3 );
		}
		if(DEBUG) std::cout << mT << "\t" << R << "\t+/- " << dR << std::endl;
    i = graph->GetN();
		graph->SetPoint(i, mT, R);
		graph->SetPointError(i, 0, dR);
	}
}

void createLcmsPlot(std::string *graphNames, std::string *prefixes, const char *fileName,
			MultiFitPlot &Rinv, MultiFitPlot &Rlcms, unsigned short int periphericity)
{
	TCanvas *canvas = new TCanvas("canvas", "R_LCMS", 900, 800);
	canvas->Divide(2,2);

	MultiFitPlot Rout(";m_{T} [GeV/c^{2}];R_{out} [fm]");

	switch( periphericity ) {
		case 1:
			Rout.yMin = 1.5;
			Rout.yMax = 9.5;
			break;
		case 2:
			Rout.yMin = 1.0;
			Rout.yMax = 6.5;
			break;
	}

	Rout.graphNames[kaon] = graphNames[kaon];
	Rout.graphNames[pion] = graphNames[pion];
	Rout.graphNames[proton] = graphNames[proton];

	Rout.theme[kaon].markerColor = 1;
	Rout.theme[kaon].lineColor = 1;
	Rout.theme[kaon].markerSize = 1.5;
	Rout.theme[kaon].markerStyle = 20;

	Rout.theme[pion].markerColor = 4;
	Rout.theme[pion].lineColor = 4;
	Rout.theme[pion].markerSize = 1.4;
	Rout.theme[pion].markerStyle = 21;

	Rout.theme[proton].markerColor = 2;
	Rout.theme[proton].lineColor = 2;
	Rout.theme[proton].markerSize = 1.5;
	Rout.theme[proton].markerStyle = 22;

	MultiFitPlot Rside(Rout);
	MultiFitPlot Rlong(Rout);
	Rlcms = MultiFitPlot(Rout);
	Rinv = MultiFitPlot(Rout);

	Rside.labels = ";m_{T} [GeV/c^{2}];R_{side} [fm]";
	Rlong.labels = ";m_{T} [GeV/c^{2}];R_{long} [fm]";
	Rlcms.labels = ";m_{T} [GeV/c^{2}];R_{LCMS} [fm]";
	Rinv.labels = ";m_{T} [GeV/c^{2}];R_{inv}/[(#sqrt{#gamma}+2)/3]^{1/2} [fm]";

	for(unsigned int j = 0; j < graphCount; ++j)
	{
		if(DEBUG) std::cout << std::endl << "  ###  " << prefixes[j] << "  ###   " << std::endl;
		fillGraph(std::string("data/") + prefixes[j] + std::string("_Rout.out"), Rout.graphs[j], j);
		fillGraph(std::string("data/") + prefixes[j] + std::string("_Rside.out"), Rside.graphs[j], j);
		fillGraph(std::string("data/") + prefixes[j] + std::string("_Rlong.out"), Rlong.graphs[j], j);
		fillGraph(std::string("data/") + prefixes[j] + std::string("_Rinv.out"), Rinv.graphs[j], j, kTRUE);
		fillGraph(std::string("data/") + prefixes[j] + std::string("_Rlcms.out"), Rlcms.graphs[j], j);
	}

	Rout.Fit();
	Rside.Fit();
	Rlong.Fit();
	Rlcms.Fit();
	Rinv.Fit();

	canvas->cd(1);
	Rout.Draw();

	canvas->cd(2);
	Rside.Draw();

	canvas->cd(3);
	Rlong.Draw();

	canvas->cd(4);
	Rlcms.Draw();


	canvas->SaveAs((std::string("output/")+fileName+std::string(".png")).c_str());
	delete canvas;

	// normalized
	Rout.labels = ";m_{T} [GeV/c^{2}];R_{out} / R_{out}^{FIT}";
	Rside.labels = ";m_{T} [GeV/c^{2}];R_{side} / R_{side}^{FIT}";
	Rlong.labels = ";m_{T} [GeV/c^{2}];R_{long} / R_{long}^{FIT}";
	Rlcms.labels = ";m_{T} [GeV/c^{2}];R_{LCMS} / R_{LCMS}^{FIT}";

	canvas = new TCanvas("canvas", "R_LCMS", 900, 800);
	canvas->SetGrid();
	gStyle->SetPadGridX(true);
	gStyle->SetPadGridY(true);
	canvas->Divide(2,2);
	canvas->cd(1);
	Rout.GetDispersionPlot(MultiFitPlot::Normalization).Draw();
	canvas->cd(2);
	Rside.GetDispersionPlot(MultiFitPlot::Normalization).Draw();
	canvas->cd(3);
	Rlong.GetDispersionPlot(MultiFitPlot::Normalization).Draw();
	canvas->cd(4);
	Rlcms.GetDispersionPlot(MultiFitPlot::Normalization).Draw();
	canvas->SaveAs((std::string("output/")+fileName+std::string("_div.png")).c_str());
	delete canvas;
	gStyle->SetPadGridX(false);
	gStyle->SetPadGridY(false);
}

void createPrfPlotAllInONe(MultiFitPlot *Rinv, MultiFitPlot *Rlcms, int nCentralities)
{
	TCanvas *canvas = new TCanvas("canvas", "R_LCMS", 2500, 600);
	canvas->Divide(nCentralities, 2);

	for(int i = 1; i <= nCentralities; ++i)
	{
		canvas->cd(i);
		Rinv[i-1].Draw();
		Rinv[i-1].Fit();
		canvas->cd(i+nCentralities);
		Rlcms[i-1].Draw();
	}
	canvas->SaveAs("output/all.png");
	delete canvas;
}

void createPrfPlots(MultiFitPlot *Rinv, MultiFitPlot *Rlcms, int nCentralities)
{
	TCanvas *canvas = 0;

	// therminator central
	nCentralities = 4;
	canvas = new TCanvas("canvas", "R_LCMS", 1200, 600);
	canvas->Divide(nCentralities, 2);
	for(int i = 1; i <= nCentralities; ++i)
	{
		canvas->cd(i);
		Rinv[i-1].Draw();
		Rinv[i-1].Fit();
		canvas->cd(i+nCentralities);
		Rlcms[i-1].labels = ";m_{T} [GeV/c^{2}];R_{LCMS} [fm]";
		Rlcms[i-1].Draw();
	}
	canvas->SaveAs("output/therminator_central.png");
	delete canvas;

	// therminator peripheral
	nCentralities = 4;
	canvas = new TCanvas("canvas", "R_LCMS", 1200, 600);
	canvas->Divide(nCentralities, 2);
	for(int i = 1; i <= nCentralities; ++i)
	{
		canvas->cd(i);
		Rinv[i-1+4].yMin = 1.5;
		Rinv[i-1+4].yMax = 7;
		Rinv[i-1+4].Draw();
		Rinv[i-1+4].Fit();
		canvas->cd(i+nCentralities);
		Rlcms[i-1+4].yMin = 1.5;
		Rlcms[i-1+4].yMax = 7;
		Rlcms[i-1+4].labels = ";m_{T} [GeV/c^{2}];R_{LCMS} [fm]";
		Rlcms[i-1+4].Draw();
	}
	canvas->SaveAs("output/therminator_peripheral.png");

	delete canvas;


	// normalized
	gStyle->SetPadGridX(true);
	gStyle->SetPadGridY(true);

	// therminator central
	nCentralities = 4;
	canvas = new TCanvas("canvas", "R_LCMS", 1200, 600);
	canvas->Divide(nCentralities, 2);
	for(int i = 1; i <= nCentralities; ++i)
	{
		canvas->cd(i);
		Rinv[i-1].labels = ";m_{T} [GeV/c^{2}];R_{inv}/ R_{inv}^{FIT}";
		Rinv[i-1].GetDispersionPlot(MultiFitPlot::Normalization).Draw();
		canvas->cd(i+nCentralities);
		Rlcms[i-1].labels = ";m_{T} [GeV/c^{2}];R_{LCMS} / R_{LCMS}^{FIT}";
		Rlcms[i-1].GetDispersionPlot(MultiFitPlot::Normalization).Draw();
	}
	canvas->SaveAs("output/therminator_central_div.png");
	delete canvas;

	// therminator peripheral
	nCentralities = 4;
	canvas = new TCanvas("canvas", "R_LCMS", 1200, 600);
	canvas->Divide(nCentralities,2);
	for(int i = 1; i <= nCentralities; ++i)
	{
		canvas->cd(i);
		Rinv[i-1+4].labels = ";m_{T} [GeV/c^{2}];R_{inv}/ R_{inv}^{FIT}";
		Rinv[i-1+4].GetDispersionPlot(MultiFitPlot::Normalization).Draw();
		canvas->cd(i+nCentralities);
		Rlcms[i-1+4].labels = ";m_{T} [GeV/c^{2}];R_{LCMS} / R_{LCMS}^{FIT}";
		Rlcms[i-1+4].GetDispersionPlot(MultiFitPlot::Normalization).Draw();
	}
	canvas->SaveAs("output/therminator_peripheral_div.png");
	delete canvas;
	gStyle->SetPadGridX(false);
	gStyle->SetPadGridY(false);
}
