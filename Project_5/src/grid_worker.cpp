#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <unistd.h>
#include <cfloat>


using namespace std;

void log(const string& msg) {
    cerr << "[WORKER] " << msg << endl;
}

float normalize(float value, float minVal, float maxVal) {
    return (maxVal - minVal == 0) ? 1.0 : ((value - minVal) / (maxVal - minVal)) + 1;
}


int main() {
    log("Worker started.");
    string line;

    while (getline(cin, line)) {
        log("Received line: " + line);

        stringstream ss(line);
        string title;
        float recentScore, allScore, originalPrice, discountedPrice, discount;
        int recentReviews, allReviews;

        getline(ss, title, ',');
        ss >> recentScore;
        ss.ignore();
        ss >> allScore;
        ss.ignore();
        ss >> originalPrice;
        ss.ignore();
        ss >> discountedPrice;
        ss.ignore();
        ss >> discount;
        ss.ignore();
        ss >> recentReviews;
        ss.ignore();
        ss >> allReviews;

        float minRecentReviews = FLT_MAX, maxRecentReviews = FLT_MIN;
        float minAllReviews = FLT_MAX, maxAllReviews = FLT_MIN;

        
        if (recentReviews < minRecentReviews) minRecentReviews = recentReviews;
        if (recentReviews > maxRecentReviews) maxRecentReviews = recentReviews;

        if (allReviews < minAllReviews) minAllReviews = allReviews;
        if (allReviews > maxAllReviews) maxAllReviews = allReviews;



        float n_discount = normalize(discount, 0, 100);
        float n_recentScore = normalize(recentScore, 0, 7);
        float n_allScore = normalize(allScore, 0, 7);
        float n_recentReviews = normalize(recentReviews, 0, 100000);
        float n_allReviews = normalize(allReviews, 0, 1000000);
        float criterion = (10 * n_discount * n_recentScore * n_allScore * n_recentReviews * n_allReviews) / (originalPrice == 0 ? 1 : originalPrice);

        

        log("Computed score for " + title + ": " + to_string(criterion));
        cout << title << "," << criterion << endl;
    }

    log("Worker finished.");
    return 0;
}
