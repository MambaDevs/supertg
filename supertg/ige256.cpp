#include "aes256.h"
#include <vector>
#include <array>
#include <cstring> 

uint8_t* ige256(const uint8_t in[], uint32_t length, const uint8_t key[32], const uint8_t iv[32], uint8_t encrypt) {
    std::vector<uint8_t> out(length); 
    std::array<uint8_t, AES_BLOCK_SIZE> iv1, iv2, chunk, buffer;
    std::array<uint32_t, EXPANDED_KEY_SIZE> expandedKey;
    uint32_t i, j;

    std::memcpy(encrypt ? iv1.data() : iv2.data(), iv, AES_BLOCK_SIZE);
    std::memcpy(encrypt ? iv2.data() : iv1.data(), iv + AES_BLOCK_SIZE, AES_BLOCK_SIZE);

    if (encrypt) {
        aes256_set_encryption_key(key, expandedKey.data());
    } else {
        aes256_set_decryption_key(key, expandedKey.data());
    }

    for (i = 0; i < length; i += AES_BLOCK_SIZE) {
        std::memcpy(chunk.data(), &in[i], AES_BLOCK_SIZE);

        for (j = 0; j < AES_BLOCK_SIZE; ++j) {
            buffer[j] = in[i + j] ^ iv1[j];
        }

        if (encrypt) {
            aes256_encrypt(buffer.data(), &out[i], expandedKey.data());
        } else {
            aes256_decrypt(buffer.data(), &out[i], expandedKey.data());
        }

        for (j = 0; j < AES_BLOCK_SIZE; ++j) {
            out[i + j] ^= iv2[j];
        }

        std::memcpy(iv1.data(), &out[i], AES_BLOCK_SIZE);
        std::memcpy(iv2.data(), chunk.data(), AES_BLOCK_SIZE);
    }

    uint8_t* result = new uint8_t[length];
    std::memcpy(result, out.data(), length);
    return result;
}

