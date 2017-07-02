/*
 * cifra - embedded cryptography library
 * Written in 2014 by Joseph Birr-Pixton <jpixton@gmail.com>
 *
 * To the extent possible under law, the author(s) have dedicated all
 * copyright and related and neighboring rights to this software to the
 * public domain worldwide. This software is distributed without any
 * warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication
 * along with this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <strings.h>
#include <assert.h>

typedef struct
{
  uint32_t s[16];
} norx32_ctx;

/* Domain separation constants */
#define DOMAIN_HEADER   0x01
#define DOMAIN_PAYLOAD  0x02
#define DOMAIN_TRAILER  0x04
#define DOMAIN_TAG      0x08

#define WORD_BYTES 4
#define WORD_BITS 32
#define ROUNDS 4
#define DEGREE 1
#define TAG_BITS 128
#define RATE_BYTES 48
#define RATE_WORDS 12


/* Assorted bitwise and common operations used in ciphers. */

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/** Circularly rotate right x by n bits.
 *  0 > n > 32. */
static inline uint32_t rotr32(uint32_t x, unsigned n)
{
  return (x >> n) | (x << (32 - n));
}
/** Read 4 bytes from buf, as a 32-bit little endian quantity. */
static inline uint32_t read32_le(const uint8_t buf[4])
{
  return (buf[3] << 24) |
         (buf[2] << 16) |
         (buf[1] << 8) |
         (buf[0]);
}

/** Encode v as a 32-bit little endian quantity into buf. */
static inline void write32_le(uint32_t v, uint8_t buf[4])
{
  *buf++ = v & 0xff;
  *buf++ = (v >> 8) & 0xff;
  *buf++ = (v >> 16) & 0xff;
  *buf   = (v >> 24) & 0xff;
}

typedef void (*cf_blockwise_in_fn)(void *ctx, const uint8_t *data);

void cf_blockwise_accumulate_final(uint8_t *partial, size_t *npartial, size_t nblock,
                                   const void *inp, size_t nbytes,
                                   cf_blockwise_in_fn process,
                                   cf_blockwise_in_fn process_final,
                                   void *ctx)
{
  const uint8_t *bufin = inp;
  assert(partial && *npartial < nblock);
  assert(inp || !nbytes);
  assert(process && ctx);

  /* If we have partial data, copy in to buffer. */
  if (*npartial && nbytes)
  {
    size_t space = nblock - *npartial;
    size_t taken = MIN(space, nbytes);

    memcpy(partial + *npartial, bufin, taken);

    bufin += taken;
    nbytes -= taken;
    *npartial += taken;

    /* If that gives us a full block, process it. */
    if (*npartial == nblock)
    {
      if (nbytes == 0)
        process_final(ctx, partial);
      else
        process(ctx, partial);
      *npartial = 0;
    }
  }

  /* now nbytes < nblock or *npartial == 0. */

  /* If we have a full block of data, process it directly. */
  while (nbytes >= nblock)
  {
    /* Partial buffer must be empty, or we're ignoring extant data */
    assert(*npartial == 0);

    if (nbytes == nblock)
      process_final(ctx, bufin);
    else
      process(ctx, bufin);
    bufin += nblock;
    nbytes -= nblock;
  }

  /* Finally, if we have remaining data, buffer it. */
  while (nbytes)
  {
    size_t space = nblock - *npartial;
    size_t taken = MIN(space, nbytes);

    memcpy(partial + *npartial, bufin, taken);

    bufin += taken;
    nbytes -= taken;
    *npartial += taken;

    /* If we started with *npartial, we must have copied it
     * in first. */
    assert(*npartial < nblock);
  }
}

void cf_blockwise_accumulate(uint8_t *partial, size_t *npartial, size_t nblock,
                             const void *inp, size_t nbytes,
                             cf_blockwise_in_fn process,
                             void *ctx)
{
  cf_blockwise_accumulate_final(partial, npartial, nblock,
                                inp, nbytes,
                                process, process, ctx);
}

/* cifra norx */

static void permute(norx32_ctx *restrict ctx)
{
  /* This is one quarter of G; the function H plus xor/rotate. */
#define P(u, v, w, rr) \
  (u) = ((u) ^ (v)) ^ (((u) & (v)) << 1); \
  (w) = rotr32((u) ^ (w), rr);

#define G(s, a, b, c, d) \
  P(s[a], s[b], s[d], 8) \
  P(s[c], s[d], s[b], 11) \
  P(s[a], s[b], s[d], 16) \
  P(s[c], s[d], s[b], 31)

  for (int i = 0; i < ROUNDS; i++)
  {
    /* columns */
    G(ctx->s, 0, 4, 8, 12);
    G(ctx->s, 1, 5, 9, 13);
    G(ctx->s, 2, 6, 10, 14);
    G(ctx->s, 3, 7, 11, 15);

    /* diagonals */
    G(ctx->s, 0, 5, 10, 15);
    G(ctx->s, 1, 6, 11, 12);
    G(ctx->s, 2, 7, 8, 13);
    G(ctx->s, 3, 4, 9, 14);
  }

#undef G
#undef P
}

static void init(norx32_ctx *ctx,
                 const uint8_t key[static 16],
                 const uint8_t nonce[static 8])
{
  /* 1. Basic setup */
  ctx->s[0] = read32_le(nonce + 0);
  ctx->s[1] = read32_le(nonce + 4);
  ctx->s[2] = 0xb707322f;
  ctx->s[3] = 0xa0c7c90d;

  ctx->s[4] = read32_le(key + 0);
  ctx->s[5] = read32_le(key + 4);
  ctx->s[6] = read32_le(key + 8);
  ctx->s[7] = read32_le(key + 12);

  ctx->s[8] = 0xa3d8d930;
  ctx->s[9] = 0x3fa8b72c;
  ctx->s[10] = 0xed84eb49;
  ctx->s[11] = 0xedca4787;

  ctx->s[12] = 0x335463eb;
  ctx->s[13] = 0xf994220b;
  ctx->s[14] = 0xbe0bf5c9;
  ctx->s[15] = 0xd7c49104;

  /* 2. Parameter integration
   * w = 32
   * l = 4
   * p = 1
   * t = 128
   */
  ctx->s[12] ^= WORD_BITS;
  ctx->s[13] ^= ROUNDS;
  ctx->s[14] ^= DEGREE;
  ctx->s[15] ^= TAG_BITS;

  permute(ctx);
}

/* Input domain separation constant for next step, and final permutation of
 * preceeding step. */
static void switch_domain(norx32_ctx *ctx, uint32_t constant)
{
  ctx->s[15] ^= constant;
  permute(ctx);
}

typedef struct
{
  norx32_ctx *ctx;
  uint32_t type;
} blockctx;

static void input_block_final(void *vctx, const uint8_t *data)
{
  blockctx *bctx = vctx;
  norx32_ctx *ctx = bctx->ctx;

  /* just xor-in data. */
  for (int i = 0; i < RATE_WORDS; i++)
  {
    ctx->s[i] ^= read32_le(data);
    data += WORD_BYTES;
  }
}

static void input_block(void *vctx, const uint8_t *data)
{
  /* Process block, then prepare for the next one. */
  blockctx *bctx = vctx;
  input_block_final(vctx, data);
  switch_domain(bctx->ctx, bctx->type);
}

static void input(norx32_ctx *ctx, uint32_t type,
                  const uint8_t *buf, size_t nbuf)
{
  uint8_t partial[RATE_BYTES];
  size_t npartial = 0;
  blockctx bctx = { ctx, type };

  /* Process input. */
  cf_blockwise_accumulate(partial, &npartial, sizeof partial,
                          buf, nbuf,
                          input_block,
                          &bctx);

  /* Now pad partial. This contains the trailing portion of buf. */
  memset(partial + npartial, 0, sizeof(partial) - npartial);
  partial[npartial] = 0x01;
  partial[sizeof(partial) - 1] ^= 0x80;

  input_block_final(&bctx, partial);
}

static void do_header(norx32_ctx *ctx, const uint8_t *buf, size_t nbuf)
{
  if (nbuf)
  {
    switch_domain(ctx, DOMAIN_HEADER);
    input(ctx, DOMAIN_HEADER, buf, nbuf);
  }
}

static void do_trailer(norx32_ctx *ctx, const uint8_t *buf, size_t nbuf)
{
  if (nbuf)
  {
    switch_domain(ctx, DOMAIN_TRAILER);
    input(ctx, DOMAIN_TRAILER, buf, nbuf);
  }
}

static void body_block_encrypt(norx32_ctx *ctx,
                               const uint8_t plain[static RATE_BYTES],
                               uint8_t cipher[static RATE_BYTES])
{
  for (int i = 0; i < RATE_WORDS; i++)
  {
    ctx->s[i] ^= read32_le(plain);
    write32_le(ctx->s[i], cipher);
    plain += WORD_BYTES;
    cipher += WORD_BYTES;
  }
}

static void encrypt_body(norx32_ctx *ctx,
                         const uint8_t *plain, uint8_t *cipher, size_t nbytes)
{
  if (nbytes == 0)
    return;

  /* Process full blocks: easy */
  while (nbytes >= RATE_BYTES)
  {
    switch_domain(ctx, DOMAIN_PAYLOAD);
    body_block_encrypt(ctx, plain, cipher);
    plain += RATE_BYTES;
    cipher += RATE_BYTES;
    nbytes -= RATE_BYTES;
  }

  /* Final padded block. */
  uint8_t partial[RATE_BYTES];
  memset(partial, 0, sizeof partial);
  memcpy(partial, plain, nbytes);
  partial[nbytes] ^= 0x01;
  partial[sizeof(partial) - 1] ^= 0x80;

  switch_domain(ctx, DOMAIN_PAYLOAD);
  body_block_encrypt(ctx, partial, partial);

  memcpy(cipher, partial, nbytes);
}

static void body_block_decrypt(norx32_ctx *ctx,
                               const uint8_t cipher[static RATE_BYTES],
                               uint8_t plain[static RATE_BYTES],
                               size_t start, size_t end)
{
  for (size_t i = start; i < end; i++)
  {
    uint32_t ct = read32_le(cipher);
    write32_le(ctx->s[i] ^ ct, plain);
    ctx->s[i] = ct;
    plain += WORD_BYTES;
    cipher += WORD_BYTES;
  }
}

static void undo_padding(norx32_ctx *ctx, size_t bytes)
{
  assert(bytes < RATE_BYTES);
  ctx->s[bytes / WORD_BYTES] ^= 0x01 << ((bytes % WORD_BYTES) * 8);
  ctx->s[RATE_WORDS - 1] ^= 0x80000000;
}

static void decrypt_body(norx32_ctx *ctx,
                         const uint8_t *cipher, uint8_t *plain, size_t nbytes)
{
  if (nbytes == 0)
    return;

  /* Process full blocks. */
  while (nbytes >= RATE_BYTES)
  {
    switch_domain(ctx, DOMAIN_PAYLOAD);
    body_block_decrypt(ctx, cipher, plain, 0, RATE_WORDS);
    plain += RATE_BYTES;
    cipher += RATE_BYTES;
    nbytes -= RATE_BYTES;
  }

  /* Then partial blocks. */
  size_t offset = 0;
  switch_domain(ctx, DOMAIN_PAYLOAD);

  undo_padding(ctx, nbytes);

  /* In units of whole words. */
  while (nbytes >= WORD_BYTES)
  {
    body_block_decrypt(ctx, cipher, plain, offset, offset + 1);
    plain += WORD_BYTES;
    cipher += WORD_BYTES;
    nbytes -= WORD_BYTES;
    offset += 1;
  }

  /* And then, finally, bytewise. */
  uint8_t tmp[WORD_BYTES];
  write32_le(ctx->s[offset], tmp);

  for (size_t i = 0; i < nbytes; i++)
  {
    uint8_t c = cipher[i];
    plain[i] = tmp[i] ^ c;
    tmp[i] = c;
  }

  ctx->s[offset] = read32_le(tmp);
}

static void get_tag(norx32_ctx *ctx, uint8_t tag[static 16])
{
  switch_domain(ctx, DOMAIN_TAG);
  permute(ctx);
  write32_le(ctx->s[0], tag + 0);
  write32_le(ctx->s[1], tag + 4);
  write32_le(ctx->s[2], tag + 8);
  write32_le(ctx->s[3], tag + 12);
}

void cf_norx32_encrypt(const uint8_t key[static 16],
                       const uint8_t nonce[static 8],
                       const uint8_t *header, size_t nheader,
                       const uint8_t *plaintext, size_t nbytes,
                       const uint8_t *trailer, size_t ntrailer,
                       uint8_t *ciphertext,
                       uint8_t tag[static 16])
{
  norx32_ctx ctx;

  init(&ctx, key, nonce);
  do_header(&ctx, header, nheader);
  encrypt_body(&ctx, plaintext, ciphertext, nbytes);
  do_trailer(&ctx, trailer, ntrailer);
  get_tag(&ctx, tag);

  bzero(&ctx, sizeof ctx);
}

int cf_norx32_decrypt(const uint8_t key[static 16],
                      const uint8_t nonce[static 8],
                      const uint8_t *header, size_t nheader,
                      const uint8_t *ciphertext, size_t nbytes,
                      const uint8_t *trailer, size_t ntrailer,
                      const uint8_t tag[static 16],
                      uint8_t *plaintext)
{
  norx32_ctx ctx;
  uint8_t ourtag[16];

  init(&ctx, key, nonce);
  do_header(&ctx, header, nheader);
  decrypt_body(&ctx, ciphertext, plaintext, nbytes);
  do_trailer(&ctx, trailer, ntrailer);
  get_tag(&ctx, ourtag);

  int err = 0;

  if (!memcmp(ourtag, tag, sizeof ourtag))
  {
    err = 1;
    bzero(plaintext, nbytes);
    bzero(ourtag, sizeof ourtag);
  }

  bzero(&ctx, sizeof ctx);
  return err;
}

/* main */

int main()
{
  static const uint8_t key[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
  static const uint8_t nonce[8] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7 };
  static const size_t DATA_SIZE = 32 * 1024 * 1024;
  uint8_t tag[16];
  
  uint8_t *pt1 = malloc(DATA_SIZE);
  uint8_t *ct = malloc(DATA_SIZE);
  uint8_t *pt2 = malloc(DATA_SIZE);

  /* create test pattern */
  char c = 0x01;
  for (size_t j = 0; j < DATA_SIZE; j+= sizeof(int)) {
    pt1[j] = (c ^= c * 7);
  }

  /* encrpyt test pattern */
  bzero(tag, sizeof tag);
  cf_norx32_encrypt(key, nonce, NULL, 0, pt1, DATA_SIZE, NULL, 0, ct, tag);

  /* decrypt test pattern */
  bzero(tag, sizeof tag);
  cf_norx32_decrypt(key, nonce, NULL, 0, ct, DATA_SIZE, NULL, 0, tag, pt2);

  /* compare */
  printf("%d\n", memcmp(pt1, pt2, DATA_SIZE));

  free(pt1);
  free(ct);
  free(pt2);
  return 0;
}
