#include "server_utils.h"
#include "team_manager.h"
#include "problem_manager.h"
#include "submission_manager.h"
#include "communication_handler.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int port = std::stoi(argv[1]);
    
    CommunicationHandler commHandler(port);
    TeamManager teamManager;
    ProblemManager problemManager;
    SubmissionManager submissionManager;
    
    commHandler.startServer();

    while (true) {
        std::string clientMessage = commHandler.receiveMessage();
        if (clientMessage.empty()) continue;
    
        if (clientMessage.find("REGISTER") == 0) {
            std::string response = teamManager.registerUser(clientMessage);
            commHandler.sendMessage(response);
        }
    }
    
    return 0;
}