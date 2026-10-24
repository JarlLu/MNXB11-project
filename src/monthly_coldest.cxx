#include "monthly_coldest.h"
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TString.h>
#include <cstdio>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <limits>

void monthly_coldest() {
// -----------------------READ INPUT FILE AND RETREIVE DATA TREE --------------
    // Open root file and define input as pointer that stores memory adress, open the file in read only 
    TFile *input = TFile::Open("datasets/rootfiles/testroot2.root", "READ");

        //if cannot find root file or file is unusable show error message
        if (!input || input->IsZombie()) 
        {
            std::cerr << "Cannot open input ROOT file!" << endl;
            return;
        }
    
    //create a pointer and find the tree and link pointer to tree, data is the tree in the root file
    TTree *tree = nullptr;
    input->GetObject("data", tree);

        //if it cannot find tree, show the error message and close the input file
        if (!tree) 
        {
            std::cerr << "Cannot find tree named data!\n";
            input->Close();
            return;
        }

        //create a pointer to vector of strings which will be defined later on 
    std::vector<std::string> *date = nullptr;
        //create a yet empty pointer to vector of a double 
    std::vector<double> *temperature = nullptr;
        //create a yet empty pointer to vector of bool which is either true or false 
    std::vector<bool> *quality = nullptr;
        //define the hour to be an integer 
    Int_t hour;
    
    // Connect each variable to its corresponding branch in the tree while & gives the memory adress of each pointer
    tree->SetBranchAddress("date", &date);
    tree->SetBranchAddress("hour", &hour);
    tree->SetBranchAddress("temperature", &temperature);
    tree->SetBranchAddress("quality", &quality);

    // ------------- FIND THE LOWEST TEMPERATURE FOR ALL MONTHS, WITH GOOD QUALITY -------------------

    // Create an array which stores 12 values of temperature in celcuis, one value for every month (0 -11), we use dataType arrayName[arraySize];
    double minTemp[12];
    //we then take that temperature and find the corresponding day, int coldestDay[0]; 3 will be stored (jan 3rd)
    int coldestDay[12];
    // links and stores the coldest date as a sting date 
    std::string coldestDate[12]; 

    //PREP THE ARRAYS 
    // Initialize the arrays before processing, we want the code to run 12 times, once for every month 
for (int m = 0; m < 12; ++m) 
    {
        // the minimum temperature of each month, starting from 45 degrees celcius
        minTemp[m] = 45.0; 
        //searches the coldest day and gives it the value of a placeholder once it finds it the value will be updated with the value
        coldestDay[m] = -1;
    }
    

    // FIND THE ACUAL DAY WHERE TEMPERATURE IS LOWEST 
    // loop through all datapoints in the input file, Long64(64 bits)
for (Long64_t i = 0; i < tree->GetEntries(); ++i) 
    {
        //read the data from the tree
        tree->GetEntry(i);

            //check if date pointers is non zero or empty and checks if temp and quality are valid, skip if not 
            if (!date || date->empty() || !temperature || !quality)
                continue;
        
        //date pointer is used to get first valid output and name it currentdata which is a non changed string 
        const std::string& currentDate = date->at(0);

        // define three integers 
        int year;
        int month;
        int day;

            // get data from the date string defined earlier, separate them into 3, if not into 3, skip
            if (sscanf(currentDate.c_str(), "%d-%d-%d", &year, &month, &day) != 3)
                continue;

            //check if months are between 1 and 12, if not skip 
            if (month < 1 || month > 12)
                continue;

        // define an integer which stores the indexes of the months (jan = 0) simular to arrays 
        int index = month - 1;

            //loop though all temp and quality vectors 
            for (size_t j = 0; j < n; ++j)
            {
                //check if quality is good and temperature is lower than the current minimum 
                if ((*quality)[j] && (*temperature)[j] < minTemp[index])
                {
                //them update the minimum temp position j of that month and note the day and date for this measuement 
                minTemp[index] = (*temperature)[j];
                coldestDay[index] = day;
                coldestDate[index] = currentDate;
                }
            }
    }

    // ----------------CREATE MONTHLY ROOT FILES --------------------

    // stores 12 pointers in files and trees
    TFile *monthFiles[12];
    TTree *monthTrees[12];

   //we define the datapype of the outputs to store the outputs -- same as input but now out
    std::vector<std::string> outDate;
    std::vector<double> outTemperature;
    std::vector<bool> outQuality;
    Int_t outHour;

    //CREATE THE 12 MONTHLY ROOT FILES 
    //we will create new files for each month with the output from the for loop and creates pointer and uses dynamic filenames 
    //RECREATE since want create a new file or override a file with the same name 
for (int m = 0; m < 12; ++m) 
    {
            monthFiles[m] = new TFile(
            Form("datasets/monthrootfiles/month_%02d.root", m + 1), "RECREATE");
            //we will create new trees for each month with the output from the for loop
            //the tree stores both the month data and the temperature data, we need both tree and file since the tree is stored in the file  
            monthTrees[m] = new TTree(
            Form("month_%02d", m + 1),
            Form("Temperature data for month %02d", m + 1));

            //we create branches again but now with the output data while creating pointers at the same time using the files we created above 
            monthTrees[m]->Branch("date", &outDate);
            monthTrees[m]->Branch("hour", &outHour);
            monthTrees[m]->Branch("temperature", &outTemperature);
            monthTrees[m]->Branch("quality", &outQuality);
    }

    //SORT THE DATA INTO THE 12 MONTHLY FILES 
    //for loop very simular to the previous for loop but now considering output 
for (Long64_t i = 0; i < tree->GetEntries(); ++i)
    {
        tree->GetEntry(i);

        if (!date || date->empty() || !temperature || !quality)
            continue;

        const std::string& currentDate = date->at(0);

        // dates are in YYYY-MM-DD which are 10 charackers, skips if not 10 charackters 
        if (currentDate.size() < 10)
            continue;

        //finds the month from the date output to verify and find index 
        int month = std::stoi(currentDate.substr(5, 2));

        if (month < 1 || month > 12)
            continue;

        int index = month - 1;

        // copies contents into output variables 
        outDate = currentDate;
        outHour = hour;
        outTemperature = *temperature;
        outQuality = *quality;

    monthTrees[index]->Fill();
    }

    

    //WRITE THE ACTUAL MONTHLY FILES 
for (int m = 0; m < 12; ++m) 
        {
        // take current month as month where object is written
        monthFiles[m]->cd();
        // write data to that month 
        monthTrees[m]->Write();
        //close the file 
        monthFiles[m]->Close();

        //prints the current month 
        std::cout << "Month " << m + 1 << ":" << std::endl;

        //days start from one thus if found and therefore larger than -1 which was assigned if not found 
        if (coldestDay[m] >= 0) 
                //the result will be printed
               { std::cout << coldestDate[m] << ", temperature = " << minTemp[m] << " C" << std::endl; } 
            else 
                    // if not found, give an error message 
                    {std::cout << "No good-quality measurements"<< std::endl;} 


        // delete the memory since the file has already been saved -- prevents memory leaks
        delete monthFiles[m];
        }

    // ---------------------------------CREATE A SCATTER PLOT : MONTHS VS COLDEST DAYS -----------------------------------------------------
    
   //create an empty graph 
    TGraph *graph = new TGraph();

    //ADD THE COLDEST DAYS TO THE SCATTER PLOTS  
for (int m = 0; m < 12; ++m) 
    {
        // days start from 1 
        if (coldestDay[m] >= 0) 
        {
            //points will have (index, month, day)
            graph->SetPoint(m, m + 1, coldestDay[m]);
        }
    }

    // create window where graph will be displayed 
    TCanvas *canvas = new TCanvas("canvas", "Coldest date of each month", 900, 600);

    //GRAPH DESIGN 
    graph->SetTitle("Coldest Date in Each Month;Month;Day of month");
    //filled circle 
    graph->SetMarkerStyle(20);
    //little bit bigger than regular 
    graph->SetMarkerSize(1.4);
    //limits to months 
    graph->GetXaxis()->SetLimits(0.5, 12.5);
    //correspond to months 
    graph->GetXaxis()->SetNdivisions(12);
    //days 
    graph->GetYaxis()->SetRangeUser(0, 32);
    //draws axes and points and draws smooth curve through them 
    graph->Draw("ACP");

    //ENDING CODE 
    //saves canvas 
    canvas->SaveAs("datasets/monthrootfiles/monthly_coldest.png");

    //release the memory 
    delete canvas;
    delete graph;

    //closes the imput file after reading 
    input->Close();

    std::cout << "Finished! Coldest-day scatter plot saved." << std::endl;
}
