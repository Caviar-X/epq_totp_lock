#include "hmac.h"
#include <stddef.h>
#include <stdint.h>
#include <time.h>

int32_t compute_totp(const char *secret, size_t secretlen, time_t timestamp,
                     size_t timestep, size_t digits) {
  static const int32_t ddivisor[] = {1,      10,      100,      1000,     10000,
                                     100000, 1000000, 10000000, 100000000};
  if (digits >= sizeof(ddivisor) / sizeof(uint32_t)) {
    return -1;
  }
  uint8_t result[20] = {0};
  size_t result_len = 20;
  time_t counter = timestamp / timestep;
  //* is it little endian? if it is,turn it big
  if ((*(uint16_t *)"\0\1" >> 8)) {
    size_t idx = 0, len = sizeof(time_t);
    time_t output = 0;
    char *pOutput = (char *)&output;
    for (; idx < len; ++idx) {
      pOutput[idx] = (counter >> ((len - idx - 1) << 3)) & 0xff;
    }
    counter = output;
  }
  hmac_sha1((const uint8_t *)secret, secretlen, (uint8_t *)&counter,
            sizeof(time_t), result, &result_len);
  int32_t offset = result[19] & 0x0f;
  int32_t truncated =
      ((result[offset] & 0x7f) << 24) | ((result[offset + 1] & 0xff) << 16) |
      ((result[offset + 2] & 0xff) << 8) | (result[offset + 3] & 0xff);
  return truncated % ddivisor[digits];
}
