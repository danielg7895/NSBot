#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <windows.h>
#include <string>

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996) 

#define RECVBUFF_LEN 8192

class NostaleSocket {

private:
	SOCKET connection_socket;
	int resultado;
	char recvBuff[RECVBUFF_LEN] = "";

public:

	NostaleSocket() {
		// inicializo winsock
		WSADATA wsaData;
		resultado = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (resultado != NO_ERROR) {
			printf("WSAStartup fallo con el error: %d\n", resultado);
			return;
		}

		// Creo el socket para poder conectarme al server.
		connection_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
		if (connection_socket == INVALID_SOCKET) {
			printf("Algo salio mal al crear el socket para conectar al server, codigo error: %ld\n", WSAGetLastError());
			WSACleanup();
			return;
		}

		setsockopt(connection_socket, SOL_SOCKET, SO_REUSEADDR, NULL, 0);
		setsockopt(connection_socket, IPPROTO_TCP, TCP_NODELAY, NULL, 0);

		// Configuro la ip y puerto de coneccion.
		sockaddr_in clientService;
		clientService.sin_family = AF_INET;
		clientService.sin_addr.s_addr = inet_addr("79.110.84.75");
		clientService.sin_port = htons(4005);

		// Me conecto al server
		resultado = connect(connection_socket, (SOCKADDR*)& clientService, sizeof(clientService));
		if (resultado == SOCKET_ERROR) {
			printf("Fallo la conexion con Nostale, codigo error: %ld\n", WSAGetLastError());
			resultado = closesocket(connection_socket);
			if (resultado == SOCKET_ERROR) {
				printf("Falle al cerrar el socket, codigo de error %ld\n", WSAGetLastError());
			}
			WSACleanup();
			return;
		}
		printf("Coneccion con el server satisfactoria\n");

	}
	 void NSend(const char* buff, int len) {

		 resultado = send(connection_socket, buff, len, 0);
		 if (resultado == SOCKET_ERROR) {
			 printf("Falle al enviar el paquete, codigo error: %ld\n", WSAGetLastError());
			 close_connection();
			 return;
		 }
		 // paquete enviado
		 do {
			 resultado = recv(connection_socket, recvBuff, RECVBUFF_LEN, 0);
			 if (resultado > 0) {
				 printf("Recibi %d bytes!\n", resultado);
				 // ... recibiendo bytes
			 }
			 else if (resultado == 0) {
				 printf("Conexion cerrada.\n");
			 }
			 else {
				 printf("recv fallo, codigo de error: %ld\n", WSAGetLastError());
			 }
		 } while (resultado > 0);
	 }

	 char* NSRecv() {
		 return recvBuff;
	 }

	 void close_connection() {
		 resultado = closesocket(connection_socket);
		 if (resultado == SOCKET_ERROR) {
			 wprintf(L"No pude cerrar coneccion, codigo de error: %ld\n", WSAGetLastError());
		 }
		 WSACleanup();
		 return;
	 }


};