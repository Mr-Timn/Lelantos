//#pragma once

/************************************************************
*	Paramaters
*		bool hide - True if input should output '*' to console. False if normal input 
*					(Does not alter what is returned)
*	Returns a string that includes all that was typed up until enter was hit
*************************************************************/
extern string getInput(bool hide) {
	string in = "";
	char ch = _getch();
	while (!(ch == 13)) {
		if (ch == 8) {
			if (in.length() > 0) {
				cout << "\b \b";
				in = in.substr(0, in.length() - 1);
			}
		} else {
			in.push_back(ch);
			if (hide) {
				cout << "*";
			} else {
				cout << ch;
			}
		}
		ch = _getch();
	}
	cout << endl;
	return in;
}
/************************************************************
*	Uses getInput(false) to determine if input was y/n
*	
*	Returns true if input is "y" or "yes" 
			false if "n" or "no"
*************************************************************/
extern bool yes_no() {
YESNO:
	string input;
	input = getInput(false);
	if (input == "y" || input == "yes") {
		return true;
	} else if (input == "n" || input == "no") {
		return false;
	} else {
		cout << "Incorrect input. (Y/N)"; goto YESNO;
	}
}
/************************************************************
*	Returns true if input is "back" or "return"
*************************************************************/
extern bool goback(string in) {
	if (in == "back" || in == "return") {
		return true;
	}
	return false;
}
/************************************************************
*	Returns true if input is "cancal" or "stop"
*************************************************************/
extern bool cancel(string in) {
	if (in == "cancel" || in == "stop") {
		return true;
	}
	return false;
}
/************************************************************
*	Uses goback(in) and cancel(in) to determine if input was "back", "return", "cancel", or "stop"
*************************************************************/
extern bool end(string in) {
	if (goback(in) || cancel(in)) {
		return true;
	}
	return false;
}

/************************************************************
*	Parameters	
*		string in - string that contains use input
*		vector<string> opt - vector array containing expected keywords
*		bool cl - True if strings similar to in will still return true
*			in = "helppppp" opt = {"help"} will return true
*	
*	Returns true if in is equal to any of the strings in opt
*************************************************************/
extern bool inputIs(string in, vector<string> opt, bool cl) {
	int t = 0;
	for (auto i : opt) {
		if (in == i) {
			return true;
		}
		/* Run command even if spelt wrong. inputIs(cmd,{"help"},true) still return true. */
		if (cl) {
			if (i[0] != in[0] || i.length() > in.length()) { continue; }
			for (size_t o = 0; o < in.length(); o++) {
				for (size_t p = 0; p < i.length(); p++) {
					if (t > 2) { return true; }
					if (i[p] == in[o]) {
						t++; continue;
					}
				}
			}
		}
	}
	return false;
}


