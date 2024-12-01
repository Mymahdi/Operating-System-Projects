#include "reducer.h"
#include <map>
#include <string>

std::map<std::string, std::pair<int, int>> reduceResults(
    const std::vector<std::map<std::string, std::pair<int, int>>>& mapperResults) {
    std::map<std::string, std::pair<int, int>> finalResults;

    for (const auto& result : mapperResults) {
        for (const auto& [product, stats] : result) {
            finalResults[product].first += stats.first;  // Aggregate remaining stock
            finalResults[product].second += stats.second; // Aggregate profit
        }
    }

    return finalResults;
}
