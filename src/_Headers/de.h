#pragma once

/************************************************************
*	Paramaters
*		char c - Cfharacter to be changed
*		int adj - How far char will be moved up or down
*
*	Returns changed chr
*************************************************************/
extern string changeChar(char c, int adj) {
	string abc = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_:~`!@#$%^&(){}[];',."; 
	int len = abc.size();
	string nstr = "\\";

	if (c == ' ') { return "?"; }
	else if (c == '?') { return " "; }

	for (size_t i = 0; i < abc.size(); i++) {
		if (abc[i] == c) {
			int new_pos = i + adj;
			if (new_pos >= len) { new_pos -= len; }
			else if (new_pos < 0) { new_pos += len; }

			return nstr = abc[new_pos];
		}
	}
	return nstr;
}
/************************************************************
*	Parameters
*		string d - String to be coded
*		string c - Random int represented as a string
*		bool code - True if coding string false if decoding
*	
*	Returns coded or decoded string
*************************************************************/
extern string codeString(string d, string c, bool code) {
	size_t ind = 0; string ndir = "";

	for (size_t i = 0; i < d.size(); i++) {
		int m = c[ind] - '0';
		if (code) { m *= -1;  ndir += changeChar(d[i], m); }
		else { ndir += changeChar(d[i], m); }

		if (ind < c.size() - 1) { ind++; }
		else { ind = 0; }
	}
	return ndir;
}