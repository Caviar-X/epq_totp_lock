#include <ctype.h>
const char base32_map[33] = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";

/**
  No type safety
 */
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/**
  return sym value or -1 if not found
 */
static int decode_sym(int sym) {
  int i;
  for (i = 0; i < 32; i++) {
    if (sym == base32_map[i]) {
      return i;
    }
  }
  return -1;
}

int base32_encode(char *dest, int destlen_chars, const void *srcbits,
                  int srclen_bits) {
  const unsigned char *src = (const unsigned char*)srcbits;
  int dstlen_need = (srclen_bits + 4) / 5 + 1, i, didx = 0;
  *dest = 0;
  if (destlen_chars < dstlen_need) {
    return 0;
  }
  for (i = 0; i < srclen_bits; i += 5) {
    int sym, sidx = i / 8, bit_offs = i % 8;
    if (bit_offs <= 3) {
      sym = src[sidx] >> (3 - bit_offs);
    } else {
      sym = src[sidx] << (bit_offs - 3);
      if (i + 1 < srclen_bits) {
        sym |= src[sidx + 1] >> (11 - bit_offs);
      }
    }
    sym &= 0x1f;
    if (srclen_bits - i < 5) {
      sym &= 0x1f << (5 + i - srclen_bits);
    }
    dest[didx++] = base32_map[sym];
  }
  for (i = 0; i < (8 - (didx % 8)) % 8; i++) {
    dest[didx++] = '=';
  }
  dest[didx] = '\0';
  return 0;
}
int base32_decode(unsigned char *dest, int destlen_bits, const char *src) {
  int out_bits = 0;
  for (; *src; src++) {
    int sym, sbits, dbits, b;
    if (isspace(*src) || *src == '-') {
      continue;
    }
    sym = decode_sym(*src);
    if (sym < 0) {
      return -1;
    }
    if (out_bits >= destlen_bits) {
      break;
    }
    sbits = MIN(5, destlen_bits - out_bits);
    if (sbits < 5) {
      sym >>= (5 - sbits);
    }
    dbits = 8 - (out_bits & 7);
    b = MIN(dbits, sbits);
    if (dbits == 8) {
      dest[out_bits / 8] = 0;
    }
    dest[out_bits / 8] |= (sym << (dbits - b)) >> (sbits - b);
    out_bits += b;
    sbits -= b;
    if (sbits > 0) {
      dest[out_bits / 8] = sym << (8 - sbits);
      out_bits += sbits;
    }
  }
  return out_bits;
}