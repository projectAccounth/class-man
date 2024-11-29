// C++ SDL2 Template for Visual Studio 2022

#include "../include/mainHeader.h"
#include "../include/button.h"
#include "../include/textBox.h"
#include "../include/buttonManager.h"
#include "../include/program.h"
#include "../include/studentInfo.h"
#include "../include/nameLocalization.h"

int activeButtonId;
int activeSLFlag = 0;

SDL_Window* mainWindow = NULL;
SDL_Renderer* mainRenderer = NULL;

// window's color, change please
SDL_Color windowColor = {255, 255, 255 ,255};

// A table for 2 students, left -> right
typedef struct stTable2 {
	textButton s1, s2;
	stTable2(int x0, int y0, const char* s1n, const char* s2n, TTF_Font* f, int tableSize) :
		s1(textButton(x0, y0, tableSize, tableSize, defaultButtonColor, s1n, defaultTextColor, hoveredButtonColor, f, CENTER, CENTER)),
		s2(textButton(x0 + tableSize + 3, y0, tableSize, tableSize, defaultButtonColor, s2n, defaultTextColor, hoveredButtonColor, f, CENTER, CENTER)) {}
} stTable2;

buttonManager studentBtns;

std::vector<std::vector<stTable2>> tables(tableColumns);
std::vector <std::vector<textBox>> labelBoxes(tableColumns);
std::vector<Student> students;

int main(int argc, char* argv[]) {

	for (int i = 0; i < studentsCount; i++) {
		students.push_back({ "None", "None", i });
	}

	int tableCount = studentsCount / 2;

	Program program;

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	mainWindow = program.createWindow(WINDOW_HEIGHT, WINDOW_WIDTH, 0, "Program");
	mainRenderer = program.createRenderer(mainWindow);

	SDL_SetRenderDrawBlendMode(mainRenderer, SDL_BLENDMODE_BLEND);

	TTF_Font* mainFont15 = TTF_OpenFont("./res/fonts/mssan-serif.ttf", 15);

	editableTextBox nameEnterBox(SDL_Rect(WINDOW_WIDTH - 200, WINDOW_HEIGHT - 40, 180, 30),
		SDL_Color(177, 177, 177, 188), "",
		defaultTextColor,
		mainFont15, LEFT, CENTER, false);

	editableTextBox saveLoadName(SDL_Rect(80 + 50 + 10, WINDOW_HEIGHT - 40, 180, 30),
		SDL_Color(177, 177, 177, 188), "",
		defaultTextColor,
		mainFont15, LEFT, CENTER, false);

	textBox savePromptLabel(SDL_Rect(80 + 50 + 10, WINDOW_HEIGHT - 70, 180, 30),
		SDL_Color(177, 177, 177, 0), "Name for the save:",
		defaultTextColor,
		mainFont15, LEFT, CENTER);

	textBox nameLabel(SDL_Rect(WINDOW_WIDTH - 200 - 50 - 10, WINDOW_HEIGHT - 70, 240, 30),
		SDL_Color(177, 177, 177, 0), "Name: None",
		defaultTextColor,
		mainFont15, LEFT, LEFT);

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
				defaultTextColor,
				mainFont15, CENTER, CENTER));
			cntr++;
		}
	}

	textButton applyButton(WINDOW_WIDTH - 200 - 50 - 10, WINDOW_HEIGHT - 40,
		50, 30,
		defaultButtonColor,
		"Apply", defaultTextColor,
		hoveredButtonColor,
		mainFont15,
		CENTER, CENTER);

	textButton saveConfButton(20 + 50 + 10, WINDOW_HEIGHT - 40,
		50, 30,
		defaultButtonColor,
		"Save", defaultTextColor,
		hoveredButtonColor,
		mainFont15,
		CENTER, CENTER);
	textButton loadConfButton(20, WINDOW_HEIGHT - 40,
		50, 30,
		defaultButtonColor,
		"Load", defaultTextColor,
		hoveredButtonColor,
		mainFont15,
		CENTER, CENTER);

	textButton cancelButton(80 + 50 + 10 + 180 + (50 + 20), WINDOW_HEIGHT - 40,
		50, 30,
		defaultButtonColor,
		"Cancel", defaultTextColor,
		hoveredButtonColor,
		mainFont15,
		CENTER, CENTER);
	textButton confirmButton(80 + 50 + 10 + 180 + 10, WINDOW_HEIGHT - 40,
		50, 30,
		defaultButtonColor,
		"Confirm", defaultTextColor,
		hoveredButtonColor,
		mainFont15,
		CENTER, CENTER);
	textButton shuffleButton(20, WINDOW_HEIGHT - 80,
		110, 30,
		defaultButtonColor,
		"Shuffle Positions", defaultTextColor,
		hoveredButtonColor,
		mainFont15,
		CENTER, CENTER);

	cancelButton.visible = false;
	confirmButton.visible = false;


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
			btn.setHoverAction([&]() {
				for (int i = 0; i < studentsCount; i++) {
					if (i != btn.id) continue;
					nameLabel.text = "Name: " + students[i].fullName;
				}
			});
		}, button);
	}

	applyButton.setAction([&]() {
		for (auto& button : studentBtns.buttons) {
			std::visit([&](auto& btn) {
				if constexpr (std::is_same_v<std::decay_t<decltype(btn)>, textButton>) {
					if (btn.id != activeButtonId) return;
					students[btn.id] = { abbreviateName(nameEnterBox.text, true), nameEnterBox.text, btn.id };
					btn.text = abbreviateName(nameEnterBox.text, true);
					std::cout << students[btn.id].name << std::endl;
					btn.loadText(mainRenderer);
					nameEnterBox.reset();
					nameEnterBox.editable = false;
					applyButton.toggleActive(false);
					std::cout << btn.text << '\n';
					SDL_StopTextInput();
				}
			}, button);
		}
	});

	saveConfButton.setAction([&]() {
		if (nameEnterBox.editable)
			return;

		activeSLFlag = 1;

		confirmButton.toggleVisiblility(true);
		cancelButton.toggleVisiblility(true);
		saveLoadName.toggleVisibility(true);
		savePromptLabel.toggleVisibility(true);
		for (auto& button : studentBtns.buttons) {
			std::visit([&](auto& btn) {
				btn.active = false;
			}, button);
		}
		saveConfButton.active = false;
		loadConfButton.active = false;

		saveLoadName.reset();
		saveLoadName.editable = true;

		SDL_StartTextInput();
	});

	loadConfButton.setAction([&]() {
		if (nameEnterBox.editable)
			return;

		activeSLFlag = 2;

		confirmButton.toggleVisiblility(true);
		cancelButton.toggleVisiblility(true);
		saveLoadName.toggleVisibility(true);
		savePromptLabel.toggleVisibility(true);
		for (auto& button : studentBtns.buttons) {
			std::visit([&](auto& btn) {
				btn.active = false;
			}, button);
		}
		saveConfButton.active = false;
		loadConfButton.active = false;

		saveLoadName.reset();
		saveLoadName.editable = true;

		SDL_StartTextInput();
	});

	cancelButton.setAction([&]() {
		activeSLFlag = 0;
		confirmButton.toggleVisiblility(false);
		cancelButton.toggleVisiblility(false);
		saveLoadName.toggleVisibility(false);
		savePromptLabel.toggleVisibility(false);
		for (auto& button : studentBtns.buttons) {
			std::visit([&](auto& btn) {
				btn.active = true;
			}, button);
		}
		saveConfButton.active = true;
		loadConfButton.active = true;

		saveLoadName.reset();
		saveLoadName.editable = false;

		SDL_StopTextInput();
	});

	confirmButton.setAction([&]() {
		confirmButton.toggleVisiblility(false);
		cancelButton.toggleVisiblility(false);
		saveLoadName.toggleVisibility(false);
		savePromptLabel.toggleVisibility(false);
		for (auto& button : studentBtns.buttons) {
			std::visit([&](auto& btn) {
				btn.active = true;
			}, button);
		}
		switch (activeSLFlag) {
		// For saving operation
		case 1:
			saveStudentDataBinary(students, saveLoadName.text);
			break;

		// For loading operation
		case 2:
			readStudentDataBinary(saveLoadName.text, studentBtns, mainRenderer, students);
			break;
		}
		activeSLFlag = 0;
		saveConfButton.active = true;
		loadConfButton.active = true;

		saveLoadName.reset();
		saveLoadName.editable = false;

		SDL_StopTextInput();
	});

	shuffleButton.setAction([&]() {
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(students.begin(), students.end(), g);

		for (auto& button : studentBtns.buttons) {
			std::visit([&](auto& btn) {
				if constexpr (std::is_same_v<std::decay_t<decltype(btn)>, textButton>) {
					for (size_t i = 0; i < students.size(); ++i) {
						if (students[i].number == btn.id) {
							btn.text = students[i].name;
							students[i].number = btn.id;
							btn.loadText(mainRenderer);
							continue;
						}
					}
				}
			}, button);
		}
	});
	
	studentBtns.loadAllText(mainRenderer);
	applyButton.loadText(mainRenderer);
	saveConfButton.loadText(mainRenderer);
	loadConfButton.loadText(mainRenderer);
	confirmButton.loadText(mainRenderer);
	cancelButton.loadText(mainRenderer);
	shuffleButton.loadText(mainRenderer);

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
			confirmButton.handleEvents(e);
			cancelButton.handleEvents(e);
			shuffleButton.handleEvents(e);

			bool chkHvr = false;
			int chkInd = 0;
			for (auto& button : studentBtns.buttons) {
				std::visit([&](auto& btn) {
					if (btn.hovered) { chkHvr = true;  return; }
					chkInd++;
					if (chkInd == studentsCount)
						nameLabel.text = "Name: None";
				}, button);
				if (chkHvr) break;
			}
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
		confirmButton.render(mainRenderer);
		cancelButton.render(mainRenderer);
		shuffleButton.render(mainRenderer);
		nameLabel.render(mainRenderer);

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