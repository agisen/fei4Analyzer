/****************************************************************************
** Author: Stefano Terzo
**
** Max-Planck-instiut fuer Physik (Werner-Heisenberg-Institut)
** Foehringer Ring 6, D-80805 Muenchen, Germany
**
****************************************************************************/

#include "Plotter.h"

Plotter::Plotter(bool quiet, int module_type)
{
    this->setModuleType(module_type);
    theFitter = new Fitter(quiet);
    quiet_ = quiet;
    empty_ = true;
    v_=0;
    minColCut_ = -1;
    minRowCut_ = -1;
    maxColCut_ = -1;
    maxRowCut_ = -1;
}
//==========================================
Plotter::~Plotter()
{
	theFitter->getStat();
	delete theFitter;
	
	if(!empty_) this->deletePlots();
	/*
	for(unsigned int i=0; i<graphs.size(); ++i)
	{
	  if(graphs[i]!=NULL) delete graphs[i];
	}
	*/
}
//================================================================================================
void Plotter::setCuts(int minCol, int minRow, int maxCol, int maxRow)
{
   minColCut_ = minCol;
   minRowCut_ = minRow;
   maxColCut_ = maxCol;
   maxRowCut_ = maxRow;
}
//================================================================================================
void Plotter::setCuts(int colRowCuts[4])
{
   minColCut_ = colRowCuts[0];
   minRowCut_ = colRowCuts[1];
   maxColCut_ = colRowCuts[2];
   maxRowCut_ = colRowCuts[3];
}
//==========================================CLUSTER TOT ANALYSIS=====================================
void Plotter::fillClusterPlots(Clusterizer::clusterMapDef &clusterMap, double noise)
{
  if(!empty_) this->deletePlots();

  TH1::AddDirectory(kFALSE);
  TH2::AddDirectory(kFALSE);
  //Calibrator *theCalibrator = new Calibrator();
  if(isQuad_)
  {
     clusterMap_cs1_all_       = new TH2I("clusterMap_cs1","Cluster Map CS1 - all chips",80*2,0,80*2,336*2,0,336*2);		   
     clusterMap_cs2_all_       = new TH2I("clusterMap_cs2","Cluster Map CS2 - all chips",80*2,0,80*2,336*2,0,336*2);		   
     clusterTotMap_cs1_all_    = new TH2D("clusterTotMap_cs1","Cluster Tot Map CS1 - all chips",80*2,0,80*2,336*2,0,336*2);	       
     clusterTotMap_cs2_all_    = new TH2D("clusterTotMap_cs2","Cluster Tot Map CS2 - all chips",80*2,0,80*2,336*2,0,336*2);	       
     clusterMeanTotMap_cs1_all_= new TH2D("clusterMeanTotMap_cs1","Cluster Mean Tot Map CS1 - all chips",80*2,0,80*2,336*2,0,336*2);
     
     clusterToT_all_ 	       = new TH1I("ToTdist","ToT Distribution",50,-0.5,49.5) ;
     one_hitToT_all_ 	       = new TH1I("ToTdistCS1","ToT Distribution CS1",50,-0.5,49.5) ;
     two_hitToT_all_ 	       = new TH1I("ToTdistCS2","ToT Distribution CS2",50,-0.5,49.5) ;
     totMax_all_	       = new TH1I("ToTdistCS2_LR","ToT Max Distribution CS2",50,-0.5,49.5) ; 	    
     totMin_all_	       = new TH1I("ToTdistCS2_UD","ToT Min Distribution CS2",50,-0.5,49.5) ; 	    
     clusterSize_all_	       = new TH1I("CSdist","Cluster Size Distribution",11,0.5,10.5);
     clusterSizeRow_all_       = new TH1I("CSdistRow","Cluster Size Distribution Rows",11,0.5,10.5);
     clusterSizeCol_all_       = new TH1I("CSdistCol","Cluster Size Distribution Cols",11,0.5,10.5);
     clusterCharge_all_	       = new TH1D("Qdist","Charge Distribution",100,0,100 ) ;
     clusterCharge_cs1_all_    = new TH1D("QdistCS1","Charge Distribution CS1",100,0,100) ;
     clusterCharge_cs2_all_    = new TH1D("QdistCS2","Charge Distribution CS2",100,0,100) ;
  }

  for(Clusterizer::clusterMapDef::iterator ev = clusterMap.begin(); ev!=clusterMap.end(); ++ev)
  {
    for(std::map<int, Clusterizer::clustersDef>::iterator chip=(*ev).second.begin(); chip!=(*ev).second.end(); ++chip)
    {
      if(clusterToT_.count((*chip).first)==0)
      {
        int cols = 80;
	int rows = 336;
	if(design25_)
	{
	  cols /=2;
	  rows *=2;
	}
      
        std::cout << "Making new histograms for chip: " << (*chip).first << std::endl;
        std::stringstream ss;
        ss.str("");
        ss << "chip"<< (*chip).first << "_ToTdist";
 	clusterToT_[(*chip).first] = new TH1I(ss.str().c_str(),"ToT Distribution",50,-0.5,49.5) ;
 	ss.str("");
        ss << "chip"<< (*chip).first << "_ToTdistCS1";
 	one_hitToT_[(*chip).first] = new TH1I(ss.str().c_str(),"ToT Distribution CS1",50,-0.5,49.5) ;
 	ss.str("");
 	ss << "chip"<< (*chip).first << "_ToTdistCS2";
 	two_hitToT_[(*chip).first] = new TH1I(ss.str().c_str(),"ToT Distribution CS2",50,-0.5,49.5) ;
	ss.str("");
	ss << "chip"<< (*chip).first << "_ToTdistCS2_LR";
	totMax_[(*chip).first] = new TH1I(ss.str().c_str(),"ToT Max Distribution CS2",50,-0.5,49.5) ;
	ss.str("");
 	ss << "chip"<< (*chip).first << "_ToTdistCS2_UD";
 	totMin_[(*chip).first] = new TH1I(ss.str().c_str(),"ToT Min Distribution CS2",50,-0.5,49.5) ;
 	ss.str("");
 	ss << "chip"<< (*chip).first << "_CSdist";
 	clusterSize_[(*chip).first]= new TH1I(ss.str().c_str(),"Cluster Size Distribution",11,0.5,10.5);
	ss.str("");
 	ss << "chip"<< (*chip).first << "_CSdistRow";
 	clusterSizeRow_[(*chip).first]= new TH1I(ss.str().c_str(),"Cluster Size Distribution Rows",11,0.5,10.5);
        ss.str("");
 	ss << "chip"<< (*chip).first << "_CSdistCol";
 	clusterSizeCol_[(*chip).first]= new TH1I(ss.str().c_str(),"Cluster Size Distribution Cols",11,0.5,10.5);
	ss.str("");
 	ss << "chip"<< (*chip).first << "_clusterMap_cs1";
 	clusterMap_cs1_[(*chip).first]= new TH2I(ss.str().c_str(),"Cluster Map CS1",cols,0,cols,rows,0,rows);
	ss.str("");
 	ss << "chip"<< (*chip).first << "_clusterMap_cs2";
 	clusterMap_cs2_[(*chip).first]= new TH2I(ss.str().c_str(),"Cluster Map CS2",cols,0,cols,rows,0,rows);
	ss.str("");
        ss << "chip"<< (*chip).first << "_clusterTotMap_cs1";
 	clusterTotMap_cs1_[(*chip).first]= new TH2D(ss.str().c_str(),"Cluster Tot Map CS1",cols,0,cols,rows,0,rows);
	ss.str("");
        ss << "chip"<< (*chip).first << "_clusterTotMap_cs2";
 	clusterTotMap_cs2_[(*chip).first]= new TH2D(ss.str().c_str(),"Cluster Tot Map CS2",cols,0,cols,rows,0,rows);
        ss.str("");
        ss << "chip"<< (*chip).first << "_clusterMeanTotMap_cs1";
 	clusterMeanTotMap_cs1_[(*chip).first]= new TH2D(ss.str().c_str(),"Cluster Mean Tot Map CS1",cols,0,cols,rows,0,rows);
	ss.str("");
        ss << "chip"<< (*chip).first << "_Qdist";
 	clusterCharge_[(*chip).first] = new TH1D(ss.str().c_str(),"Charge Distribution",100,0,100	) ;
 	ss.str("");
        ss << "chip"<< (*chip).first << "_QdistCS1";
 	clusterCharge_cs1_[(*chip).first] = new TH1D(ss.str().c_str(),"Charge Distribution CS1",100,0,100) ;
 	ss.str("");
 	ss << "chip"<< (*chip).first << "_QdistCS2";
 	clusterCharge_cs2_[(*chip).first] = new TH1D(ss.str().c_str(),"Charge Distribution CS2",100,0,100) ;
      }
      
      for( Clusterizer::clustersDef::iterator clus=(*chip).second.begin(); clus!=(*chip).second.end(); ++clus )
      {
    	 int cToT=0;
         double chargeSum = 0;
	 int maxRow, maxCol, minRow, minCol, maxRowTot, minRowTot , maxColTot, minColTot, maxTot, minTot;
	 maxRow=minRow= (*clus).second[0].row;
	 maxCol=minCol= (*clus).second[0].col;
	 maxTot=minTot=minRowTot=maxRowTot=maxColTot=minColTot=(*clus).second[0].tot;
	 unsigned int cSize = (*clus).second.size();
	 bool bad_cluster = false;
	 
	 for(unsigned int hit=0; hit<cSize; hit++)
    	 {
	   if( minColCut_ > -1 && clus->second[hit].col < minColCut_ ) continue;
	   if( minRowCut_ > -1 && clus->second[hit].row < minRowCut_ ) continue;
	   if( maxColCut_ > -1 && clus->second[hit].col > maxColCut_ ) continue;
           if( maxRowCut_ > -1 && clus->second[hit].row > maxRowCut_ ) continue;
	   
	   
	   if(noise > 0 && 1.*hitMap_[(*chip).first]->GetBinContent((*clus).second[hit].col+1,(*clus).second[hit].row+1)/hitMap_[(*chip).first]->GetEntries() > noise) 
	   { 
	      bad_cluster = true;
	      continue;
	   }
	   
	   int& tot=(*clus).second[hit].tot;
           //double charge = theCalibrator->calib( (*clus).second[hit] );
	   double charge = 0;
           chargeSum += charge;
    	   cToT+=tot;
           if( (*clus).second[hit].row > maxRow ) 
	   {
	     maxRow = (*clus).second[hit].row;
	     maxRowTot = (*clus).second[hit].tot;
	   }
	   if( (*clus).second[hit].row < minRow ) 
	   {
	     minRow = (*clus).second[hit].row;
	     minRowTot = (*clus).second[hit].tot;
	   }
	   if( (*clus).second[hit].col > maxCol ) 
	   {
	     maxCol = (*clus).second[hit].col;
	     maxColTot = (*clus).second[hit].tot;
	   }
	   if( (*clus).second[hit].col < minCol ) 
	   {
	     minCol = (*clus).second[hit].col;
	     minColTot = (*clus).second[hit].tot;
	   }
	   if(tot>maxTot) maxTot=tot;
	   if(tot<minTot) minTot=tot;
	   
	   if(cSize==1) 
	   {
	      clusterMap_cs1_[(*chip).first]->Fill((*clus).second[hit].col, (*clus).second[hit].row);
	      clusterTotMap_cs1_[(*chip).first]->Fill((*clus).second[hit].col, (*clus).second[hit].row,cToT);
	   }
	   if(cSize==2) 
	   { 
	      clusterMap_cs2_[(*chip).first]->Fill((*clus).second[hit].col, (*clus).second[hit].row);
	      clusterTotMap_cs2_[(*chip).first]->Fill((*clus).second[hit].col, (*clus).second[hit].row,cToT);
	   }
	   
	   if(isQuad_)
	   {
	      int col = (*clus).second[hit].col;					     	
	      int row = (*clus).second[hit].row;					        
	      this->quadEncode((*chip).first, col, row);				     	
	      if(cSize==1) 
	      {									        
	        clusterMap_cs1_all_->Fill(col, row);	        
	        clusterTotMap_cs1_all_->Fill(col, row,cToT);
	      }
	      if(cSize==2) 
	      { 
	         clusterMap_cs2_all_->Fill(col, row);
	         clusterTotMap_cs2_all_->Fill(col, row,cToT);
	      }
	   }
	   
    	 }
	 
	 if(bad_cluster) continue;
	 
    	 if(cSize!=0)  
	 {
	    clusterSize_[(*chip).first]->Fill(cSize);
	    clusterSizeRow_[(*chip).first]->Fill( maxRow-minRow+1 );
	    clusterSizeCol_[(*chip).first]->Fill( maxCol-minCol+1 );
	 }
    	 else std::cout << "WARNING: cluster size 0" << std::endl;
	 
	 if(cSize==2)
	 {				
	   if(maxRow==minRow)
	   {	  
	     totMax_[(*chip).first]->Fill(maxTot);
	   }
	   else
	   {
	     totMin_[(*chip).first]->Fill(minTot);
	   }
	   
	   if( (maxTot-minTot)<0 ) clusterTotMap_cs2_[(*chip).first]->Fill((maxCol+minCol)*0.5,(maxRow+minRow)*0.5);				   
	 }
	 				  
         clusterToT_[(*chip).first]->Fill(cToT);
         if(chargeSum !=0 ) clusterCharge_[(*chip).first]->Fill(chargeSum);
    	 if( cSize==1 ) 
         {
           one_hitToT_[(*chip).first]->Fill(cToT);
           if(chargeSum !=0 )clusterCharge_cs1_[(*chip).first]->Fill(chargeSum);
         }
    	 if( cSize==2 ) 
         {
            two_hitToT_[(*chip).first]->Fill(cToT);
            if(chargeSum !=0 )clusterCharge_cs2_[(*chip).first]->Fill(chargeSum);
         }
      }
    }
  }
  
  for(std::map<int, TH1I*>::iterator chip=clusterToT_.begin(); chip!=clusterToT_.end(); ++chip)
  {   
    
    clusterMeanTotMap_cs1_[(*chip).first]->Divide(clusterTotMap_cs1_[(*chip).first],clusterMap_cs1_[(*chip).first],1.0,1.0,"B");
    
    if(isQuad_) 						    
    {								    
       clusterToT_all_	     ->Add( (*chip).second			     );
       one_hitToT_all_	     ->Add( one_hitToT_[(*chip).first]  	     );
       two_hitToT_all_	     ->Add( two_hitToT_[(*chip).first]  	     );
       totMax_all_	     ->Add( totMax_[(*chip).first]	     );  
       totMin_all_	     ->Add( totMin_[(*chip).first]	     );  
       clusterSize_all_	     ->Add( clusterSize_[(*chip).first]	        );  
       clusterSizeRow_all_   ->Add( clusterSizeRow_[(*chip).first]   );  
       clusterSizeCol_all_   ->Add( clusterSizeCol_[(*chip).first]   );  
       clusterCharge_all_    ->Add( clusterCharge_[(*chip).first]    );  
       clusterCharge_cs1_all_->Add( clusterCharge_cs1_[(*chip).first]   );
       clusterCharge_cs2_all_->Add( clusterCharge_cs2_[(*chip).first]	);
    }
  }
  if(isQuad_) clusterMeanTotMap_cs1_all_->Divide(clusterTotMap_cs1_all_,clusterMap_cs1_all_,1.0,1.0,"B");						    
  
  empty_=false;
}
  
//============================================
void Plotter::fillHitPlots(EventMaker::hitMapDef& hitMap)
{ 
  if(isQuad_) hitMap_all_ = new TH2I("HitMap_all", "HitMap_all",80*2,0,80*2,336*2,0,336*2);

  for(EventMaker::hitMapDef::iterator ev = hitMap.begin(); ev!=hitMap.end(); ++ev)
  {
    for(std::map<int, EventMaker::hitsDef>::iterator chip=(*ev).second.begin(); chip!=(*ev).second.end(); ++chip)
    {
    
      if(hitMap_.count((*chip).first)==0)
      {
        std::cout << "Making new histograms for chip: " << (*chip).first << std::endl;
 	ss_.str("");
	ss_ << "chip"<< (*chip).first << "_HitMap";
	int rows = 336;
	int cols = 80;
	if(design25_)
	{
	   rows *=2;
	   cols /=2;
	}
 	hitMap_[(*chip).first]= new TH2I(ss_.str().c_str(), ss_.str().c_str(),cols,0,cols,rows,0,rows);
      }
 
      for(unsigned int h=0; h<(*chip).second.size(); h++)
      {
        if( minColCut_ > -1 && chip->second[h].col < minColCut_ ) continue;
	if( minRowCut_ > -1 && chip->second[h].row < minRowCut_ ) continue;
	if( maxColCut_ > -1 && chip->second[h].col > maxColCut_ ) continue;
	if( maxRowCut_ > -1 && chip->second[h].row > maxRowCut_ ) continue;
      
        hitMap_[(*chip).first]->Fill((*chip).second[h].col, (*chip).second[h].row);
      
      	if(isQuad_)
      	{
           int col = (*chip).second[h].col;	        		       
           int row = (*chip).second[h].row;	        			       
      	   this->quadEncode((*chip).first, col, row);
           hitMap_all_->Fill(col, row); 	       
      	}
      } 
    }
  }
  
}
//============================================
void Plotter::quadEncode(const int chip, int &col, int &row)
{
  if(chip == 0)
  { 
  	col = col;
	row = 335-row;
  }
  if(chip == 1)
  {
  	col += 80;
	row = 335-row;
  }
  if(chip == 2)
  {
  	col = 159 - col;
	row += 336;
  }
  if(chip == 3)
  {
  	col = 79 - col;
	row += 336;
  }
}
//==================================================================
void Plotter::setModuleType(int module_type)
{
	if(module_type == 4) 
	{ 
	   std::cout << "Quad module analysis" << std::endl;
	   isQuad_=true ;
	}
	else isQuad_=false;
	
	if(module_type == 25)
	{
	   std::cout << "FE-I4 with 500x25um arrangment analysis" << std::endl;
	   design25_ = true;
	}
	else design25_ = false;
}
//=========================FIT HISTOS=========================================
void Plotter::fitPlots(double voltage)
{
  if(voltage == 0) v_++;
  else             v_=voltage;
  
  for(std::map<int, TH1I*>::iterator chip=clusterToT_.begin(); chip!=clusterToT_.end(); ++chip)
  {   
    ToT_all_[chip->first][v_] = theFitter->fit( (*chip).second		      );
    ToT_cs1_[chip->first][v_] = theFitter->fit( two_hitToT_[(*chip).first]        );
    ToT_cs2_[chip->first][v_] = theFitter->fit( one_hitToT_[(*chip).first]        );
    //theFitter->fit( clusterCharge_[(*chip).first]	);
    //theFitter->fit( clusterCharge_cs1_[(*chip).first] );
    //theFitter->fit( clusterCharge_cs2_[(*chip).first] );
  }
  if(isQuad_)
  {
    ToT_mod_all_[v_] = theFitter->fit( clusterToT_all_ );
    ToT_mod_cs1_[v_] = theFitter->fit( one_hitToT_all_ );
    ToT_mod_cs2_[v_] = theFitter->fit( two_hitToT_all_ );
    //theFitter->fit( clusterCharge_all_	);
    //theFitter->fit( clusterCharge_cs1_all_	);
    //theFitter->fit( clusterCharge_cs2_all_	);
  }
}
//=========================WRITEOUT HISTOS=========================================
void Plotter::writePlots(std::string rootFileName, bool bunch)
{ 
  
  TFile outRootFile(rootFileName.c_str(),"recreate");
  
  for(std::map<int, TH1I*>::iterator chip=clusterToT_.begin(); chip!=clusterToT_.end(); ++chip)
  {
    ss_.str("");
    ss_ << "chip"<< (*chip).first;
    TDirectory *dir = outRootFile.mkdir(ss_.str().c_str());
    dir->cd();
    
    (*chip).second->Write();
    two_hitToT_[(*chip).first]->Write();
    one_hitToT_[(*chip).first]->Write();
    clusterSize_[(*chip).first]->Write();
    clusterSizeRow_[(*chip).first]->Write();
    clusterSizeCol_[(*chip).first]->Write();
    totMax_[(*chip).first]->Write();
    totMin_[(*chip).first]->Write();
    hitMap_[(*chip).first]->Write();
    clusterMap_cs1_[(*chip).first]->Write();
    clusterMap_cs2_[(*chip).first]->Write();
    clusterTotMap_cs1_[(*chip).first]->Write();
    clusterTotMap_cs2_[(*chip).first]->Write();
    clusterMeanTotMap_cs1_[(*chip).first]->Write();
    clusterCharge_[(*chip).first]->Write();
    clusterCharge_cs1_[(*chip).first]->Write();
    clusterCharge_cs2_[(*chip).first]->Write();	         
  }
  
  if(isQuad_)
  {
     outRootFile.cd();
     
     clusterToT_all_->Write();
     one_hitToT_all_->Write();
     two_hitToT_all_->Write();
     totMax_all_->Write();
     totMin_all_->Write();
     clusterSize_all_->Write();
     clusterSizeRow_all_->Write();
     clusterSizeCol_all_->Write();
     clusterCharge_all_->Write();
     clusterCharge_cs1_all_->Write();
     clusterCharge_cs2_all_->Write();
     clusterMap_cs1_all_->Write();	 
     clusterMap_cs2_all_->Write();	 
     clusterTotMap_cs1_all_->Write();    
     clusterTotMap_cs2_all_->Write();   
     clusterMeanTotMap_cs1_all_->Write();
     hitMap_all_->Write();
  }
  
  outRootFile.Close();
  this->deletePlots();
  
}
//========================================================================================================
void Plotter::showGraph(std::vector<double> correction_factors,unsigned int fit_function)
{ 
  TCanvas *canvas_mod;
  std::vector<TCanvas*> canvas;

  TFile outRootFile("tempOutput.root","recreate");

  for(std::map<int, std::map<double,Fitter::fitResultDef> >::iterator chip=ToT_all_.begin(); chip!=ToT_all_.end(); ++chip)
  { 
    ss_.str("");
    ss_ << "canvas_chip_" << chip->first;
    canvas.push_back( new TCanvas(ss_.str().c_str(),ss_.str().c_str(),200,10,900,700) );
    canvas.back()->cd();
    double hix=0,hiy=0,lox=0,loy=0;
    std::vector<TGraphErrors*> graphs;
    
    graphs.push_back( new TGraphErrors( ToT_all_.size() ) );
    graphs.push_back( new TGraphErrors( ToT_cs1_.size() ) );
    graphs.push_back( new TGraphErrors( ToT_cs2_.size() ) );

    std::map<double, Fitter::fitResultDef> vec;
    double correction_factor;
    if( correction_factors.size() > chip->first ) 
            correction_factor = correction_factors[chip->first]; //2.46 2.42 2.75 2.49 2.59
    else    correction_factor = 1;
    for(unsigned int i=0; i<graphs.size(); ++i)
    {
      if(i==0) vec = ToT_all_[chip->first];
      if(i==1) vec = ToT_cs1_[chip->first];
      if(i==2) vec = ToT_cs2_[chip->first];
      unsigned int p=0;
      for(std::map<double, Fitter::fitResultDef>::iterator vbias=vec.begin(); vbias!=vec.end(); ++vbias)
      {
    	 double x = vbias->first; 
    	 double y;
	 if(fit_function == 2) 
	    y = vbias->second.MPV*correction_factor;
	 else    
	    y = vbias->second.MP*correction_factor;
	                
    	 graphs[i]->SetPoint(p,x,y);
    	 graphs[i]->SetPointError(p,0,0.1*y);
    	 if(x>hix) hix=x;
    	 if(y>hiy) hiy=y;
    	 if(x<lox) lox=x;
    	 if(y<loy) loy=y;
    	 p++;
      }
    }

    TH1F *hr = canvas.back()->cd()->DrawFrame(lox,loy,hix+abs(hix-lox)*0.1,hiy*1.1);
    hr->SetTitle("");
    hr->SetTitleSize(0.06);
    hr->SetTitleFont(132);
    //set x-axis
    hr->GetXaxis()->SetTitle("Bias voltage [V]");
    hr->GetXaxis()->SetLabelFont(132);
    hr->GetXaxis()->SetLabelSize(0.06);
    hr->GetXaxis()->SetTitleSize(0.06);
    hr->GetXaxis()->SetTitleFont(132);
    //set y-axis
    hr->GetYaxis()->SetTitle("Collected charge [ke]");
    hr->GetYaxis()->SetLabelFont(132);
    hr->GetYaxis()->SetLabelSize(0.06);
    hr->GetYaxis()->SetTitleOffset(0.70);
    hr->GetYaxis()->SetTitleSize(0.06);
    hr->GetYaxis()->SetTitleFont(132);

    for(unsigned int i=0; i<graphs.size(); ++i)
    {
       graphs[i]->SetMarkerStyle(20);
       graphs[i]->SetMarkerSize(2);
       graphs[i]->SetMarkerColor(2+i);
       graphs[i]->SetFillColor(2+i);
       graphs[i]->SetFillStyle(3004+i);
       graphs[i]->Draw("PE3");
    }
    canvas.back()->Write();
  }
  
  if(isQuad_)
  {  
     double hix=0,hiy=0,lox=0,loy=0;
     canvas_mod = new TCanvas("canvas_module","title",200,10,900,700);
     canvas_mod->cd();
     
     std::vector<TGraphErrors*>          graphs_mod;
     graphs_mod.push_back( new TGraphErrors( ToT_mod_all_.size() ) );
     graphs_mod.push_back( new TGraphErrors( ToT_mod_cs1_.size() ) );
     graphs_mod.push_back( new TGraphErrors( ToT_mod_cs2_.size() ) );
     
     std::map<double, Fitter::fitResultDef> vec;
     double correction_factor;
     if(correction_factors.size() != 0) 
                correction_factor = correction_factors.back();
     else       correction_factor = 1;
     for(unsigned int i=0; i<graphs_mod.size(); ++i)							  
     {  												  
        												  
       if(i==0) vec = ToT_mod_all_;									  
       if(i==1) vec = ToT_mod_cs1_;									  
       if(i==2) vec = ToT_mod_cs2_;									  
       unsigned int p=0;										  
       for(std::map<double, Fitter::fitResultDef>::iterator vbias=vec.begin(); vbias!=vec.end(); ++vbias) 
       {  												  
     	  double x = vbias->first;									  
     	  double y = vbias->second.MP*correction_factor;						  
     	  graphs_mod[i]->SetPoint(p,x,y);									  
     	  graphs_mod[i]->SetPointError(p,0,0.1*y);								  
     	  if(x>hix) hix=x;										  
     	  if(y>hiy) hiy=y;										  
     	  if(x<lox) lox=x;										  
     	  if(y<loy) loy=y;										  
     	  p++;												  
       }												  
     }													  
  
     TH1F *hr = canvas_mod->cd()->DrawFrame(lox,loy,hix+abs(hix-lox)*0.1,hiy*1.1);
     hr->SetTitle("");
     hr->SetTitleSize(0.06);
     hr->SetTitleFont(132);
     //set x-axis
     hr->GetXaxis()->SetTitle("Bias voltage [V]");
     hr->GetXaxis()->SetLabelFont(132);
     hr->GetXaxis()->SetLabelSize(0.06);
     hr->GetXaxis()->SetTitleSize(0.06);
     hr->GetXaxis()->SetTitleFont(132);
     //set y-axis
     hr->GetYaxis()->SetTitle("Collected charge [ke]");
     hr->GetYaxis()->SetLabelFont(132);
     hr->GetYaxis()->SetLabelSize(0.06);
     hr->GetYaxis()->SetTitleOffset(0.70);
     hr->GetYaxis()->SetTitleSize(0.06);
     hr->GetYaxis()->SetTitleFont(132);
  
     for(unsigned int i=0; i<graphs_mod.size(); ++i)
     { 
     	graphs_mod[i]->SetMarkerStyle(20);
     	graphs_mod[i]->SetMarkerSize(2);
     	graphs_mod[i]->SetMarkerColor(2+i);
     	graphs_mod[i]->SetFillColor(2+i);
     	graphs_mod[i]->SetFillStyle(3004+i);
     	graphs_mod[i]->Draw("PE3");
     }
     
     canvas_mod->Write();
  }
  
  
  //canvas->Write();
  outRootFile.Close();
}
//========================================================================================================
void Plotter::deletePlots(void)
{

  if(isQuad_)											      
  {
    if(clusterMap_cs1_all_!=NULL)       delete clusterMap_cs1_all_;	     		    
    if(clusterMap_cs2_all_!=NULL)       delete clusterMap_cs2_all_;	     		    
    if(clusterTotMap_cs1_all_!=NULL)    delete clusterTotMap_cs1_all_;       		    
    if(clusterTotMap_cs2_all_!=NULL)    delete clusterTotMap_cs2_all_;       		    
    if(clusterMeanTotMap_cs1_all_!=NULL)delete clusterMeanTotMap_cs1_all_;   		    
    if(hitMap_all_!=NULL)               delete hitMap_all_;		     		    
    if(clusterToT_all_!=NULL)   	delete clusterToT_all_; 	     						     
    if(one_hitToT_all_!=NULL)   	delete one_hitToT_all_; 	     						     
    if(two_hitToT_all_!=NULL)   	delete two_hitToT_all_; 	     						     
    if(totMax_all_!=NULL)   		delete totMax_all_;		     					    
    if(totMin_all_!=NULL)   		delete totMin_all_;		     					    
    if(clusterSize_all_!=NULL) 	    	delete clusterSize_all_;	     						     
    if(clusterSizeRow_all_!=NULL)   	delete clusterSizeRow_all_;	     						    
    if(clusterSizeCol_all_!=NULL)   	delete clusterSizeCol_all_;	     						    
    if(clusterCharge_all_!=NULL)        delete clusterCharge_all_;	     									     
    if(clusterCharge_cs1_all_!=NULL) 	delete clusterCharge_cs1_all_;       						     
    if(clusterCharge_cs2_all_!=NULL) 	delete clusterCharge_cs2_all_;       						     
  }

  for(std::map<int, TH1I*>::iterator chip=clusterToT_.begin(); chip!=clusterToT_.end(); ++chip)
  {	    											      
    if((*chip).second                          !=NULL) delete (*chip).second;   							      
    if(two_hitToT_[(*chip).first] 	       !=NULL) delete two_hitToT_[(*chip).first];
    if(one_hitToT_[(*chip).first] 	       !=NULL) delete one_hitToT_[(*chip).first];
    if(clusterSize_[(*chip).first] 	       !=NULL) delete clusterSize_[(*chip).first];
    if(clusterSizeRow_[(*chip).first]          !=NULL) delete clusterSizeRow_[(*chip).first];
    if(clusterSizeCol_[(*chip).first]          !=NULL) delete clusterSizeCol_[(*chip).first];
    if(totMax_[(*chip).first]   	       !=NULL) delete totMax_[(*chip).first];
    if(totMin_[(*chip).first]   	       !=NULL) delete totMin_[(*chip).first];
    if(hitMap_[(*chip).first]   	       !=NULL) delete hitMap_[(*chip).first];
    if(clusterMap_cs1_[(*chip).first]          !=NULL) delete clusterMap_cs1_[(*chip).first];
    if(clusterMap_cs2_[(*chip).first]          !=NULL) delete clusterMap_cs2_[(*chip).first];
    if(clusterTotMap_cs1_[(*chip).first]       !=NULL) delete clusterTotMap_cs1_[(*chip).first];
    if(clusterTotMap_cs2_[(*chip).first]       !=NULL) delete clusterTotMap_cs2_[(*chip).first];
    if(clusterMeanTotMap_cs1_[(*chip).first]   !=NULL) delete clusterMeanTotMap_cs1_[(*chip).first];
    if(clusterCharge_[(*chip).first] 	       !=NULL) delete clusterCharge_[(*chip).first];
    if(clusterCharge_cs1_[(*chip).first]       !=NULL) delete clusterCharge_cs1_[(*chip).first];
    if(clusterCharge_cs2_[(*chip).first]       !=NULL) delete clusterCharge_cs2_[(*chip).first];
  }
  clusterToT_.clear();
  one_hitToT_.clear();
  two_hitToT_.clear();
  clusterSize_.clear();
  clusterSizeRow_.clear();
  clusterSizeCol_.clear();
  totMax_.clear();
  totMin_.clear();
  hitMap_.clear();
  clusterMap_cs1_.clear();
  clusterMap_cs2_.clear();
  clusterTotMap_cs1_.clear();
  clusterTotMap_cs2_.clear();
  clusterMeanTotMap_cs1_.clear();
  clusterCharge_.clear();
  clusterCharge_cs1_.clear();
  clusterCharge_cs2_.clear();
  
  
  empty_ = true;
}
