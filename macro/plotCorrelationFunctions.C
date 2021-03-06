#include <iostream>
#include <vector>
#include <TCanvas.h>
#include <TH1D.h>
#include <TChain.h>
#include <TChainElement.h>
#include <TFile.h>
#include <TFileMerger.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TLatex.h>

#define LABELTEST false
const int rebinFactor = 2;
const int pairTypesNumber = 8;
const int colorTable[] = {1,2,4,6,8,9,12,46};

typedef struct
{
	TH1D
		*cf1D,
		*cfSH00,
		*cfSH10,
		*cfSH11,
		*cfSH20,
		*cfSH22;
	std::string
		centrality,
		model,
		particle;
} CorrelationFunction;

const Int_t
		markerStyle = 0,
		markerColor = 0,
		lineColor = 4;
Width_t
		lineWidth = 2;
const Float_t
		titleSize = 0.08,
		titleOffset = 1.3,
		labelOffset = 0.005,
		labelSize = 0.06,
		legendX1 = 0.65,
		legendY1 = 0.40,
		legendX2 = 0.999,
		legendY2 = 0.70,
		markerSize = 2;

void loadCentralityBin(
		const char *directory,
		const char *model,
		const char *centrality,
		std::vector<CorrelationFunction> &pipi,
		std::vector<CorrelationFunction> &pp,
		std::vector<CorrelationFunction> &kk,
		std::vector<CorrelationFunction> &kp,
		std::vector<CorrelationFunction> &pik,
		std::vector<CorrelationFunction> &piku,
		std::vector<CorrelationFunction> &pip,
		std::vector<CorrelationFunction> &pipu
	);

void Draw(const char* pairName, const char* canvasName, std::vector<CorrelationFunction> &plots, bool isIdentical = true);
void DrawSinglePlot(TH1D* correlationFunction, const char *title, unsigned int&);
void SetRanges(TH1D* correlationFunction, const char* pairType, const char* functionType);
void Rebin(TH1D *plot);

int plotCorrelationFunctions()
{
	std::vector<CorrelationFunction> 
		pipi,
		pp,
		kk,
		kp,
		pik,
		piku,
		pip,
		pipu;

	loadCentralityBin(
		"/mnt/data2/Models/tpi_output/lhyquid3vb2",
		"lhyquid3v",
		"b = 2.3 fm",
		pipi, pp, kk, kp, pik, piku, pip, pipu);

	// loadCentralityBin(
	// 	"/mnt/data2/Models/tpi_output/lhyquid3vb3",
	// 	"lhyquid3v",
	// 	"b = 3.1 fm",
	// 	pipi, pp, kk, kp, pik, piku, pip, pipu);

	loadCentralityBin(
		"/mnt/data2/Models/tpi_output/lhyquid3vb5.7",
		"lhyquid3v",
		"b = 5.7 fm",
		pipi, pp, kk, kp, pik, piku, pip, pipu);

	loadCentralityBin(
		"/mnt/data2/Models/tpi_output/lhyquid3vb7.4",
		"lhyquid3v",
		"b = 7.4 fm",
		pipi, pp, kk, kp, pik, piku, pip, pipu);

	// loadCentralityBin(
	// 	"/mnt/data2/Models/tpi_output/lhyquid3vb8.7",
	// 	"lhyquid3v",
	// 	"b = 8.7 fm",
	// 	pipi, pp, kk, kp, pik, piku, pip, pipu);

	// loadCentralityBin(
	// 	"/mnt/data2/Models/tpi_output/lhyquid3vb9.9",
	// 	"lhyquid3v",
	// 	"b = 9.9 fm",
	// 	pipi, pp, kk, kp, pik, piku, pip, pipu);

	loadCentralityBin(
		"/mnt/data2/Models/tpi_output/lhyquid3vb10.9",
		"lhyquid3v",
		"b = 10.9 fm",
		pipi, pp, kk, kp, pik, piku, pip, pipu);

	loadCentralityBin(
		"/mnt/data2/Models/tpi_output/lhyquid3vb11.9",
		"lhyquid3v",
		"b = 11.9 fm",
		pipi, pp, kk, kp, pik, piku, pip, pipu);

	gStyle->SetOptStat(0);
	gStyle->SetLabelSize(labelSize, "xyz");
	gStyle->SetTitleSize(titleSize, "t");
	gStyle->SetPadTopMargin(0.02);
	gStyle->SetPadBottomMargin(0.14);
	gStyle->SetPadLeftMargin(0.22);
	gStyle->SetPadRightMargin(0.03);

	Draw("#pi-#pi","pipi",pipi);
	Draw("K-K","kk",kk);
	Draw("p-p","pp",pp);
	Draw("K-p","kp",kp, false);
	Draw("#pi-K like sign","pik",pik, false);
	Draw("#pi-K unlike sign","piku",piku, false);
	Draw("#pi-p like sign","pip",pip, false);
	Draw("#pi-p unlike sign","pipu",pipu, false);

	return 0;
}

void loadCentralityBin(
		const char *directory,
		const char *model,
		const char *centrality,
		std::vector<CorrelationFunction> &pipi,
		std::vector<CorrelationFunction> &pp,
		std::vector<CorrelationFunction> &kk,
		std::vector<CorrelationFunction> &kp,
		std::vector<CorrelationFunction> &pik,
		std::vector<CorrelationFunction> &piku,
		std::vector<CorrelationFunction> &pip,
		std::vector<CorrelationFunction> &pipu
	)
{
	CorrelationFunction cf;

	std::vector<CorrelationFunction> *correlationFunctions[pairTypesNumber];

	TChain 
		*chPipi,
		*chPp,
		*chKk,
		*chKp,
		*chPik,
		*chPiku,
		*chPip,
		*chPipu,
		*chains[pairTypesNumber];
	std::string sDirectory = directory;
	cf.model = model;
	cf.centrality = centrality;

	for(int i = 0; i < pairTypesNumber; ++i)
		chains[i] = new TChain;

	chPipi = chains[0];
	chPp = chains[1];
	chKk = chains[2];
	chKp = chains[3];
	chPik = chains[4];
	chPiku = chains[5];
	chPip = chains[6];
	chPipu = chains[7];

	correlationFunctions[0] = &pipi;
	correlationFunctions[1] = &pp;
	correlationFunctions[2] = &kk;
	correlationFunctions[3] = &kp;
	correlationFunctions[4] = &pik;
	correlationFunctions[5] = &piku;
	correlationFunctions[6] = &pip;
	correlationFunctions[7] = &pipu;

	if(LABELTEST)
	{
		for(int i = 0; i < pairTypesNumber; ++i)
			for(int j = 0; j < 1; ++j)
			{
				cf.cf1D = new TH1D;
				cf.cfSH00 = new TH1D;
				cf.cfSH10 = new TH1D;
				cf.cfSH11 = new TH1D;
				cf.cfSH20 = new TH1D;
				cf.cfSH22 = new TH1D;
				correlationFunctions[i]->push_back(cf);
			}
		return;
	}

	chPipi->Add( (sDirectory+std::string("/outfilecf53a.root")).c_str() );
	chPp->Add( (sDirectory+std::string("/outfileppcf58a.root")).c_str() );
	chKk->Add( (sDirectory+std::string("/outfilekkcf54a.root")).c_str() );
	chKp->Add( (sDirectory+std::string("/outfilekpcf*.root")).c_str() );
	chPik->Add( (sDirectory+std::string("/outfilepikcf*.root")).c_str() );
	chPiku->Add( (sDirectory+std::string("/outfilepikulcf*.root")).c_str() );
	chPip->Add( (sDirectory+std::string("/outfilepipcf*.root")).c_str() );
	chPipu->Add( (sDirectory+std::string("/outfilepipulcf*.root")).c_str() );

	for(int i =0; i < pairTypesNumber ; ++i)
	{
		TObjArray *fileElements = chains[i]->GetListOfFiles();
        TIter next(fileElements);
        TChainElement *chEl=0;
		TFileMerger merger(kFALSE,kFALSE);
		merger.SetMsgPrefix("[merger]");
		merger.SetNotrees(kFALSE);
        while( (chEl = (TChainElement*)next()) )
		{
			if(!merger.AddFile(chEl->GetTitle()))
				std::cerr << "Could not add file: " << chEl->GetTitle() << std::endl;
			merger.Merge();
			// break; //////// DEBUG!!!
        }
        if(strcmp(merger.GetOutputFileName(),"") == 0)
        	continue;

		TFile *file = new TFile(merger.GetOutputFileName());

		if(file == 0)
		{
			std::cout << "NULL POINTER: *" << merger.GetOutputFileName() << "* !" << std::endl;
			continue;
		}
		if(file->IsZombie())
		{
			std::cout << "ZOMBIE: *" << merger.GetOutputFileName() << "* !" << std::endl;
			continue;
		}

		cf.cf1D = (TH1D*) file->Get("cnumn1da");
		cf.cf1D->Divide((TH1D*) file->Get("cdenn1da"));

		file->GetObject("CfnReYlm00NonIdCYlmTrue", cf.cfSH00);
		if(cf.cfSH00)
		{
			// file->GetObject("CfnReYlm00NonIdCYlmTrue", cf.cfSH00);
			// file->GetObject("CfnReYlm10NonIdCYlmTrue", cf.cfSH10);
			// file->GetObject("CfnReYlm11NonIdCYlmTrue", cf.cfSH11);
			// file->GetObject("CfnReYlm20NonIdCYlmTrue", cf.cfSH20);
			// file->GetObject("CfnReYlm22NonIdCYlmTrue", cf.cfSH22);
			file->GetObject("CfnReYlm00NonIdCYlm", cf.cfSH00);
			file->GetObject("CfnReYlm10NonIdCYlm", cf.cfSH10);
			file->GetObject("CfnReYlm11NonIdCYlm", cf.cfSH11);
			file->GetObject("CfnReYlm20NonIdCYlm", cf.cfSH20);
			file->GetObject("CfnReYlm22NonIdCYlm", cf.cfSH22);

		}


		if(!cf.cfSH00)
		{
			file->GetObject("CfnReYlm00NonIdCYlm", cf.cfSH00);
			if(cf.cfSH00)
			{
				file->GetObject("CfnReYlm00NonIdCYlm", cf.cfSH00);
				file->GetObject("CfnReYlm10NonIdCYlm", cf.cfSH10);
				file->GetObject("CfnReYlm11NonIdCYlm", cf.cfSH11);
				file->GetObject("CfnReYlm20NonIdCYlm", cf.cfSH20);
				file->GetObject("CfnReYlm22NonIdCYlm", cf.cfSH22);
			}
		}

		if(!cf.cfSH00)
		{
			file->GetObject("CfnReYlm00IdLCYlm", cf.cfSH00);
			if(cf.cfSH00)
			{
				file->GetObject("CfnReYlm00IdLCYlm", cf.cfSH00);
				file->GetObject("CfnReYlm10IdLCYlm", cf.cfSH10);
				file->GetObject("CfnReYlm11IdLCYlm", cf.cfSH11);
				file->GetObject("CfnReYlm20IdLCYlm", cf.cfSH20);
				file->GetObject("CfnReYlm22IdLCYlm", cf.cfSH22);
			}
		}
		if(cf.cfSH00 && cf.cfSH10 && cf.cfSH11 && cf.cfSH20 && cf.cfSH22)
			correlationFunctions[i]->push_back(cf);
		else
		{	
			std::cout << "[ERROR] Could not load all SH functions from file *" << chEl->GetTitle() << "* !!!" << std::endl;
			if(!cf.cfSH00)
				std::cout << "[ERROR] Problem loading SH00" << std::endl;
			if(!cf.cfSH10)
				std::cout << "[ERROR] Problem loading SH10" << std::endl;
			if(!cf.cfSH11)
				std::cout << "[ERROR] Problem loading SH11" << std::endl;
			if(!cf.cfSH20)
				std::cout << "[ERROR] Problem loading SH20" << std::endl;
			if(!cf.cfSH22)
				std::cout << "[ERROR] Problem loading SH22" << std::endl;

		}
	}
}

void Draw(const char* pairName, const char* canvasName, std::vector<CorrelationFunction> &plots, bool isIdentical)
{
	unsigned int i, k;
	int canvasNumber1 = 0;
	TLegend *lhyquid3vlegend;
	TCanvas *canv;
	if(isIdentical)
	{
		canv = new TCanvas(canvasName, canvasName, 1000, 1000);
		canv->Divide(2,2);
	}
	else
	{
		canv = new TCanvas(canvasName, canvasName, 1000, 1000);
		canv->Divide(2,2);
	}

	// 
	// lhyquid3v
	//
	lhyquid3vlegend = new TLegend(legendX1, legendY1, legendX2, legendY2);
	lhyquid3vlegend->SetEntrySeparation(0.05);
	lhyquid3vlegend->SetMargin(0.2);
	lhyquid3vlegend->SetTextSize(0.06);

			
	k = 0;
	canv->cd(++canvasNumber1);
	for(i = 0 ; i < plots.size(); ++i)
		if(plots[i].model.compare("lhyquid3v") == 0)
		{
			DrawSinglePlot(plots[i].cfSH00, (/*std::string(pairName) +*/ std::string(";k* (GeV/c);#RgothicC^{0}_{0}")).c_str(), k);
			SetRanges(plots[i].cfSH00, canvasName, "SH00");
			lhyquid3vlegend->AddEntry(plots[i].cfSH00, plots[i].centrality.c_str(),"l");
		}
	if(plots.size() > 0)
	{
		lhyquid3vlegend->Draw();
		lhyquid3vlegend->SetFillColor(0);
	}

	if(!isIdentical)
	{
		k = 0;
		canv->cd(++canvasNumber1);
		for(i = 0 ; i < plots.size(); ++i)
			if(plots[i].model.compare("lhyquid3v") == 0)
			{
				DrawSinglePlot(plots[i].cfSH11, (/*std::string(pairName) +*/ std::string(";k* (GeV/c);#RgothicC^{1}_{1}")).c_str(), k);
				SetRanges(plots[i].cfSH11, canvasName, "SH11");
			}
	}

	k = 0;
	canv->cd(++canvasNumber1);
	for(i = 0 ; i < plots.size(); ++i)
		if(plots[i].model.compare("lhyquid3v") == 0)
		{
			DrawSinglePlot(plots[i].cfSH20, (/*std::string(pairName) +*/ std::string(";k* (GeV/c);#RgothicC^{0}_{2}")).c_str(), k);
			SetRanges(plots[i].cfSH20, canvasName, "SH20");
		}

	k = 0;
	canv->cd(++canvasNumber1);
	for(i = 0 ; i < plots.size(); ++i)
		if(plots[i].model.compare("lhyquid3v") == 0)
		{
			DrawSinglePlot(plots[i].cfSH22, (/*std::string(pairName) +*/ std::string(";k* (GeV/c);#RgothicC^{2}_{2}")).c_str(), k);
			SetRanges(plots[i].cfSH22, canvasName, "SH22");
		}

	if(isIdentical)
	{
		canv->cd(++canvasNumber1);
		TLatex Tl;
   	Tl.SetTextSize(0.16);
   	Tl.DrawLatex(0.4,0.5, (std::string("#font[12]{")+std::string(pairName)+std::string("}")).c_str());
	}

	// canv->SaveAs((std::string(canvasName)+std::string(".eps")).c_str());
	canv->SaveAs((std::string(canvasName)+std::string(".png")).c_str());
	// canv->SaveAs((std::string(canvasName)+std::string(".root")).c_str());
}

void DrawSinglePlot(TH1D* correlationFunction, const char* title, unsigned int &i)
{
	Rebin(correlationFunction);
	correlationFunction->SetAxisRange(0,0.2);
	if(i == 0)
	{
		correlationFunction->Draw("HISTL");
		correlationFunction->GetYaxis()->SetTitleSize(titleSize);
		correlationFunction->GetYaxis()->SetTitleOffset(titleOffset);
		correlationFunction->GetYaxis()->SetLabelSize(labelSize);
		correlationFunction->GetYaxis()->SetLabelOffset(labelOffset);
		correlationFunction->GetXaxis()->SetTitleSize(titleSize-0.01);
		correlationFunction->GetXaxis()->SetTitleOffset(titleOffset-0.3);
		correlationFunction->GetXaxis()->SetLabelOffset(labelOffset);
		correlationFunction->GetXaxis()->SetLabelSize(labelSize);
		correlationFunction->SetTitle(title);
	}
	else
		correlationFunction->Draw("HISTL SAME");
	correlationFunction->SetLineColor(colorTable[i++]);
	correlationFunction->SetLineWidth(lineWidth);
	// correlationFunction->SetMarkerColor(i++);
	// correlationFunction->SetLineColor(16);

	correlationFunction->SetMarkerStyle(34);
}

void SetRanges(TH1D* correlationFunction, const char* pairType, const char* functionType)
{
	std::string
		sFunctionType = functionType,
		sPairType = pairType;
	correlationFunction->GetXaxis()->SetNdivisions(905);
	correlationFunction->GetYaxis()->SetNdivisions(905);
	if(sFunctionType.compare("1D") == 0 && sPairType.compare("kk") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(0.9,2.1);
		// correlationFunction->GetYaxis()->SetRangeUser(0.9,2.1);
		// correlationFunction->SetAxisRange(0.9,2.1,"Y");
		correlationFunction->SetMinimum(0.9);
		correlationFunction->SetMaximum(2.1);
	}
	else if(sFunctionType.compare("1D") == 0 && sPairType.compare("kp") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(0,1.1);
		// correlationFunction->GetYaxis()->SetRangeUser(0,1.1);
		// correlationFunction->SetAxisRange(0,1.1,"Y");
		correlationFunction->SetAxisRange(0,0.1);
		correlationFunction->SetMinimum(0.5);
		correlationFunction->SetMaximum(1.1);
	}
	else if(sFunctionType.compare("1D") == 0 && sPairType.compare("pik") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(0.4,1.1);
		// correlationFunction->GetYaxis()->SetRangeUser(0.4,1.1);
		// correlationFunction->SetAxisRange(0.4,1.1,"Y");
		correlationFunction->SetAxisRange(0,0.06);
		correlationFunction->SetMinimum(0.75);
		correlationFunction->SetMaximum(1.05);
	}
	else if(sFunctionType.compare("1D") == 0 && sPairType.compare("piku") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(0.9,2.1);
		// correlationFunction->GetYaxis()->SetRangeUser(0.9,2.1);
		// correlationFunction->SetAxisRange(0.9,2.1,"Y");
		correlationFunction->SetAxisRange(0,0.06);
		correlationFunction->SetMinimum(0.9);
		correlationFunction->SetMaximum(1.3);
	}
	else if(sFunctionType.compare("1D") == 0 && sPairType.compare("pip") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(0.4,1.1);
		// correlationFunction->GetYaxis()->SetRangeUser(0.4,1.1);
		// correlationFunction->SetAxisRange(0.4,1.1,"Y");
		correlationFunction->SetAxisRange(0,0.06);
		correlationFunction->SetMinimum(0.75);
		correlationFunction->SetMaximum(1.05);
	}
	else if(sFunctionType.compare("1D") == 0 && sPairType.compare("pipi") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(0.9,2.1);
		// correlationFunction->GetYaxis()->SetRangeUser(0.9,2.1);
		// correlationFunction->SetAxisRange(0.9,2.1,"Y");
		correlationFunction->SetMinimum(0.9);
		correlationFunction->SetMaximum(2.1);
	}
	else if(sFunctionType.compare("1D") == 0 && sPairType.compare("pipu") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(0.9,2.1);
		// correlationFunction->GetYaxis()->SetRangeUser(0.9,2.1);
		// correlationFunction->SetAxisRange(0.9,2.1,"Y");
		correlationFunction->SetAxisRange(0,0.06);
		correlationFunction->SetMinimum(0.9);
		correlationFunction->SetMaximum(1.3);
	}
	else if(sFunctionType.compare("1D") == 0 && sPairType.compare("pp") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(0.1,1.1);
		// correlationFunction->GetYaxis()->SetRangeUser(0.1,1.1);
		// correlationFunction->SetAxisRange(0.1,1.1,"Y");
		correlationFunction->SetMinimum(0.1);
		correlationFunction->SetMaximum(1.1);
	}

	else if(sFunctionType.compare("SH00") == 0 && sPairType.compare("kk") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(0.9,2.1);
		// correlationFunction->GetYaxis()->SetRangeUser(0.9,2.1);
		// correlationFunction->SetAxisRange(0.9,2.1,"Y");
		correlationFunction->SetMinimum(0.9);
		correlationFunction->SetMaximum(2.1);
	}
	else if(sFunctionType.compare("SH00") == 0 && sPairType.compare("kp") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(0,1.1);
		// correlationFunction->GetYaxis()->SetRangeUser(0,1.1);
		// correlationFunction->SetAxisRange(0,1.1,"Y");
		correlationFunction->SetAxisRange(0,0.12);
		correlationFunction->SetMinimum(0.7);
		correlationFunction->SetMaximum(1.1);
	}
	else if(sFunctionType.compare("SH00") == 0 && sPairType.compare("pik") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(0.4,1.1);
		// correlationFunction->GetYaxis()->SetRangeUser(0.4,1.1);
		// correlationFunction->SetAxisRange(0.4,1.1,"Y");
		correlationFunction->SetAxisRange(0,0.12);
		correlationFunction->SetMinimum(0.85);
		correlationFunction->SetMaximum(1.05);
	}
	else if(sFunctionType.compare("SH00") == 0 && sPairType.compare("piku") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(0.9,2.1);
		// correlationFunction->GetYaxis()->SetRangeUser(0.9,2.1);
		// correlationFunction->SetAxisRange(0.9,2.1,"Y");
		correlationFunction->SetAxisRange(0,0.12);
		correlationFunction->SetMinimum(0.99);
		correlationFunction->SetMaximum(1.15);
	}
	else if(sFunctionType.compare("SH00") == 0 && sPairType.compare("pip") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(0.4,1.1);
		// correlationFunction->GetYaxis()->SetRangeUser(0.4,1.1);
		// correlationFunction->SetAxisRange(0.4,1.1,"Y");
		correlationFunction->SetAxisRange(0,0.12);
		correlationFunction->SetMinimum(0.85);
		correlationFunction->SetMaximum(1.01);
	}
	else if(sFunctionType.compare("SH00") == 0 && sPairType.compare("pipi") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(0.9,2.1);
		// correlationFunction->GetYaxis()->SetRangeUser(0.9,2.1);
		// correlationFunction->SetAxisRange(0.9,2.1,"Y");
		correlationFunction->SetMinimum(0.9);
		correlationFunction->SetMaximum(2.1);
	}
	else if(sFunctionType.compare("SH00") == 0 && sPairType.compare("pipu") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(0.9,2.1);
		// correlationFunction->GetYaxis()->SetRangeUser(0.9,2.1);
		// correlationFunction->SetAxisRange(0.9,2.1,"Y");
		correlationFunction->SetAxisRange(0,0.12);
		correlationFunction->SetMinimum(0.98);
		correlationFunction->SetMaximum(1.15);
	}
	else if(sFunctionType.compare("SH00") == 0 && sPairType.compare("pp") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(0.1,1.1);
		// correlationFunction->GetYaxis()->SetRangeUser(0.1,1.1);
		// correlationFunction->SetAxisRange(0.1,1.1,"Y");
		correlationFunction->SetMinimum(0.2);
		correlationFunction->SetMaximum(1.1);
	}

	else if(sFunctionType.compare("SH11") == 0 && sPairType.compare("kp") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.02,0.02);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.02,0.02);
		// correlationFunction->SetAxisRange(-0.02,.02,"Y");
		correlationFunction->SetAxisRange(0,0.12);
		correlationFunction->SetMinimum(-0.02);
		correlationFunction->SetMaximum(0.02);
	}
	else if(sFunctionType.compare("SH11") == 0 && sPairType.compare("pik") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.01,0.03);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.01,0.03);
		// correlationFunction->SetAxisRange(-0.01,0.03,"Y");
		correlationFunction->SetAxisRange(0,0.12);
		correlationFunction->SetMinimum(-0.005);
		correlationFunction->SetMaximum(0.023);
	}
	else if(sFunctionType.compare("SH11") == 0 && sPairType.compare("piku") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.025,0.005);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.025,0.005);
		// correlationFunction->SetAxisRange(-0.025,0.005,"Y");
		correlationFunction->SetAxisRange(0,0.12);
		correlationFunction->SetMinimum(-0.03);
		correlationFunction->SetMaximum(0.0025);
	}
	else if(sFunctionType.compare("SH11") == 0 && sPairType.compare("pip") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.005,0.03);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.005,0.03);
		// correlationFunction->SetAxisRange(-0.005,0.03,"Y");
		correlationFunction->SetAxisRange(0,0.12);
		correlationFunction->SetMinimum(-0.005);
		correlationFunction->SetMaximum(0.03);
	}
	else if(sFunctionType.compare("SH11") == 0 && sPairType.compare("pipu") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.06,0.01);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.06,0.01);
		// correlationFunction->SetAxisRange(-0.06,0.01,"Y");
		correlationFunction->SetAxisRange(0,0.12);
		correlationFunction->SetMinimum(-0.03);
		correlationFunction->SetMaximum(0.005);
	}

	else if(sFunctionType.compare("SH20") == 0 && sPairType.compare("kk") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.11,0.11);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.11,0.11);
		// correlationFunction->SetAxisRange(-0.11,0.11,"Y");
		correlationFunction->SetMinimum(-0.06);
		correlationFunction->SetMaximum( 0.03);
	}
	else if(sFunctionType.compare("SH20") == 0 && sPairType.compare("kp") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.02,0.02);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.02,0.02);
		// correlationFunction->SetAxisRange(-0.02,0.02,"Y");
		correlationFunction->SetAxisRange(0,0.12);
		correlationFunction->SetMinimum(-0.015);
		correlationFunction->SetMaximum(0.015);
	}
	else if(sFunctionType.compare("SH20") == 0 && sPairType.compare("pik") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.01,0.01);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.01,0.01);
		// correlationFunction->SetAxisRange(-0.01,0.01,"Y");
		correlationFunction->SetAxisRange(0,0.12);
		correlationFunction->SetMinimum(-0.002);
		correlationFunction->SetMaximum(0.002);
	}
	else if(sFunctionType.compare("SH20") == 0 && sPairType.compare("piku") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.01,0.01);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.01,0.01);
		// correlationFunction->SetAxisRange(-0.01,0.01,"Y");
		correlationFunction->SetAxisRange(0,0.12);
		correlationFunction->SetMinimum(-0.004);
		correlationFunction->SetMaximum(0.002);
	}
	else if(sFunctionType.compare("SH20") == 0 && sPairType.compare("pip") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.01,0.01);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.01,0.01);
		// correlationFunction->SetAxisRange(-0.01,0.01,"Y");
		correlationFunction->SetAxisRange(0,0.12);
		correlationFunction->SetMinimum(-0.002);
		correlationFunction->SetMaximum(0.004);
	}
	else if(sFunctionType.compare("SH20") == 0 && sPairType.compare("pipi") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.1,0.1);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.1,0.1);
		// correlationFunction->SetAxisRange(-0.1,0.1,"Y");
		correlationFunction->SetMinimum(-0.1);
		correlationFunction->SetMaximum(0.03);
	}
	else if(sFunctionType.compare("SH20") == 0 && sPairType.compare("pipu") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.015,0.015);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.015,0.015);
		// correlationFunction->SetAxisRange(-0.015,015,"Y");
		correlationFunction->SetAxisRange(0,0.12);
		correlationFunction->SetMinimum(-0.004);
		correlationFunction->SetMaximum(0.001);
	}
	else if(sFunctionType.compare("SH20") == 0 && sPairType.compare("pp") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.1,0.1);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.1,0.1);
		// correlationFunction->SetAxisRange(-0.1,0.1,"Y");
		correlationFunction->SetMinimum(-0.1);
		correlationFunction->SetMaximum(0.1);
	}

	else if(sFunctionType.compare("SH22") == 0 && sPairType.compare("kk") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.11,0.11);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.11,0.11);
		// correlationFunction->SetAxisRange(-0.11,0.11,"Y");
		correlationFunction->SetMinimum(-0.02);
		correlationFunction->SetMaximum(0.02);
	}
	else if(sFunctionType.compare("SH22") == 0 && sPairType.compare("kp") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.02,0.02);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.02,0.02);
		// correlationFunction->SetAxisRange(-0.02,0.02,"Y");
		correlationFunction->SetAxisRange(0,0.12);
		correlationFunction->SetMinimum(-0.01);
		correlationFunction->SetMaximum(0.01);
	}
	else if(sFunctionType.compare("SH22") == 0 && sPairType.compare("pik") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.005,005);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.005,0.005);
		// correlationFunction->SetAxisRange(-0.005,0.005,"Y");
		correlationFunction->SetAxisRange(0,0.12);
		correlationFunction->SetMinimum(-0.004);
		correlationFunction->SetMaximum(0.002);
	}
	else if(sFunctionType.compare("SH22") == 0 && sPairType.compare("piku") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.015,0.015);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.015,0.015);
		// correlationFunction->SetAxisRange(-0.015,0.015,"Y");
		correlationFunction->SetAxisRange(0,0.12);
		correlationFunction->SetMinimum(-0.002);
		correlationFunction->SetMaximum(0.005);
	}
	else if(sFunctionType.compare("SH22") == 0 && sPairType.compare("pip") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.01,0.01);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.01,0.01);
		// correlationFunction->SetAxisRange(-0.01,0.01,"Y");
		correlationFunction->SetAxisRange(0,0.12);
		correlationFunction->SetMinimum(-0.005);
		correlationFunction->SetMaximum(0.005);
	}
	else if(sFunctionType.compare("SH22") == 0 && sPairType.compare("pipi") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.1,0.1);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.1,0.1);
		// correlationFunction->SetAxisRange(-0.1,0.1,"Y");
		correlationFunction->SetMinimum(-0.03);
		correlationFunction->SetMaximum(0.02);
	}
	else if(sFunctionType.compare("SH22") == 0 && sPairType.compare("pipu") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.02,0.02);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.02,0.02);
		// correlationFunction->SetAxisRange(-0.02,0.02,"Y");
		correlationFunction->SetAxisRange(0,0.12);
		correlationFunction->SetMinimum(-0.005);
		correlationFunction->SetMaximum(0.006);
	}
	else if(sFunctionType.compare("SH22") == 0 && sPairType.compare("pp") == 0)
	{
		// correlationFunction->GetYaxis()->SetLimits(-0.1,0.1);
		// correlationFunction->GetYaxis()->SetRangeUser(-0.1,0.1);
		// correlationFunction->SetAxisRange(-0.1,0.1,"Y");
		correlationFunction->SetMinimum(-0.1);
		correlationFunction->SetMaximum(0.1);
	}
	else
	{
		std::cout << "Unknown pair type: " << sPairType << " and function name: " << sFunctionType << std::endl;
	}
}

void Rebin(TH1D *plot) {
	if(rebinFactor == 1)
		return;
	plot->Rebin(rebinFactor);
	plot->Scale(1.0/rebinFactor);
}
