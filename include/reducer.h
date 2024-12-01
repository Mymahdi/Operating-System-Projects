#ifndef REDUCER_H
#define REDUCER_H

#include <vector>
#include <string>
#include <map>

// Reducer function to combine mapper outputs
std::map<std::string, std::pair<int, int>> reduceResults(const std::vector<std::map<std::string, std::pair<int, int>>>& results);

#endif // REDUCER_H
