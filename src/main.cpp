// C++ SDL2 Template for Visual Studio 2022

#include "../include/mainHeader.h"
#include "../include/button.h"
#include "../include/textBox.h"
#include "../include/buttonManager.h"
#include "../include/program.h"
#include "../include/studentInfo.h"

int activeButtonId;
int tableRows = 4, tableColumns = 4;
int tbSize = 60;

SDL_Window* mainWindow = NULL;
SDL_Renderer* mainRenderer = NULL;

// window's color, change please
SDL_Color windowColor = {255, 255, 255 ,255};

std::vector<std::string> students(30, "");

// A table for 2 students, left -> right
typedef struct stTable2 {
	textButton s1, s2;
	stTable2(int x0, int y0, const char* s1n, const char* s2n, TTF_Font* f, int tableSize) :
		s1(textButton(x0, y0, tableSize, tableSize, SDL_Color(177, 177, 177, 255), s1n, SDL_Color(0, 0, 0, 255), SDL_Color(122, 122, 122, 255), f, CENTER, CENTER)),
		s2(textButton(x0 + tableSize + 3, y0, tableSize, tableSize, SDL_Color(177, 177, 177, 255), s2n, SDL_Color(0, 0, 0, 255), SDL_Color(122, 122, 122, 255), f, CENTER, CENTER)) {}
} stTable2;

buttonManager studentBtns;

std::vector<std::vector<stTable2>> tables(tableColumns);
std::vector <std::vector<textBox>> labelBoxes(tableColumns);

int main(int argc, char* argv[]) {

	int tableCount = students.size() / 2;

	Program program;

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	mainWindow = program.createWindow(WINDOW_HEIGHT, WINDOW_WIDTH, 0, "Program");
	mainRenderer = program.createRenderer(mainWindow);

	SDL_SetRenderDrawBlendMode(mainRenderer, SDL_BLENDMODE_BLEND);

	TTF_Font* mainFont15 = TTF_OpenFont("./res/fonts/mssan-serif.ttf", 15);

	editableTextBox nameEnterBox(SDL_Rect(WINDOW_WIDTH - 200, WINDOW_HEIGHT - 40, 180, 30),
		SDL_Color(177, 177, 177, 188), "",
		SDL_Color(0, 0, 0, 255),
		mainFont15, LEFT, CENTER, false);

	editableTextBox saveLoadName(SDL_Rect(80 + 50 + 10, WINDOW_HEIGHT - 40, 180, 30),
		SDL_Color(177, 177, 177, 188), "",
		SDL_Color(0, 0, 0, 255),
		mainFont15, LEFT, CENTER, false);

	textBox savePromptLabel(SDL_Rect(80 + 50 + 10, WINDOW_HEIGHT - 70, 180, 30),
		SDL_Color(177, 177, 177, 0), "Name for the save:",
		SDL_Color(0, 0, 0, 255),
		mainFont15, LEFT, CENTER);

	saveLoadName.toggleVisibility(false);
	savePromptLabel.toggleVisibility(false);


	for (int i = 0; i < tableColumns; ++i) {
		for (int j = 0; j < tableRows; ++j) {
			if (j == tableColumns - 1 && i == 0) continue;
			tables[i].push_back(stTable2(tbSize * (i + 1) + 2 * tbSize * i, tbSize * (j + 1) + tbSize * j, "None", "None", mainFont15, tbSize));
		}
	}

	for (auto& rows : tables) {
		for (auto& table : rows) {
			studentBtns.add(table.s1);
			studentBtns.add(table.s2);
		}
	}
	int cntr = 1;
	for (int i = 0; i < tableColumns; ++i) {
		for (int j = 0; j < tableRows; ++j) {
			if (j == tableRows - 1 && i == 0) continue;
			labelBoxes[j].push_back(textBox(SDL_Rect(tbSize * (3 * i + 1), tbSize * 2 * j, tbSize * 2 + 10, tbSize),
				SDL_Color(177, 177, 177, 0), std::to_string(cntr),
				SDL_Color(0, 0, 0, 255),
				mainFont15, CENTER, CENTER));
			cntr++;
		}
	}

	textButton applyButton(WINDOW_WIDTH - 200 - 50 - 10, WINDOW_HEIGHT - 40,
		50, 30,
		SDL_Color(177, 177, 177, 255),
		"Apply", SDL_Color(0, 0, 0, 255),
		SDL_Color(122, 122, 122, 255),
		mainFont15,
		CENTER, CENTER);
	textButton saveConfButton(20 + 50 + 10, WINDOW_HEIGHT - 40,
		50, 30,
		SDL_Color(177, 177, 177, 255),
		"Save", SDL_Color(0, 0, 0, 255),
		SDL_Color(122, 122, 122, 255),
		mainFont15,
		CENTER, CENTER);
	textButton loadConfButton(20, WINDOW_HEIGHT - 40,
		50, 30,
		SDL_Color(177, 177, 177, 255),
		"Load", SDL_Color(0, 0, 0, 255),
		SDL_Color(122, 122, 122, 255),
		mainFont15,
		CENTER, CENTER);


	for (auto& button : studentBtns.buttons) {
		std::visit([&](auto& btn) {
			btn.setAction([&]() {
				activeButtonId = btn.id;
				applyButton.toggleActive(true);
				nameEnterBox.reset();
				nameEnterBox.editable = true;
				SDL_StartTextInput();
				std::cout << btn.id << '\n';
			});
		}, button);
	}

	applyButton.setAction([&]() {
		for (auto& button : studentBtns.buttons) {
			std::visit([&](auto& btn) {
				if constexpr (std::is_same_v<std::decay_t<decltype(btn)>, textButton>) {
					if (btn.id != activeButtonId) return;
					btn.text = nameEnterBox.text.c_str();
					btn.loadText(mainRenderer);
					nameEnterBox.reset();
					nameEnterBox.editable = false;
					applyButton.toggleActive(false);
					SDL_StopTextInput();
				}
			}, button);
		}
	});

	saveConfButton.setAction([&]() {
		for (auto& button : studentBtns.buttons) {
			std::visit([&](auto& btn) {
				if constexpr (std::is_same_v<std::decay_t<decltype(btn)>, textButton>) {
					Student stData = { btn.text, btn.id };
					saveStudentDataBinary(stData, "studentData.dat");
				}
			}, button);
		}
	});
	
	studentBtns.loadAllText(mainRenderer);
	applyButton.loadText(mainRenderer);
	saveConfButton.loadText(mainRenderer);
	loadConfButton.loadText(mainRenderer);

	applyButton.toggleActive(false);

	bool isRunning = true;

	while (isRunning) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			isRunning = program.processEvent(e);
			studentBtns.handleAllEvent(e);
			nameEnterBox.handleEvent(e);
			applyButton.handleEvents(e);
			saveConfButton.handleEvents(e);
			loadConfButton.handleEvents(e);
			saveLoadName.handleEvent(e);
		}
		
		SDL_SetRenderDrawColor(mainRenderer, windowColor.r, windowColor.g, windowColor.b, windowColor.a);
		SDL_RenderClear(mainRenderer);

		studentBtns.renderAll(mainRenderer);
		nameEnterBox.render(mainRenderer);
		applyButton.render(mainRenderer);
		saveConfButton.render(mainRenderer);
		loadConfButton.render(mainRenderer);
		saveLoadName.render(mainRenderer);
		savePromptLabel.render(mainRenderer);

		for (auto& boxes : labelBoxes) {
			for (auto& box : boxes) {
				box.render(mainRenderer);
			}
		}

		SDL_RenderPresent(mainRenderer);
	}
	program.onQuit(mainWindow, mainRenderer);

	return 0;
}