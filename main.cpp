#include <iostream>
#include <fstream>
#include <list>
#include <iterator>
#include <vector>
#include<ctype.h>

int main() {
	std::ifstream listFile = std::ifstream("WordleList.txt");
	if (listFile.is_open()) {
		std::list<char*> wordList;
		int wordCount = 0;

		//Import word list
		char* importWord = new char[5];
		while (listFile >> importWord) {
			for (int i = 0; i < 5; i++) {
				importWord[i] = tolower(importWord[i]);
			}
			wordList.push_back(importWord);
			importWord = new char[5];
			wordCount++;
		}
		std::cout << "end of import, " << wordCount << " words in list.\n";

		std::vector<char> excludedLetters;
		excludedLetters.push_back('.');
		std::vector<char> includedLetters;
		includedLetters.push_back('.');
		char confirmedLetters[6] = "     ";

		char wordGuess[6] = "     ";
		char validationString[6];
		for (int i = 0; i < 6; i++) {
			if (wordList.size() < 1) {
				std::cout << "\nNo words remaining in dictionary.";
				return 0;
			}
			//Find optimal word from list
			//or random word
			int random = rand() % wordList.size();
			std::list<char*>::iterator it = wordList.begin();
			std::advance(it, random);
			strcpy_s(wordGuess, *it);

			//Get choice validated by user input
			std::cout << "\nGuess: " << wordGuess << "\n";
			std::cout << "Answer: ";
			std::cin >> validationString;
			std::cout << "\n";

			for (int i = 0; i < 5; i++) {
				if (validationString[i] == 'N') {
					if (std::count(excludedLetters.begin(), excludedLetters.end(), wordGuess[i]) == 0) {
						excludedLetters.push_back(wordGuess[i]);
					}
				}
				if (validationString[i] == 'Y') {
					if (std::count(includedLetters.begin(), includedLetters.end(), wordGuess[i]) == 0) {
						includedLetters.push_back(wordGuess[i]);
					}
				}
				if (validationString[i] == 'G') {
					if (std::count(includedLetters.begin(), includedLetters.end(), wordGuess[i]) == 0) {
						includedLetters.push_back(wordGuess[i]);
					}
					confirmedLetters[i] = wordGuess[i];
				}
			}

			//trim list based on known criteria
			strcpy_s(wordGuess, "     ");
			wordList.push_front(wordGuess);
			std::list<char*>::iterator slowIt = wordList.begin();
			it = slowIt;
			for (it++; it != wordList.end(); it++) {
				bool excluded = false;
				for (int w = 0; w < excludedLetters.size(); w++) {
					for (int i = 0; i < 5; i++) {
						if ((*it)[i] == excludedLetters[w]) {
							excluded = true;
						}
					}
				}
				if (excluded) {
					wordList.erase(it);
					it = slowIt;
				}
				else {
					bool included = false;
					int numInclusions = 0;
					for (int w = 0; w < includedLetters.size(); w++) {
						for (int i = 0; i < 5; i++) {
							if ((*it)[i] == includedLetters[w]) {
								numInclusions++;
								break;
							}
						}
					}
					if (numInclusions == includedLetters.size() - 1) {
						included = true;
					}
					
					if (!included) {
						wordList.erase(it);
						it = slowIt;
					}
					else {
						bool confirmed = true;
						for (int i = 0; i < 5; i++) {
							if ((*it)[i] != confirmedLetters[i] && confirmedLetters[i] != ' ') {
								confirmed = false;
							}
						}
						if (!confirmed) {
							wordList.erase(it);
							it = slowIt;
						}
					}
				}
				slowIt = it;
			}
			wordList.erase(wordList.begin());

			//test word list
			std::cout << "Words remaining in dictionary: \n";
			for (std::list<char*>::iterator it = wordList.begin(); it != wordList.end(); it++) {
				std::cout << *it << " ";
			}
		}

	}
	else {
		std::cout << "Bad file read\n";
	}

	return 0;
}