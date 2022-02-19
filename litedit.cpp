#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>

long unsigned int topline = 0;
long unsigned int cursorv = 0;
long unsigned int cursorh = 0;
std::vector<std::string> lines;
std::vector<std::string> saved;
std::string input;
std::string previnput;
bool quit = false;
bool noprev = false;
bool unsavedchanges = false;
std::stack<std::vector<std::string>> history;
std::stack<std::vector<std::string>> ahead;

int theloop() {
	if (quit == true) {
		return 0;
	}
	std::cout << "     ";
	for (long unsigned int i = 0; i < cursorh; i++) std::cout << ' ';

	std::cout << "*\n     12345678901234567890\n";
	for (int i = 0; i < 10; i++) {
		long unsigned int currentline = topline + i;
		long unsigned int displaynumber = currentline + 1;
		if (displaynumber < 10 && currentline == cursorv) std::cout << "*  " << displaynumber;
		if (displaynumber < 10 && currentline != cursorv) std::cout << "   " << displaynumber;
		if (displaynumber >= 10 && currentline == cursorv) std::cout << "* " << displaynumber;
		if (displaynumber >= 10 && currentline != cursorv) std::cout << "  " << displaynumber;
		if (lines.size() > currentline) {
			std::cout << "|" << lines[currentline];
		}
		std::cout << "\n";
	}
	std::cout << "     12345678901234567890\nEnter command:";
	if (input.length() != 0) previnput = input;
	std::getline(std::cin, input);
	std::cout << "\n";
	
	if (input.length() == 0) {
		if (previnput.length() == 0) {
			std::cout << "No previous command.\n\n";
			noprev = true;
		}
		input = previnput;
	}

	if (input[0] == 'q') {
		if (unsavedchanges == true) {
			std::cout << "You have unsaved changes.\nAre you sure you want to quit (y or n)?\n";
			std::cin >> input;
			if (input[0] == 'n') {
				std::cout << "\n";
				theloop();
			}
			std::cout << "\n\n";
		}
		
		quit = true;
		theloop();
	}

	if (input[0] == 's' && input[1] == 'a') {
		std::string tempinput = input;
		tempinput.erase(0, 5);
		std::ofstream write;
		write.open(tempinput, std::ios_base::app);
		for (std::string ne : lines) {
				write << ne << "\n";
		}
		write.close();
		saved = lines;
		unsavedchanges = false;
	}

	if (input[0] == 'd') {
		if (input.length() > 2) {
			cursorh += atoi(&input[2]);
			if (cursorh > 19) cursorh = 19;
			theloop();
		}
		cursorh++;
		if (cursorh > 19) cursorh = 19;
		theloop();
	}

	if (input[0] == 'a') {
		if (input.length() > 2) {
			cursorh -= atoi(&input[2]);
			if (cursorh > 50000) cursorh = 0;
			theloop();
		}
		cursorh--;
		if (cursorh > 50000) cursorh = 0;
		theloop();
	}


	if (input[0] == 's' && (input.length() == 1 || input[1] != 'a')){
		if (input.length() > 2) {
			for (int i = 0; i < atoi(&input[2]); i++) {
				bool flag = true;
				if (cursorv < lines.size() - 1) {
					cursorv++;
					flag = false;
				}
				if (flag == false && cursorv > 10) topline = cursorv - 9;
				if (flag == true && topline != cursorv) topline++;
			}
			
			theloop();
		}
		bool flag = true;
		if (cursorv < lines.size() - 1) {
			cursorv++;
			flag = false;
		}
		if (flag == false && cursorv == topline +10) topline = cursorv - 9;
		if (flag == true && topline != cursorv) topline++;
		theloop();
	}
	

	if (input[0] == 'w') {
		if (input.length() > 2) {
			for (int i = 0; i < atoi(&input[2]); i++) {
				if (topline == cursorv && topline != 0) topline--;
				if (cursorv != 0) cursorv--;
			}
			theloop();
		}
		if (topline == cursorv && topline != 0) topline--;
		if (cursorv != 0) cursorv--;
		theloop();
	}

	

	if (input[0] == 'i') {
		input.erase(0, 2);
		bool skip = false;
		history.push(lines);
		int tempcursorh = cursorh - 1;
		int tempcursorv = cursorv;
		int remaining = lines[cursorv].length() - cursorh;
		int implength = input.length();
		if (remaining < implength) lines[cursorv].resize(lines[cursorv].length() + implength - remaining, ' ');
		//std::size_t a = lines[cursorv].length();
		if (lines[cursorv].length() > 20) lines[cursorv].resize(20);
		for (std::size_t i = 1; i <= input.length(); i++) {
			tempcursorh++;
			if (tempcursorh > 19) {
				tempcursorh = 0;
				tempcursorv += 1;

				if (tempcursorv == 30) {
					i = 256;
					tempcursorv--;
					skip = true;
				}
				if (tempcursorv >= (int)lines.size()) lines.resize(lines.size() + 1);
				int remaining = lines[tempcursorv].length();
				int implength = input.length() - i;
				if (remaining < implength) lines[tempcursorv].resize(lines[tempcursorv].length() + implength - remaining + 1, ' ');
				
			}
			if (skip == false) {
				lines[tempcursorv][tempcursorh] = input[i - 1];
			}
		}
		unsavedchanges = true;
		theloop();
	}

	if (input[0] == 'u') {
		bool hasundone = false;
		if (history.size() != 0) {
			ahead.push(lines);
			lines = history.top();
			history.pop();
			if (lines != saved) unsavedchanges = true;
			if (lines == saved) unsavedchanges = false;
			hasundone = true;
			theloop();
		}
		if (hasundone == false) std::cout << "Cannot undo.\n\n";
		theloop();
	}

	if (input[0] == 'r') {
		bool hasredone = false;
		if (ahead.size() != 0) {
			history.push(lines);
			lines = ahead.top();
			ahead.pop();
			hasredone = true;
			if (lines != saved) unsavedchanges = true;
			if (lines == saved) unsavedchanges = false;
			theloop();
		}
		if (hasredone == false) std::cout << "Cannot redo.\n\n";
		theloop();
	}

	noprev = false;
	theloop();
	return 0;
};

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cout << "Too few command-line arguments.\n";
		return 1;
	}

	if (argc > 2) {
		std::cout << "Too many command-line arguments.\n";
		return 1;
	}
	

	int no_lines = 0;
	std::string line;
	std::ifstream file(argv[1]);
	while (std::getline(file, line)) {
		if (no_lines > 29) {
			std::cout << "File " << argv[1] <<" has too many lines.\n";
			return 2;
		}
		if (line.length() > 20) {
			std::cout << "At least one line is too long.";
			return 3;
		}
		lines.push_back(line);
		no_lines++;
	}
	if (no_lines == 0) {
		std::cout << "The file couldn't be opened.";
		return 4;
	}
	file.close();
	saved = lines;
	theloop();
	std::cout.clear();
	std::cout << "Goodbye!\n";



}