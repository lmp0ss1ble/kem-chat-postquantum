#include <winsock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
extern "C" {
#include "src/api.h"
#include "src/randombytes.h"
}
#include "src/aes256.hpp"
#define NTESTS 5
#pragma warning(disable: 4996)

#define PASTER(x, y) x##_##y
#define EVALUATOR(x, y) PASTER(x, y)
#define NAMESPACE(fun) EVALUATOR(PQCLEAN_NAMESPACE, fun)
#define CRYPTO_BYTES           PQCLEAN_MCELIECE8192128F_CLEAN_CRYPTO_BYTES
#define CRYPTO_PUBLICKEYBYTES  PQCLEAN_MCELIECE8192128F_CLEAN_CRYPTO_PUBLICKEYBYTES
#define CRYPTO_SECRETKEYBYTES  PQCLEAN_MCELIECE8192128F_CLEAN_CRYPTO_SECRETKEYBYTES
#define CRYPTO_CIPHERTEXTBYTES PQCLEAN_MCELIECE8192128F_CLEAN_CRYPTO_CIPHERTEXTBYTES
#define crypto_kem_keypair  PQCLEAN_MCELIECE8192128F_CLEAN_crypto_kem_keypair
#define crypto_kem_enc      PQCLEAN_MCELIECE8192128F_CLEAN_crypto_kem_enc
#define crypto_kem_dec      PQCLEAN_MCELIECE8192128F_CLEAN_crypto_kem_dec
#define KEY_LEN    32
#pragma warning(disable: 4996)

char DefaultMassage[256] = "qwerty";

uint8_t pk[CRYPTO_PUBLICKEYBYTES];
uint8_t sk[CRYPTO_SECRETKEYBYTES];
uint8_t general_aes_key[KEY_LEN];

SOCKET Connections[100];
int Counter = 0;

void enc_to_msg(char* buf, ByteArray enc, size_t lenght) {
	for (int i = 0; i < enc.size(); i++) {
		buf[i] = enc[i];
	}
}

void uint8_t_to_ByteArray(ByteArray key, unsigned char* sharedSecret_b, size_t lenght) {
	for (int i = 0; i < lenght; i++) {
		key.push_back(sharedSecret_b[i]);
	}
}

void ClientHandler1() {

	ByteArray key1;

	uint8_t_to_ByteArray(key1, (unsigned char*)general_aes_key, sizeof(general_aes_key));

	ByteArray::size_type dec_len;

	ByteArray dec;

	uint8_t msg[256] = { 0 };

	while (true) {
		recv(Connections[0], (char*)msg, sizeof(msg), NULL);
		if (sizeof(msg) != 0) {
			dec_len = Aes256::decrypt(key1, msg, sizeof(msg), dec);
			enc_to_msg((char*)msg, dec, sizeof(msg));
		}
		dec.clear();
		std::cout<< "Client:" << msg << std::endl;
		
	}
}

int main(int argc, char* argv[]) {
	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("192.168.1.70");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);
	SOCKET newConnection;
	//for (int i = 0; i < 100; i++) {

		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

		if (newConnection == 0) {
			std::cout << "Error #2\n";
		}
		else {
			std::cout << "Client Connected!\n";

			Connections[0] = newConnection;
			Counter++;

			if (Counter > 0) {
				char msg[256] = "connected";
				send(Connections[0], msg, sizeof(msg), NULL);
			}

			uint8_t cipherText[CRYPTO_CIPHERTEXTBYTES];
			uint8_t sharedSecret_b[CRYPTO_BYTES];

			PQCLEAN_MCELIECE8192128F_CLEAN_crypto_kem_keypair(pk, sk);
			
			send(Connections[0], (char*)pk, sizeof(pk), NULL);

			Sleep(10);

			recv(Connections[0], (char*)cipherText, sizeof(cipherText), NULL);

			PQCLEAN_MCELIECE8192128F_CLEAN_crypto_kem_dec(sharedSecret_b, cipherText, sk);

			send(Connections[0], (char*)sharedSecret_b, sizeof(sharedSecret_b), NULL);

			Sleep(2000);

			char msg1[256] = { 0 };
			ByteArray::size_type enc_len;
			ByteArray enc, aes_key;
			
			for (int j = 0; j < KEY_LEN; j++) {
				general_aes_key[j] = sharedSecret_b[j];
			}

			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler1, (LPVOID)(0), NULL, NULL);

			uint8_t_to_ByteArray(aes_key, (unsigned char*)sharedSecret_b, sizeof(sharedSecret_b));

			while (true) {
				std::cin.getline(msg1, sizeof(msg1));
				enc_len = Aes256::encrypt(aes_key, (unsigned char*)msg1, sizeof(msg1), enc);
				enc_to_msg(msg1, enc, sizeof(msg1));
				enc.clear();
				send(Connections[0], msg1, sizeof(msg1), NULL);
				Sleep(10);
			}
		}
	//}
	system("pause");
	return 0;
}