//// PROJECT->Properties->Linker->System->Stack Reserve Size=134217728      It is 128Mb
//
//#include <stddef.h>
//#include <stdint.h>
//#include <stdio.h>
//#include <string.h>
//
///*���� �� ���������� � ���� �++ ���� ��, � �� �++, �� ����� ��� ����������, ���� ��������� ��� �����������, ��� ��� ��:*/
//
//extern "C" {
//#include "api.h"
//#include "randombytes.h"
//}
//
//
//#define NTESTS 5
//
//static void printbytes(const uint8_t *x, size_t xlen) {
//    size_t i;
//    for (i = 0; i < xlen; i++) {
//        printf("%d ", x[i]);
//    }
//    printf("\n");
//}
//
//#define PASTER(x, y) x##_##y
//#define EVALUATOR(x, y) PASTER(x, y)
//#define NAMESPACE(fun) EVALUATOR(PQCLEAN_NAMESPACE, fun)
//
//#define CRYPTO_BYTES           PQCLEAN_MCELIECE8192128F_CLEAN_CRYPTO_BYTES
//#define CRYPTO_PUBLICKEYBYTES  PQCLEAN_MCELIECE8192128F_CLEAN_CRYPTO_PUBLICKEYBYTES
//#define CRYPTO_SECRETKEYBYTES  PQCLEAN_MCELIECE8192128F_CLEAN_CRYPTO_SECRETKEYBYTES
//#define CRYPTO_CIPHERTEXTBYTES PQCLEAN_MCELIECE8192128F_CLEAN_CRYPTO_CIPHERTEXTBYTES
//
//
//#define crypto_kem_keypair  PQCLEAN_MCELIECE8192128F_CLEAN_crypto_kem_keypair
//#define crypto_kem_enc      PQCLEAN_MCELIECE8192128F_CLEAN_crypto_kem_enc
//#define crypto_kem_dec      PQCLEAN_MCELIECE8192128F_CLEAN_crypto_kem_dec
//
//
//
//
//
//int main(void) {
//    //��� ���������� (�������), ������� ������ (����� ������� ��������������� ������) ������������ �����. �� ���� - ��,
//    //������� � ������� ����� �������������� ���������� ���. � sharedSecret_a �������_1 ����� ���������� ���� ������� ���
//    //���������, � � sharedSecret_b �������_2 ����� ��������� ������������� ���� �������� 1 ����� ���������� ���������.
//    uint8_t sharedSecret_a[CRYPTO_BYTES], sharedSecret_b[CRYPTO_BYTES];
//
//    //��������� ���� ��� ������������� ��������
//    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
//
//    //����������, � ������� ����� ����������� ������������� ����� (������������� ���������� �� sharedSecret_a)
//    uint8_t cipherText[CRYPTO_CIPHERTEXTBYTES];
//
//    //��������� ���� ������������� ��������
//    uint8_t sk[CRYPTO_SECRETKEYBYTES];
//
//
//    //���������� ��������� � ��������� ����� � ���������� �� � ��������������� ����������
//    PQCLEAN_MCELIECE8192128F_CLEAN_crypto_kem_keypair(pk, sk);
//
//    //���������� ��������� ����� ������ � ���������� ��� � sharedSecret_a. ������� ��� � ������� ���������� �����,
//    //���������� ��������� ���������� � cipherText
//    PQCLEAN_MCELIECE8192128F_CLEAN_crypto_kem_enc(cipherText, sharedSecret_a, pk);
//
//    //�������� cipherText, ��������� ��� � ������� ���������� �����, ���������� ��� � sharedSecret_b. ��� ����������
//    //������ ���������� ���������� sharedSecret_a
//    PQCLEAN_MCELIECE8192128F_CLEAN_crypto_kem_dec(sharedSecret_b, cipherText, sk);
//
//
//
//    //��������
//    printf("\AFTER Decapsulation\n");
//    printf("\sharedSecret_a\n");
//    printbytes(sharedSecret_a, CRYPTO_BYTES);
//    printf("\sharedSecret_b\n");
//    printbytes(sharedSecret_b, CRYPTO_BYTES);
//    printf("\ncipherText\n");
//    printbytes(cipherText, CRYPTO_CIPHERTEXTBYTES);
//    printf("\n\n");
//
//    return 0;
//}
//
///*
//�������:
//� ���� ���������� ������� ��������. ������������ ����� ������ � ����� ������. ������ ���������� (����� �������, ����� -
//�� �������, ��������� ����� ���� �����. � �� ������� ��������� ����� ������ � ���������� ����������� �����.
//����������� ������ � ������� ��������� ������.
//
//� ����� ������ ������������ ������ ���������� ��� ����� �������: ���� � ������ �����������. ��������, ��� ��� � ����
//������������� ������ ���� ���������.
//
//(
//1�� - ������ - ���, ������� ��� ������������.
//2�  - ������, ������� ��� ������� � ���������� ����������.
//)
//
//��� �������
//�� ��� ���� ���������� KEM - Key Encapsulation Mechanism (�������� ������������ �����). �� ������ ����� �������� 
//��� � ���������. ������ ����� �����. ��� �������, �����, ���� �� ����, ��� ����������, ������, ��� ��� ��������.
//*/