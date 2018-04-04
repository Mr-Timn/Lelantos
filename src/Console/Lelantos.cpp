// Lelantos.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>						// cin cout 
#include <windows.h>					// shellapi?
#include <fstream>						// fstream ofstream ifstream
#include <shellapi.h>					// shellexecute
#include <conio.h>						// _getch()
#include <vector>						// vector
#include <string>						// string
#include <experimental/filesystem>		// fs
#include <shlobj.h>						// shortcuts
#include <cstdio>						// remove rename

#include "gl.h"  // includes global variables
#include "ge.h"  // handles general functions 
#include "de.h"  // handles decode functions
#include "co.h"  // handles control functions
#include "di.h"  // handles directory functions


using namespace std;
namespace fs = std::experimental::filesystem;

const vector<string> Program_Commands = { "debug", "help", "list", "shortcuts", "esc", "openfile", "adddir", "editdir", "seldir", "clearsh" };
const vector<string> Edit_Commands = { "remove", "editpass", "editshortut", "hide", "reveal", "preventaccess", "grantaccess" };
const vector<string> Selected_Commands = { "list", "content", "goto", "open", "openf", "sh", "up" };
const vector<string> Console_Commands = { "clear" };

int STATUS = 0; 
vector<string> directories, shortcuts, passwords;

int runCommand(string cmd) {
	//TODO - shorten amount variables
	string code = to_string(rand() % 9999999);
	ofstream list("Directories.txt", ios_base::app);
	ofstream paa;
	string directory, shortcut, password, cDir, cPass, pass, ndir, name, guess, npass, nsh;
	string in, sh;
	string dir;
	string lastdir, orgdir;

	if (inputIs(cmd, { "debug" }, true)) {
	DEBUG:
		cout << "Password: "; in = getInput(true);
		if (end(in)) { return 1; }
		if (in == DEBUG_PASS) {
			while (true) {
				cout << "Command: "; in = getInput(false);
				if (end(in)) { return 1; }
				if (in == "dir") { printList(directories, "directories", 0); }
				else if (in == "pass") { printList(passwords, "passwords", 0); }
				else { cout << "Not a command" << endl; }
			}
		} else {
			cout << "Wrong password."; goto DEBUG;
		}
	} 
	else if (inputIs(cmd, { "help" }, true)) {
		cout << "This function is yet to exist" << endl;
	} 
	else if (inputIs(cmd, { "list" }, true)) {
		printList(Program_Commands, "commands", 0);
	} 
	else if (inputIs(cmd, { "shortcuts", "sh" }, true)) {
		printList(shortcuts, "shortcuts", 1);
	}
	else if (inputIs(cmd, { "esc", "escape", "leave", "close" }, false)) {
		STATUS = 2;
	} 
	else if (inputIs(cmd, { "openfile", "opendir" }, true)) {
	OPENDIR: 
		cout << "What directory would you like to open? "; string sh = getInput(false); 
		if (isShortcut(&shortcuts, sh)) {
			string dir = getDirectoryFromShortcut(&shortcuts, &directories, sh);
			string pass = getPasswordFromShortcut(&shortcuts, &passwords, sh);
			TRY_PASS:
			if (pass == DEFAULT_PASS){
				openDirectory(dir);
			} else {
				cout << "Password: "; guess = getInput(false);
				if (cancel(guess)) { return 1; } 
				else if (goback(guess)) { goto OPENDIR; } 
				else if (guess == pass) { openDirectory(dir); }
				else { cout << "Wrong password." << endl; goto TRY_PASS; }
			}
		} else {
			cout << "Not a directory."; goto OPENDIR;
		}
	} 
	else if (inputIs(cmd, { "adddir" }, true)) {
	NEWDIR: 
		cout << "What is the directory that you would like to add? "; directory = getInput(false);
		cout << "Opening directory..." << endl; openDirectory(directory); 
		if (!directoryExists(directory)) { cout << "Directory failed to open" << endl; goto NEWDIR; }
	NEWSHORTCUT: 
		cout << "Choose a shortcut. "; shortcut = getInput(false); if (goback(shortcut)) { goto NEWDIR; }
		if (isShortcut(&shortcuts, shortcut)) { cout << "That shortcut is already being used." << endl; goto NEWSHORTCUT; }
	CHOOSENEWPASS:
		cout << "Add a password? "; in = getInput(false);
		if (goback(in)) { goto NEWSHORTCUT; }
		if (inputIs(in, {"y", "yes"}, false)) {
	NEWPASS:
			cout << "Password: "; 
			in = getInput(false);
			if (goback(in)) {
				goto NEWSHORTCUT;
			} else if (inputIs(cmd, {DEFAULT_PASS}, false)) {
				cout << "Invalid password." << endl;
				goto NEWPASS;
			} else {
				password = in;
			}
		} else if (inputIs(in, { "n", "no" }, true)) {
			password = DEFAULT_PASS;
		} else { 
			goto CHOOSENEWPASS;
		}

		appendToDirectoryList(shortcut, code, codeString(directory, code, true), codeString(password, code, true));
			
		cout << "Successfully added "<< directory << " to list of directories." << endl;
		reloadDirectories(&directories, &passwords, &shortcuts);
	} 
	else if (inputIs(cmd, { "editdir" }, true)) {
	EDITDIR:
		cout << "Type the directory that you would like to edit."; sh = getInput(false); 
		if (end(sh)) { return 1; }
		if (isShortcut(&shortcuts, sh)) {
			dir = getDirectoryFromShortcut(&shortcuts, &directories, sh); 
			pass = getPasswordFromShortcut(&shortcuts, &passwords, sh);
	WRONGPASS:
			if (!(pass == DEFAULT_PASS)){
				cout << "Password: "; 
				guess = getInput(false); 
				if (goback(guess)) { 
					goto EDITDIR; 
				} else if (cancel(guess)) { 
					return 1; 
				}
				if (!(guess == pass) || guess == DEFAULT_PASS) { 
					cout << "Wrong password" << endl; 
					goto WRONGPASS; 
				} 
			}
			while (true) {
				cout << "What would you like to edit. Type 'list' for options. "; in = getInput(false); 
				if (end(in)) { break; }
				if (in == "list") {
					printList(Edit_Commands, "commands", 0);
				} else if (in == "editpass") {
					cout << "Type new password: "; npass = getInput(false);
					if (goback(in)) { break; }
					nullLineInDirectoryList(getLineFromShortcut(&shortcuts, sh), &directories, &passwords, &shortcuts);
					refreshDirectoryList(&directories, &passwords, &shortcuts);
					appendToDirectoryList(sh, code, codeString(dir, code, true), codeString(npass, code, true));
					reloadDirectories(&directories, &passwords, &shortcuts);
				} else if (in == "remove") {
					int line = getLineFromShortcut(&shortcuts, sh);
					nullLineInDirectoryList(line, &directories, &passwords, &shortcuts);
					refreshDirectoryList(&directories, &passwords, &shortcuts);
					reloadDirectories(&directories, &passwords, &shortcuts);
				}
				else if (in == "editshortcut") {
					cout << "Type new shortcut: "; nsh = getInput(false);
					if (goback(nsh)) { break; }
					nullLineInDirectoryList(getLineFromShortcut(&shortcuts, sh), &directories, &passwords, &shortcuts);
					refreshDirectoryList(&directories, &passwords, &shortcuts);
					appendToDirectoryList(nsh, code, codeString(dir, code, true), codeString(pass, code, true));
					reloadDirectories(&directories, &passwords, &shortcuts);
				} else if (in == "hide") {
					changeFileProperty(dir, 0);
				} else if (in == "reveal") {
					changeFileProperty(dir, 1);
				} else if (in == "readonly") {
					changeFileProperty(dir, 2); 
				} else if (in == "preventaccess"){
					if (!directoryExists(dir + "\\.preventaccess.txt")){
						paa.open(dir+"\\.preventaccess.txt");
						paa << "true" << endl;
						changeFileProperty(dir + "\\.preventaccess.txt", 0);
						cout << "Preventing access" << endl;
					} else {
						cout << "Already preventing access" << endl;
					}
				} else if (in == "grantaccess"){
					if (!directoryExists(dir + "\\.preventaccess.txt")) {
						cout << "This directory is already accessible." << endl;
					} else {
						fs::remove_all(dir + "\\.preventaccess.txt");
						cout << "Public access granted to this directory." << endl;
					}
				} else {
					cout << in << " is not a command" << endl;
				}
			}				
		} else {
			cout << sh << " is not a directory." << endl; goto EDITDIR;
		}
	} 
	else if (inputIs(cmd, { "seldir" }, true)) {
	SELECTDIR:
		cout << "Type the directory that you would like to select. "; sh = getInput(false);
		if (end(sh)) { return 1; }
		if (isShortcut(&shortcuts, sh)) {
			dir = lastdir = orgdir = getDirectoryFromShortcut(&shortcuts, &directories, sh); 
			pass = getPasswordFromShortcut(&shortcuts, &passwords, sh);
		WRONGPASS2:
			if (!(pass == DEFAULT_PASS)) {
				cout << "Password: ";
				guess = getInput(false);
				if (goback(guess)) { 
					goto SELECTDIR; 
				}
				else if (cancel(guess)) { 
					return 1; 
				}
				if (!(guess == pass) || guess == DEFAULT_PASS) { 
					cout << "Wrong password" << endl; 
					goto WRONGPASS2; 
				}
			}
			while (true) {
				cout << "Type a command. Type 'list' for options. "; in = getInput(false);
				if (end(in)) { break; }
				if (in == "list") {
					printList(Selected_Commands, "commands", 0);
				} else if (in == "content") {
					listDirectoryContent(dir, 0);
				} else if (in == "open"){
					openDirectory(dir);
				} else if (in.substr(0,5) == "openf"){
					ndir = dir + "\\" + in.substr(6);
					if (directoryExists(ndir)) {
						openDirectory(ndir);
					} else {
						if (!(getChildDirectoryFromInt(dir,in.substr(6)) == "NULL")) {
							openDirectory(getChildDirectoryFromInt(dir, in.substr(6)));
						} else {
							cout << ndir << " does not exist" << endl;
						}
					}
				} else if (in == "up"){
					dir = lastdir;
				}
				else if (in.substr(0, 4) == "goto") {
					ndir = dir + "\\" + in.substr(5);
					if (directoryExists(ndir)) {
						dir = ndir;
					}
					else {
						if (!(getChildDirectoryFromInt(dir, in.substr(5)) == "NULL")) {
							dir = getChildDirectoryFromInt(dir, in.substr(5));
						} else {
							cout << ndir << " does not exist" << endl;
						}
					}
				} else if (in == "sh"){
				DESKTOPSH:
					cout << "Name: "; name = getInput(false);
					if (!fs::is_directory(SH_FOLDER + "\\" + name)) {
						createDesktopSHDirectory(SH_FOLDER, name, dir, pass, true);
					} else {
						if (!directoryExists(CSIDL_DESKTOPDIRECTORY + "\\" + name + ".lnk")) {
							createDesktopSHDirectory(SH_FOLDER, name, dir, pass, false);
						} else {
							cout << "This shortcut already exsists." << endl; goto DESKTOPSH;
						}
					}
				} else {
					cout << in << " is not a command" << endl;
				}
			}
		}
		else {
			cout << sh << " is not a directory." << endl; goto SELECTDIR;
		}
	} 
	else if (inputIs(cmd, { "clearsh" }, true)) {
		cout << "Are you sure you want to clear all shortcuts? ";
		if (yes_no()) {
			fs::remove_all(SH_FOLDER);
			CreateDirectory(SH_FOLDER.c_str(), NULL);
			cout << "Removed all shortcuts" << endl;
		}
	}
	else { 
		cout << "That is not a command." << endl;
	}

	return 0;
}

int main(int argc, char* argv[]) {
	/* Initialize Program */
	HWND window = GetConsoleWindow();
	string cmd;

	SetConsoleTitle("Lelantos Console");
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), { (short)CONSOLE_SIZE[0], (short)CONSOLE_SIZE[1] });
	MoveWindow(window, GetLargestConsoleWindowSize(GetStdHandle(STD_OUTPUT_HANDLE)).X/2, GetLargestConsoleWindowSize(GetStdHandle(STD_OUTPUT_HANDLE)).Y/2, CONSOLE_SIZE[0], CONSOLE_SIZE[1], FALSE);

	srand((unsigned int)time(NULL));
	printStart();

	/* First Time Use */
	if (!directoryExists("Directories.txt")) {
		ofstream newDirectoryFile;
			newDirectoryFile.open("Directories.txt");
			newDirectoryFile << "-- -- DirectoryList --" << endl;
		changeFileProperty("Directories.txt", 0); 
		changeFileProperty("Directories.txt", 3);		
	}
	if (!directoryExists(SH_FOLDER)) {
		CreateDirectory(SH_FOLDER.c_str(), NULL);
	}

	while (true) {
		/* Update Local Vectors */
		reloadDirectories(&directories, &passwords, &shortcuts);

		/* Get User Input and Run Program */
		cout << "Please type a command. Type list for commands. "; 
		cmd = getInput(false);
		runCommand(cmd);

		/* Exit Program */
		if (STATUS == 1 || STATUS == 2) { break; }
	}

	if (STATUS == 1) { system("PAUSE"); }
	return 0;
}

