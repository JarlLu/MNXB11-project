#include "Weekday.h"

#include <TAxis.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1D.h>
#include <THStack.h>
#include <TLegend.h>
#include <TMultiGraph.h>
#include <TPaveText.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "Csv2TTree.h"
#include "DataPoint.h"

namespace project {

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

std::vector<DataPoint> weekend_filter(std::vector<DataPoint> datapoints) {
    std::vector<DataPoint> filtered_points;
    for (DataPoint dp : datapoints) {
        int const weekday = dp.get_day_of_week();
        if (weekday == 6 || weekday == 7) {
            filtered_points.push_back(dp);
        };
    };
    auto year = datapoints[0].get_year();
    std::string output_filename = "weekends_in_" + std::to_string(year);

    return filtered_points;
}

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

    std::string output_file_path =
        "images/" + output_file_name + "_" + std::to_string(year) + ".png";
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

    // analysis
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
    axes->SetMaximum(16);
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
    std::string output_file_path = "images/" + output_file_name + ".png";
    canvas->SaveAs(output_file_path.c_str());

    delete legend;
    delete canvas;
    delete stack;

    for (TH1D* hist : hists) {
        delete hist;
    }
};

void plot_weekday_temp_trends(std::string csv_file_path,
                              std::string output_file_name,
                              std::vector<int> years) {

    std::vector<DataPoint> datapoints = parse_csv_file(csv_file_path);

    if (datapoints.empty() || years.empty()) {
        std::cerr << "No data or years provided.\n";
        return;
    }

    // Weekday labels.
    const char* weekdays[7] = {"Monday", "Tuesday",  "Wednesday", "Thursday",
                               "Friday", "Saturday", "Sunday"};

    // Distinct colours for each weekday.
    const int colours[7] = {kBlue + 2,  kMagenta + 2, kCyan + 2, kRed + 2,
                            kGreen + 2, kOrange + 2,  kBlack};

    // One graph for each weekday.
    TMultiGraph* multigraph = new TMultiGraph();

    std::vector<TGraph*> graphs;

    // Store temperatures for each weekday across all years.
    std::array<std::vector<double>, 7> weekday_temperatures;

    // Calculate and print the yearly statistics.
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nYearly Temperature Statistics\n";
    std::cout << "---------------------------------------------\n";
    std::cout << "Year    Mean (C)    Std Dev (C)\n";
    std::cout << "---------------------------------------------\n";

    for (int year : years) {

        std::vector<DataPoint> filtered_points = year_filter(datapoints, year);

        std::array<double, 7> weekday_average =
            average_temp_by_weekday(filtered_points);

        for (int day = 0; day < 7; ++day) {
            weekday_temperatures[day].push_back(weekday_average[day]);
        }

        // Calculate yearly mean and standard deviation.
        if (!filtered_points.empty()) {
            double sum = 0.0;

            for (const DataPoint& dp : filtered_points) {
                sum += dp.get_temperature();
            }

            double mean = sum / filtered_points.size();
            double squared_diff_sum = 0.0;

            for (const DataPoint& dp : filtered_points) {
                double diff = dp.get_temperature() - mean;
                squared_diff_sum += diff * diff;
            }

            double std_dev =
                std::sqrt(squared_diff_sum / filtered_points.size());

            std::cout << year << "    " << mean << "       " << std_dev << '\n';
        } else {
            std::cout << year << "    No data available\n";
        }
    }

    std::cout << "---------------------------------------------\n\n";

    // Create a graph for each weekday.
    for (int day = 0; day < 7; ++day) {

        TGraph* graph = new TGraph(static_cast<int>(years.size()));

        graph->SetName(("weekday_" + std::to_string(day)).c_str());

        graph->SetTitle(weekdays[day]);

        for (std::size_t i = 0; i < years.size(); ++i) {
            graph->SetPoint(static_cast<int>(i), years[i],
                            weekday_temperatures[day][i]);
        }

        graph->SetLineColor(colours[day]);
        graph->SetLineWidth(2);

        graph->SetMarkerColor(colours[day]);
        graph->SetMarkerStyle(20);
        graph->SetMarkerSize(0.7);

        multigraph->Add(graph, "LP");
        graphs.push_back(graph);
    }

    // Create canvas.
    TCanvas* canvas = new TCanvas("weekday_trend_canvas",
                                  "Temperature Trends by Weekday", 1200, 700);

    multigraph->SetTitle("Average Temperature by Weekday;"
                         "Year;"
                         "Mean Temperature (#circC)");

    multigraph->Draw("A");
    multigraph->GetXaxis()->LabelsOption("v");

    multigraph->GetXaxis()->SetLimits(years.front() - 0.5, years.back() + 0.5);

    multigraph->GetYaxis()->SetTitle("Mean Temperature (#circC)");

    multigraph->GetXaxis()->SetTitle("Year");
    canvas->SetBottomMargin(0.18);

    // Display a legend.
    // Leave space on the right for the legend.
    canvas->SetRightMargin(0.25);

    // Draw the legend outside the graph.
    TLegend* legend = new TLegend(0.77, 0.35, 0.98, 0.65);

    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    legend->SetTextSize(0.03);

    for (int day = 0; day < 7; ++day) {
        legend->AddEntry(graphs[day], weekdays[day], "lp");
    }

    legend->Draw();

    canvas->SetGrid();

    canvas->Modified();
    canvas->Update();

    std::string output_file_path = "images/" + output_file_name + ".png";

    canvas->SaveAs(output_file_path.c_str());

    std::cout << "canvas saved!\n";

    // Clean up.
    delete legend;
    delete canvas;
    delete multigraph;
}

} // namespace project