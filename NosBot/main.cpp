#include "nostale_sockets.cpp"
#include "main.h"
#include <vector>
#include "Tools.h"
#include "NosCrypto.h"


void main() {

}

void main_headless() {
	// not used in this project, method just for testing.

	NostaleSocket nostaleSocket;

	
	std::string loginPacket = login("", "");
	int len = loginPacket.length();
	login_encrypt(loginPacket);

	nostaleSocket.NSend(loginPacket.c_str(), len);
	printf(login_decrypt(nostaleSocket.NSRecv()).c_str());
	

	//printf(originalDecrypt.c_str());

	
	//printf(decryptLoginServer(send).c_str());*/
}

std::string login(std::string user, std::string password) {
	MD5 md5;
	DWORD seed;

	for (int i = 0; i < 150; i++) { 
		seed = generateSeed(0x989680);
	}

	std::string nostalePath = "C:\\Program Files (x86)\\Nostale_ES";
	std::string md5NostaleClientX = md5.digestFile((nostalePath + "\\NostaleClient.exe").c_str());
	std::string md5NostaleClient = md5.digestFile((nostalePath + "\\NostaleClientX.exe").c_str());
	std::string md5_hash = md5NostaleClient + md5NostaleClientX + user;


	std::string opcode = "NoS0575";

	seed = generateSeed(0x989680) + 0x86111;
	std::string randomNum1 = generateRandomNum(&seed);

	std::string passSHA512 = sha512(password);
	std::string randomNum2 = int2hex_str((int)generateSeed(0x989680));
	std::string gameVersion = "0.9.3.3103";
	std::string bothMD5 = md5.digestString(md5_hash);
	std::string packet;
	packet += opcode;
	packet += " " + randomNum1;
	packet += " " + user + " " + passSHA512;
	packet += " NONE " + randomNum2;
	packet += 0x0B;
	packet += gameVersion + " 0 " + bothMD5;
	packet += 0x0A;


	//std::cout << "NoS0577 30663063383665352D663763322D343233382D623436342D396139653733663834643339 geka600 NONE_CII 0063FE7A\v0.9.3.3102 0 5798BB156CBD95D1EB3B270FB626D026\n" << std::endl;

	return packet;
}
