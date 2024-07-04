//// PROJECT->Properties->Linker->System->Stack Reserve Size=134217728      It is 128Mb
//
//#include <stddef.h>
//#include <stdint.h>
//#include <stdio.h>
//#include <string.h>
//
///*Если мы подключаем к коду с++ файл си, а не с++, то нужно его подключать, явно обозначая для компилятора, что это си:*/
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
//    //Это переменные (массивы), которые хранят (будут хранить сгенерированные неявно) симметричные ключи. То есть - те,
//    //которые в будущем будут использоваться алгоритмом аэс. В sharedSecret_a Абонент_1 будет записывать свой ключАЭС при
//    //генерации, а в sharedSecret_b Абонент_2 будет сохранять дешифрованный ключ абонента 1 после дешифровки сообщения.
//    uint8_t sharedSecret_a[CRYPTO_BYTES], sharedSecret_b[CRYPTO_BYTES];
//
//    //Публичный ключ для криптосистемы МакЭлиса
//    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
//
//    //Переменная, в которую будет сохраняться зашифрованный текст (зашифрованная информация из sharedSecret_a)
//    uint8_t cipherText[CRYPTO_CIPHERTEXTBYTES];
//
//    //Секретный ключ криптосистемы МакЭлиса
//    uint8_t sk[CRYPTO_SECRETKEYBYTES];
//
//
//    //Генерируем секретный и публичный ключи и записываем их в соответствующие переменные
//    PQCLEAN_MCELIECE8192128F_CLEAN_crypto_kem_keypair(pk, sk);
//
//    //Генерируем случайный общий секрет и записываем его в sharedSecret_a. Шифруем его с помощью публичного ключа,
//    //записываем результат шифрования в cipherText
//    PQCLEAN_MCELIECE8192128F_CLEAN_crypto_kem_enc(cipherText, sharedSecret_a, pk);
//
//    //Получаем cipherText, дешифруем его с помощью секретного ключа, записываем его в sharedSecret_b. Эта переменная
//    //должна получиться идентичной sharedSecret_a
//    PQCLEAN_MCELIECE8192128F_CLEAN_crypto_kem_dec(sharedSecret_b, cipherText, sk);
//
//
//
//    //Печатаем
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
//Задание:
//В чате подключить систему МакЭлиса. Генерировать общий секрет с обеих сторон. Дальше передавать (можно печатая, можно -
//не печатая, публичные ключи друг другу. С их помощью шифровать общий секрет и отправлять шифрованный текст.
//Дешифровать тексты с помощью секретных ключей.
//
//В конце каждый пользователь должен напечатать два общих секрета: свой и своего собеседника. Очевидно, что они у двух
//пользователей должны быть идентичны.
//
//(
//1ый - секрет - тот, который был сгенерирован.
//2й  - секрет, который был получен в результате дешифровки.
//)
//
//Что гуглить
//Всё это дело называется KEM - Key Encapsulation Mechanism (механизм инкапсуляции ключа). Ты можешь найти описание 
//его в интернете. Только общую схему. Ищи попроще, чтобы, если не ясно, что происходит, понять, как оно работает.
//*/