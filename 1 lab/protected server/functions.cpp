#include "functions.h"

bool flag_exit = false;
extern std::map<int, UserData*> au;
extern std::string MSG6;
extern std::string MSG7;
extern std::string MSG8;
extern std::string MSG9;
extern std::string MSG10;
extern std::string MSG11;

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
std::string func1(std::string question)
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

std::string func2(UserData* data)
{
	return "USER_" + std::to_string(data->id);
}

void func3(websock ws, json parsed)
{
	UserData* data = ws->getUserData();

	json payload;
	payload["command"] = MSG6;
	payload["user_from"] = data->id;
	payload["text"] = parsed["text"];

	UserData* rec = au[parsed["user_to"]];

	ws->publish(func2(rec), payload.dump());
}

void func4(websock ws, json parsed)
{
	UserData* data = ws->getUserData();

	json payload;
	payload["command"] = MSG7;
	payload["text"] = parsed["text"];
	payload["user_from"] = data->id;

	ws->publish(MSG11, payload.dump());
}

std::string func5(UserData* data, bool status)
{

	json payload;
	payload["command"] = MSG8;
	payload["online"] = status;
	payload["user_id"] = data->id;
	payload["name"] = data->name;

	return payload.dump();

}

void func6(websock ws, json parsed)
{
	UserData* data = ws->getUserData();
	data->name = parsed["name"]; // обновляем имя
	ws->publish(MSG11, func5(data, true)); // сообщаем об этом
}

std::string func7(websock ws, json parsed)
{
	UserData* data = ws->getUserData();

	json payload;
	payload["command"] = MSG10;
	payload["text"] = parsed["text"];
	payload["user_from"] = data->id;

	ws->publish(MSG11, payload.dump());

	json receive;
	receive["command"] = MSG10;
	receive["text"] = func1(parsed["text"]);
	receive["user_to"] = data->id;
	receive["name"] = "ChatBotServer5000";


	return receive.dump();
}