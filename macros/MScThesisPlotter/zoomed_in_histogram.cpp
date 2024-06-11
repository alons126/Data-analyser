void zoomed_in_histogram() {
    // Create a canvas
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);

    // Create a histogram
    TH1D *h1 = new TH1D("h1", "Histogram;X-axis;Y-axis", 100, -4, 4);

    // Fill the histogram with random data
    h1->FillRandom("gaus", 10000);

    // Draw the main histogram
    h1->Draw();

    // Clone the main histogram
    string CloneName = h1->GetName();
    TH1D *h2 = (TH1D *) h1->Clone((CloneName + "_zoomin").c_str());
    string Histogram_title = h2->GetTitle();
    h2->SetTitle((Histogram_title + " (zoomin)").c_str());

    // Define the region to zoom in
    Double_t x1 = -1.0;  // x-axis lower bound
    Double_t x2 =  1.0;  // x-axis upper bound
    Double_t y1 =  0.0;  // y-axis lower bound
    Double_t y2 =  100.0; // y-axis upper bound

    // Create a new pad for the zoomed-in area
    TPad *pad = new TPad("pad", "Zoomed-In Region", 0.55, 0.55, 0.95, 0.95);
    pad->SetFillColor(0);  // Set pad color to white
    pad->Draw();
    pad->cd();

    // Draw the zoomed-in histogram
    h2->GetXaxis()->SetRangeUser(x1, x2);
    h2->GetYaxis()->SetRangeUser(y1, y2);
    h2->Draw();

    // Return to the main canvas
    c1->cd();

//    // Draw a box around the zoomed-in region on the main histogram
//    TBox *box = new TBox(x1, y1, x2, y2);
//    box->SetLineColor(kRed);
//    box->SetLineWidth(2);
//    box->SetFillStyle(0);  // No fill
//    box->Draw();
}

