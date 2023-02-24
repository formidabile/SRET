#include "functions.h"
#include "protection.h"

std::map<int, UserData*> au;
const int cipher1[] = { 229, 165, 486, 486,
									422, 229, 229, 426,
									153, 438, 253, 29,
									153, 125, 29, 269,
									174, 29, 479, 153,
									253, 494, 382, 153,
									366, 29, 29, 479,
									153, 174, 29, 479,
									29, 109, 494, 45,
									29, 302, 411 };
const int cipher2[] = { 422, 502, 502, 53,
						502, 426, 153, 116,
						206, 318, 153, 253,
						494, 190, 29, 153,
						29, 479, 45, 29,
						109, 29, 302, 153,
						45, 253, 29, 153,
						463, 109, 206, 479,
						174, 153, 510, 494,
						382, 382, 463, 206,
						109, 302, 411, 411, 411 };
const int cipher3[] = { 310, 341, 502, 326,
						373, 326, 21, 426,
						153, 149, 29, 366,
						318, 174, 174, 29,
						109, 153, 253, 494,
						382, 153, 366, 29,
						29, 479, 153, 302,
						29, 45, 29, 93,
						45, 29, 302, 411, 411, 411 };
const int cipher4[] = { 310, 341, 502, 326,
						373, 326, 21, 426,
						153, 37, 526, 109,
						45, 318, 494, 334,
						153, 454, 494, 93,
						253, 526, 479, 29,
						153, 253, 494, 382,
						153, 366, 29, 29,
						479, 153, 302, 29,
						45, 29, 93, 45,
						29, 302, 411, 411, 411 };
const int cipher5[] = { 422, 502, 502, 53,
							502, 426, 153, 438,
							253, 29, 153, 93,
							206, 302, 29, 153,
							526, 479, 45, 29,
							174, 109, 526, 45,
							269, 153, 526, 382,
							153, 366, 109, 206,
							125, 29, 479, 411, 411, 411 };
const int cipher6[] = { 357, 502, 373, 37, 341, 438,
								422, 342, 454, 229, 21 };
const int cipher7[] = { 357, 165, 213, 181, 373, 
								486, 342, 454, 229, 21 };
const int cipher8[] = { 229, 438, 341, 
								438, 165, 229 };
const int cipher9[] = { 229, 422, 438, 342, 
						326, 341, 454, 422 };
const int cipher10[] = { 454, 229, 21, 342, 438, 
							53, 342, 213, 53, 438 };
const int cipher11[] = { 486, 53, 454, 
								454, 53, 326 };
std::string MSG1 = protection12(cipher1, 546, 17);
std::string MSG2 = protection12(cipher2, 546, 17);
std::string MSG3 = protection12(cipher3, 546, 17);
std::string MSG4 = protection12(cipher4, 546, 17);
std::string MSG5 = protection12(cipher5, 546, 17);
std::string MSG6 = protection12(cipher6, 546, 17);
std::string MSG7 = protection12(cipher7, 546, 17);
std::string MSG8 = protection12(cipher8, 546, 17);
std::string MSG9 = protection12(cipher9, 546, 17);
std::string MSG10 = protection12(cipher10, 546, 17);
std::string MSG11 = protection12(cipher11, 546, 17);

int main()
{
	int luid = 10;
	unsigned cu = 0;
	//protection3();protection4();protection5();protection6();
	protection7();protection8();
	if (!protection13())
	{
		std::cout << MSG5;
		exit(-1);
	}
	__asm {
		xor eax, eax
		jz label
		__emit 0xe9

	label:
		mov ebx, eax
	}
	__asm {
		push 0x74; JMP >> JZ
		push offset here
		call protection14
		add esp, 8
	here: JMP short here
	}

	std::string up;
	std::cout << "Enter the password: ";
	std::cin >> up;
	protection1(up);

	uWS::App app = uWS::App();
	app.get("/", [](auto* response, auto* request) {
		response->writeHeader("Content-Type", "text/html; charset=utf8"); // записываем HTML-текст
		response->end("Hello, this is <b>ChatBotServer</b> 5000"); // сам текст
		});
	app.ws<UserData>("/*", {
		// []() {} - лямбда-функция

		// открытие соединения
		.open = [&luid, &cu](websock ws) {
			
			UserData* data = ws->getUserData(); // получаем данные пользователя
			data->id = luid++;
			data->name = "UnnamedUser#" + std::to_string(data->id); 
			
			// фиксируем подключившегося пользователя
			au[data->id] = data;
			std::cout << "New user conndected: " << data->id << std::endl;
			std::cout << "Total users connected: " << ++cu << std::endl;
			ws->subscribe(func2(data));
			ws->publish(MSG11, func5(data, true)); // уведомление об онлайне "новичка"
			ws->subscribe(MSG11);

			for (auto item : au)
				ws->send(func5(item.second, true), uWS::OpCode::TEXT);

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
			std::string c = parsed_data["command"];

			std::cout << "Got command: " << c << std::endl;

			// обработка команды

			if (c == MSG6)
				func3(ws, parsed_data);

			if (c == MSG7)
				func4(ws, parsed_data);

			if (c == MSG9)
				func6(ws, parsed_data);

			if (c == MSG10)
				ws->send(func7(ws, parsed_data), uWS::OpCode::TEXT);

		},

			// закрытие соединения
			.close = [&luid, &cu](websock ws, int, std::string_view) {
				
				UserData* data = ws->getUserData();
				ws->publish(MSG11, func5(data, false), uWS::OpCode::TEXT); // уведомление об оффлайне
				au.erase(data->id); // удаление данных об отключившемся пользователе
				std::cout << "User " + std::to_string(data->id) + " disconnected" << std::endl;
				std::cout << "Total users connected: " << --cu << std::endl;

			}
		});

	app.listen(9001, [](auto*) {});
	app.run();
}