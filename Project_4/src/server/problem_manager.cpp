#include "../includes/problem_manager.h"
#include <iostream>
using namespace std;

// Add a problem to the database
void ProblemManager::addProblem(const string& problemId, const string& problemStatement) {
    problemDatabase[problemId] = problemStatement;
}

// Check if a problem is available
bool ProblemManager::isProblemAvailable(const string& problemId) const {
    return problemDatabase.find(problemId) != problemDatabase.end();
}

string ProblemManager::getNextProblem() {
    // Return the next problem (replace with actual logic)
    return "Sample Problem Description";
}

// Get the problem statement
string ProblemManager::getProblemStatement(const string& problemId) const {
    if (isProblemAvailable(problemId)) {
        return problemDatabase.at(problemId);
    } else {
        return "Problem not found!";
    }
}

// List all problems
void ProblemManager::listProblems() const {
    for (const auto& problem : problemDatabase) {
        cout << "Problem ID: " << problem.first << " | Statement: " << problem.second << endl;
    }
}
