/*
  base32.h
  Written By J.Y 2024.12.9
  library for base32 encoding/decoding
*/

#ifndef BASE32_H
#define BASE32_H
/**
  @out dest

  if success return 0
*/
int base32_encode(char *dest, int destlen_chars, const void *srcbits,
                  int srclen_bits);
/**
  @out dest

  if success return 0
*/
int base32_decode(unsigned char *dest, int destlen_bits, const char *src);
#endif