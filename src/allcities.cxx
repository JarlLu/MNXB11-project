#include <TCanvas.h>
#include <TImage.h>
#include <TLatex.h>
#include <TPad.h>
#include <allcities.h>
#include <iostream>

void allcities() {

    TCanvas* canvas = new TCanvas(
        "combined_canvas", "Monthly Coldest Days - Four Cities", 1600, 1200);

    // Leave space at the top for one overall title.
    canvas->SetTopMargin(0.08);
    canvas->Divide(2, 2);

    const char* filenames[4] = {
        "datasets/monthrootfiles/monthly_coldest_LUND.png",
        "datasets/monthrootfiles/monthly_coldest_UPPSALA.png",
        "datasets/monthrootfiles/monthly_coldest_KARLSTAD.png",
        "datasets/monthrootfiles/monthly_coldest_HALMSTAD.png"};

    for (int i = 0; i < 4; ++i) {
        canvas->cd(i + 1);

        TImage* image = TImage::Open(filenames[i]);

        if (!image) {
            std::cerr << "Could not open image: " << filenames[i] << '\n';
            continue;
        }

        image->Draw("x");
    }

    // Draw one overall title on the main canvas.
    canvas->cd();

    canvas->SaveAs("datasets/monthrootfiles/all_cities_2x2.png");

    delete canvas;
}
