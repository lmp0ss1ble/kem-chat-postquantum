/*
  This file is for Niederreiter encryption
*/

#include "encrypt.h"

#include "params.h"
#include "randombytes.h"
#include "util.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "gf.h"

static inline uint8_t same_mask(uint16_t x, uint16_t y) {
    uint32_t mask;

    mask = x ^ y;
    mask -= 1;
    mask >>= 31;
    mask = ~mask + 1;// -mask;

    return (uint8_t)mask;
}

/* output: e, an error vector of weight t */
static void gen_e(unsigned char *e) {
    size_t i, j;
    int eq;

    uint16_t ind[ SYS_T ];
    uint8_t *ind8 = (uint8_t *)ind;
    uint8_t mask;
    unsigned char val[ SYS_T ];

    while (1) {
        randombytes(ind8, sizeof(ind));
        // Copy to uint16_t ind in a little-endian way
        for (i = 0; i < sizeof(ind); i += 2) {
            ind[i / 2] = ind8[i + 1] << 8 | ind8[i];
        }

        for (i = 0; i < SYS_T; i++) {
            ind[i] &= GFMASK;
        }

        // check for repetition

        eq = 0;

        for (i = 1; i < SYS_T; i++) {
            for (j = 0; j < i; j++) {
                if (ind[i] == ind[j]) {
                    eq = 1;
                }
            }
        }

        if (eq == 0) {
            break;
        }
    }

    for (j = 0; j < SYS_T; j++) {
        val[j] = 1 << (ind[j] & 7);
    }

    for (i = 0; i < SYS_N / 8; i++) {
        e[i] = 0;

        for (j = 0; j < SYS_T; j++) {
            mask = same_mask((uint16_t)i, (ind[j] >> 3));

            e[i] |= val[j] & mask;
        }
    }
}

/* input: public key pk, error vector e */
/* output: syndrome s */
static void syndrome(unsigned char *s, const unsigned char *pk, const unsigned char *e) {
    unsigned char b, row[SYS_N / 8];
    const unsigned char *pk_ptr = pk;

    int i, j;

    for (i = 0; i < SYND_BYTES; i++) {
        s[i] = 0;
    }

    for (i = 0; i < PK_NROWS; i++) {
        for (j = 0; j < SYS_N / 8; j++) {
            row[j] = 0;
        }

        for (j = 0; j < PK_ROW_BYTES; j++) {
            row[ SYS_N / 8 - PK_ROW_BYTES + j ] = pk_ptr[j];
        }

        row[i / 8] |= 1 << (i % 8);

        b = 0;
        for (j = 0; j < SYS_N / 8; j++) {
            b ^= row[j] & e[j];
        }

        b ^= b >> 4;
        b ^= b >> 2;
        b ^= b >> 1;
        b &= 1;

        s[ i / 8 ] |= (b << (i % 8));

        pk_ptr += PK_ROW_BYTES;
    }
}

void PQCLEAN_MCELIECE8192128F_CLEAN_encrypt(unsigned char *s, unsigned char *e, const unsigned char *pk) {
    gen_e(e);

    syndrome(s, pk, e);
}

