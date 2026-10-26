#include "Weekday.h"

#include <TAxis.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1D.h>
#include <THStack.h>
#include <TLegend.h>
#include <TPaveText.h>

#include <array>
#include <string>
#include <vector>

#include "Csv2TTree.h"
#include "DataPoint.h"

namespace project {

/**
 * @brief Filters out DataPoints for a given year.
 *
 * @param datapoints
 * @param year
 * @return std::vector<DataPoint>
 */
std::vector<DataPoint> year_filter(std::vector<DataPoint> datapoints,
                                   int year) {
    /** */
    std::vector<DataPoint> filtered_points;

    for (DataPoint dp : datapoints) {
        if (dp.get_year() == year && dp.good_quality()) {
            filtered_points.push_back(dp);
        };
    };
    return filtered_points;
};

/**
 * @brief Calculates the average temperature for each day of the week.
 *
 * @param datapoints
 * @return std::array<double, 7>
 */
std::array<double, 7>
average_temp_by_weekday(std::vector<DataPoint> datapoints) {
    // plot average temperature for each weekday
    double temperature_sum[7] =
        {}; // stores sum of temperatures for each weekday
    int temperature_day_count[7] =
        {}; // keeps track of how many temperature measurements has been added
            // to each weekday sum

    for (DataPoint dp : datapoints) {
        int weekday = dp.get_day_of_week();

        if (weekday >= 1 &&
            weekday <= 7) { // check that day of week is within bounds
            temperature_sum[weekday - 1] += dp.get_temperature();
            temperature_day_count[weekday - 1]++;
        };
    };

    std::array<double, 7> weekday_average =
        {}; // used to store average temp for each weekday

    for (int i = 0; i < 7; ++i) {
        if (temperature_day_count[i] > 0) {
            weekday_average[i] = temperature_sum[i] / temperature_day_count[i];
        } else {
            weekday_average[i] = 1000.0;
        };
    };
    return weekday_average;
};
/**
 * @brief Create histogram of average temperature for each day of the week (one
 * year)
 *
 * @param csv_file_path
 * @param output_file_name
 * @param year
 */
void plot_average_temp_by_weekday(std::string csv_file_path,
                                  std::string output_file_name, int year) {
    std::vector<DataPoint> datapoints = parse_csv_file(csv_file_path);
    std::vector<DataPoint> filtered_points = year_filter(datapoints, year);
    std::array<double, 7> weekday_average =
        average_temp_by_weekday(filtered_points);
    std::string title =
        "Average Temperature by Weekday;Day of the Week;Mean Temperature "
        "(#circC)";

    TH1D* hist = new TH1D("Weekday Temperature", title.c_str(), 7, 0.5, 7.5);

    const char* weekdays[7] = {"Monday", "Tuesday",  "Wednesday", "Thursday",
                               "Friday", "Saturday", "Sunday"};

    // label bins
    for (int i = 0; i < 7; i++) {
        hist->GetXaxis()->SetBinLabel(i + 1, weekdays[i]);
        hist->SetBinContent(i + 1, weekday_average[i]);
    };

    hist->SetMinimum(0);
    hist->SetMaximum(15);
    // Draw histogram

    TCanvas* canvas =
        new TCanvas("weekday_temp_canvas", title.c_str(), 1000, 600);

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

    std::string output_file_path = "images/hannas_plots/" + output_file_name +
                                   "_" + std::to_string(year) + ".png";
    canvas->Update();
    canvas->SaveAs(output_file_path.c_str());

    delete canvas;
    delete hist;
};

/**
 * @brief Create histogram of average temperature for each day of the week
 * (multiple years)
 *
 * @param csv_file_path
 * @param output_file_name
 * @param years
 */
void plot_av_temp_weekday_comparison(std::string csv_file_path,
                                     std::string output_file_name,
                                     std::vector<int> years) {
    std::vector<DataPoint> datapoints = parse_csv_file(csv_file_path);

    std::string title =
        "Average Temperature by Weekday;Day of the Week;Mean Temperature "
        "(#circC)";

    const char* weekdays[7] = {"Monday", "Tuesday",  "Wednesday", "Thursday",
                               "Friday", "Saturday", "Sunday"};

    // create histogram stack
    THStack* stack =
        new THStack("temp_weekday_comparison", "Average Temperature by Weekday;"
                                               "Day of the Week;"
                                               "Mean Temperature (#circC)");

    // Colours for the different years.
    const int colours[] = {kMagenta - 2, kCyan - 2,  kAzure - 2,
                           kOrange - 2,  kGreen - 2, kRed - 2};

    std::vector<TH1D*> hists;

    for (std::size_t i = 0; i < years.size(); ++i) {
        int year = years[i];

        std::vector<DataPoint> filtered_points = year_filter(datapoints, year);
        std::array<double, 7> weekday_average =
            average_temp_by_weekday(filtered_points);

        std::string hist_name = "weekday_temperature_" + std::to_string(year);

        std::string hist_title =
            "Average Temperature by Weekday in " + std::to_string(year);

        TH1D* hist =
            new TH1D(hist_name.c_str(), hist_title.c_str(), 7, 0.5, 7.5);

        hist->SetDirectory(nullptr);

        // label bins
        for (int j = 0; j < 7; j++) {
            hist->GetXaxis()->SetBinLabel(j + 1, weekdays[j]);
            hist->SetBinContent(j + 1, weekday_average[j]);
        };

        hist->SetMinimum(0);
        hist->SetMaximum(15);

        hist->SetFillColor(colours[i % 6]);
        hist->SetLineColor(colours[i % 6]);
        hists.push_back(hist);
        stack->Add(hist);
    }

    TCanvas* canvas = new TCanvas("weekday_comparison_canvas",
                                  "Weekday Temperature Comparison", 1000, 600);

    // Had to add this since I could not get axes to show any other way...
    TH1D* axes = new TH1D("axes",
                          "Average Temperature by Weekday;Day of the Week;Mean "
                          "Temperature (#circC)",
                          7, 0.5, 7.5);

    axes->SetMinimum(0);
    axes->SetMaximum(20);
    axes->SetStats(false);

    for (int j = 0; j < 7; ++j) {
        axes->GetXaxis()->SetBinLabel(j + 1, weekdays[j]);
    }

    axes->Draw("AXIS");

    // manually adding title beacuse I couldn't fix it any other way
    TPaveText* main_title = new TPaveText(0.15, 0.91, 0.85, 0.98, "NDC");

    main_title->AddText("Average Temperature by Weekday");
    main_title->SetFillStyle(0);
    main_title->SetBorderSize(0);
    main_title->SetTextFont(62);
    main_title->SetTextSize(0.035);
    main_title->Draw();

    stack->Draw("NOSTACK BAR SAME");

    TLegend* legend = new TLegend(0.75, 0.65, 0.90, 0.88);

    for (std::size_t k = 0; k < years.size(); ++k) {
        legend->AddEntry(hists[k], std::to_string(years[k]).c_str(), "f");
    }

    legend->Draw();
    canvas->Modified();
    canvas->Update();
    std::string output_file_path =
        "images/hannas_plots/" + output_file_name + ".png";
    canvas->SaveAs(output_file_path.c_str());

    delete legend;
    delete canvas;
    delete stack;

    for (TH1D* hist : hists) {
        delete hist;
    }
};

} // namespace project