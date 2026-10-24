#include "climate_change.h"
#include "DataPoint.h"


#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>

#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TLegend.h>

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
            throw std::invalid_argument("This year has no data");
        }
        return temperature_sum/entries_in_year;
    }

    double moving_average(const std::vector<double>& average_temperatures,int year, int window_size, int first_year, int last_year) {
        double sum=0;
        if (window_size%2==1 || window_size<=0) {
            throw std::invalid_argument("Invalid moving average window");
        }
        if (year-first_year<window_size/2 || last_year-year<window_size/2-1) {
            throw std::invalid_argument("Invalid year for moving average");
        }
        for (int i =year-first_year-window_size/2 ;i<year-first_year+1+window_size/2; ++i) {
            sum += average_temperatures[i];
        }
        return sum/window_size;

    }
    void plot_average_temperature(TTree* tree, int first_year, int last_year, std::string output_file_path) {
        int n_years=last_year-first_year+1;
        TGraph* graph = new TGraph(n_years);
        TGraph* moving_average_10_graph= new TGraph();
        TGraph* moving_average_20_graph= new TGraph();

        std::vector<double> yearly_averages;
        yearly_averages.reserve(n_years);
        for (int year = first_year; year <= last_year; ++year) {
            yearly_averages.push_back(average_temperature(tree, year));}
        int a=0;
        int b=0;
        for (int i = 0; i < n_years; ++i) {
            int year =first_year + i;
            
            graph->SetPoint(i,year,yearly_averages[i]);
            if (i>4 && i < n_years-4) {
                moving_average_10_graph->SetPoint(a,year-0.5,moving_average(yearly_averages, year, 10,first_year,last_year));
                a+=1;
            }
            if (i>9 && i< n_years-9) {
                moving_average_20_graph->SetPoint(b,year-0.5,moving_average(yearly_averages, year, 20, first_year, last_year));
                b+=1;
            }
        }
        TCanvas* canvas = new TCanvas("Average_temperature_plot","Average Temperature by year",1000, 600);
        graph->SetTitle("Average annual temperature;Year;Average temperature (#circC)");
        graph->SetMarkerStyle(20);
        graph->SetMarkerSize(0.5);
        graph->Draw("AP");

        //moving_average_10_graph->SetMarkerStyle(20);
        //moving_average_10_graph->SetMarkerSize(0.5);
        moving_average_10_graph->SetLineColor(kGreen);
        moving_average_10_graph->Draw("L SAME");

        //moving_average_20_graph->SetMarkerStyle(20);
        //moving_average_20_graph->SetMarkerSize(0.5);
        moving_average_20_graph->SetLineColor(kRed);
        moving_average_20_graph->Draw("L SAME");

        TLegend* legend = new TLegend(0.65, 0.75, 0.88, 0.88);

        legend->AddEntry(graph, "Annual average", "p");
        legend->AddEntry(moving_average_10_graph, "10-year moving average", "lp");
        legend->AddEntry(moving_average_20_graph, "20-year moving average", "lp");

        legend->Draw();

        canvas->Update();
        canvas->SaveAs(output_file_path.c_str());
    }
}