#include "climate_change.h"
#include "DataPoint.h"

#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

#include <TAxis.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TTree.h>

namespace project {
// Opens root file and generates plot in images folder
void climate_change(std::string root_file_path) {

    // open file
    TFile* root_file = TFile::Open(root_file_path.c_str(), "READ");

    // Making sure file opened properly
    if (root_file == nullptr || root_file->IsZombie()) {
        std::cerr << "Error opening root file \n";
        return;
    }
    // get tree from root file
    TTree* tree = dynamic_cast<TTree*>(root_file->Get("data"));
    // make sure tree was found
    if (tree == nullptr) {
        std::cerr << "Error: could not find TTree data\n";
        root_file->Close();
        return;
    }

    // Call plotting function for whole dataset
    plot_average_temperature(tree, 1722, 2022,
                             "./images/average_temperature_1722-2022.png");
    // close root file
    root_file->Close();
}
// Calculates the mean temperature of all entries belonging to specified year
double average_temperature(TTree* tree, int year) {
    // initialize datapoint object
    DataPoint* datapoint = nullptr;
    // set branch address to the newly initialized datapoint object
    tree->SetBranchAddress("datapoint", &datapoint);

    // Get amount of entries in tree
    Long64_t entries = tree->GetEntries();
    // Initialize temporary variables for the loop
    Long64_t entries_in_year = 0;
    double_t temperature_sum = 0.0;
    // Loop through whole tree, summing up temperature values of entries
    // belonging to specified year
    for (Long64_t i = 0; i < entries; ++i) {
        tree->GetEntry(i);
        if (datapoint->get_year() == year) {
            entries_in_year += 1;
            temperature_sum += datapoint->get_temperature();
        }
    }
    // Check to make sure the specified year has any datapoints in it
    if (entries_in_year == 0) {
        throw std::invalid_argument("This year has no data");
    }
    // Return the mean
    return temperature_sum / entries_in_year;
}
// Calculates the moving average between year-window_size/2 and
// year+window_size/2 using average annual temperature values in a vector
double moving_average(const std::vector<double>& average_temperatures, int year,
                      int window_size, int first_year, int last_year) {
    // Initialize temporary variable
    double sum = 0;
    // Make sure window size is valid i.e positive and even
    if (window_size % 2 == 1 || window_size <= 0) {
        throw std::invalid_argument("Invalid moving average window");
    }
    // Make sure the window doesnt go outside bounds of dataset
    if (year - first_year < window_size / 2 ||
        last_year - year < window_size / 2 - 1) {
        throw std::invalid_argument("Invalid year for moving average");
    }
    // Loop over years between year-window_size/2 and year-window_size/2+1 and
    // collect sum of average temperatures
    for (int i = year - first_year - window_size / 2;
         i < year - first_year + 1 + window_size / 2; ++i) {
        sum += average_temperatures[i];
    }
    // return moving average value
    return sum / window_size;
}
// Plots the average temperature for each year in the dataset and 10/20 year
// moving averages with deviation from mean
void plot_average_temperature(TTree* tree, int first_year, int last_year,
                              std::string output_file_path) {
    // Calculate/initialize number of years
    int n_years = last_year - first_year + 1;
    // Initialize pointers to TGraph objects for each element of plot
    TGraph* hot_graph = new TGraph();
    TGraph* cold_graph = new TGraph();
    TGraph* moving_average_10_graph = new TGraph();
    TGraph* moving_average_20_graph = new TGraph();
    TGraph* reference_line = new TGraph();

    // Initialize vector for yearly_averages so that each value only has to be
    // calculated once
    std::vector<double> yearly_averages;
    yearly_averages.reserve(n_years);
    // Calculate and insert the average temperature into yearly_averages vector
    for (int year = first_year; year <= last_year; ++year) {
        yearly_averages.push_back(average_temperature(tree, year));
    }
    // Calculate average of annual average temperatures to be used in making
    // deviation from mean
    double average =
        std::accumulate(yearly_averages.begin(), yearly_averages.end(), 0.0) /
        yearly_averages.size();
    // Initialize integer variables to keep track off entries of each TGraph
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    // Loop over all years and making points in graphs
    for (int i = 0; i < n_years; ++i) {
        // Calculate and initialize current year
        int year = first_year + i;
        // Check if current years average temperature is above mean then put
        // point in hot_graph(red)
        if (yearly_averages[i] >= average) {
            hot_graph->SetPoint(a, year, yearly_averages[i]);
            a += 1;
        }
        // If previous was not true check if current years average is below the
        // mean then put point in cold_graph(blue)
        else if (yearly_averages[i] < average) {
            cold_graph->SetPoint(b, year, yearly_averages[i]);
            b += 1;
        }
        // If valid year for ten year moving average put point in
        // moving_average_10_graph(green)
        if (i > 4 && i < n_years - 4) {
            moving_average_10_graph->SetPoint(
                c, year - 0.5,
                moving_average(yearly_averages, year, 10, first_year,
                               last_year));
            c += 1;
        }
        // If valid year for twenty year moving average put point in
        // moving_average_20_graph(orange)
        if (i > 9 && i < n_years - 9) {
            moving_average_20_graph->SetPoint(
                d, year - 0.5,
                moving_average(yearly_averages, year, 20, first_year,
                               last_year));
            d += 1;
        }
    }

    // create reference line for total mean
    reference_line->SetPoint(0, first_year, average);
    reference_line->SetPoint(1, last_year, average);

    // Create TCanvas pointer where all of the graphs will be drawn on
    TCanvas* canvas = new TCanvas("Average_temperature_plot",
                                  "Average Temperature by year", 1000, 600);
    // setting title and minimum/maximum of plot
    cold_graph->SetTitle("Average annual temperature in Uppsala;Year;Average "
                         "temperature (#circC)");
    cold_graph->SetMinimum(0.0);
    cold_graph->SetMaximum(10.0);

    // Changing settings of markers and drawing cold_graph
    cold_graph->SetMarkerStyle(20);
    cold_graph->SetMarkerSize(0.5);
    cold_graph->SetMarkerColor(kBlue);
    cold_graph->Draw("AP");

    // Changing settings of markers and drawing hot_graph on same graph
    hot_graph->SetMarkerStyle(20);
    hot_graph->SetMarkerSize(0.5);
    hot_graph->SetMarkerColor(kRed);
    hot_graph->Draw("P SAME");

    // Changing line settings and drawing reference_line graph on same graph
    reference_line->SetLineStyle(2); // 2 is dashed
    reference_line->SetLineWidth(4);
    reference_line->SetLineColor(kBlack);
    reference_line->Draw("L SAME");

    // Changing line settings and drawing 10 year moving average on same graph
    moving_average_10_graph->SetLineColor(kGreen + 2);
    moving_average_10_graph->SetLineWidth(3);
    moving_average_10_graph->Draw("L SAME");

    // Changing line settings and drawing 20 year moving average on same graph
    moving_average_20_graph->SetLineColor(kOrange + 7);
    moving_average_20_graph->SetLineWidth(3);
    moving_average_20_graph->Draw("L SAME");

    // Create and draw legend in top right of canvas
    TLegend* legend = new TLegend(0.65, 0.75, 0.88, 0.88);
    legend->AddEntry(hot_graph, "Annual average over total average", "p");
    legend->AddEntry(cold_graph, "Annual average below total average", "p");
    legend->AddEntry(reference_line, "Total Average");
    legend->AddEntry(moving_average_10_graph, "10-year moving average", "lp");
    legend->AddEntry(moving_average_20_graph, "20-year moving average", "lp");
    legend->Draw();

    // update canvas with all drawings
    canvas->Update();
    // Save file with specified path/name
    canvas->SaveAs(output_file_path.c_str());
}
} // namespace project