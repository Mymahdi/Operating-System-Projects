#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <poll.h>
#include <chrono>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define TIME_LIMIT 60
#define EVALUATION_PORT 65432 

using namespace std;

std::mutex mtx;

struct Submission {
    std::string code;
    int problemIndex;
    double timeTaken;
    std::string result;
};

struct ClientInfo {
    int socket;
    std::string username;
    std::string role;
    std::vector<Submission> submissions;
};

std::vector<ClientInfo> clients;
std::vector<std::string> problems = {
    "Problem 1: add_numbers(a, b) → Return the sum of two integers.",
    "Problem 2: reverse_string(s) → Reverse a string (e.g., \"hello\" → \"olleh\").",
    "Problem 3: is_palindrome(s) → Check if a string is a palindrome (e.g., \"madam\")."
};

bool is_disconnected(int socket) {
    struct pollfd pfd;
    pfd.fd = socket;
    pfd.events = POLLIN;
    if (poll(&pfd, 1, 0) > 0) {
        char buf[1];
        int result = recv(socket, buf, 1, MSG_PEEK | MSG_DONTWAIT);
        return result == 0;
    }
    return false;
}


void broadcast(const std::string& message) {
    sockaddr_in broadcastAddr;
    int broadcastSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (broadcastSocket < 0) return;

    int broadcastEnable = 1;
    setsockopt(broadcastSocket, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(PORT);
    broadcastAddr.sin_addr.s_addr = INADDR_BROADCAST;

    sendto(broadcastSocket, message.c_str(), message.size(), 0,
           (sockaddr*)&broadcastAddr, sizeof(broadcastAddr));
    close(broadcastSocket);
}

void send_to(int socket, const std::string& message) {
    send(socket, message.c_str(), message.length(), 0);
}

std::string recv_from(int socket) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_received = recv(socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received <= 0) return "";
    return std::string(buffer);
}

std::string evaluate_single_answer(const std::string& answer) {
    int eval_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (eval_sock == -1) return "Failed";

    sockaddr_in eval_addr {};
    eval_addr.sin_family = AF_INET;
    eval_addr.sin_port = htons(EVALUATION_PORT);
    eval_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(eval_sock, (sockaddr*)&eval_addr, sizeof(eval_addr)) == -1)
        return "Failed";

    send_to(eval_sock, answer);
    std::string result = recv_from(eval_sock);
    close(eval_sock);
    return result;
}

// auto calculate_score = [](const ClientInfo& player) {
//     double score = 0.0;
//     double bonus = 0.0;

//     for (const auto& sub : player.submissions) {
//         if (sub.result != "PASS") continue;

//         int base = (sub.problemIndex == 0) ? 1 :
//                    (sub.problemIndex == 1) ? 3 :
//                    (sub.problemIndex == 2) ? 5 : 0;

//         if (sub.timeTaken <= (TIME_LIMIT * 0.5)) {
//             bonus = 0.5;
//         } 
//         else if (sub.timeTaken <= (TIME_LIMIT * 0.75)) {
//             bonus = 0.2;
//         }
//         score += base * (1 + bonus);
//     }
//     return score;
// };


auto calculate_score = [](const ClientInfo& player) {
    double score = 0.0;
    double bonus = 0.0;

    for (const auto& sub : player.submissions) {
        std::cout << "Evaluating submission for problem index " << sub.problemIndex << std::endl;
        std::cout << "Result: " << sub.result << ", Time Taken: " << sub.timeTaken << std::endl;

        if (sub.result != "PASS") {
            std::cout << "Skipping because result != Passed\n";
            continue;
        }

        int base = (sub.problemIndex == 0) ? 1 :
                   (sub.problemIndex == 1) ? 3 :
                   (sub.problemIndex == 2) ? 5 : 0;

        if (sub.timeTaken <= (TIME_LIMIT * 0.5)) {
            bonus = 0.5;
        } 
        else if (sub.timeTaken <= (TIME_LIMIT * 0.75)) {
            bonus = 0.2;
        } else {
            bonus = 0.0;
        }

        double sub_score = base * (1 + bonus);
        score += sub_score;

        std::cout << "Base: " << base << ", Bonus: " << bonus << ", Sub-score: " << sub_score << ", Total so far: " << score << "\n";
    }

    std::cout << "Final score: " << score << std::endl;
    return score;
};


void handle_problem_flow(ClientInfo* coder, ClientInfo* navigator) {
    for (size_t i = 0; i < problems.size(); ++i) {
        send_to(coder->socket, "Problem " + std::to_string(i + 1) + ": " + problems[i]);
        send_to(navigator->socket, "Waiting for coder to solve Problem " + std::to_string(i + 1));

        std::string solution;
        bool shared = false;
        auto start_time = std::chrono::steady_clock::now();

        struct pollfd fds[2];
        fds[0].fd = coder->socket;
        fds[0].events = POLLIN;
        fds[1].fd = navigator->socket;
        fds[1].events = POLLIN;

        while (true) {
            if (std::chrono::steady_clock::now() - start_time > std::chrono::seconds(TIME_LIMIT)) {
                broadcast("Time's up for Problem " + std::to_string(i + 1));
                std::cout << "[LOG] Time's up for Problem " << (i + 1) << std::endl;
                break;
            }

            
            int ret = poll(fds, 2, 100); 
            if (ret < 0) {
                std::cerr << "[ERROR] poll() failed.\n";
                break;
            } else if (ret == 0) {
                continue; 
            }

            if (fds[0].revents & POLLIN) {
                char buffer[BUFFER_SIZE];
                memset(buffer, 0, BUFFER_SIZE);
                int bytes_received = recv(coder->socket, buffer, BUFFER_SIZE, 0);
                if (bytes_received <= 0) {
                    std::cout << "[LOG] Coder disconnected.\n";
                    send_to(navigator->socket, "Coder disconnected. Session terminated.");
                    broadcast("Coder disconnected. Session terminated.");
                    close(coder->socket);
                    close(navigator->socket);
                    {
                        std::lock_guard<std::mutex> lock(mtx);
                        clients.clear();
                    }
                    return;
                }
                std::string msg(buffer);
                if (msg.find("/share") != std::string::npos) {
                    shared = true;
                    std::cout << "[LOG] Coder shared solution.\n";
                    send_to(navigator->socket, "Coder's solution:\n" + solution);
                    send_to(navigator->socket, "Approve or Deny?");
                } else {
                    solution += msg + "\n";
                    std::cout << "[LOG] Coder wrote: " << msg << std::endl;
                }
            }

            if (shared && (fds[1].revents & POLLIN)) {
                char nav_buffer[BUFFER_SIZE];
                memset(nav_buffer, 0, BUFFER_SIZE);
                int bytes_received = recv(navigator->socket, nav_buffer, BUFFER_SIZE, 0);
                if (bytes_received <= 0) {
                    std::cout << "[LOG] Navigator disconnected.\n";
                    send_to(coder->socket, "Navigator disconnected. Session terminated.");
                    broadcast("Navigator disconnected. Session terminated.");
                    close(coder->socket);
                    close(navigator->socket);
                    {
                        std::lock_guard<std::mutex> lock(mtx);
                        clients.clear();
                    }
                    return;
                }

                std::string nav_reply(nav_buffer);
                std::cout << "[LOG] Navigator replied: " << nav_reply << std::endl;

                if (nav_reply == "approve") {
                    broadcast("Solution approved! Evaluating...");
                    auto end_time = std::chrono::steady_clock::now();
                    double time_taken = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();

                    std::string result = evaluate_single_answer(solution);
                    std::cout << "[LOG] Evaluation result: " << result << std::endl;

                    Submission sub = {solution, static_cast<int>(i), time_taken, result};
                    coder->submissions.push_back(sub);

                    break; 
                }
                
                else {
                    send_to(coder->socket, "Solution denied. Try again.");
                    std::cout << "[LOG] Solution denied. Restarting..." << std::endl;
                    shared = false;
                    solution.clear();
                    start_time = std::chrono::steady_clock::now();
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    broadcast("All problems completed.");
    std::cout << "[LOG] All problems completed." << std::endl;

    double coder_score = calculate_score(*coder);
    std::string coder_msg = "Final Score for " + coder->username + ": " + std::to_string(coder_score);
    std::cout << "[LOG] " << coder_msg << std::endl;

    send_to(coder->socket, coder_msg);
    send_to(navigator->socket, coder_msg);
}


void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received <= 0) {
        std::cout << "[LOG] Client disconnected before sending username." << std::endl;
        close(client_socket);
        return;
    }
    std::string username(buffer);
    std::cout << "[LOG] Received username: " << username << std::endl;

    {
        std::lock_guard<std::mutex> lock(mtx);
        for (const auto& client : clients) {
            if (client.username == username) {
                send_to(client_socket, "Error: Username already taken. Connection terminated.");
                std::cout << "[LOG] Duplicate username: " << username << ". Connection terminated.\n";
                close(client_socket);
                return;
            }
        }
    }

    send_to(client_socket, "Username received.");

    memset(buffer, 0, BUFFER_SIZE);
    bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received <= 0) {
        std::cout << "[LOG] Client disconnected after sending username: " << username << std::endl;
        std::lock_guard<std::mutex> lock(mtx);
        for (auto& client : clients) {
            send_to(client.socket, "Client " + username + " disconnected. Session terminated.");
        }
        close(client_socket);
        clients.clear(); 
        return;
    }
    std::string role(buffer);
    std::cout << "[LOG] Received role: " << role << std::endl;

    send_to(client_socket, "Role received: " + role);

    ClientInfo new_client = {client_socket, username, role};

    {
        std::lock_guard<std::mutex> lock(mtx);
        clients.push_back(new_client);
    
        std::thread([client_socket, username]() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                if (is_disconnected(client_socket)) {
                    std::lock_guard<std::mutex> lock(mtx);
                    std::cout << "[LOG] " << username << " disconnected unexpectedly.\n";
    
                    for (const auto& c : clients) {
                        if (c.socket != client_socket) {
                            send_to(c.socket, username + " disconnected. Session terminated.");
                            close(c.socket);
                        }
                    }
    
                    close(client_socket);
                    clients.clear();
                    break;
                }
            }
        }).detach();
    }
    

    if (clients.size() == 2) {
        ClientInfo* coder = nullptr;
        ClientInfo* navigator = nullptr;

        for (auto& c : clients) {
            if (c.role == "coder") coder = &c;
            else if (c.role == "navigator") navigator = &c;
        }

        if (coder && navigator) {
            std::cout << "[LOG] Starting problem flow with coder: " << coder->username
                      << " and navigator: " << navigator->username << std::endl;
            std::thread(handle_problem_flow, coder, navigator).detach();
        }
    }
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Failed to create socket.\n";
        return 1;
    }

    sockaddr_in server_addr {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed.\n";
        return 1;
    }

    if (listen(server_socket, 2) < 0) {
        std::cerr << "Listen failed.\n";
        return 1;
    }

    std::cout << "Server is listening on port " << PORT << "...\n";

    while (true) {
        sockaddr_in client_addr {};
        socklen_t client_size = sizeof(client_addr);
        int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_size);
        if (client_socket >= 0) {
            std::cout << "[LOG] New client connected." << std::endl;
            std::thread(handle_client, client_socket).detach();
        }
    }

    close(server_socket);
    return 0;
}