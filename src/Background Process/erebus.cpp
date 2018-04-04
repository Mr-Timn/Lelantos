// lelantosbp.cpp : Defines the entry point for the console application.

/*
	Windows Explorer - CabinetWClass
*/

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

#include <thread>
#include <chrono>

#include "gl.h"  // includes global variables
#include "ge.h"  // handles general functions 
#include "de.h"  // handles decode functions
#include "co.h"  // handles control functions
#include "di.h"  // handles directory functions

using namespace std;


vector <string> dir, fold, opened, pass, sh;

int main(int argc, char* argv[]) {
	//hideconsole();
	
	while (true) {
		if (!directoryExists("Directories.txt")) { Sleep(75);  continue; }

		reloadDirectories(&dir, &pass, &sh);
		for (size_t i = 0; i < dir.size(); i++) {
			string folder = getDirectoryNameFromPath(dir[i]);
			HWND find = FindWindow("CabinetWClass", folder.c_str());
			char name[256]; GetWindowText(find, name, sizeof(name));

			bool isProtected = directoryExists(dir[i] + "\\.preventaccess.txt");

			if (find && isProtected) {
				ifstream p(dir[i] + "\\.preventaccess.txt"); string b;
				p >> b;
				if (b == "true") {
					cout << "Found " << name << endl;
					closeWindow(find);
				}
			} else if (find == NULL && isProtected) {
				ifstream p(dir[i] + "\\.preventaccess.txt"); string b;
				p >> b;
				if (b == "false") {
					cout << "Changing to true" << endl;
					fstream paa(dir[i] + "\\.preventaccess.txt");
					paa << "true" << endl;
				}
			}
		}
		Sleep(75);
	}

    return 0;
}

