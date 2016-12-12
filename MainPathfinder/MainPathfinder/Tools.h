#pragma once
#include <Windows.h>
#include <string>

//using namespace std;

// Gets the current working directory
std::string CurrPath() {
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring ws(buffer);					// convert wchar_t* to wstring
	std::string str(ws.begin(), ws.end());	// convert wstring to string
	std::string::size_type pos = str.find_last_of("\\/");
	return str.substr(0, pos);
}

void InvalidInput() {
	std::cout << std::endl << "Error: Invalid input." << std::endl << std::endl;
	std::cin.clear();
#undef max() // resolves conflict with std::max() from Windows.h
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}