// C++ SDL2 Template for Visual Studio 2022

#include "../include/mainHeader.h"
#include "../include/program.h"
#include "../include/studentInfo.h"
#include "../include/nameLocalization.h"

int activeButtonId = -1;
int activeSLFlag = 0;

SDL_Window* mainWindow = NULL;
SDL_Renderer* mainRenderer = NULL;

static std::random_device rd;
static std::mt19937 g(rd());

// window's color, change please
SDL_Color windowColor = {255, 255, 255 ,255};

// A table for 2 students, left -> right
typedef struct stTable2 {
	TextButton s1, s2;
	stTable2(double x0, double y0, double tableSize, TTF_Font* f, SDL_Renderer*& renderer, const char* s1n, const char* s2n) :
		s1(TextButton(
			{ tableSize, tableSize, false },
			{ x0, y0, false },
			std::nullopt,
			renderer,
			defaultButtonColor,
			hoveredButtonColor,
			defaultTextColor,
			s1n, f
		)),
		s2(TextButton(
			{ tableSize, tableSize, false },
			{ x0 + tableSize + 3, y0, false },
			std::nullopt,
			renderer,
			defaultButtonColor,
			hoveredButtonColor,
			defaultTextColor,
			s2n, f
		)) {}
} stTable2;

ButtonManager studentBtns;

std::vector<std::vector<stTable2>> tables(tableColumns);
std::vector <std::vector<TextBox>> labelBoxes(tableColumns);
std::vector<Student> students;
auto helpText = R"(
To load/save the file with the specified name:

Saving: the output file is put in the current working directory of the program.

Loading the file requires the file to be put in the same directory as the program executable.

With file names, if you only type the name only, the file extension will be automatically specified as "txt". If you want extensions other than .txt, you can choose to save the file with ANY extension, but to load them, you will be required to enter the name a
nd the EXACT extension of the file.
)";

void setButtonAction(
	TextButton& btn,
	TextButton& applyButton,
	EditableTextBox& nameEnterBox,
	TextBox& nameLabel
) {
	btn.setAction([&]() {
		for (auto& b : studentBtns.buttons) {
			std::get<TextButton>(b).changeButtonColor(defaultButtonColor, mainRenderer);
			std::get<TextButton>(b).changeHoverColor(hoveredButtonColor, mainRenderer);
		}
		activeButtonId = btn.getId();
		applyButton.toggleActive(true);
		nameEnterBox.reset();
		nameEnterBox.editable = true;
		SDL_StartTextInput();
		btn.changeButtonColor({ 126, 244, 126, 255 }, mainRenderer);
		btn.changeHoverColor({ 110, 222, 110, 255 }, mainRenderer);
		});
	btn.setHoverAction([&]() {
		for (int i = 0; i < studentsCount; i++) {
			if (i != btn.getId()) continue;
			nameLabel.text = "Name: " + students[i].fullName;
		}
	});
}

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

	bool helpWindowPresent = false;
	SDL_Window* helpWindow = nullptr;
	SDL_Renderer* helpWindowRenderer = nullptr;

	SDL_SetRenderDrawBlendMode(mainRenderer, SDL_BLENDMODE_BLEND);

	TTF_Font* mainFont15 = TTF_OpenFont("./res/fonts/mssan-serif.ttf", 15);

	EditableTextBox nameEnterBox(
		{ 180, 30, false },
		{ WINDOW_WIDTH - 200, WINDOW_HEIGHT - 40, false },
		std::nullopt,
		mainRenderer,
		{ 177, 177, 177, 188 },
		defaultTextColor,
		mainFont15,
		LEFT
	);

	EditableTextBox saveLoadName(
		{ 180, 30, false },
		{ 145, WINDOW_HEIGHT - 40 },
		std::nullopt,
		mainRenderer,
		SDL_Color{ 177, 177, 177, 188 },
		defaultTextColor,
		mainFont15,
		LEFT
	);

	TextBox savePromptLabel(
		{ 180.0, 30.0, false },
		{ 140.0, WINDOW_HEIGHT - 70.0, false },
		std::nullopt,
		mainRenderer,
		{ 177, 177, 177, 0 },
		"Name for the save:",
		defaultTextColor,
		mainFont15,
		LEFT, CENTER
	);

	TextBox nameLabel(
		{ 240, 30, false },
		{ WINDOW_WIDTH - 260, WINDOW_HEIGHT - 70, false },
		std::nullopt,
		mainRenderer,
		{ 177, 177, 177, 0 },
		"Name: None",
		defaultTextColor,
		mainFont15,
		LEFT, LEFT
	);

	/*TextBox helpBox(
		{ 0, 0, 500, 200 },
		SDL_Color{ 255, 255, 255, 255 },
		helpText,
		defaultTextColor,
		mainFont15,
		LEFT, LEFT
	);*/

	saveLoadName.visible = false;
	savePromptLabel.visible = false;


	for (int i = 0; i < tableColumns; ++i) {
		for (int j = 0; j < tableRows; ++j) {
			if (j == tableColumns - 1 && i == 0) continue;
			tables[i].push_back(stTable2(
				tbSize * (i + 1) + 2 * tbSize * i, tbSize * (j + 1) + tbSize * j,
				tbSize,
				mainFont15,
				mainRenderer,
				"None", "None"
			));
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
			labelBoxes[j].push_back(
				TextBox(
					{ tbSize * 2 + 10.0, tbSize, false },
					{ tbSize * (3.0 * i + 1.0), static_cast<double>(tbSize * 2 * j), false },
					std::nullopt,
					mainRenderer,
					{ 177, 177, 177, 0 },
					std::to_string(cntr),
					defaultTextColor,
					mainFont15
				));
			cntr++;
		}
	}

	TextButton applyButton(
		{ 50, 30, false },
		{ WINDOW_WIDTH - 260, WINDOW_HEIGHT - 40, false },
		std::nullopt,
		mainRenderer,
		defaultButtonColor,
		hoveredButtonColor,
		defaultTextColor,
		"Apply",
		mainFont15
	);

	TextButton saveConfButton(
		{ 50, 30, false },
		{ 80, WINDOW_HEIGHT - 40, false },
		std::nullopt,
		mainRenderer,
		defaultButtonColor,
		hoveredButtonColor,
		defaultTextColor,
		"Save", 
		mainFont15
	);
	TextButton loadConfButton(
		{ 50, 30, false },
		{ 20, WINDOW_HEIGHT - 40, false },
		std::nullopt,
		mainRenderer,
		defaultButtonColor,
		hoveredButtonColor,
		defaultTextColor,
		"Load",
		mainFont15
	);

	TextButton cancelButton(
		{ 50, 30, false },
		{ 390, WINDOW_HEIGHT - 40, false },
		std::nullopt,
		mainRenderer,
		defaultButtonColor,
		hoveredButtonColor,
		defaultTextColor,
		"Cancel", 
		mainFont15
	);
	TextButton confirmButton(
		{ 50, 30, false },
		{ 330, WINDOW_HEIGHT - 40, false },
		std::nullopt,
		mainRenderer,
		defaultButtonColor,
		hoveredButtonColor,
		defaultTextColor,
		"Confirm", 
		mainFont15
	);
	TextButton shuffleButton(
		{ 110, 30, false },
		{ 20, WINDOW_HEIGHT - 80, false },
		std::nullopt,
		mainRenderer,
		defaultButtonColor,
		hoveredButtonColor,
		defaultTextColor,
		"Shuffle Positions", 
		mainFont15
	);

	TextButton helpButton(
		{ 20, 20, false },
		{ 270, WINDOW_HEIGHT - 70, false },
		std::nullopt,
		mainRenderer,
		defaultButtonColor,
		hoveredButtonColor,
		defaultTextColor,
		"?",
		mainFont15
	);

	cancelButton.visible = false;
	confirmButton.visible = false;
	helpButton.visible = false;

	for (auto& button : studentBtns.buttons) {
		auto& btn = std::get<TextButton>(button);
		setButtonAction(btn, applyButton, nameEnterBox, nameLabel);
	}

	applyButton.setAction([&]() {
		for (auto& button : studentBtns.buttons) {
			auto& btn = std::get<TextButton>(button);

			if (btn.getId() != activeButtonId) continue;

			students[btn.getId()] = {
				abbreviateName(nameEnterBox.getText(), true),
				nameEnterBox.getText(),
				btn.getId()
			};

			btn.text = abbreviateName(nameEnterBox.getText(), true);

			btn.changeButtonColor(defaultButtonColor, mainRenderer);
			btn.changeHoverColor(hoveredButtonColor, mainRenderer);
			btn.loadText(mainRenderer);

			nameEnterBox.reset();
			nameEnterBox.editable = false;
			activeButtonId = -1;

			applyButton.active = false;

			SDL_StopTextInput();
		}
		});

	saveConfButton.setAction([&]() {
		if (nameEnterBox.editable)
			return;

		activeSLFlag = 1;

		confirmButton.visible = true;
		cancelButton.visible = true;
		saveLoadName.visible = true;
		savePromptLabel.visible = true;
		for (auto& button : studentBtns.buttons) {
			std::get<TextButton>(button).active = false;
		}
		saveConfButton.active = false;
		loadConfButton.active = false;
		helpButton.visible = true;

		saveLoadName.reset();
		saveLoadName.editable = true;

		SDL_StartTextInput();
	});

	loadConfButton.setAction([&]() {
		if (nameEnterBox.editable)
			return;

		activeSLFlag = 2;

		confirmButton.visible = true;
		cancelButton.visible = true;
		saveLoadName.visible = true;
		savePromptLabel.visible = true;
		for (auto& button : studentBtns.buttons) {
			std::get<TextButton>(button).active = false;
		}
		saveConfButton.active = false;
		loadConfButton.active = false;
		helpButton.visible = true;

		saveLoadName.reset();
		saveLoadName.editable = true;

		SDL_StartTextInput();
		});

	cancelButton.setAction([&]() {
		activeSLFlag = 0;
		confirmButton.visible = false;
		cancelButton.visible = false;
		saveLoadName.visible = false;
		savePromptLabel.visible = false;
		for (auto& button : studentBtns.buttons) {
			auto& btn = std::get<TextButton>(button);
			btn.active = true;
			btn.changeButtonColor(defaultButtonColor, mainRenderer);
			btn.changeHoverColor(hoveredButtonColor, mainRenderer);
		}
		saveConfButton.active = true;
		loadConfButton.active = true;
		helpButton.visible = false;

		saveLoadName.reset();
		saveLoadName.editable = false;

		SDL_StopTextInput();
		});

	confirmButton.setAction([&]() {
		confirmButton.visible = false;
		cancelButton.visible = false;
		saveLoadName.visible = false;
		savePromptLabel.visible = false;
		for (auto& button : studentBtns.buttons) {
			std::get<TextButton>(button).active = true;
		}
		switch (activeSLFlag) {
			// For saving operation
		case 1:
			saveStudentDataBinary(students, saveLoadName.getText());
			break;

			// For loading operation
		case 2:
			readStudentDataBinary(saveLoadName.getText(), studentBtns, mainRenderer, students);
			break;
		}
		activeSLFlag = 0;
		saveConfButton.active = true;
		loadConfButton.active = true;
		helpButton.visible = false;

		saveLoadName.reset();
		saveLoadName.editable = false;

		SDL_StopTextInput();
		});

	auto shuffleStudentPositions = [&](std::vector<Student>& students) {
		for (std::size_t i = 0; i < students.size(); i++) {
			std::uniform_int_distribution<std::size_t> dist(i, students.size() - 1);
			std::size_t j = dist(g);
			std::swap(students[i].number, students[j].number);
		}
	};

	shuffleButton.setAction([&]() {
		shuffleStudentPositions(students);

		for (auto& button : studentBtns.buttons) {
			auto& btn = std::get<TextButton>(button);
			for (size_t i = 0; i < students.size(); ++i) {
				if (students[i].number != btn.getId()) continue;

				btn.text = students[i].name;
				students[i].number = btn.getId();

				btn.loadText(mainRenderer);
				break;
			}
		}
	});

	helpButton.setAction([&]() {
		if (!helpWindowPresent) {
			helpWindowPresent = true;
			helpWindow = program.createWindow(200, 500, false, "Help");
			helpWindowRenderer = program.createRenderer(helpWindow);
		}
		else {
			SDL_DestroyWindow(helpWindow);
			SDL_DestroyRenderer(helpWindowRenderer);
			helpWindow = nullptr;
			helpWindowRenderer = nullptr;
			helpWindowPresent = false;
		}
	});

	applyButton.toggleActive(false);

	// studentBtns.initializeAll(mainRenderer);
	for (auto& button : studentBtns.buttons) {
		std::get<TextButton>(button).loadText(mainRenderer);
	}

	bool isRunning = true;

	while (isRunning) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_WINDOWEVENT: {
				if (e.window.event != SDL_WINDOWEVENT_CLOSE) break;
				if (e.window.windowID == SDL_GetWindowID(mainWindow))
					isRunning = false;
				else if (e.window.windowID == SDL_GetWindowID(helpWindow)) {
					SDL_DestroyWindow(helpWindow);
					SDL_DestroyRenderer(helpWindowRenderer);
					helpWindow = nullptr;
					helpWindowRenderer = nullptr;
					helpWindowPresent = false;
				}
				break;
			}
			case SDL_QUIT: {
				isRunning = false;
				break;
			}
			}
			studentBtns.handleAllEvent(e);
			applyButton.handleEvents(e);
			nameEnterBox.handleEvents(e);
			saveConfButton.handleEvents(e);
			loadConfButton.handleEvents(e);
			saveLoadName.handleEvents(e);
			confirmButton.handleEvents(e);
			cancelButton.handleEvents(e);
			shuffleButton.handleEvents(e);
			helpButton.handleEvents(e);

			bool chkHvr = false;
			int chkInd = 0;
			for (auto& button : studentBtns.buttons) {
				std::visit([&](auto& btn) {
					if (btn.isHovered()) { chkHvr = true;  return; }
					chkInd++;
					if (chkInd == studentsCount)
						nameLabel.text = "Name: None";
				}, button);
				if (chkHvr) break;
			}
		}

		SDL_SetRenderDrawColor(mainRenderer, windowColor.r, windowColor.g, windowColor.b, windowColor.a);
		SDL_RenderClear(mainRenderer);

		studentBtns.renderAll();
		nameEnterBox.render();
		applyButton.render();
		saveConfButton.render();
		loadConfButton.render();
		saveLoadName.render();
		savePromptLabel.render();
		confirmButton.render();
		cancelButton.render();
		shuffleButton.render();
		nameLabel.render();
		helpButton.render();
		if (helpWindowRenderer) {
			SDL_SetRenderDrawColor(helpWindowRenderer, windowColor.r, windowColor.g, windowColor.b, windowColor.a);
			SDL_RenderClear(helpWindowRenderer);
			// helpBox.render();
			SDL_RenderPresent(helpWindowRenderer);
		}


		for (auto& boxes : labelBoxes) {
			for (auto& box : boxes) {
				box.render();
			}
		}

		SDL_RenderPresent(mainRenderer);
	}
	if (helpWindow) SDL_DestroyWindow(helpWindow);
	if (helpWindowRenderer) SDL_DestroyRenderer(helpWindowRenderer);
	program.onQuit(mainWindow, mainRenderer);

	return 0;
}