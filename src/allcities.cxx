#include <TCanvas.h>
#include <TImage.h>
#include <TLatex.h>
#include <TPad.h>
#include <allcities.h>
#include <iostream>
#include <TDatime.h>

void allcities() {
    //we make a big canvas -- the title does not matter 
    TCanvas* canvas = new TCanvas(
        "combined_canvas",
        "Monthly Coldest Days - Four Cities",
        1600, 1200
    );
    
        // Divide the canvas into four pads
    canvas->Divide(2, 2);

    //each element is pointer to c style string using an array of 4
    const char* filenames[4] = {
        "datasets/monthrootfiles/monthly_coldest_LUND.png",
        "datasets/monthrootfiles/monthly_coldest_UPPSALA.png",
        "datasets/monthrootfiles/monthly_coldest_KARLSTAD.png",
        "datasets/monthrootfiles/monthly_coldest_HALMSTAD.png"};

    for (int i = 0; i < 4; ++i) {
        //resolves a root pad issue 
        canvas->cd(i + 1);
        TImage* image = TImage::Open(filenames[i]);

        //if not valid 
        if (!image) {
            std::cerr << "Could not open image: " << filenames[i] << '\n';
            continue;
        }
        //draw without frame 
        image->Draw("x");
    }

    TDatime now;
    TLatex timestamp;
    timestamp.SetNDC();
    timestamp.SetTextSize(0.025);
    timestamp.SetTextColor(kGray + 2);
    timestamp.DrawLatex(0.02, 0.01, now.AsString());

    canvas->SaveAs("datasets/monthrootfiles/all_cities_2x2.png");
    delete canvas;
}
