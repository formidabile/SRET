#include <iostream>
#include <string>
#include <map>
#include <regex>
#include <uwebsockets/App.h>
#include <nlohmann/json.hpp>

using namespace std;

bool flag_exit = false; // флаг выхода из чата

// База вопросов и ответов
std::map<std::string, std::string> questions_answers = 
{
	{"(Hello|Good.*morning)", "Hi"},
	{"(How.*are.*you|How.*do.*you.*do)", "I'm fine"},
	{"(What.*(is)?.*your.*name|Who.*are.*you)", "My name is ServerChat5000"},
	{"What.*is.*the.*weather.*like.*today", "Today is warm and sunny"},
	{"(Goodbye|BB|Bye)", "Good luck"}
	
};

// функция обработки пользовательского сообщения
// возвращает ответ бота  
std::string bot_reply(std::string question)
{
	std::string reply = "";
	for (auto item : questions_answers)
	{
		// поиск шаблона, используя регулярные выражения
		auto pattern = std::regex(".*" + item.first + ".*", std::regex_constants::icase);
		// regex_constants::icase - регистр не имеет значения

		// поиск шаблона сообщения о выходе
		auto exit_pattern = std::regex(".*(goodbye|bye|bb).*", std::regex_constants::icase);

		// если пользоватеьский текст совпадает с шаблоном
		if (regex_match(question, pattern)) {
			// если шаблон совпадает с сообщением о выходе
			if (std::regex_match(question, exit_pattern))
				flag_exit = true;
			reply += item.second + ". "; // ответ найден
		}
	}
	if (reply == "")
		reply = "Please, repeat your message...";
	return reply;
}

const string PRIVATE_MSG = "PRIVATE_MSG";
const string PUBLIC_MSG = "PUBLIC_MSG";
const string STATUS = "STATUS";
const string SET_NAME = "SET_NAME";
const string MSG_TO_BOT = "MSG_TO_BOT";
const string COMMON = "COMMON";

// хранение данных о подлючившемся пользователе
struct UserData {
	int id;
	std::string name;
};

typedef uWS::WebSocket<false, true, UserData>* websock;
using json = nlohmann::json;
map<int, UserData*> all_users;

string personal_channel(UserData* data)
{
	return "USER_" + to_string(data->id);
}

void process_private_msg(websock ws, json parsed)
{
	UserData* data = ws->getUserData();

	json payload;
	payload["command"] = PRIVATE_MSG;
	payload["user_from"] = data->id;
	payload["text"] = parsed["text"];

	UserData* recepient = all_users[parsed["user_to"]];

	ws->publish(personal_channel(recepient), payload.dump());
}

void process_public_msg(websock ws, json parsed)
{
	UserData* data = ws->getUserData();

	json payload;
	payload["command"] = PUBLIC_MSG;
	payload["text"] = parsed["text"];
	payload["user_from"] = data->id;

	ws->publish(COMMON, payload.dump());
}

std::string user_status(UserData* data, bool status)
{

	json payload;
	payload["command"] = STATUS;
	payload["online"] = status;
	payload["user_id"] = data->id;
	payload["name"] = data->name;

	return payload.dump();

}

void process_set_name(websock ws, json parsed)
{
	UserData* data = ws->getUserData();
	data->name = parsed["name"]; // обновляем имя
	ws->publish(COMMON, user_status(data, true)); // сообщаем об этом
}

std::string process_msg_to_bot(websock ws, json parsed)
{
	UserData* data = ws->getUserData();

	json payload;
	payload["command"] = MSG_TO_BOT;
	payload["text"] = parsed["text"];
	payload["user_from"] = data->id;

	ws->publish(COMMON, payload.dump());

	json receive;
	receive["command"] = MSG_TO_BOT;
	receive["text"] = bot_reply(parsed["text"]);
	receive["user_to"] = data->id;
	receive["name"] = "ChatBotServer5000";
	

	return receive.dump();
}

int main() 
{
	int latest_user_id = 10; // ID последнего подлючившегося
	unsigned connected_users = 0;

	uWS::App app = uWS::App();
	app.get("/", [](auto* response, auto* request) {
		response->writeHeader("Content-Type", "text/html; charset=utf8"); // записываем HTML-текст
		response->end("Hello, this is <b>ChatBotServer</b> 5000"); // сам текст
		});
	app.ws<UserData>("/*", {
		// []() {} - лямбда-функция

		// открытие соединения
		.open = [&latest_user_id, &connected_users](websock ws) {
			UserData* data = ws->getUserData(); // получаем данные пользователя
			data->id = latest_user_id++;
			data->name = "UnnamedUser#" + to_string(data->id);
			all_users[data->id] = data;
			std::cout << "New user conndected: " << data->id << std::endl;
			std::cout << "Total users connected: " << ++connected_users << std::endl;
			ws->subscribe(personal_channel(data));
			ws->publish(COMMON, user_status(data, true)); // уведомление об онлайне "новичка"
			ws->subscribe(COMMON);

			for (auto item : all_users)
				ws->send(user_status(item.second, true), uWS::OpCode::TEXT);
		
		},

		// получение информации от клиента
		.message = [](websock ws, std::string_view message, uWS::OpCode opcode) {
			json parsed_data;
			try {
				parsed_data = json::parse(message); // парсинг json
			}
			catch (const json::parse_error& err) {
				std::cout << err.what() << std::endl;
				return; // до свидания)))
			}
			std::string command = parsed_data["command"];

			std::cout << "Got command: " << command << std::endl;
		
			if (command == PRIVATE_MSG)
				process_private_msg(ws, parsed_data);

			if (command == PUBLIC_MSG)
				process_public_msg(ws, parsed_data);

			if (command == SET_NAME)
				process_set_name(ws, parsed_data);

			if (command == MSG_TO_BOT)
				ws->send(process_msg_to_bot(ws, parsed_data), uWS::OpCode::TEXT);

		},

		// закрытие соединения
		.close = [&latest_user_id, &connected_users](websock ws, int, std::string_view) {
			UserData* data = ws->getUserData();
			ws->publish(COMMON, user_status(data, false), uWS::OpCode::TEXT);
			all_users.erase(data->id);
			std::cout << "User " + to_string(data->id) + " disconnected" << std::endl;
			std::cout << "Total users connected: " << --connected_users << std::endl;
		}
		});

	app.listen(9001, [](auto*) {});
	app.run();

	// ToDO: внедрить бота
}