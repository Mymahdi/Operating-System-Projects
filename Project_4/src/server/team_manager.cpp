
#include "../includes/team_manager.h"
#include <iostream>
#include <algorithm>

using namespace std;

void TeamManager::registerTeam(const string& teamName) {
    if (teams.find(teamName) != teams.end()) {
        cout << "Team '" << teamName << "' is already registered." << endl;
        return;
    }
    teams.insert(teamName);
    cout << "Team '" << teamName << "' successfully registered." << endl;
}

string TeamManager::registerUser(const string& userData) {
    return "User registered successfully: " + userData;
}

bool TeamManager::isTeamRegistered(const string& teamName) const {
    return teams.find(teamName) != teams.end();
}

void TeamManager::listTeams() const {
    cout << "Registered Teams:" << endl;
    for (const auto& team : teams) {
        cout << "- " << team << endl;
    }
}
