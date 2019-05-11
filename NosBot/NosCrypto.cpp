#include "NosCrypto.h"
#include "Tools.h"
#include <iostream>

DWORD prevNum = 0;
DWORD generateSeed(DWORD num) {
	DWORD edx = (DWORD)((int)prevNum * (int)0x8088405);
	++edx;
	prevNum = edx;
	edx = edx * num;
	return edx;
}

std::string generateRandomNum(DWORD *seed) {
	BYTE ecx = 0x0A;
	BYTE resto = 0x00;
	std::string strRandomBytes = "";
	//DWORD64 randomBytes = 0x00;
	//BYTE counter = 0;
	int counter = 0;

	while (*seed != 0) {
		resto = 0x00;
		resto = *seed % ecx;
		*seed = *seed / ecx;

		// dec esi
		resto += 0x30;
		if (resto > 0x3A) {
			resto += 0x07;
		}

		counter += resto;
		if (strRandomBytes.length() == 7) break;
		strRandomBytes = (char)resto + strRandomBytes;
		if (strRandomBytes.length() > 7) strRandomBytes = strRandomBytes.substr(0, 8);

		//randomBytes += resto << (counter * 8);
		//++counter;
	}

	return strRandomBytes; // devuelve en hex
}

// ################################## LOGIN ENCRYPT/DECRYPT METHODS ##############################################

std::string login_decrypt(std::string packet) {
	// server to client login packets decryption.
	for (int i = 0; i < packet.length(); ++i) {
		packet[i] -= 0x0F;
		if (packet[i] == 0x0A) {
			break;
		}
		if (packet[i] == 0x20) {
			continue;
		}
	}
	return packet;
}

std::string login_decrypt_server(std::string packet) {
	// method used on server side to decrypt client packets.
	for (int i = 0; i < packet.length(); ++i) {
		packet[i] = (packet[i] - 0x0F) ^ 0xC3;
	}
	return packet;
}

std::string login_encrypt(std::string &packet) {
	// client to server packet encryption
	for (int i = 0; i < packet.length(); ++i) {
		packet[i] = (packet[i] ^ 0xC3) + 0x0F;
	}
	return packet;
}


int test = 0;
// ################################## INGAME ENCRYPT/DECRYPT METHODS ##########################################
std::string ingame_decrypt(std::string packet) {
	if (test != 10) {
		test++;
		return "test";
	}
	std::string decrypted_packet = "";
	BYTE opc_len = 0;
	BYTE current = 0;
	BYTE data_len = 0;
	std::string current_packet = "";

	int i = 0;
	for (i = 0; i < packet.length(); i++) {
		if ((uint8_t)packet[i] == 0xFF) {
			i++;
			break;
		}
	}

	if (i >= packet.length()) {
		printf("ERROR: i es mas grande que el paquete antes de siquiera intentar desencriptar!!\n");
		return "";
	}

	while (true) {
		current = packet[i];
		if ((uint8_t)current == 0xFF) {
			//printf("En 0xFF, aumentando i\n");
			//printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
			i++; // fin paquete actual
			continue;
		}
		i++;
		data_len = ingame_data_length(current);
		if ((uint8_t)current < 0x80) {
			// jump
			//printf("DATA LENGTH letras: %d\n", data_len);

			current_packet = packet.substr(i, data_len);
			decrypted_packet += ingame_decrypt_letters(current_packet);
			i += data_len;
		}
		else {
			//printf("DATA LENGTH NUMEROS: %d\n", data_len);

			data_len = (data_len / 2) + (data_len % 2);
			current_packet = packet.substr(i, data_len);
			decrypted_packet += ingame_decrypt_numbers(current_packet, data_len);
			i += data_len;//9
		}
		//printf("Data len: %d\n", data_len);
		if ( (i+20 >= packet.length()) || ((uint8_t)packet[i] == 0xD8)) {
			i++; // fin paquete completo
			break;
		}
	}
	printf("\n\n\n");

	return decrypted_packet;
}


std::string ingame_decrypt_letters(std::string packet) {
	for (int i = 0; i < packet.length(); i++) {
		packet[i] = packet[i] ^ 0xFF;
	}
	return packet;
}


BYTE ingame_data_length(BYTE byt) {
	byt = (byt & 0x7F) & 0xFF;

	return byt;
}


std::string ingame_decrypt_numbers(std::string packet, int len) { // packet decryption
	BYTE table[16] = {0x00, 0x20, 0x2D, 0x2E, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x0A, 0x20};
	BYTE msb = 0x00;
	BYTE lsb = 0x00;
	std::string current_packet = packet;

	for (int i = 0; i < len; i++) {
		// Obtengo la parte mas significativa del byte.
		msb = packet[i] & 0xF0; // AND AL, F0
		msb = msb >> 0x04;
		current_packet[i] = table[(uint16_t)msb];

		if ((len - i) == 1) {
			break;
		}

		// obtengo la parte menos significativa del byte.
		lsb = packet[i] & 0x0F; // AND AL, 0x0F
		lsb = lsb & 0xFF;

		current_packet[i + 1] = table[(uint16_t)lsb];

	}

	return current_packet;
}


std::string ingame_encrypt(std::string packet) {
	return "";
}


// ################################## INGAME SEND ENCRYPT/DECRYPT METHODS ##########################################

std::string ingame_encrypt(NSPacket packet) {
	return "";
}

BYTE encrypt_eax_zero(BYTE byte) {
	BYTE DL = 0; // MOV DL, BYTE PTR DS:[EDX+ESI-1]
	BYTE CL = 0; // MOV CL, BYTE PTR DS:[EBX+48]

	CL += 0x40;
	DL += CL;

	return DL;
}

BYTE encrypt_eax_one(BYTE byte) {
	BYTE DL = 0; // MOV DL, BYTE PTR DS:[EDX+ESI-1]
	BYTE CL = 0; // MOV CL, BYTE PTR DS:[EBX+48]

	CL += 0x40;
	DL -= CL;

	return DL;
}

BYTE encrypt_eax_two(BYTE byte) {
	BYTE DL = 0; // MOV DL, BYTE PTR DS:[EDX+ESI-1]
	DL = DL ^ 0xC3;

	BYTE CL = 0; // MOV CL, BYTE PTR DS:[EBX+48]
	CL += 0x40;

	DL += CL;

	return DL;
}

BYTE encrypt_eax_three(BYTE byte) {
	BYTE DL = 0; // MOV DL, BYTE PTR DS:[EDX+ESI-1]
	DL = DL ^ 0xC3;

	BYTE CL = 0; // MOV CL, BYTE PTR DS:[EBX+48]
	CL += 0x40;

	DL -= CL;

	return DL;
}