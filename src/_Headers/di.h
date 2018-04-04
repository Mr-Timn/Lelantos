#pragma once

namespace fs = std::experimental::filesystem;

/************************************************************
*	Returns true if directory exists on local computer
*************************************************************/
extern bool directoryExists(string dir) {
	struct stat buffer;
	return (stat(dir.c_str(), &buffer) == 0);
}
/************************************************************
*	Updates directories, passwords, and shortcuts from local directory list
*************************************************************/
extern void reloadDirectories(vector<string> *directories, vector<string> *passwords, vector<string> *shortcuts) {
	ifstream list("Directories.txt");
	directories->clear(); shortcuts->clear(); passwords->clear();
	string s, c, d, p;
	while (list >> s >> c >> d >> p) {
		if (s == "--") { continue; }
		shortcuts->push_back(s);
		passwords->push_back(codeString(p, c, false));
		directories->push_back(codeString(d, c, false));
	}
}
/************************************************************
*	Returns directory corresponding to given shortcut
*************************************************************/
extern string getDirectoryFromShortcut(vector<string> *shortcuts, vector<string> *directories, string str) {
	for (size_t i = 0; i < shortcuts->size(); i++) {
		if (shortcuts->at(i) == str) {
			return directories->at(i);
		}
	}
	return NULL;
}
/************************************************************
*	Returns password corresponding to given shortcut
*************************************************************/
extern string getPasswordFromShortcut(vector<string> *shortcuts, vector<string> *passwords, string str) {
	for (size_t i = 0; i < shortcuts->size(); i++) {
		if (shortcuts->at(i) == str) {
			return passwords->at(i);
		}
	}
	return NULL;
}
/************************************************************
*	Returns true if shortcut exists in local directory list
*************************************************************/
extern bool isShortcut(vector<string> *shortcuts, string str) {
	for (size_t i = 0; i < shortcuts->size(); i++) {
		if (shortcuts->at(i) == str) {
			return true;
		}
	}
	return false;
}
/************************************************************
*	Returns line where shortcut exists in local directory list
*************************************************************/
extern int getLineFromShortcut(vector<string> *shortcuts, string str) {
	for (size_t i = 0; i < shortcuts->size(); i++) {
		if (shortcuts->at(i) == str) {
			return i;
		}
	}
	return NULL;
}
/************************************************************
*	Parameters
*		string dir - Parent directory
*		string c - Child number in directory
*	Returns child from given directory
*************************************************************/
extern string getChildDirectoryFromInt(string dir, string c) {
	int i = 0;
	for (auto & p : fs::directory_iterator(dir)) {
		if (to_string(i++) == c) {
			fs::path path = p;
			string ndir = path.u8string();
			return ndir;
		}
	}
	return "NULL";
}
/************************************************************
*	
*************************************************************/
extern string getDirectoryNameFromPath(string dir) {
	for (int i = dir.length(); i > 0; i--) {
		if (dir[i] == '\\') {
			return dir.substr(i + 1, dir.length());
		}
	}
	return "";
}
/************************************************************
*	Appeneds new directory to local directory list
*	Parameters
*		string sh - shortcut
*		string code - number to code and decode directory and password
*		string dir - directory
*		string pass - directory password
*************************************************************/
extern void appendToDirectoryList(string sh, string code, string dir, string pass) {
	ofstream list; list.open("Directories.txt", ios_base::app);
	list << sh << " " << code << " " << dir << " " << pass << endl;
	list.close();
}
/************************************************************
*	Updates local directory list from program directory, password, and shortcut vectors
*************************************************************/
extern void refreshDirectoryList(vector<string> *directories, vector<string> *passwords, vector<string> *shortcuts) {
	remove("Directories.txt");
	fstream list; list.open("Directories.txt", ios_base::out | ios_base::trunc);
	list << "-- -- DirectoryList --" << endl;
	for (size_t i = 0; i < directories->size(); i++) {
		if (inputIs(directories->at(i), {"", " ", "-",}, false)) { continue; }
		string code = to_string(rand() % 999999 + 100);
		list << shortcuts->at(i) << " " << code << " " << codeString(directories->at(i), code, true) << " " << codeString(passwords->at(i), code, true) << endl;
	}
	list.close();
}
/************************************************************
*	Removes line in local directory list 
*************************************************************/
extern void nullLineInDirectoryList(int line, vector<string> *directories, vector<string> *passwords, vector<string> *shortcuts) {
	for (size_t i = 0; i < directories->size(); i++) {
		if (i == line) {
			directories->at(i) = "";
			shortcuts->at(i) = "";
			passwords->at(i) = "";
		}
	}
}
/************************************************************
*	Opens directory 
*		if directory does not allow universal access it temoparily enables it to be opened
*************************************************************/
extern void openDirectory(string dir) {
	if (directoryExists(dir + "\\.preventaccess.txt")) {
		cout << "Initial access prevented. Opening..." << endl;

		fstream paa(dir + "\\.preventaccess.txt");
		paa << "opening" << endl;
		ShellExecute(NULL, NULL, dir.c_str(), NULL, NULL, SW_RESTORE);
		Sleep(3000);
		fstream pab(dir + "\\.preventaccess.txt");
		pab << "false" << endl;
	}
	else {
		ShellExecute(NULL, NULL, dir.c_str(), NULL, NULL, SW_RESTORE);
	}
}
/************************************************************
*	Closes given window
*************************************************************/
extern void closeWindow(HWND win) {
	PostMessage(win, WM_CLOSE, 0, 0);
}
/************************************************************
*	Lists all content in a given directory
*************************************************************/
extern void listDirectoryContent(string dir, int method) {
	int i = 0;
	for (auto & p : fs::directory_iterator(dir)) {
		switch (method) {
		case 0: cout << i++ << ".) " << p << endl; break;
		default: cout << p << endl; break;
		}
	}
}
/************************************************************
*	Changes File Property
*	Parameters 
*		string dir - Directory to change
*		int attr - Which property to change
*			0 - set hidden
*			1 - set unhidden
*			2 - set readonly
*************************************************************/
extern void changeFileProperty(string dir, int attr) {
	switch (attr) {
	case 0:
		SetFileAttributes(dir.c_str(), GetFileAttributes(dir.c_str()) | FILE_ATTRIBUTE_HIDDEN);
		cout << "Changed file attribute to hidden." << endl;
		break;
	case 1:
		SetFileAttributes(dir.c_str(), GetFileAttributes(dir.c_str()) & ~FILE_ATTRIBUTE_HIDDEN);
		cout << "Changed file attribute to normal" << endl;
		break;
	case 2:
		SetFileAttributes(dir.c_str(), GetFileAttributes(dir.c_str()) | FILE_ATTRIBUTE_READONLY);
		cout << "Changed file attribute to readonly" << endl;
		break;
	default: /*cout << "Error: Incorrect File Property.";*/ break;
	}
}
/************************************************************
*	Creates shortcut link on desktop
*	Parameters
*		LPCSTR lpszPathObj - Application to create shortcut to
*		LPCSTR lpszDesc - Description for link, unimportant
*		LPCSTR lpszFileDir - Folder where application exists so it runs as if it was ran there
*		wstring linkName - Name of link on desktop
*************************************************************/
extern void CreateLink(LPCSTR lpszPathObj, LPCSTR lpszDesc, LPCSTR lpszFileDir, wstring linkName) {
	CoInitialize(NULL);
	HRESULT hres;
	IShellLink* psl = NULL;

	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
	if (SUCCEEDED(hres)) {
		IPersistFile* ppf;

		psl->SetPath(lpszPathObj);
		psl->SetDescription(lpszDesc);
		psl->SetWorkingDirectory(lpszFileDir);

		hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

		if (SUCCEEDED(hres)) {
			CHAR pMyPath[MAX_PATH];
			WCHAR wsz[MAX_PATH];
			const wchar_t* n = linkName.c_str();

			SHGetFolderPath(0, CSIDL_DESKTOPDIRECTORY, 0, 0, pMyPath);
			MultiByteToWideChar(CP_ACP, 0, pMyPath, -1, wsz, MAX_PATH);
			wcscat_s(wsz, n);

			hres = ppf->Save(wsz, TRUE);
			ppf->Release();
		}
		else {
			cout << "Error 2" << endl;
		}
		psl->Release();
	}
	else {
		cout << "Error 1" << endl;
	}
	//return hres;
}
/************************************************************
*	Creates link on Desktop using CreateLink function
*	Paramters
*		string Folder - Parent directory to hold shortcut folder
*		string name - Name of new shortcut folder
*		string dir - That Link will open
*		string pass - Password if directory is password protected
*		bool createDir - Creates new sh.exe in SH_FOLDER
*************************************************************/
extern void createDesktopSHDirectory(string Folder, string name, string dir, string pass, bool createDir) {
	string newdir = Folder + "\\" + name;
	string fulldir = fs::current_path().u8string() + "\\" + newdir;
	string txtdir = newdir + "\\info.txt";
	string exedir = fulldir + "\\sh.exe";
	string code = to_string(rand() % 9999999);
	string nm = "\\" + name + ".lnk";
	wstring wname; wname.assign(nm.begin(), nm.end());

	fs::create_directory(newdir);
	ofstream infofile(txtdir); changeFileProperty(txtdir, 0);
	infofile << code << " " << codeString(dir, code, true) << " " << codeString(pass, code, true) << endl;

	if (createDir) { fs::copy("sh.exe", newdir); }
	CreateLink(exedir.c_str(), "Link to file", fulldir.c_str(), wname);
}
