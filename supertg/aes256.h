#ifndef AES256_H
#define AES256_H

#include <cstdint> 
#include <array>  

constexpr size_t AES_BLOCK_SIZE = 16;
constexpr size_t EXPANDED_KEY_SIZE = 60;

void aes256_set_encryption_key(const uint8_t key[32], uint32_t expandedKey[EXPANDED_KEY_SIZE]);
void aes256_set_decryption_key(const uint8_t key[32], uint32_t expandedKey[EXPANDED_KEY_SIZE]);

void aes256_encrypt(const uint8_t in[AES_BLOCK_SIZE], uint8_t out[AES_BLOCK_SIZE], const uint32_t expandedKey[EXPANDED_KEY_SIZE]);
void aes256_decrypt(const uint8_t in[AES_BLOCK_SIZE], uint8_t out[AES_BLOCK_SIZE], const uint32_t expandedKey[EXPANDED_KEY_SIZE]);

#endif  // AES256_H

