
#include "../includes/communication_handler.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

CommunicationHandler::CommunicationHandler(int port) : serverPort(port) {}