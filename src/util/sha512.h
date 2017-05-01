//
//  sha512.h
//

#ifndef rv_sha512_h
#define rv_sha512_h

namespace riscv {

static const int SHA512_OUTPUT_BYTES = 64;

struct sha512_ctx_t
{
    uint64_t chain[8];
    uint8_t block[128];
    uint64_t nbytes;
};

void sha512_init(struct sha512_ctx_t *ctx);
void sha512_update(struct sha512_ctx_t *ctx, const unsigned char *data, uint64_t bytes);
void sha512_final(struct sha512_ctx_t *ctx, uint8_t result[SHA512_OUTPUT_BYTES]);

}

#endif
