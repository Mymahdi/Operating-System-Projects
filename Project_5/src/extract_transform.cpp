#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <unistd.h>

using namespace std;

struct GameData {
    string title;
    float originalPrice;
    float discountedPrice;
    int recentReviews;
    int allReviews;

    float discountPercentage;

    int RecentReviewsSummary;
    int AllReviewsSummary;
};

int reviewToScore(const string& summary) {
    if (summary == "Overwhelmingly Positive") return 7;
    if (summary == "Very Positive") return 6;
    if (summary == "Positive") return 5;
    if (summary == "Mostly Positive") return 4;
    if (summary == "Mixed") return 3;
    if (summary == "Mostly Negative") return 2;
    if (summary == "Negative") return 1;
    if (summary == "Very Negative") return 0;
    return 3;
}

float cleanPrice(const string& priceStr) {
    string cleaned = priceStr;
    cleaned.erase(remove(cleaned.begin(), cleaned.end(), '$'), cleaned.end());
    return stof(cleaned);
}

int extractUserCount(const string& str) {
    size_t num_start = str.find("the ");
    if (num_start == string::npos) return 0;
    num_start += 4;
    size_t num_end = str.find(" user", num_start);
    if (num_end == string::npos) return 0;

    string number_str;
    for (size_t i = num_start; i < num_end; i++) {
        if (isdigit(str[i])) {
            number_str += str[i];
        }
    }

    return number_str.empty() ? 0 : stoi(number_str);
}

void processCSV(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    getline(file, line);
    
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }
            GameData game;
            game.title = tokens[0];
            game.originalPrice = cleanPrice(tokens[1]);
            game.discountedPrice = cleanPrice(tokens[2]);
            game.RecentReviewsSummary = reviewToScore(tokens[5]);
            game.AllReviewsSummary = reviewToScore(tokens[6]);
            game.recentReviews = extractUserCount(tokens[7]);
            game.allReviews = extractUserCount(tokens[8]);

            if (game.originalPrice > 0) {
                game.discountPercentage = (1 - (game.discountedPrice / game.originalPrice)) * 100;
            } else {
                game.discountPercentage = 0;
            }

            cout << game.title << ","
                 << game.RecentReviewsSummary << ","
                 << game.AllReviewsSummary << ","
                 << game.originalPrice << ","
                 << game.discountedPrice << ","
                 << game.discountPercentage << ","
                 << game.recentReviews  << ","
                 << game.allReviews << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " input_csv1 [input_csv2 ...]" << endl;
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        processCSV(argv[i]);
    }

    return 0;
}
