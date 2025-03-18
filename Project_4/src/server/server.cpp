

#include "../includes/server_utils.h"
#include "../includes/team_manager.h"
#include "../includes/problem_manager.h"
#include "../includes/submission_manager.h"
#include "../includes/communication_handler.h"

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
        else if (clientMessage == "GET_PROBLEM") {
            std::string problem = problemManager.getNextProblem();
            commHandler.sendMessage(problem);
        }
        else if (clientMessage.find("SUBMIT") == 0) {
            std::string result = submissionManager.evaluateSubmission(clientMessage);
            commHandler.sendMessage(result);
        }
    }
    
    return 0;
}
