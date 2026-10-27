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
// -----------------------READ INPUT FILE AND RETREIVE DATA TREE --------------
    // Open root file and define input as pointer that stores memory adress and converts string to C-style string ~ array, open the file in read only  
    TFile *root_file=TFile::Open(root_file_path.c_str(), "READ");

        //if cannot find root file or file is unusable show error message
        if (!root_file || root_file->IsZombie()) 
        {
            std::cerr << "Cannot open input ROOT file!" << std::endl;
            return;
        }
    
    //create a pointer and find the tree and link pointer to tree, data is the tree in the root file which the pointer tree links to 
    // Ttree stores data in branches, we make a pointer to it to be able to fetch the memory adress the pointer is called tree. The Ttree in this case is called data 
    TTree *tree = nullptr;
    root_file->GetObject("data", tree);

        //if pointer is false and cannot find Ttree named data, show the error message and close the input file
        if (!tree) {
            std::cerr << "Cannot find tree named data!\n";
            root_file->Close();
            return;
        }
    
    // from project namespace, create pointer to DataPoint object as nullpointer -- will be used later on 
    project::DataPoint* datapoint = nullptr;
    // Create a datapoint object that will store the data
    project::DataPoint outPoint;
 
    //connect the datapoint branch in the tree to the datapoint pointer  and store the memory adress if valid, give error if cannot be done 
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

    //create struct ~ group of related variables; stores information about the coldest day
    //initial values given below; these are the default variables 
    struct ColdestInfo {
        double temperature = 45.0;
        int day = -1;
        std::string date;
        };

    // use map ~ associative container that contains value pairs <year, month> is key which is associated with coldestinfo(variable) struct in coldest-map
    std::map<std::pair<int, int>, ColdestInfo> coldest;

    // FINDING THE COLDEST DAYS: Find the coldest good-quality measurement for every year and month.
    // We loop through all the data entries 
    for (Long64_t i = 0; i < tree->GetEntries(); ++i) {
        tree->GetEntry(i);

        //if invalid, skip 
        if (!datapoint)
            continue;

        // currentDate stores the unchanged date returned by get_date (from DataPoint), same for temp and quality 
        const std::string currentDate = datapoint->get_date();
        const double temp = datapoint->get_temperature();
        const bool good = datapoint->good_quality();

        // define integers 
        int year;
        int month;
        int day;

        //scans string using Cstring of currentdate, passes memory adress and checks if 3 values are sucesfully read 
        if (sscanf(currentDate.c_str(), "%d-%d-%d", &year, &month, &day) != 3)
            continue;

        //checks data input whether values are allowed and quality is good
        if (month < 1 || month > 12 || day < 1 || day > 31 || !good)
            continue;

        // use the map featuring year and month as key and have it intepret as Coldestinfo default numbers if it doesnt already exist 
        auto& result = coldest[{year, month}];


        // so now we store the temperature value as the lowest temperature if it is the lower than the lowest 
        // updates the coldest temp and the corresponding month and date 
        if (temp < result.temperature) {
            result.temperature = temp;
            result.day = day;
            result.date = currentDate;
        }
    }

    // ----------------CREATE MONTHLY ROOT FILES --------------------

    // stores 12 pointers in files and trees using arrays which will be filed later 
    //  The tree array is in the tfile 
    TFile *monthFiles[12] = {};
    TTree *monthTrees[12] = {};


    //CREATE THE 12 MONTHLY ROOT FILES 
    //we will create new files for each month with the output from the for loop and creates pointer and uses dynamic filenames  
for (int m = 0; m < 12; ++m) 
    {
            //we will create new files for each month with the output from the for loop and creates pointer and uses dynamic filenames 
            //RECREATE since we want to override a file with the same name
            monthFiles[m] = TFile::Open(Form("datasets/monthrootfiles/month_%02d.root", m + 1), "RECREATE");
        
        // check if file is VALID  
        if (!monthFiles[m] || monthFiles[m]->IsZombie()) 
        {
            std::cerr << "Cannot create monthly ROOT file "<< m + 1 << "!\n";
            
            //cleanup all monthly files opened and current path for all previous loops if something went wrong 
            for (int j = 0; j <= m; ++j) {
                if (monthFiles[j]) {
                    monthFiles[j]->Close();
                    delete monthFiles[j];
                }
            } 
            //cleanup origional input 
            root_file->Close();
            delete root_file;
            return;
        } 

        // the current month is the active directory 
        monthFiles[m]->cd();

         //we will create new trees for each month with the temperature date output 
        //the tree stores both the month data and the temperature data, we need both tree and file since the tree is stored in the file  
        monthTrees[m] = new TTree(Form("month_%02d", m + 1), Form("Temperature data for month %02d", m + 1));

        // create branch that stores output data as the same format as input data 
        monthTrees[m]->Branch("datapoint", &outPoint);
    }

    // -----------------------------WRITE THE FILES AND FILL THE TREES WITH COLDEST DATA --------------------------
    for (int m = 0; m < 12; ++m) 
        {
            //set current month as active directory 
            monthFiles[m]->cd();
            //show which month it is working on 
            std::cout << "Month " << m + 1 << ":" << std::endl;

            for (const auto& entry : coldest)
            {
                // Extract the month from key <year, MONTH> 
                const int month = entry.first.second;

                // Get the coldest measurement for this year and month;  <year, month> COLDESTINFO 
                const ColdestInfo& result = entry.second;

                    // check if entry belongs to current month 
                    if (month == m + 1) {
                        std::cout << " - Month " << month << ": ";

                        // check validity 
                        if (result.day >= 0){
                            // Copy the coldest measurement into outPoint; hout is 0 and quality is true 
                            outPoint.set_values(result.date, 0, result.temperature, true );
                            //put in TREE
                            monthTrees[m]->Fill();

                            std::cout << result.date << ", temperature = " << result.temperature << " C\n"; 
                        }
                        
                        else 
                            {std::cout << "No good-quality measurements\n";}
                        }
                }

            // write current month tree and its data into the root FILE
            monthTrees[m]->Write();
            // close the file and delete the memory since the file has already been saved -- prevents memory leaks
            monthFiles[m]->Close();
            //mark file and trees empty -- cleanup -- further on we use the coldest map 
            monthFiles[m] = nullptr;
            monthTrees[m] = nullptr;
        } 



    // ---------------------------------CREATE A SCATTER PLOT : MONTHS VS COLDEST DAYS -----------------------------------------------------
//start canvas of 1000 by 600 pixels 
TCanvas* canvas = new TCanvas("canvas", "Coldest Day by Month", 1000, 600);

//take some colours
int colors[] = {
    kBlue, kRed, kGreen + 2, kMagenta,
    kOrange + 7, kCyan + 2, kViolet, kBlack, 
    kAzure + 2, kPink + 7, kTeal + 3
};

// Different colors to the graph 
int colorIndex = 0;

//create map that stores pointers to years called yearGraphs 
std::map<int, TGraph*> yearGraphs;

//CREATE ONE GRAPH PER YEAR 
// loop through all entries of the map 
for (const auto& entry : coldest) {
    // get the exact year from the map key - entry 
    const int year = entry.first.first;

    // do we already have this graph 
    if (yearGraphs.find(year) == yearGraphs.end()) {
        //create new root graph for each year 
        TGraph* graph = new TGraph();
        graph->SetName(("year_" + std::to_string(year)).c_str());
        graph->SetTitle("Coldest Day of Each Month;Month;Day of Month");
        graph->SetMarkerStyle(20);
        graph->SetMarkerSize(1.4);
        graph->SetMarkerColor(colors[colorIndex % 11]);
        graph->SetLineColor(colors[colorIndex % 11]);

        //store the graph for every year  
        yearGraphs[year] = graph;
        //each year different color 
        ++colorIndex;
    }

// --------------------------------------------- FILL EACH YEARS GRAPH -----------------------------------
//create anotther map now having the number of datapoints -- each year <year, pointrcount> 
std::map<int, int> pointCounts;

//loop through coldest map 
for (const auto& entry : coldest) {
    // extract the year and month from the key 
    const int year = entry.first.first;
    const int month = entry.first.second;
    //store the reference of the Coldestinfot as result coldestinfo stored year, month 
    const ColdestInfo& result = entry.second;

    if (result.day >= 0) {
        yearGraphs[year]->SetPoint(pointCounts[year]++, month, result.day);
    }
}

// -------------------- SETTING UP CALCULATIONS ----------------------
 //CALCULATING THE AVERAGE 
 // we create a new graph 
TGraph* averageGraph = new TGraph();

//this does not really matter 
averageGraph->SetName("aver.");
averageGraph->SetTitle("Average coldest day");

// we define three arrays 
double daySums[12] = {};
int monthCounts[12] = {};
double monthlyAverages[12] = {};


// -------------------- CALCULATING THE MONTHLY AVERAGES --------------------


// Loop through all entries in the coldest map
for (const auto& entry : coldest) {
    // Extract the month and the corresponding coldest information
    const int month = entry.first.second;
    const ColdestInfo& result = entry.second;

    // Only use valid results and valid calendar months
    if (result.day >= 0 && month >= 1 && month <= 12) {
        // Add the day number to the sum for this calendar month using index 
        daySums[month - 1] += result.day;

        // Count the valid results for this calendar month
        monthCounts[month - 1]++;
    }
}

// Calculate the average day number for each calendar month
for (int m = 0; m < 12; ++m) {
    // valid 
    if (monthCounts[m] > 0) {
        // divide sum of day numbers by the number of valid months 
        monthlyAverages[m] =
            daySums[m] / monthCounts[m];
    }
}

// -------------------- CALCULATING THE MEAN OF THE MONTHLY AVERAGES --------------------

// Define variables for calculating the mean
double sumAverages = 0.0;
int numberOfMonths = 0;
double meanOfAverages = 0.0;

// Loop through all calendar months
for (int m = 0; m < 12; ++m) {
    // Only include months that have valid averages
    if (monthCounts[m] > 0) {
        // Add the monthly average to the sum of averages
        sumAverages += monthlyAverages[m];

        // Count this month as a valid month
        numberOfMonths++;
    }

// Calculate the mean if at least one month has valid data - mean is calculated as average of averages 
if (numberOfMonths > 0) {
    // Divide the sum of monthly averages by the number of valid months
    meanOfAverages = sumAverages / numberOfMonths;
}

// -------------------- CALCULATING THE STANDARD DEVIATION --------------------

// Define variables for calculating the standard deviation
double squaredDifferences = 0.0;
double standardDeviation = 0.0;

// calculate standard deviation 
if (numberOfMonths > 0) {
    // Loop through all calendar months
    for (int m = 0; m < 12; ++m) {
        // Only valid
        if (monthCounts[m] > 0) {
            // Calculate (xi - mean)
            double difference = monthlyAverages[m] - meanOfAverages;

            // calculate (xi - mean)² 
            squaredDifferences += difference * difference;
        }
    }

    // sqrt ((xi - mean)² / number of months) 
    standardDeviation = std::sqrt(squaredDifferences / numberOfMonths);

}

// -------------------- FILLING THE AVERAGE GRAPH --------------------

// Define the index of the next graph point
int averagePoint = 0;

// Loop through all calendar months
for (int m = 0; m < 12; ++m) {
    // Only add points for months that have valid averages
    if (monthCounts[m] > 0) {
        // Add the month number as x and the average day number as y
        averageGraph->SetPoint( averagePoint++, m + 1, monthlyAverages[m] );
    }
}


// -------------------- SETTIING UP THE PLOTS ------------------------

// Style the average graph in black with square markers.
averageGraph->SetMarkerStyle(21);  // Filled square
averageGraph->SetMarkerSize(1.6);
averageGraph->SetMarkerColor(kBlack);
averageGraph->SetLineColor(kBlack);
averageGraph->SetLineWidth(3);

//set canvas as active 
canvas->cd();

// this is for the mega graph featuring four plots with four titles. we want each to have their own title 
std::string city = "UNKNOWN";

// the name is in the root file path so we check for that, if not found, move on to the next line 
if (root_file_path.find("LUND") != std::string::npos)
    city = "LUND";
else if (root_file_path.find("UPPSALA") != std::string::npos)
    city = "UPPSALA";
else if (root_file_path.find("KARLSTAD") != std::string::npos)
    city = "KARLSTAD";
else if (root_file_path.find("HALMSTAD") != std::string::npos)
    city = "HALMSTAD";

//design the scatter drame and put in title, s% will then be the city name. the city name is a C string that can be inplemented by Form()
TH1F* frame = canvas->DrawFrame( 0.5, 0, 12.5, 32, Form("Coldest Day of Each Month in %s (2020-2025);Month;Day of Month", city.c_str()));

// design the frame; the x axis gets 
frame->GetXaxis()->SetNdivisions(12);

// Create the legend
TLegend* legend = new TLegend(0.90, 0.10, 0.99, 0.30);
legend->SetTextSize(0.025);

// ----------------------  MAKING THE PLOT FOR EACH YEAR ON ONE GRAPH ----------------------------
// Draw each year's points and connecting lines for every 
for (const auto& entry : yearGraphs) {
    TGraph* graph = entry.second;
    graph->Draw("LP SAME");
    legend->AddEntry(graph, std::to_string(entry.first).c_str(), "lp" );
}

// ----------------------- ADD THE AVERAGE PLOT -------------------------------------
averageGraph->Draw("LP SAME");
legend->AddEntry(averageGraph, "Average", "lp");

// Display the mean and standard deviation separately from the legend since it doesnot fit
TLatex text;
//use normalized device coordinates to position the text
text.SetNDC();         
text.SetTextSize(0.030);
text.SetTextColor(kBlack);

// put mean and standard deviation on graph. 
text.DrawLatex(0.84, 0.96, Form("Mean = %.2f days", meanOfAverages));
text.DrawLatex(0.84, 0.93, Form("Std. dev. = %.2f days", standardDeviation));

//then draw the lagend 
legend->Draw();

// update the picture 
canvas->Modified();
canvas->Update();

//SAVING THE IMAGE 
//define image path so image wont be overriddden and save 
std::string image_path = "datasets/monthrootfiles/monthly_coldest_" + city + ".png"; canvas->SaveAs(image_path.c_str());
// where to fine the image 
std::cout << "Saved image: " << image_path << std::endl;

//---------------------------------------- CLEANUP AND CLOSE --------------------------------- 
delete legend;
delete canvas;
for (auto& entry : yearGraphs) {delete entry.second; }
delete averageGraph;
    
//close the imput file after reading 
root_file->Close();

std::cout << "Finished! Coldest-day scatter plot saved in monthrootfiles" << std::endl;
}
}
} // namespace project
