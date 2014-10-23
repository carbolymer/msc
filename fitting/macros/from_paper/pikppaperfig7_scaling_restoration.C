void preparehist(TH1D *hist, int ht)
{
  int cols[3] = { kRed, kBlue, kBlue };
  int mars[3] = { 20, 24, 25 };

  hist->SetLineColor(cols[ht]);
  hist->SetMarkerColor(cols[ht]);
  hist->SetMarkerStyle(mars[ht]);
  hist->SetMarkerSize(1.6);
  //  hist->GetXaxis()->SetRange(1,149);


  hist->GetXaxis()->SetTitleSize(0.065);
  hist->GetXaxis()->SetLabelSize(0.065);

  hist->GetYaxis()->SetTitleSize(0.065);
  hist->GetYaxis()->SetLabelSize(0.065);

//   hist->GetXaxis()->SetTitleColor(0);
//   hist->GetYaxis()->SetTitleColor(0);

//   hist->GetXaxis()->SetLabelColor(0);
//   hist->GetYaxis()->SetLabelColor(0);

  hist->GetYaxis()->SetTitleOffset(0.75);

  hist->GetXaxis()->SetNdivisions(4,5,0);
  hist->GetYaxis()->SetNdivisions(4,5,0);

  hist->GetXaxis()->CenterTitle();
  hist->GetYaxis()->CenterTitle();

  //  hist->SetTitle(";q_{LCMS} [GeV/c];C(q)");

}

void preparegraph(TGraphErrors *hist, int ht, int ct)
{
  int cols[3] = { kBlue, kGreen+2, kRed };
  int mars[9] = { 21, 20, 33, 25, 24, 27, 28, 29, 34 };

  hist->SetLineColor(cols[ht]);
  hist->SetMarkerColor(cols[ht]);
  hist->SetMarkerStyle(mars[ht + 3*ct]);
  hist->SetMarkerSize(1.2);
//   hist->GetXaxis()->SetRange(1,149);


//   hist->GetXaxis()->SetTitleSize(0.06);
//   hist->GetXaxis()->SetLabelSize(0.06);

//   hist->GetYaxis()->SetTitleSize(0.06);
//   hist->GetYaxis()->SetLabelSize(0.06);

//   hist->GetXaxis()->SetTitleColor(0);
//   hist->GetYaxis()->SetTitleColor(0);

//   hist->GetXaxis()->SetLabelColor(0);
//   hist->GetYaxis()->SetLabelColor(0);

//   hist->GetXaxis()->SetNdivisions(4,5,0);
//   hist->GetYaxis()->SetNdivisions(3,5,0);

  //  hist->SetTitle(";q_{LCMS} [GeV/c];C(q)");

}

void preparepad()
{
  gPad->SetFillColor(0);
  gPad->SetFillStyle(4000);
  gPad->SetTopMargin(0.005);
  gPad->SetRightMargin(0.005);
  gPad->SetBottomMargin(0.14);
  gPad->SetLeftMargin(0.12);
  gPad->SetTickx(1);
  gPad->SetTicky(1);

}

TGraphErrors *getsumgraph(TList *lista)
{
  TGraphErrors *grsum = new TGraphErrors(0);
  grsum->Merge(lista);
  
  Int_t pc=0;
  
  TIter next(lista, kIterForward);
  TObject *obj;
  while (obj = next()) {
    for (int iter=0; iter<((TGraphErrors *) obj)->GetN(); iter++)
      grsum->GetEY()[pc++] = ((TGraphErrors *) obj)->GetEY()[iter];
  }
  //     obj->Draw(next.GetOption());
  
  //   for (int iter=0; iter<grmd9ls[1]->GetN(); iter++)
  //     grsum->GetEY()[iter+grmdls[1]->GetN()] = grmd9ls[1]->GetEY()[iter];
  //   for (int iter=0; iter<grmdls[1]->GetN(); iter++)
  //     grsum->GetEY()[iter+grmdls[1]->GetN()+grmd9ls[1]->GetN()] = grmd2ls[1]->GetEY()[iter];
  return grsum;
}

TF1 *drawlinefit(TGraphErrors *grin)
{
  TF1 *funf = new TF1("funf","[1]*TMath::Power(x,[2])");
  funf->SetParameters(0,10,-0.5);
  funf->SetRange(0.18,1.15);
  funf->SetLineWidth(1);
  funf->SetLineColor(kGray+4);
  funf->SetLineStyle(2);
  
  grin->Fit(funf);

  Double_t dval = 0.0;
  for (int ix=0; ix<grin->GetN(); ix++)
    dval += TMath::Abs(grin->GetY()[ix] - funf->Eval(grin->GetX()[ix]))/grin->GetY()[ix];
  cout << "Average deviation " << dval/grin->GetN() << endl;

  return funf;
}

TGraphErrors *getgammascaledgraph(TGraphErrors *grin, double mass)
{
  TGraphErrors *grout = new TGraphErrors(*grin);
  
  double kt, bt, gt, sf;
  
  for (int ix=0; ix<grout->GetN(); ix++) {
    kt = TMath::Sqrt(grout->GetX()[ix]*grout->GetX()[ix] - mass*mass);
    bt = kt/grout->GetX()[ix];
    gt = 1.0/TMath::Sqrt(1-bt*bt);
    sf = TMath::Sqrt((2+TMath::Sqrt(gt))/3.0);

    grout->GetY()[ix] = grin->GetY()[ix]/sf;
    grout->GetEY()[ix] = grin->GetEY()[ix]/sf;
  }

  return grout;
}

TGraphErrors *getprfavgraph(TGraphErrors *grino, TGraphErrors *grins, TGraphErrors *grinl, double mass)
{
  TCanvas *cnew = new TCanvas("cnew","cnew",700,500);

  TGraphErrors *grout = new TGraphErrors(*grino);
  
  double kt, bt, gt, sf;
  
  double ri, roe, rse, rle;
  
  for (int ix=0; ix<grout->GetN(); ix++) {
    kt = TMath::Sqrt(grout->GetX()[ix]*grout->GetX()[ix] - mass*mass);
    bt = kt/grout->GetX()[ix];
    gt = 1.0/TMath::Sqrt(1-bt*bt);
    //    sf = TMath::Sqrt((2+TMath::Sqrt(gt))/3.0);

    ri = getrinvfromlcms(grino->GetY()[ix], grins->GetY()[ix], grinl->GetY()[ix], gt);
    roe = grino->GetEY()[ix]/grino->GetY()[ix];
    rse = grins->GetEY()[ix]/grins->GetY()[ix];
    rle = grinl->GetEY()[ix]/grinl->GetY()[ix];

    grout->GetY()[ix] = ri;
    grout->GetEY()[ix] = ri*((roe+rse+rle)/3);
  }

  return grout;
}

TGraphErrors *getlcmsavgraph(TGraphErrors *grino, TGraphErrors *grins, TGraphErrors *grinl)
{
  TCanvas *cnew = new TCanvas("cnew","cnew",700,500);

  TGraphErrors *grout = new TGraphErrors(*grino);
  
  double ri, roe, rse, rle;
  
  for (int ix=0; ix<grout->GetN(); ix++) {
    ri = getrinvfromlcms(grino->GetY()[ix], grins->GetY()[ix], grinl->GetY()[ix], 1.0);
    roe = grino->GetEY()[ix]/grino->GetY()[ix];
    rse = grins->GetEY()[ix]/grins->GetY()[ix];
    rle = grinl->GetEY()[ix]/grinl->GetY()[ix];

    grout->GetY()[ix] = ri;
    grout->GetEY()[ix] = ri*(roe+rse+rle);
  }

  return grout;
}


void pikppaperfig7()
{
  gStyle->SetOptStat(1000000000);
  gStyle->SetStatBorderSize(0);
  gStyle->SetTextFont(42);
  gStyle->SetLabelFont(42,"X");
  gStyle->SetTitleFont(42,"X");
  gStyle->SetLabelFont(42,"Y");
  gStyle->SetTitleFont(42,"Y");
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(0);
  gStyle->SetFrameLineWidth(1);

  gROOT->LoadMacro("getrinvfromlcms.C");

  TFile *infiles[8];
  infiles[0] = new TFile("b2.3_invs.root");
  infiles[1] = new TFile("b3.1_invs.root");
  infiles[2] = new TFile("b5.7_invs.root");
  infiles[3] = new TFile("b7.4_invs.root");
  infiles[4] = new TFile("b8.7_invs.root");
  infiles[5] = new TFile("b9.9_invs.root");
  infiles[6] = new TFile("b10.9_invs.root");
  infiles[7] = new TFile("b11.9_invs.root");

  TFile *infilespp[8];
  infilespp[0] = new TFile("fitgraphs.sh.pp.M0.root");
  infilespp[1] = new TFile("fitgraphs.sh.pp.M1.root");
  infilespp[2] = new TFile("fitgraphs.sh.pp.M2.root");
  infilespp[3] = new TFile("fitgraphs.sh.pp.M3.root");
  infilespp[4] = new TFile("fitgraphs.sh.pp.M4.root");
  infilespp[5] = new TFile("fitgraphs.sh.pp.M5.root");
  infilespp[6] = new TFile("fitgraphs.sh.pp.M6.root");
  infilespp[7] = new TFile("fitgraphs.sh.pp.M7.root");

  TFile *infileskk[8];
  infileskk[0] = new TFile("fitgraphs.sh.kk.M1.root");
  infileskk[1] = new TFile("fitgraphs.sh.kk.M1.root");
  infileskk[2] = new TFile("fitgraphs.sh.kk.M2.root");
  infileskk[3] = new TFile("fitgraphs.sh.kk.M3.root");
  infileskk[4] = new TFile("fitgraphs.sh.kk.M4.root");
  infileskk[5] = new TFile("fitgraphs.sh.kk.M5.root");
  infileskk[6] = new TFile("fitgraphs.sh.kk.M6.root");
  infileskk[7] = new TFile("fitgraphs.sh.kk.M7.root");

  TFile *infilesrr[8];
  infilesrr[0] = new TFile("fitgraphs.sh.rr.M1.root");
  infilesrr[1] = new TFile("fitgraphs.sh.rr.M1.root");
  infilesrr[2] = new TFile("fitgraphs.sh.rr.M2.root");
  infilesrr[3] = new TFile("fitgraphs.sh.rr.M3.root");
  infilesrr[4] = new TFile("fitgraphs.sh.rr.M4.root");
  infilesrr[5] = new TFile("fitgraphs.sh.rr.M5.root");
  infilesrr[6] = new TFile("fitgraphs.sh.rr.M6.root");
  infilesrr[7] = new TFile("fitgraphs.sh.rr.M7.root");

  TGraphErrors *grp2c0 = getprfavgraph((TGraphErrors *) infilespp[1]->Get("Pion0"), 
				       (TGraphErrors *) infilespp[1]->Get("Pion1"),
				       (TGraphErrors *) infilespp[1]->Get("Pion2"),
				       0.13957);
  preparegraph(grp2c0, 0, 1);

  TGraphErrors *grk2c0 = getprfavgraph((TGraphErrors *) infileskk[1]->Get("Kaon0"), 
				       (TGraphErrors *) infileskk[1]->Get("Kaon1"),
				       (TGraphErrors *) infileskk[1]->Get("Kaon2"),
				       0.493677);
  preparegraph(grk2c0, 1, 1);

  TGraphErrors *grr2c0 = getprfavgraph((TGraphErrors *) infilesrr[1]->Get("Proton0"), 
				       (TGraphErrors *) infilesrr[1]->Get("Proton1"),
				       (TGraphErrors *) infilesrr[1]->Get("Proton2"),
				       0.938272);
  preparegraph(grr2c0, 2, 1);

  TCanvas *canfig1 = new TCanvas("pikpfig7","pikpfig7",500,333);
  preparepad();

  //  canfig1->Divide(1,3,0.0001,0.0001);

  // Rinv plot

  canfig1->cd(1);
  preparepad();

  TH1D *ramkao = new TH1D("ramkao",";#it{m}_{T} (GeV/#it{c});#it{R}_{inv},#it{R}_{eff} (fm)",100,0.15,1.55);
  ramkao->SetMinimum(1.01);
  ramkao->SetMaximum(10.4);
  preparehist(ramkao, 0);
  ramkao->Draw();
  
  // 1st centrality

  TGraphErrors *grp0c0 = (TGraphErrors *) infiles[0]->Get("Pion5");
  preparegraph(grp0c0, 0, 0);
  grp0c0->Draw("P");

  TGraphErrors *grk0c0 = (TGraphErrors *) infiles[0]->Get("Kaon5");
  preparegraph(grk0c0, 1, 0);
  grk0c0->Draw("P");

  TGraphErrors *grr0c0 = (TGraphErrors *) infiles[0]->Get("Proton5");
  preparegraph(grr0c0, 2, 0);
  grr0c0->Draw("P");

  grp2c0->Draw("P");
  grk2c0->Draw("P");
  grr2c0->Draw("P");

  int cols[3] = { kBlue, kGreen+2, kRed };
  int mars[9] = { 21, 20, 33, 25, 24, 27, 28, 29, 34 };


  canfig1->cd(1);
  TLatex lat;
  lat.SetTextSize(0.065);
  lat.DrawLatex(0.35, 4.2, "Pions");
  lat.DrawLatex(0.35, 3.4, "Kaons");
  lat.DrawLatex(0.35, 2.6, "Protons");

  TMarker m1(0.88, 7.2, mars[0]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[0]); m1.DrawMarker(0.31, 4.45);
  TMarker m1(0.91, 7.2, mars[3]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[0]); m1.DrawMarker(0.27, 4.45);
  //  TMarker m1(0.94, 7.2, mars[6]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[0]); m1.DrawMarker(0.96, 9.4);

  TMarker m1(0.88, 7.2, mars[1]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[1]); m1.DrawMarker(0.31, 3.65);
  TMarker m1(0.91, 7.2, mars[4]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[1]); m1.DrawMarker(0.27, 3.65);
  //  TMarker m1(0.94, 7.2, mars[7]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[1]); m1.DrawMarker(0.96, 8.7);

  TMarker m1(0.88, 7.2, mars[2]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[2]); m1.DrawMarker(0.31, 2.85);
  TMarker m1(0.91, 7.2, mars[5]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[2]); m1.DrawMarker(0.27, 2.85);
  //  TMarker m1(0.94, 7.2, mars[8]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[2]); m1.DrawMarker(0.96, 7.0);

//   canfig1->cd(2);
//   TLatex lat;
//   lat.SetTextSize(0.065);
//  lat.DrawLatex(0.7, 9.2, "Centrality");
  lat.DrawLatex(0.75, 3.4, "#it{R}_{inv}");
  lat.DrawLatex(0.75, 2.6, "#it{R}_{eff}");
  //  lat.DrawLatex(1.0, 7.1, "60-70\%");

  TMarker m1(0.88, 7.2, mars[0]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[0]); m1.DrawMarker(0.71, 3.65);
  TMarker m1(0.91, 7.2, mars[3]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[0]); m1.DrawMarker(0.71, 2.85);
  //  TMarker m1(0.94, 7.2, mars[6]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[0]); m1.DrawMarker(0.86, 7.3);

  TMarker m1(0.88, 7.2, mars[1]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[1]); m1.DrawMarker(0.67, 3.65);
  TMarker m1(0.91, 7.2, mars[4]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[1]); m1.DrawMarker(0.67, 2.85);
  //  TMarker m1(0.94, 7.2, mars[7]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[1]); m1.DrawMarker(0.91, 7.3);

  TMarker m1(0.88, 7.2, mars[2]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[2]); m1.DrawMarker(0.63, 3.65);
  TMarker m1(0.91, 7.2, mars[5]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[2]); m1.DrawMarker(0.63, 2.85);
  //  TMarker m1(0.94, 7.2, mars[8]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[2]); m1.DrawMarker(0.96, 7.3);

//   canfig1->cd(3);
  lat.DrawLatex(0.6, 9.5, "PbPb @ 2.76 ATeV");
  lat.DrawLatex(0.6, 8.7, "3+1D Hydro + Therminator");
  lat.DrawLatex(0.6, 7.9 , "pion, kaon, proton femtoscopy");

  return;

  TList *o1 = new TList();
  o1->Add(grp0c0);
  o1->Add(grk0c0);
  o1->Add(grr0c0);

  TGraphErrors *grsumo1 = getsumgraph(o1);

  TF1 *funo1 = drawlinefit(grsumo1);
  funo1->SetLineWidth(1);
  funo1->SetLineStyle(2);
  funo1->SetLineColor(kGray+3);
  funo1->SetRange(0.18, 1.49);
  //  funo1->Draw("SAME");


  


  // 2nd centrality

  TGraphErrors *grp0c1 = (TGraphErrors *) infiles[3]->Get("Pion5");
  preparegraph(grp0c1, 0, 1);
  grp0c1->Draw("P");

  TGraphErrors *grk0c1 = (TGraphErrors *) infiles[3]->Get("Kaon5");
  preparegraph(grk0c1, 1, 1);
  grk0c1->Draw("P");

  TGraphErrors *grr0c1 = (TGraphErrors *) infiles[3]->Get("Proton5");
  preparegraph(grr0c1, 2, 1);
  grr0c1->Draw("P");

  TList *o2 = new TList();
  o2->Add(grp0c1);
  o2->Add(grk0c1);
  o2->Add(grr0c1);

  TGraphErrors *grsumo2 = getsumgraph(o2);

  TF1 *funo2 = drawlinefit(grsumo2);
  funo2->SetLineWidth(1);
  funo2->SetLineStyle(2);
  funo2->SetLineColor(kGray+3);
  funo2->SetRange(0.18, 1.49);
  //  funo2->Draw("SAME");

  // 3rd centrality

  TGraphErrors *grp0c2 = (TGraphErrors *) infiles[7]->Get("Pion5");
  preparegraph(grp0c2, 0, 2);
  grp0c2->Draw("P");

  TGraphErrors *grk0c2 = (TGraphErrors *) infiles[7]->Get("Kaon5");
  preparegraph(grk0c2, 1, 2);
  grk0c2->Draw("P");

  TGraphErrors *grr0c2 = (TGraphErrors *) infiles[7]->Get("Proton5");
  preparegraph(grr0c2, 2, 2);
  grr0c2->GetY()[0] = -10.0;
  grr0c2->GetEY()[0] = 0.0;
  grr0c2->Draw("P");

  TList *o3 = new TList();
  o3->Add(grp0c2);
  o3->Add(grk0c2);
  o3->Add(grr0c2);

  TGraphErrors *grsumo3 = getsumgraph(o3);

  TF1 *funo3 = drawlinefit(grsumo3);
  funo3->SetLineWidth(1);
  funo3->SetLineStyle(2);
  funo3->SetLineColor(kGray+4);
  funo3->SetRange(0.18, 1.49);
  //  funo3->Draw("SAME");

  //  return;

  // Rinv scaled plot
  canfig1->cd(2);
  preparepad();

  TH1D *ramkas = new TH1D("ramkas",";#it{m}_{T} (GeV/#it{c}) ;#it{R}_{inv}/\\sqrt{(\\sqrt{\\gamma_{t}}+2)/3}   (fm)",100,0.15,1.55);
  ramkas->SetMinimum(1.01);
  ramkas->SetMaximum(10.4);
  preparehist(ramkas, 0);
  ramkas->Draw();

  // 1st centrality
  
  TGraphErrors *grp1c0 = getgammascaledgraph(grp0c0, 0.13957);
  preparegraph(grp1c0, 0, 0);
  grp1c0->Draw("P");

  TGraphErrors *grk1c0 = getgammascaledgraph(grk0c0, 0.493677);
  preparegraph(grk1c0, 1, 0);
  grk1c0->Draw("P");

  TGraphErrors *grr1c0 = getgammascaledgraph(grr0c0, 0.938272);
  preparegraph(grr1c0, 2, 0);
  grr1c0->Draw("P");

  TList *s1 = new TList();
  s1->Add(grp1c0);
  s1->Add(grk1c0);
  s1->Add(grr1c0);

  TGraphErrors *grsums1 = getsumgraph(s1);

  TF1 *funs1 = drawlinefit(grsums1);
  funs1->SetLineWidth(1);
  funs1->SetLineStyle(2);
  funs1->SetLineColor(kGray+3);
  funs1->SetRange(0.18, 1.49);
  funs1->Draw("SAME");

  // 2nd centrality
  
  TGraphErrors *grp1c1 = getgammascaledgraph(grp0c1, 0.13957);
  preparegraph(grp1c1, 0, 1);
  grp1c1->Draw("P");

  TGraphErrors *grk1c1 = getgammascaledgraph(grk0c1, 0.493677);
  preparegraph(grk1c1, 1, 1);
  grk1c1->Draw("P");

  TGraphErrors *grr1c1 = getgammascaledgraph(grr0c1, 0.938272);
  preparegraph(grr1c1, 2, 1);
  grr1c1->Draw("P");

  TList *s2 = new TList();
  s2->Add(grp1c1);
  s2->Add(grk1c1);
  s2->Add(grr1c1);

  TGraphErrors *grsums2 = getsumgraph(s2);

  TF1 *funs2 = drawlinefit(grsums2);
  funs2->SetLineWidth(1);
  funs2->SetLineStyle(2);
  funs2->SetLineColor(kGray+3);
  funs2->SetRange(0.18, 1.49);
  funs2->Draw("SAME");

  // 3rd centrality
  
  TGraphErrors *grp1c2 = getgammascaledgraph(grp0c2, 0.13957);
  preparegraph(grp1c2, 0, 2);
  grp1c2->Draw("P");

  TGraphErrors *grk1c2 = getgammascaledgraph(grk0c2, 0.493677);
  preparegraph(grk1c2, 1, 2);
  grk1c2->Draw("P");

  TGraphErrors *grr1c2 = getgammascaledgraph(grr0c2, 0.938272);
  preparegraph(grr1c2, 2, 2);
  grr1c2->Draw("P");

  TList *s3 = new TList();
  s3->Add(grp1c2);
  s3->Add(grk1c2);
  s3->Add(grr1c2);

  TGraphErrors *grsums3 = getsumgraph(s3);

  TF1 *funs3 = drawlinefit(grsums3);
  funs3->SetLineWidth(1);
  funs3->SetLineStyle(2);
  funs3->SetLineColor(kGray+3);
  funs3->SetRange(0.18, 1.49);
  funs3->Draw("SAME");

  //  return;

  // longitudinal plot

  TGraphErrors *grp2c0 = getlcmsavgraph((TGraphErrors *) infilespk[0]->Get("Pion0"), 
					(TGraphErrors *) infilespk[0]->Get("Pion1"),
					(TGraphErrors *) infilespk[0]->Get("Pion2"));
  preparegraph(grp2c0, 0, 0);

  TGraphErrors *grk2c0 = getlcmsavgraph((TGraphErrors *) infilespk[0]->Get("Kaon0"), 
					(TGraphErrors *) infilespk[0]->Get("Kaon1"),
					(TGraphErrors *) infilespk[0]->Get("Kaon2"));
  preparegraph(grk2c0, 1, 0);

  TGraphErrors *grr2c0 = getlcmsavgraph((TGraphErrors *) infilespp[0]->Get("Proton0"), 
					(TGraphErrors *) infilespp[0]->Get("Proton1"),
					(TGraphErrors *) infilespp[0]->Get("Proton2"));
  preparegraph(grr2c0, 2, 0);

  TGraphErrors *grp2c1 = getlcmsavgraph((TGraphErrors *) infilespk[3]->Get("Pion0"), 
					(TGraphErrors *) infilespk[3]->Get("Pion1"),
					(TGraphErrors *) infilespk[3]->Get("Pion2"));
  preparegraph(grp2c1, 0, 1);

  TGraphErrors *grk2c1 = getlcmsavgraph((TGraphErrors *) infilespk[3]->Get("Kaon0"), 
					(TGraphErrors *) infilespk[3]->Get("Kaon1"),
					(TGraphErrors *) infilespk[3]->Get("Kaon2"));
  preparegraph(grk2c1, 1, 1);

  TGraphErrors *grr2c1 = getlcmsavgraph((TGraphErrors *) infilespp[3]->Get("Proton0"), 
					(TGraphErrors *) infilespp[3]->Get("Proton1"),
					(TGraphErrors *) infilespp[3]->Get("Proton2"));
  preparegraph(grr2c1, 2, 1);

  TGraphErrors *grp2c2 = getlcmsavgraph((TGraphErrors *) infilespk[7]->Get("Pion0"), 
					(TGraphErrors *) infilespk[7]->Get("Pion1"),
					(TGraphErrors *) infilespk[7]->Get("Pion2"));
  preparegraph(grp2c2, 0, 2);

  TGraphErrors *grk2c2 = getlcmsavgraph((TGraphErrors *) infilespk[7]->Get("Kaon0"), 
					(TGraphErrors *) infilespk[7]->Get("Kaon1"),
					(TGraphErrors *) infilespk[7]->Get("Kaon2"));
  preparegraph(grk2c2, 1, 2);

  TGraphErrors *grr2c2 = getlcmsavgraph((TGraphErrors *) infilespp[7]->Get("Proton0"), 
					(TGraphErrors *) infilespp[7]->Get("Proton1"),
					(TGraphErrors *) infilespp[7]->Get("Proton2"));
  preparegraph(grr2c2, 2, 2);

  canfig1->cd(3);
  preparepad();

  TH1D *ramkal = new TH1D("ramkal",";m_{T} (GeV/#it{c});#it{R}_{LCMS} (fm)",100,0.15,1.55);
  ramkal->SetMinimum(1.01);
  ramkal->SetMaximum(12.2);
  preparehist(ramkal, 0);
  ramkal->Draw();

  // 1st centrality
  
  grp2c0->Draw("P");

  grk2c0->Draw("P");

  grr2c0->Draw("P");

  TList *l1 = new TList();
  l1->Add(grp2c0);
  l1->Add(grk2c0);
  l1->Add(grr2c0);

  TGraphErrors *grsuml1 = getsumgraph(l1);

  TF1 *funl1 = drawlinefit(grsuml1);
  funl1->SetLineWidth(1);
  funl1->SetLineStyle(2);
  funl1->SetLineColor(kGray+3);
  funl1->SetRange(0.18, 1.49);
  funl1->Draw("SAME");

  // 2nd centrality
  
  grp2c1->Draw("P");

  grk2c1->Draw("P");

  grr2c1->Draw("P");

  TList *l2 = new TList();
  l2->Add(grp2c1);
  l2->Add(grk2c1);
  l2->Add(grr2c1);

  TGraphErrors *grsuml2 = getsumgraph(l2);

  TF1 *funl2 = drawlinefit(grsuml2);
  funl2->SetLineWidth(1);
  funl2->SetLineStyle(2);
  funl2->SetLineColor(kGray+3);
  funl2->SetRange(0.18, 1.49);
  funl2->Draw("SAME");

  // 3rd centrality
  
  grp2c2->Draw("P");

  grk2c2->Draw("P");

  grr2c2->Draw("P");

  TList *l3 = new TList();
  l3->Add(grp2c2);
  l3->Add(grk2c2);
  l3->Add(grr2c2);

  TGraphErrors *grsuml3 = getsumgraph(l3);

  TF1 *funl3 = drawlinefit(grsuml3);
  funl3->SetLineWidth(1);
  funl3->SetLineStyle(2);
  funl3->SetLineColor(kGray+3);
  funl3->SetRange(0.18, 1.49);
  funl3->Draw("SAME");

  int cols[3] = { kBlue, kGreen+2, kRed };
  int mars[9] = { 21, 20, 33, 25, 24, 27, 28, 29, 34 };

  canfig1->cd(1);
  TLatex lat;
  lat.SetTextSize(0.065);
  lat.DrawLatex(1.0, 9.2, "Pions");
  lat.DrawLatex(1.0, 8.5, "Kaons");
  lat.DrawLatex(1.0, 7.8, "Protons");

  TMarker m1(0.88, 7.2, mars[0]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[0]); m1.DrawMarker(0.86, 9.4);
  TMarker m1(0.91, 7.2, mars[3]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[0]); m1.DrawMarker(0.91, 9.4);
  TMarker m1(0.94, 7.2, mars[6]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[0]); m1.DrawMarker(0.96, 9.4);

  TMarker m1(0.88, 7.2, mars[1]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[1]); m1.DrawMarker(0.86, 8.7);
  TMarker m1(0.91, 7.2, mars[4]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[1]); m1.DrawMarker(0.91, 8.7);
  TMarker m1(0.94, 7.2, mars[7]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[1]); m1.DrawMarker(0.96, 8.7);

  TMarker m1(0.88, 7.2, mars[2]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[2]); m1.DrawMarker(0.86, 7.0);
  TMarker m1(0.91, 7.2, mars[5]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[2]); m1.DrawMarker(0.91, 7.0);
  TMarker m1(0.94, 7.2, mars[8]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[2]); m1.DrawMarker(0.96, 7.0);

  canfig1->cd(2);
  TLatex lat;
  lat.SetTextSize(0.065);
  lat.DrawLatex(1.0, 9.2, "Centrality");
  lat.DrawLatex(1.0, 8.5, "0-5\%");
  lat.DrawLatex(1.0, 7.8, "20-30\%");
  lat.DrawLatex(1.0, 7.1, "60-70\%");

  TMarker m1(0.88, 7.2, mars[0]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[0]); m1.DrawMarker(0.86, 8.7);
  TMarker m1(0.91, 7.2, mars[3]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[0]); m1.DrawMarker(0.86, 8.0);
  TMarker m1(0.94, 7.2, mars[6]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[0]); m1.DrawMarker(0.86, 7.3);

  TMarker m1(0.88, 7.2, mars[1]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[1]); m1.DrawMarker(0.91, 8.7);
  TMarker m1(0.91, 7.2, mars[4]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[1]); m1.DrawMarker(0.91, 8.0);
  TMarker m1(0.94, 7.2, mars[7]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[1]); m1.DrawMarker(0.91, 7.3);

  TMarker m1(0.88, 7.2, mars[2]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[2]); m1.DrawMarker(0.96, 8.7);
  TMarker m1(0.91, 7.2, mars[5]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[2]); m1.DrawMarker(0.96, 8.0);
  TMarker m1(0.94, 7.2, mars[8]); m1.SetMarkerSize(1.2); m1.SetMarkerColor(cols[2]); m1.DrawMarker(0.96, 7.3);

  canfig1->cd(3);
  lat.DrawLatex(0.6, 11.0, "PbPb @ 2.76 ATeV");
  lat.DrawLatex(0.6, 10.0, "3+1D Hydro + Therminator");
  lat.DrawLatex(0.6, 9.0 , "pion, kaon, proton femtoscopy");

}
