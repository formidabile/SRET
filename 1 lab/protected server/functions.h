#ifndef FUNCTIONS
#define FUNCTIONS

#include <iostream>
#include <string>
#include <map>
#include <regex>
#include <uwebsockets/App.h>
#include <nlohmann/json.hpp>

struct UserData;
typedef uWS::WebSocket<false, true, UserData>* websock;
using json = nlohmann::json;

// processing message to bot
std::string func1(std::string question);
// return name of personal channel
std::string func2(UserData* data);
// processing message for the user
void func3(websock ws, json parsed);
// processing message for everybody
void func4(websock ws, json parsed);
// processing status of the user: online or offline
std::string func5(UserData* data, bool status);
// set name for user
void func6(websock ws, json parsed);
// process command MSG_TO_BOT
std::string func7(websock ws, json parsed);



struct UserData {
	int id;
	std::string name;
};

#endif