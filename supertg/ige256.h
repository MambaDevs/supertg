#ifndef IGE256_H
#define IGE256_H

#include <cstdint> 

uint8_t* ige256(const uint8_t in[], uint32_t length, const uint8_t key[32], const uint8_t iv[32], uint8_t encrypt);

#endif  // IGE256_H
