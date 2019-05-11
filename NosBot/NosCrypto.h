#pragma once
#include <string>
#include <Windows.h>
#include "bot.h"

// Login encrypt/decrypt methods
std::string login_decrypt(std::string packet);
std::string login_encrypt(std::string& packet);
std::string login_decrypt_server(std::string packet);

// Ingame RECV encrypt/decrypt methods
std::string ingame_decrypt(std::string packet);
std::string ingame_decrypt_letters(std::string packet);
BYTE ingame_data_length(BYTE byt);
std::string ingame_decrypt_numbers(std::string packet, int len);
std::string ingame_encrypt(std::string packet);

// Ingame SEND encrypt/decrypt methods
std::string ingame_encrypt(NSPacket packet);
BYTE encrypt_eax_zero(BYTE byte);
BYTE encrypt_eax_one(BYTE byte);
BYTE encrypt_eax_two(BYTE byte);
BYTE encrypt_eax_three(BYTE byte);

DWORD generateSeed(DWORD num);
std::string generateRandomNum(DWORD* seed);