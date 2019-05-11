#include "Tools.h"
#pragma warning(disable:4996)

std::string int2hex_str(int num) {
	static char buffer[8];
	sprintf(buffer, "%08X", num);
	return std::string(buffer);
}

std::string byte2hex_str(BYTE num) {
	static char buffer[2];
	sprintf(buffer, "%.2X", num);
	return std::string(buffer);
}