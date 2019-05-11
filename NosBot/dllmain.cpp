#include <stdio.h>
#include <Windows.h>

#include "xHacking/include/xHacking.h"
#include "xHacking/include/Utilities.h"
#include "xHacking/include/Loader.h"
#include "xHacking/include/Detour.h"

#include "NosCrypto.h"

#ifdef _DEBUG
#pragma comment(lib, "xHacking_d.lib")
#else
#pragma comment(lib, "xHacking.lib")
#endif

bool logged_in = false;

using namespace xHacking;

Detour<int, SOCKET, char*, int, int>* recvDetour = NULL;
Detour<int, SOCKET, const char*, int, int>* sendDetour = NULL;

int WINAPI nos_recv(SOCKET s, char* buf, int len, int flags) {
	int ret = (*recvDetour)(s, buf, len, flags);

	__asm PUSHAD;
	__asm PUSHFD;

	if (!logged_in) {
		printf("[S->C] %s\n", login_decrypt(buf).c_str());
		logged_in = true;
	}
	else {
		//printf("\n\n\n[S->C] ========== Paquete desencriptado ==========\n %s", ingame_decrypt(buf).c_str());
	}

	__asm POPFD;
	__asm POPAD;

	return ret;
}

int WINAPI nos_send(SOCKET s, const char* buf, int len, int flags) {
	int ret = (*sendDetour)(s, buf, len, flags);

	__asm PUSHAD;
	__asm PUSHFD;

	printf("%s\n", buf);

	__asm POPFD;
	__asm POPAD;

	return ret;
}

void Hooks() {
	recvDetour = new Detour<int, SOCKET, char*, int, int>();
	recvDetour->Wait((char*)"WSOCK32.dll", (char*)"recv", (BYTE*)nos_recv);

	sendDetour = new Detour<int, SOCKET, const char*, int, int>();
	sendDetour->Wait((char*)"ws2_32.dll", (char*)"send", (BYTE*)nos_send);
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, DWORD reserved) {
	if (reason == DLL_PROCESS_ATTACH) {

		CreateConsole();

		Hooks();
	}

	return true;
}