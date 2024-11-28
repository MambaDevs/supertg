#include "aes256.h"
#include <vector>
#include <cstring> 

uint8_t *cbc256(const uint8_t in[], uint32_t length, const uint8_t key[32], uint8_t iv[16], uint8_t encrypt) {
    std::vector<uint8_t> out(length);
    uint8_t nextIv[AES_BLOCK_SIZE];
    uint32_t expandedKey[EXPANDED_KEY_SIZE];
    uint32_t i, j;

    std::memcpy(out.data(), in, length);

    if (encrypt) {
        aes256_set_encryption_key(key, expandedKey);

        for (i = 0; i < length; i += AES_BLOCK_SIZE) {
            for (j = 0; j < AES_BLOCK_SIZE; ++j)
                out[i + j] ^= iv[j];

            aes256_encrypt(&out[i], &out[i], expandedKey);
            std::memcpy(iv, &out[i], AES_BLOCK_SIZE);
        }
    } else {
        aes256_set_decryption_key(key, expandedKey);

        for (i = 0; i < length; i += AES_BLOCK_SIZE) {
            std::memcpy(nextIv, &out[i], AES_BLOCK_SIZE);
            aes256_decrypt(&out[i], &out[i], expandedKey);

            for (j = 0; j < AES_BLOCK_SIZE; ++j)
                out[i + j] ^= iv[j];

            std::memcpy(iv, nextIv, AES_BLOCK_SIZE);
        }
    }

    uint8_t *result = new uint8_t[length];
    std::memcpy(result, out.data(), length);
    return result;
}

