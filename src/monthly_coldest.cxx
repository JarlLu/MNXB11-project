#include "monthly_coldest.h"
#include "DataPoint.h"
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


namespace project {
void monthly_coldest(std::string root_file_path) {
// -----------------------READ INPUT FILE AND RETREIVE DATA TREE --------------
    // Open root file and define input as pointer that stores memory adress, open the file in read only 


    TFile *root_file=TFile::Open(root_file_path.c_str(), "READ");

        //if cannot find root file or file is unusable show error message
        if (!root_file || root_file->IsZombie()) 
        {
            std::cerr << "Cannot open input ROOT file!" << std::endl;
            return;
        }
    
    //create a pointer and find the tree and link pointer to tree, data is the tree in the root file
    
    TTree *tree = nullptr;
    root_file->GetObject("data", tree);

        //if it cannot find tree, show the error message and close the input file
        if (!tree) 
        {
            std::cerr << "Cannot find tree named data!\n";
            root_file->Close();
            return;
        }

    project::DataPoint* datapoint = nullptr;
    project::DataPoint outPoint;

    if (tree->SetBranchAddress("datapoint", &datapoint) < 0) {
        std::cerr << "cannot read branch datapoint :( ! \n"; 
        root_file -> Close();
        return;
    }

    std::cout << "Input file: " << root_file_path << '\n';
    std::cout << "Tree entries: " << tree->GetEntries() << '\n';
    tree->Print();

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
            if (!datapoint)
                continue;

        const std::string currentDate = datapoint->get_date();
        const double temp = datapoint->get_temperature();
        const bool good = datapoint->good_quality();


        // define three integers 
        int year;
        int month;
        int day;

            // get data from the date string defined earlier, separate them into 3, if not into 3, skip
            if (sscanf(currentDate.c_str(), "%d-%d-%d", &year, &month, &day) != 3)
                continue;

            //check if months are between 1 and 12, if not skip 
            if (month < 1 || month > 12 || day < 1 || day > 31 || !good)
               continue;

        // define an integer which stores the indexes of the months (jan = 0) simular to arrays 
        int index = month - 1;

          if (temp < minTemp[index]) {
            minTemp[index] = temp;
            coldestDay[index] = day;
            coldestDate[index] = currentDate;
        }

    
    }

    // ----------------CREATE MONTHLY ROOT FILES --------------------

    // stores 12 pointers in files and trees
    TFile *monthFiles[12] = {};
    TTree *monthTrees[12] = {};

   

    //CREATE THE 12 MONTHLY ROOT FILES 
    //we will create new files for each month with the output from the for loop and creates pointer and uses dynamic filenames 
    //RECREATE since want create a new file or override a file with the same name 
for (int m = 0; m < 12; ++m) 
    {
            monthFiles[m] = TFile::Open(Form("datasets/monthrootfiles/month_%02d.root", m + 1), "RECREATE");
            //we will create new trees for each month with the output from the for loop
            //the tree stores both the month data and the temperature data, we need both tree and file since the tree is stored in the file  
            

        if (!monthFiles[m] || monthFiles[m]->IsZombie()) 
        {
            std::cerr << "Cannot create monthly ROOT file "<< m + 1 << "!\n";
    
            for (int j = 0; j <= m; ++j) {
                if (monthFiles[j]) 
                {
                    monthFiles[j]->Close();
                    delete monthFiles[j];
                }

            } 
            root_file->Close();
            delete root_file;
            return;

        } 

        monthFiles[m]->cd();

        monthTrees[m] = new TTree(
        Form("month_%02d", m + 1),
        Form("Temperature data for month %02d", m + 1));

        // Store the same DataPoint object format as the input.
        monthTrees[m]->Branch("datapoint", &outPoint);
    }
    

    //SORT THE DATA INTO THE 12 MONTHLY FILES 
    //for loop very simular to the previous for loop but now considering output 
for (Long64_t i = 0; i < tree->GetEntries(); ++i)
    {
        tree->GetEntry(i);

        if (!datapoint)
            continue;

        const std::string& currentDate = datapoint->get_date();
     
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

        outPoint.set_values(
        datapoint->get_date(),
        datapoint->get_hour(),
        datapoint->get_temperature(),
        datapoint->good_quality()
        );

        int index = month - 1;
        monthTrees[index]->Fill();
    } 


    //WRITE THE ACTUAL MONTHLY FILES 
for (int m = 0; m < 12; ++m) 
        {
            std::cout << "DEBUG A: entering month " << m + 1 << std::endl;

            monthFiles[m]->cd();
            std::cout << "DEBUG B: changed directory" << std::endl;

            monthTrees[m]->Write();
             std::cout << "DEBUG C: tree written" << std::endl;

            std::cout << "Month " << m + 1 << ":" << std::endl;
            /*
        // take current month as month where object is written
        monthFiles[m]->cd();
        // write data to that month 
        monthTrees[m]->Write();
            */

        //prints the current month 
        std::cout << "Month " << m + 1 << ":" << std::endl;

        //days start from one thus if found and therefore larger than -1 which was assigned if not found 
        if (coldestDay[m] >= 0) 
                //the result will be printed
               { std::cout << coldestDate[m] << ", temperature = " << minTemp[m] << " C" << std::endl; } 
            else 
                    // if not found, give an error message 
                    {std::cout << "No good-quality measurements"<< std::endl;} 

        // close the file and delete the memory since the file has already been saved -- prevents memory leaks
        std::cout << "DEBUG: about to close month " << m + 1 << std::endl;
        monthFiles[m]->Close();
        std::cout << "DEBUG: closed month " << m + 1 << std::endl;
        //delete monthFiles[m];

        monthFiles[m] = nullptr;
        monthTrees[m] = nullptr;
        std::cout << "DEBUG: cleanup finished for month " << m + 1 << std::endl;
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
    root_file->Close();

    std::cout << "Finished! Coldest-day scatter plot saved in monthrootfiles" << std::endl;
}
}
