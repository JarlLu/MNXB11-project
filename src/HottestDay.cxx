#include "Csv2TTree.h"
#include "DataPoint.h"
#include "HottestDay.h"


#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <array>

#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TLegend.h>
#include <TH1D.h>
#include <TPaveText.h>

#include <chrono>


namespace project {

    std::vector<DataPoint> year_filter(std::vector<DataPoint> datapoints, int year) {
        // selects data from one year only
        std::vector<DataPoint> filtered_points;

        for (DataPoint dp : datapoints) {
            if (dp.get_year() == year && dp.good_quality()) {
                filtered_points.push_back(dp);
            };
        };
        return filtered_points;
    };

    std::array<double, 7>  average_temp_by_weekday(std::vector<DataPoint> datapoints) {
        // plot average temperature for each weekday
        double temperature_sum[7] =  {}; // stores sum of temperatures for each weekday
        int temperature_day_count[7] = {}; // keeps track of how many temperature measurements has been added to each weekday sum

        for (DataPoint dp : datapoints) {
            int weekday = dp.get_day_of_week();

            if (weekday >= 1 && weekday <= 7) { // check that day of week is within bounds
                temperature_sum[weekday-1] += dp.get_temperature();
                temperature_day_count[weekday-1] ++;
            };
        };

        std::array<double, 7>  weekday_average = {}; // used to store average temp for each weekday

        for (int i = 0; i<7; ++i) {
            if (temperature_day_count[i] > 0) {
                weekday_average[i] = temperature_sum[i] / temperature_day_count[i];
            } else {
                weekday_average[i] = 1000.0;
            };
        };
        return weekday_average;
    };

    void plot_average_temp_by_weekday(std::string csv_file_path, std::string output_file_name, int year) {
        std::vector<DataPoint> datapoints = parse_csv_file(csv_file_path);
        std::vector<DataPoint> filtered_points = year_filter(datapoints, year);
        std::array<double, 7> weekday_average = average_temp_by_weekday(filtered_points);
        std::string title = "Average Temperature by Weekday;Day of the Week;Mean Temperature (#circC)";

        TH1D* hist = new TH1D(
            "Weekday Temperature",
            title.c_str(),
            7,
            0.5,
            7.5
        );

        const char* weekdays[7] = {
            "Monday",
            "Tuesday",
            "Wednesday",
            "Thursday",
            "Friday",
            "Saturday",
            "Sunday"
        };

        // label bins
        for (int i = 0; i < 7; i++) {
            hist->GetXaxis()->SetBinLabel(i + 1, weekdays[i]);
            hist->SetBinContent(i + 1, weekday_average[i]);
        };

        hist->SetMinimum(0);
        hist->SetMaximum(10);
        // Draw histogram

        TCanvas* canvas = new TCanvas(
            "weekday_temp_canvas",
            title.c_str(),
            1000,
            600
        );
        
        hist->SetFillColor(kAzure - 9);
        hist->SetLineColor(kAzure + 2);
        hist->SetBarWidth(0.7);
        hist->SetBarOffset(0.15);
        

        hist->Draw("BAR");
        hist->SetStats(false);
        
        TPaveText* info = new TPaveText(0.72, 0.40, 0.88, 0.55, "NDC");
        info->SetFillColor(kWhite);
        info->SetFillStyle(1001);
        info->SetBorderSize(1); 
        info->SetTextAlign(12);
        info->SetTextSize(0.03);

        info->AddText(("Year: " + std::to_string(year)).c_str());
        info->AddText("Location: Lund");
        info->AddText("Data: SMHI");

        info->Draw();



        
        std::string output_file_path = "images/hannas_plots/" + output_file_name + "_" + std::to_string(year) + ".png";
        canvas->Update();
        canvas->SaveAs(output_file_path.c_str());
    
        delete canvas;
        delete hist;
    
    };

} // namespace project