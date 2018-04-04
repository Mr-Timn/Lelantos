#pragma once

using namespace std;

/************************************************************
*	Returns int[] of the size of console
*		int[0] - Horizontal size in pixels
*		int[1] - Vertical size in pixels
*************************************************************/
int* getWindowDimensions() {
	RECT Rect; GetWindowRect(GetConsoleWindow(), &Rect);
	int x = Rect.right - Rect.left;
	int y = Rect.bottom - Rect.top;
	static int dim[] = { x, y };
	return dim;
}

void hideconsole() {
	HWND hWnd = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(hWnd, 0);
}
/************************************************************
*	End result of trying to get text centered in middle of screen
	Only look correct on console size of 675 currently
*************************************************************/
void printCentered(string text) {
	int len = 80;
	for (size_t i = 0; i < ((len-1) - text.length()) / 2; i++) {
		cout << " ";
	}
	cout << text << endl;
}
/************************************************************
*	Prints large ascii text "Lelantos" to console
*************************************************************/
void printStart() {
	printCentered("| ------------------------------------------------------ |");
	printCentered("| .____          .__                 __                  |");
	printCentered("| |    |    ____ |  | _____    _____/  |_  ____  ______  |");
	printCentered("| |    |  _/ __ \\|  | \\__  \\  /    \\   __\\/  _ \\/  ___/  |");
	printCentered("| |    |__\\  ___/|  |__/ __ \\|   |  \\  | (  <_> )___ \\   |");
	printCentered("| |_______ \\___  >____(____  /___|  /__|  \\____/____  >  |");
	printCentered("|         \\/   \\/          \\/     \\/                \\/   |");
	printCentered("| ------------------------------------------------------ |");
}
/************************************************************
*	Paramaters
*		const vector<string> list - List of strings
*		string name - No use currently, reserved for potential use.
*		int method - Determines how the console will print each item in list
*************************************************************/
void printList(const vector<string> list, string name, int method) {
	cout << endl;
	for (size_t i = 0; i < list.size(); i++) {
		switch (method) {
		case 0:
			if (i == 0) { cout << "| "; }
			cout << list[i] << " | ";
			break;
		case 1:
			cout << "\t" << list[i] << endl;
			break;
		default: cout << list[i] << " "; break;
		}
		if (i + 1 == list.size()) cout << endl << endl;
	}
}