#include "monthly_coldest.h"
#include "DataPoint.h"

#include <TAxis.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1F.h>
#include <TLegend.h>
#include <TString.h>
#include <TTree.h>

#include <TLatex.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace project {
void monthly_coldest(std::string root_file_path) {
    // -----------------------READ INPUT FILE AND RETREIVE DATA TREE
    // -------------- Open root file and define input as pointer that stores
    // memory adress and converts string to C-style string ~ array, open the
    // file in read only
    TFile* root_file = TFile::Open(root_file_path.c_str(), "READ");

    // if cannot find root file or file is unusable show error message
    if (!root_file || root_file->IsZombie()) {
        std::cerr << "Cannot open input ROOT file!" << std::endl;
        return;
    }

    // create a pointer and find the tree and link pointer to tree, data is the
    // tree in the root file
    TTree* tree = nullptr;
    root_file->GetObject("data", tree);

    // if it cannot find tree, show the error message and close the input file
    if (!tree) {
        std::cerr << "Cannot find tree named data!\n";
        root_file->Close();
        return;
    }

    // from project namespace, create pointer to DataPoint object as nullpointer
    // -- will be used later on
    project::DataPoint* datapoint = nullptr;
    // Create a datapoint object that will store the data
    project::DataPoint outPoint;

    // connect the branch to the pointer bby giving memory adress, check for
    // error and close the file
    if (tree->SetBranchAddress("datapoint", &datapoint) < 0) {
        std::cerr << "cannot read branch datapoint :( ! \n";
        root_file->Close();
        return;
    }

    // Show path of input file and show amount of tree entries
    std::cout << "Input file: " << root_file_path << '\n';
    std::cout << "Tree entries: " << tree->GetEntries() << '\n';
    tree->Print();

    // ------------- FIND THE LOWEST TEMPERATURE FOR ALL MONTHS, WITH GOOD
    // QUALITY -------------------

    // Store the coldest measurement for each year/month combination.
    // Key: {year, month}; month ranges from 1 to 12.
    struct ColdestInfo {
        double temperature = std::numeric_limits<double>::infinity();
        int day = -1;
        std::string date;
    };

    std::map<std::pair<int, int>, ColdestInfo> coldest;

    // Find the coldest good-quality measurement for every year and month.
    for (Long64_t i = 0; i < tree->GetEntries(); ++i) {
        tree->GetEntry(i);

        if (!datapoint)
            continue;

        const std::string currentDate = datapoint->get_date();
        const double temp = datapoint->get_temperature();
        const bool good = datapoint->good_quality();

        int year, month, day;

        if (sscanf(currentDate.c_str(), "%d-%d-%d", &year, &month, &day) != 3)
            continue;

        if (month < 1 || month > 12 || day < 1 || day > 31 || !good)
            continue;

        // Create or retrieve the entry for this year and month.
        auto& result = coldest[{year, month}];

        if (temp < result.temperature) {
            result.temperature = temp;
            result.day = day;
            result.date = currentDate;
        }
    }

    // ----------------CREATE MONTHLY ROOT FILES --------------------

    // stores 12 pointers in files and trees
    TFile* monthFiles[12] = {};
    TTree* monthTrees[12] = {};

    // CREATE THE 12 MONTHLY ROOT FILES
    // we will create new files for each month with the output from the for loop
    // and creates pointer and uses dynamic filenames RECREATE since want create
    // a new file or override a file with the same name
    for (int m = 0; m < 12; ++m) {
        monthFiles[m] = TFile::Open(
            Form("datasets/monthrootfiles/month_%02d.root", m + 1), "RECREATE");
        // we will create new trees for each month with the output from the for
        // loop the tree stores both the month data and the temperature data, we
        // need both tree and file since the tree is stored in the file

        if (!monthFiles[m] || monthFiles[m]->IsZombie()) {
            std::cerr << "Cannot create monthly ROOT file " << m + 1 << "!\n";

            for (int j = 0; j <= m; ++j) {
                if (monthFiles[j]) {
                    monthFiles[j]->Close();
                    delete monthFiles[j];
                }
            }
            root_file->Close();
            delete root_file;
            return;
        }

        monthFiles[m]->cd();

        monthTrees[m] =
            new TTree(Form("month_%02d", m + 1),
                      Form("Temperature data for month %02d", m + 1));

        // Store the same DataPoint object format as the input.
        monthTrees[m]->Branch("datapoint", &outPoint);
    }

    // SORT THE DATA INTO THE 12 MONTHLY FILES
    // for loop very simular to the previous for loop but now considering output
    for (Long64_t i = 0; i < tree->GetEntries(); ++i) {
        tree->GetEntry(i);

        if (!datapoint)
            continue;

        const std::string& currentDate = datapoint->get_date();

        // define three integers
        int year;
        int month;
        int day;

        // get data from the date string defined earlier, separate them into 3,
        // if not into 3, skip
        if (sscanf(currentDate.c_str(), "%d-%d-%d", &year, &month, &day) != 3)
            continue;

        // check if months are between 1 and 12, if not skip
        if (month < 1 || month > 12)
            continue;

        outPoint.set_values(datapoint->get_date(), datapoint->get_hour(),
                            datapoint->get_temperature(),
                            datapoint->good_quality());

        int index = month - 1;
        monthTrees[index]->Fill();
    }

    // WRITE THE ACTUAL MONTHLY FILES
    for (int m = 0; m < 12; ++m) {
        std::cout << "DEBUG A: entering month " << m + 1 << std::endl;

        monthFiles[m]->cd();
        std::cout << "DEBUG B: changed directory" << std::endl;

        monthTrees[m]->Write();
        std::cout << "DEBUG C: tree written" << std::endl;

        std::cout << "Month " << m + 1 << ":" << std::endl;

        // Print the coldest result for each year in this month.
        for (const auto& entry : coldest) {
            const int year = entry.first.first;
            int month = entry.first.second;
            const ColdestInfo& result = entry.second;

            if (month == m + 1) {
                std::cout << year << " - Month " << month << ": ";

                if (result.day >= 0) {
                    std::cout << result.date
                              << ", temperature = " << result.temperature
                              << " C\n";
                } else {
                    std::cout << "No good-quality measurements\n";
                }
            }
        }

        // close the file and delete the memory since the file has already been
        // saved -- prevents memory leaks
        std::cout << "DEBUG: about to close month " << m + 1 << std::endl;
        monthFiles[m]->Close();
        std::cout << "DEBUG: closed month " << m + 1 << std::endl;
        // delete monthFiles[m];

        monthFiles[m] = nullptr;
        monthTrees[m] = nullptr;
        std::cout << "DEBUG: cleanup finished for month " << m + 1 << std::endl;
    }

    // ---------------------------------CREATE A SCATTER PLOT : MONTHS VS
    // COLDEST DAYS -----------------------------------------------------

    TCanvas* canvas = new TCanvas("canvas", "Coldest Day by Month", 1000, 600);

    std::map<int, TGraph*> yearGraphs;

    int colors[] = {kBlue,       kRed,      kGreen + 2, kMagenta,
                    kOrange + 7, kCyan + 2, kViolet,    kBlack,
                    kAzure + 2,  kPink + 7, kTeal + 3};

    int colorIndex = 0;

    // Create one graph for each year
    for (const auto& entry : coldest) {
        const int year = entry.first.first;

        if (yearGraphs.find(year) == yearGraphs.end()) {
            TGraph* graph = new TGraph();
            graph->SetName(("year_" + std::to_string(year)).c_str());
            graph->SetTitle("Coldest Day of Each Month;Month;Day of Month");
            graph->SetMarkerStyle(20);
            graph->SetMarkerSize(1.4);
            graph->SetMarkerColor(colors[colorIndex % 11]);
            graph->SetLineColor(colors[colorIndex % 11]);

            yearGraphs[year] = graph;
            ++colorIndex;
        }
    }

    // Fill each year's graph
    std::map<int, int> pointCounts;

    for (const auto& entry : coldest) {
        const int year = entry.first.first;
        const int month = entry.first.second;
        const ColdestInfo& result = entry.second;

        if (result.day >= 0) {
            yearGraphs[year]->SetPoint(pointCounts[year]++, month, result.day);
        }
    }

    // Calculate the average coldest day for each month across years.
    TGraph* averageGraph = new TGraph();

    averageGraph->SetName("aver.");
    averageGraph->SetTitle("Average coldest day");

    double daySums[12] = {};
    int monthCounts[12] = {};

    for (const auto& entry : coldest) {
        const int month = entry.first.second;
        const ColdestInfo& result = entry.second;

        if (result.day >= 0 && month >= 1 && month <= 12) {
            daySums[month - 1] += result.day;
            monthCounts[month - 1]++;
        }
    }

    // Calculate the mean and standard deviation of the 12 monthly averages.
    double sumAverages = 0.0;
    int numberOfMonths = 0;
    double meanOfAverages = 0.0;
    double standardDeviation = 0.0;

    for (int m = 0; m < 12; ++m) {
        if (monthCounts[m] > 0) {
            sumAverages += daySums[m] / monthCounts[m];
            numberOfMonths++;
        }
    }

    if (numberOfMonths > 0) {
        meanOfAverages = sumAverages / numberOfMonths;

        double squaredDifferences = 0.0;

        for (int m = 0; m < 12; ++m) {
            if (monthCounts[m] > 0) {
                double averageDay = daySums[m] / monthCounts[m];

                squaredDifferences += (averageDay - meanOfAverages) *
                                      (averageDay - meanOfAverages);
            }
        }

        standardDeviation = std::sqrt(squaredDifferences / numberOfMonths);

        std::cout << "Mean of monthly averages: " << meanOfAverages
                  << " days\n";

        std::cout << "Standard deviation of monthly averages: "
                  << standardDeviation << " days\n";
    }

    int averagePoint = 0;

    for (int m = 0; m < 12; ++m) {
        if (monthCounts[m] > 0) {
            double averageDay = daySums[m] / monthCounts[m];

            averageGraph->SetPoint(averagePoint++, m + 1, averageDay);
        }
    }

    std::cout << "\nMonthly average day numbers:\n";

    for (int m = 0; m < 12; ++m) {
        if (monthCounts[m] > 0) {
            std::cout << "Month " << m + 1 << ": "
                      << daySums[m] / monthCounts[m] << " (from "
                      << monthCounts[m] << " years)\n";
        }
    }

    std::cout << "Average graph points: " << averageGraph->GetN() << '\n';

    // Style the average graph in black with square markers.
    averageGraph->SetMarkerStyle(21); // Filled square
    averageGraph->SetMarkerSize(1.6);
    averageGraph->SetMarkerColor(kBlack);
    averageGraph->SetLineColor(kBlack);
    averageGraph->SetLineWidth(3);

    // Draw all years on the same plot
    // Draw the axes first, with fixed limits.
    canvas->cd();

    std::string city = "UNKNOWN";

    if (root_file_path.find("LUND") != std::string::npos)
        city = "LUND";
    else if (root_file_path.find("UPPSALA") != std::string::npos)
        city = "UPPSALA";
    else if (root_file_path.find("KARLSTAD") != std::string::npos)
        city = "KARLSTAD";
    else if (root_file_path.find("HALMSTAD") != std::string::npos)
        city = "HALMSTAD";

    TH1F* frame = canvas->DrawFrame(
        0.5, 0, 12.5, 32,
        Form("Coldest Day of Each Month in %s (2020-2025);Month;Day of Month",
             city.c_str()));
    frame->GetXaxis()->SetNdivisions(12);

    // Create the legend.
    TLegend* legend = new TLegend(0.90, 0.10, 0.99, 0.30);
    legend->SetTextSize(0.025);

    // Draw each year's points and connecting lines.
    for (const auto& entry : yearGraphs) {
        TGraph* graph = entry.second;
        graph->Draw("LP SAME");

        legend->AddEntry(graph, std::to_string(entry.first).c_str(), "lp");
    }

    // Draw the average last, so it appears on top.
    std::cout << "Average graph points: " << averageGraph->GetN() << std::endl;

    averageGraph->Draw("LP SAME");
    legend->AddEntry(averageGraph, "Average", "lp");

    // Display the mean and standard deviation separately from the legend.
    TLatex text;
    text.SetNDC(); // Use normalized canvas coordinates.
    text.SetTextSize(0.030);
    text.SetTextColor(kBlack);

    text.DrawLatex(0.84, 0.96, Form("Mean = %.2f days", meanOfAverages));

    text.DrawLatex(0.84, 0.93,
                   Form("Std. dev. = %.2f days", standardDeviation));

    legend->Draw();

    canvas->Modified();
    canvas->Update();
    // canvas->SaveAs("datasets/monthrootfiles/monthly_coldest.png");

    std::string image_path =
        "datasets/monthrootfiles/monthly_coldest_" + city + ".png";

    canvas->SaveAs(image_path.c_str());

    std::cout << "Saved image: " << image_path << std::endl;
    delete legend;

    // release the memory
    delete canvas;

    for (auto& entry : yearGraphs) {
        delete entry.second;
    }

    delete averageGraph;

    // closes the imput file after reading
    root_file->Close();

    std::cout << "Finished! Coldest-day scatter plot saved in monthrootfiles"
              << std::endl;
}
} // namespace project
