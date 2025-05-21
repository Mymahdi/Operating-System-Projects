#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

void log(const string& msg) {
    cerr << "[FINAL OUTPUT] " << msg << endl;
}

struct GameEntry {
    string title;
    double score;

    bool operator<(const GameEntry& other) const {
        return score > other.score;
    }
};

int main() {
    log("Final output stage started.");

    ofstream output("../output/GameRanking.csv");
    if (!output.is_open()) {
        cerr << "Failed to open output file!" << endl;
        return 1;
    }

    output << "Game Title,Score\n";

    vector<GameEntry> entries;
    string line;
    int count = 0;

    if (!getline(cin, line)) {
        log("No input received.");
        return 1;
    }

    while (getline(cin, line)) {
        stringstream ss(line);
        string title, scoreStr;
        if (!getline(ss, title, ',')) continue;
        if (!getline(ss, scoreStr)) continue;

        try {
            double score = stod(scoreStr);
            entries.push_back({title, score});
        } catch (...) {
            log("Invalid score for line: " + line);
        }
    }

    sort(entries.begin(), entries.end());

    for (const auto& entry : entries) {
        output << entry.title << "," << entry.score << "\n";
    }

    if (entries.empty()) {
        log("No valid data to write to output file.");
    } else {
        log("Successfully wrote " + to_string(entries.size()) + " entries to output/GameRanking.csv.");
    }

    output.close();
    return 0;
}
