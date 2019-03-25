#include <gtest/gtest.h>
#include <stdio.h>

#include "crypto/blake2/blake2b.h"
#include "crypto/blake2/blake2s.h"

// Deterministic sequences (Fibonacci generator).

static void selftest_seq(uint8_t *out, size_t len, size_t seed) {
  size_t i;
  uint32_t t, a, b;

  a = static_cast<uint32_t>(0xDEAD4BAD * seed);  // prime
  b = 1;

  for (i = 0; i < len; i++) {  // fill the buf
    t = a + b;
    a = b;
    b = t;
    out[i] = static_cast<uint8_t>((t >> 24) & 0xFF);
  }
}

TEST(Blake2b, Correctness) {
  // grand hash of hash results
  const uint8_t blake2b_res[32] = {
      0xC2, 0x3A, 0x78, 0x00, 0xD9, 0x81, 0x23, 0xBD, 0x10, 0xF5, 0x06,
      0xC6, 0x1E, 0x29, 0xDA, 0x56, 0x03, 0xD7, 0x63, 0xB8, 0xBB, 0xAD,
      0x2E, 0x73, 0x7F, 0x5E, 0x76, 0x5A, 0x7B, 0xCC, 0xD4, 0x75};
  // parameter sets
  const size_t b2b_md_len[4] = {20, 32, 48, 64};
  const size_t b2b_in_len[6] = {0, 3, 128, 129, 255, 1024};

  size_t i, j, outlen, inlen;
  uint8_t in[1024], md[64], key[64];
  blake2b_ctx ctx;

  // 256-bit hash for testing
  if (blake2b_init(&ctx, 32, NULL, 0)) {
    FAIL() << "Can not init";
  }

  for (i = 0; i < 4; i++) {
    outlen = b2b_md_len[i];
    for (j = 0; j < 6; j++) {
      inlen = b2b_in_len[j];

      selftest_seq(in, inlen, inlen);  // unkeyed hash
      blake2b(md, outlen, NULL, 0, in, inlen);
      blake2b_update(&ctx, md, outlen);  // hash the hash

      selftest_seq(key, outlen, outlen);  // keyed hash
      blake2b(md, outlen, key, outlen, in, inlen);
      blake2b_update(&ctx, md, outlen);  // hash the hash
    }
  }

  // compute and compare the hash of hashes
  blake2b_final(&ctx, md);

  EXPECT_EQ(memcmp(md, blake2b_res, 32), 0) << "hashes are different";
}

TEST(Blake2s, Correctness) {
  // Grand hash of hash results.
  const uint8_t blake2s_res[32] = {
      0x6A, 0x41, 0x1F, 0x08, 0xCE, 0x25, 0xAD, 0xCD, 0xFB, 0x02, 0xAB,
      0xA6, 0x41, 0x45, 0x1C, 0xEC, 0x53, 0xC5, 0x98, 0xB2, 0x4F, 0x4F,
      0xC7, 0x87, 0xFB, 0xDC, 0x88, 0x79, 0x7F, 0x4C, 0x1D, 0xFE};
  // Parameter sets.
  const size_t b2s_md_len[4] = {16, 20, 28, 32};
  const size_t b2s_in_len[6] = {0, 3, 64, 65, 255, 1024};

  size_t i, j, outlen, inlen;
  uint8_t in[1024], md[32], key[32];
  blake2s_ctx ctx;

  // 256-bit hash for testing.
  if (blake2s_init(&ctx, 32, nullptr, 0)) {
    FAIL() << "Can not init";
  }

  for (i = 0; i < 4; i++) {
    outlen = b2s_md_len[i];
    for (j = 0; j < 6; j++) {
      inlen = b2s_in_len[j];

      selftest_seq(in, inlen, inlen);  // unkeyed hash
      blake2s(md, outlen, nullptr, 0, in, inlen);
      blake2s_update(&ctx, md, outlen);  // hash the hash

      selftest_seq(key, outlen, outlen);  // keyed hash
      blake2s(md, outlen, key, outlen, in, inlen);
      blake2s_update(&ctx, md, outlen);  // hash the hash
    }
  }

  // Compute and compare the hash of hashes.
  blake2s_final(&ctx, md);

  EXPECT_EQ(memcmp(md, blake2s_res, 32), 0) << "hashes are different";
}

TEST(Blake2s, UnkeyedInit) {
  blake2s_ctx ctx1, ctx2;

  blake2s_init(&ctx1, 32, nullptr, 0);
  blake2s_256_init(&ctx2);

  const char *in = "hello";
  size_t len = 5;

  unsigned char out1[32];
  unsigned char out2[32];

  blake2s_update(&ctx1, in, len);
  blake2s_update(&ctx2, in, len);

  blake2s_final(&ctx1, out1);
  blake2s_final(&ctx2, out2);

  EXPECT_EQ(memcmp(out1, out2, 32), 0) << "hashes are different";
}