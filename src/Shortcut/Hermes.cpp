// Hermes.cpp : Defines the entry point for the console application.
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

int main(int argc, char* argv[]) {
	string c, d, p, dir, pass, guess;
	if (!directoryExists("info.txt")) {
		return 1;
	}
	ifstream file("info.txt");

	file >> c >> d >> p;
	dir = codeString(d, c, false);
	pass = codeString(p, c, false);

	if (pass == DEFAULT_PASS) {
		openDirectory(dir);
	}
	else {
		cout << "Password: "; guess = getInput(true);
		if (guess == pass) {
			openDirectory(dir);
		}
	}
	return 0;
}
