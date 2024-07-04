#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <winsock2.h>
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

uint8_t pk[CRYPTO_PUBLICKEYBYTES];

uint8_t general_aes_key[KEY_LEN];

SOCKET Connection;

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
		recv(Connection, (char*)msg, sizeof(msg), NULL);
		if (sizeof(msg) != 0) {
			dec_len = Aes256::decrypt(key1, msg, sizeof(msg), dec);
			enc_to_msg((char*)msg, dec, sizeof(msg));
		}
		dec.clear();

		std::cout << "Server:" << msg << std::endl;

	}
}

int CheckConnection(char msg[256]) {
	if (msg[0] == "c"[0]) {
		if (msg[1] == "o"[0]) {
			if (msg[2] == "n"[0]) {
				if (msg[3] == "n"[0]) {
					if (msg[4] == "e"[0]) {
						if (msg[5] == "c"[0]) {
							if (msg[6] == "t"[0]) {
								if (msg[7] == "e"[0]) {
									if (msg[8] == "d"[0]) {
										return 1;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return 0;
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
	addr.sin_addr.s_addr = inet_addr("192.168.1.70");//your ip
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error: failed connect to server.\n";
		return 1;
	}
	//std::cout << "Connected!\n";

	uint8_t con[256];
	recv(Connection, (char*)con, sizeof(con), NULL);
	std::cout << con;

	Sleep(2000);

	recv(Connection, (char*)pk, sizeof(pk), NULL);

	ByteArray key, enc;
	ByteArray::size_type enc_len;

	uint8_t cipherText[CRYPTO_CIPHERTEXTBYTES];
	uint8_t sharedSecret[CRYPTO_BYTES];
	uint8_t sharedSecret1[CRYPTO_BYTES];

	PQCLEAN_MCELIECE8192128F_CLEAN_crypto_kem_enc(cipherText, sharedSecret, pk);

	send(Connection, (char*)cipherText, sizeof(cipherText), NULL);

	Sleep(5000);

	recv(Connection, (char*)sharedSecret1, sizeof(sharedSecret1), NULL);
	bool k = true;
	for (int i = 0; i < CRYPTO_BYTES; i++) {
		if (sharedSecret[i] != sharedSecret1[i]) {
			k = false;
		}
	}
	if (k) {
		std::cout << "\nConnected!\n";

		for (int i = 0; i < KEY_LEN; i++) {
			general_aes_key[i] = sharedSecret[i];
		}

		char msg1[256] = { 0 };
		ByteArray::size_type enc_len;
		ByteArray enc, aes_key;

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler1, NULL, NULL, NULL);

		uint8_t_to_ByteArray(aes_key, (unsigned char*)sharedSecret, sizeof(sharedSecret));

		while (true) {
			std::cin.getline(msg1, sizeof(msg1));
			enc_len = Aes256::encrypt(aes_key, (unsigned char*)msg1, sizeof(msg1), enc);
			enc_to_msg(msg1, enc, sizeof(msg1));
			enc.clear();
			send(Connection, msg1, sizeof(msg1), NULL);
			Sleep(10);
		}
	}
	system("pause");
	return 0;
}