
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TString.h>

#include <iostream>
#include <string>
#include <vector>
#include <limits>

void monthly_coldest() {

    // Open input ROOT file
    TFile *input = TFile::Open("datasets/rootfiles/testroot2.root", "READ");

        //if cannot find root file show error message
        if (!input || input->IsZombie()) {
            std::cerr << "Cannot open input ROOT file!\n";
            return;
            }
    
    //create a pointer and find the tree and link pointer to tree
    TTree *tree = nullptr;
    input->GetObject("data", tree);

        //if it cannot find tree, show the error message 
        if (!tree) {
            std::cerr << "Cannot find tree named data!\n";
            input->Close();
            return;
            }


    //////////////////////////read the branches

    //create a pointer to a sting that is later defined since date is a string
    // we define the variable of date 
    std::string *date = nullptr;

    //define variables, hour is an integer, temperature has a significance of 2
    // Bool is used since quality is either good or bad -- true or false 
    Int_t hour;
    Double_t temperature;
    Bool_t quality;

    // now we define the branches. we give them an title and a memory adress in the branch
    //& allows for pointer setting and running at the same time 
    //we now connect the variable of date to the branch
    tree->SetBranchAddress("date", &date);
    tree->SetBranchAddress("hour", &hour);
    tree->SetBranchAddress("temperature", &temperature);
    tree->SetBranchAddress("quality", &quality);

    ///////////////////////////// working out the minimum 

    // Monthly minimum temperatures
    // we have 12 months. januar = 0 and december = 11 
    // we want to store decimal numbers 
    // minTemp is an array
    // we use dataType arrayName[arraySize];
    //the array stores 12 values of temperature in celcius. each value is the coldest date of that month.
    double minTemp[12];
    //we then take that temperature and find the corresponding day 
    // if the coldest day was for example on the 3 of january then 
    // int coldestDay[0]; 3 will be stored 
    int coldestDay[12];
    // links and stores the coldest date as a sting date 
    /*std::string coldestDate[12]; */


    // Initialize the arrays before processing any data
    // we want the code to run 12 times, once for every month 
    for (int m = 0; m < 12; ++m) {
        // the minimum temperature of each month, starting from 45 degrees celcius
        minTemp[m] = 45.0; 
        //searches the coldest day and gives it the value of a placeholder once it finds it the value will be updated with the value
        coldestDay[m] = -1;
        }

    // Loop through all entries in the tree start with 0 and add 1  
    // Long64_t is data type -- used for large entries 
    // keep looping for the amount of datapoints until the amount of datapoints run out. 
    for (Long64_t i = 0; i < tree->GetEntries(); ++i) 
    {
        //read the data from the tree
        tree->GetEntry(i);

        
        //Extract the month and day from the date
        int year, month, day;
        //takes the string and seperates it into different datatypes and attaching pointers while running
        sscanf(date->c_str(), "%d-%d-%d", &year, &month, &day);
        // for the scatter we need sperarate month and day varibles 
        int index = month - 1;  // January = 0, December = 11
    

            // we will now create a for loop to check wether the datapoint is teh coldest temp so far 
            // if the quality is okay and the temperature is lower than the lowest temperature of that month 
            if (quality && temperature < minTemp[index]) {
                // we store the new minimum temperature 
                minTemp[index] = temperature;
                //we store the day 
                coldestDay[index] = day;
                //and we store the complete date - including the year since, it is a string we use the previously defined pointer 
                /*coldestDate[index] = *date;*/
                }

    }

    // Output files and trees for each month
    //create pointers to the created monthly files and creates pointers to trees 
    TFile *monthFiles[12];
    TTree *monthTrees[12];


    /////////////////////////////////////////// PROCESS OUTPUT
   /* std::string outDate; */
   //we define the datapype of the outputs -- same as input but now out
    Int_t outHour;
    Double_t outTemperature;
    Bool_t outQuality;

    //we will create new files for each month with the output from the for loop and creates pointer to it 
    //form creates dynamic filenames 
    //RECREATE since want create a new file or override a file with the same name 
    for (int m = 0; m < 12; ++m) 
    {
            monthFiles[m] = new TFile(
            Form("datasets/rootfiles/month_%02d.root", m + 1),
            "RECREATE");
            //we will create new trees for each month with the output from the for loop
            //the tree stores both the month data and the temperature data 
            //we need both tree and file since the tree is stored in the file  
            monthTrees[m] = new TTree(
            Form("month_%02d", m + 1),
            Form("Temperature data for month %02d", m + 1));

            //we create branches again but now with the output data while creating pointers at the same time 
            monthTrees[m]->Branch("date", &outDate);
            monthTrees[m]->Branch("hour", &outHour);
            monthTrees[m]->Branch("temperature", &outTemperature);
            monthTrees[m]->Branch("quality", &outQuality);
    }

    //////////////////////////////////////////////////// Process every entry -- cleanup 
    ///// go over all the data 
    for (Long64_t i = 0; i < tree->GetEntries(); ++i) {
        tree->GetEntry(i);

        //Checks for date size; needs to be 10 charackters -- skips if not 
        if (!date || date->size() < 10)
            continue;

        // Dates are formatted YYYY-MM-DD
        //takes string 
        //takes the 2 charackters at position 5 (converts 01 to 1) for month 
        //takes the 2 charckters at position 8 for day 
        int month = std::stoi(date->substr(5, 2));
        int day = std::stoi(date->substr(8, 2));

        // months are 0 to 11, if more or less, entry is skipped 
        if (month < 1 || month > 12)
            continue;

        // Save this measurement to its month's ROOT file
        int index = month - 1;

        // copies ... into ... 
        outDate = *date;
        outHour = hour;
        outTemperature = temperature;
        outQuality = quality;

    
    /*monthTrees[index]->Fill(); */

    /*
        // Find the coldest good-quality measurement
        if (quality && temperature < minTemp[index]) {
            minTemp[index] = temperature;
            coldestDay[index] = day;
            coldestDate[index] = *date;
        }
    */
    }

    // Write the 12 monthly ROOT files
    for (int m = 0; m < 12; ++m) {
        monthFiles[m]->cd();
        monthTrees[m]->Write();
        monthFiles[m]->Close();

        std::cout << "Month " << m + 1 << ": ";

        if (coldestDay[m] >= 0) {
            std::cout << coldestDate[m]
                      << ", temperature = "
                      << minTemp[m] << " C\n";
        } else {
            std::cout << "No good-quality measurements\n";
        }

        delete monthFiles[m];
    }

    ////////////////////////////////////////////////////////////Create scatter plot: month vs day of coldest date
    TGraph *graph = new TGraph();

    int point = 0;

    for (int m = 0; m < 12; ++m) {
        if (coldestDay[m] >= 0) {
            graph->SetPoint(point, m + 1, coldestDay[m]);
            ++point;
        }
    }

    TCanvas *canvas = new TCanvas(
        "canvas", "Coldest date of each month", 900, 600);

    graph->SetTitle(
        "Coldest Date in Each Month;Month;Day of month");

    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(1.4);
    graph->GetXaxis()->SetLimits(0.5, 12.5);
    graph->GetXaxis()->SetNdivisions(12);
    graph->GetYaxis()->SetRangeUser(0, 32);

    graph->Draw("AP");

    canvas->SaveAs("datasets/rootfiles/monthly_coldest.png");

    // Save the graph in a ROOT file too
    TFile *summary = new TFile(
        "datasets/rootfiles/monthly_coldest.root", "RECREATE");

    graph->Write("coldest_dates");
    canvas->Write("coldest_dates_canvas");

    summary->Close();

    delete summary;
    delete canvas;
    delete graph;

    input->Close();

    std::cout << "\nFinished! Monthly files and scatter plot saved.\n";
}
