#include "climate_change.h"
#include "DataPoint.h"


#include <string>
#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TAxis.h>

namespace project {
    void climate_change(std::string root_file_path) {
    TFile *root_file=TFile::Open(root_file_path.c_str(), "READ"); //open file
    
    if (root_file==nullptr || root_file->IsZombie()) { //Making sure file opened properly
        std::cerr << "Error opening root file \n";
        return;
    }
    TTree *tree = dynamic_cast<TTree*>(root_file->Get("data")); //get tree from root file

    if (tree == nullptr) { //make sure tree was found
        std::cerr << "Error: could not find TTree data\n";
        root_file->Close();
        return;}
    plot_average_temperature(tree,1722,2022,"./images/average_temperature_1722-2022.png");
    
    root_file->Close();

    }
    double average_temperature(TTree* tree, int year) {
        DataPoint *datapoint=nullptr; //initialize datapoint object
        tree->SetBranchAddress("datapoint", &datapoint); //set branch address to the newly initialized datapoint object

        Long64_t entries = tree->GetEntries();
        Long64_t entries_in_year=0;
        double_t temperature_sum=0.0;

        for (Long64_t i=0; i < entries; ++i) {
            tree->GetEntry(i);
            if (datapoint->get_year()==year) {
                entries_in_year +=1;
                temperature_sum += datapoint->get_temperature();
            }
        }
        if (entries_in_year==0) {
            return 0.0;
        }
        return temperature_sum/entries_in_year;
    }
    void plot_average_temperature(TTree* tree, int first_year, int last_year, std::string output_file_path) {
        int n_years=last_year-first_year+1;
        TGraph* graph = new TGraph(n_years);

        for (int i = 0; i < n_years; ++i) {
            int year =first_year + i;
            double average_temp = average_temperature(tree,year);

            graph->SetPoint(i,year,average_temp);
        }
        TCanvas* canvas = new TCanvas("Average_temperature_plot","Average Temperature by year",1000, 600);
        graph->SetTitle("Average annual temperature;Year;Average temperature (#circC)");
        graph->SetMarkerStyle(20);
        graph->SetMarkerSize(0.5);
        graph->Draw("AP");
        canvas->Update();
        canvas->SaveAs(output_file_path.c_str());
    }
}