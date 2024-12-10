/**
*  @file totp.h
*  written by J.Y (Synth Magic) 2024.12.10
*/

#ifndef TOTP_H
#define TOTP_H

#include <stdint.h>

int32_t compute_totp(const char *secret, size_t secretlen, time_t timestamp,
                     size_t timestep, size_t digits);

#endif // TOTP_H