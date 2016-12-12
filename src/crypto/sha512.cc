//
//  sha512.cc
//

/*
 * derived from SUPERCOP: https://bench.cr.yp.to/supercop.html
 * 
 * Copyright (c) 2011 Stanford University.
 * Copyright (c) 2014 Cryptography Research, Inc.
 * Released under the MIT License.  See LICENSE for license information.
 */

#include <cstdint>
#include <cstring>
#include <cassert>

#include "host-endian.h"
#include "sha512.h"

using namespace riscv;

static const uint64_t
sha512_init_state[8] = {
    0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1, 
    0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179
};

static const uint64_t
sha512_k[80] = {
    0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
    0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
    0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
    0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
    0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 
    0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
    0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
    0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
    0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
    0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b, 
    0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
    0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
    0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
    0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
    0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec, 
    0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
    0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
    0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
    0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
    0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817
};

static inline uint64_t rotate_r(uint64_t x, int d) {
    return (x >> d) | (x << (64 - d));
}

static inline uint64_t S0 (uint64_t h1) {
    return rotate_r(h1, 28) ^ rotate_r(h1, 34) ^ rotate_r(h1, 39);
}

static inline uint64_t S1 (uint64_t h4) {
    return rotate_r(h4,14) ^ rotate_r(h4,18) ^ rotate_r(h4,41);
}

static inline uint64_t s0 (uint64_t a) {
    return rotate_r(a,1) ^ rotate_r(a,8) ^ a>>7;
}

static inline uint64_t s1 (uint64_t b) {
    return rotate_r(b,19) ^ rotate_r(b,61) ^ b>>6;
}

static inline uint64_t ch (uint64_t h4, uint64_t h5, uint64_t h6) {
    return h6^(h4 & (h6^h5));
}

static inline uint64_t maj(uint64_t h1, uint64_t h2, uint64_t h3) {
    return (h1&h2) ^ (h3&(h1^h2));
}

static void sha512_process_block(struct sha512_ctx_t *ctx)
{
    uint64_t i, tmp, a, b,
        *w = (uint64_t *) ctx->block,
        *state = ctx->chain,
        h0 = state[0], h1 = state[1], h2 = state[2], h3 = state[3],
        h4 = state[4], h5 = state[5], h6 = state[6], h7 = state[7];

    /* Clang doesn't unswitch this automatically */
    for (i=0; i<16; i++) {
        /* load up the input word for this round */
        tmp = w[i] = htobe64(w[i]);
        tmp = tmp + h7 + S1(h4) + ch(h4,h5,h6) + sha512_k[i];
  
        /* shift register */
        h7 = h6; h6 = h5; h5 = h4;
        h4 = h3 + tmp;
        h3 = h2; h2 = h1; h1 = h0;
        h0 = tmp + maj(h1,h2,h3) + S0(h1);
    }
  
    for (; i<80; i++) {
        /* load up the input word for this round */
        a   = w[(i+1 ) & 15];
        b   = w[(i+14) & 15];
        tmp = w[i&15] = s0(a) + s1(b) + w[i&15] + w[(i+9) & 15];
        tmp = tmp + h7 + S1(h4) + ch(h4,h5,h6) + sha512_k[i];
  
        /* shift register */
        h7 = h6; h6 = h5; h5 = h4;
        h4 = h3 + tmp;
        h3 = h2; h2 = h1; h1 = h0;
        h0 = tmp + maj(h1,h2,h3) + S0(h1);
    }
 
    state[0] += h0;
    state[1] += h1;
    state[2] += h2;
    state[3] += h3;
    state[4] += h4;
    state[5] += h5;
    state[6] += h6;
    state[7] += h7;
}

void riscv::sha512_init(struct sha512_ctx_t *ctx)
{
    ctx->nbytes = 0;
    memcpy(ctx->chain, sha512_init_state, sizeof(sha512_init_state));
    memset(ctx->block, 0, sizeof(ctx->block));
}

void riscv::sha512_update(struct sha512_ctx_t *ctx, const unsigned char *data, uint64_t bytes)
{
    assert(ctx->nbytes < 1ull<<56);
    assert(bytes < 1ull<<56);
    
    while (bytes) {
        uint64_t fill = ctx->nbytes % 128, accept = 128 - fill;
        if (accept > bytes) accept = bytes;
        ctx->nbytes += accept;
        memcpy(ctx->block + fill, data, accept);
        
        if (fill+accept == 128) {
            sha512_process_block(ctx);
        }

        bytes -= accept;
        data += accept;
    }
    
    assert(ctx->nbytes < 1ull<<56);
}

void riscv::sha512_final(struct sha512_ctx_t *ctx, uint8_t result[64])
{
    uint64_t fill = ctx->nbytes % 128, i;
    ctx->block[fill++] = 0x80;
    if (fill > 112) {
        memset(ctx->block + fill, 0, 128-fill);
        sha512_process_block(ctx);
        fill = 0;
    }
    memset(ctx->block + fill, 0, 112-fill);
    
    uint64_t highCount = 0, lowCount = htobe64((ctx->nbytes * 8));
    memcpy(&ctx->block[112],&highCount,8);
    memcpy(&ctx->block[120],&lowCount,8);
    sha512_process_block(ctx);
    for (i=0; i<8; i++) {
        ctx->chain[i] = htobe64(ctx->chain[i]);
    }
    memcpy(result, ctx->chain, sizeof(ctx->chain));
    sha512_init(ctx);
}
