#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <gmp.h>

// Fonction pour calculer (base^exposant) % mod (exponentiation modulaire)
void mod_exp(mpz_t result, mpz_t base, mpz_t exposant, mpz_t mod) {
    mpz_set_ui(result, 1);
    mpz_mod(base, base, mod);
    gmp_printf("[mod_exp] Calcul de %Zd^%Zd %% %Zd\n", base, exposant, mod);

    mpz_t temp_base, temp_exposant;
    mpz_init(temp_base);
    mpz_init(temp_exposant);
    mpz_set(temp_base, base);
    mpz_set(temp_exposant, exposant);

    while (mpz_cmp_ui(temp_exposant, 0) > 0) {
        if (mpz_odd_p(temp_exposant)) {             // (result * temp_base) % mod
            mpz_mul(result, result, temp_base);     // result * temp_base
            mpz_mod(result, result, mod);           // result % mod
        }
        mpz_fdiv_q_ui(temp_exposant, temp_exposant, 2);
        mpz_mul(temp_base, temp_base, temp_base);   // temp_base * temp_base
        mpz_mod(temp_base, temp_base, mod);         // temp_base % mod
    }

    mpz_clear(temp_base);
    mpz_clear(temp_exposant);
}


void xor_encrypt(char *plaintext, char *ciphertext, size_t length, mpz_t key) {
    size_t key_size = (mpz_sizeinbase(key, 2) + 7) / 8;
    unsigned char *key_bytes = malloc(key_size);
    mpz_export(key_bytes, NULL, 1, 1, 0, 0, key);

    printf("[xor_encrypt] Clé en octets : ");
    for (size_t i = 0; i < mpz_sizeinbase(key, 2); i++) {
        printf("%02x ", key_bytes[i]);
    }
    printf("\n");

    for (size_t i = 0; i < length; i++) {
        ciphertext[i] = plaintext[i] ^ key_bytes[i % key_size];
        printf("[xor_encrypt] plaintext[%zu] = %02x, key_bytes[%zu %% %zu] = %02x → XOR = %02x\n",
               i, plaintext[i], i, key_size, key_bytes[i % key_size], ciphertext[i]);
    }
    ciphertext[length] = '\0';
    free(key_bytes);
}

void xor_decrypt(char *ciphertext, char *decrypted, size_t length, mpz_t key) {
    xor_encrypt(ciphertext, decrypted, length, key);
}

void publicParams(mpz_t p, mpz_t g) {
    mpz_set_ui(p, 6997);
    mpz_set_ui(g, 863);
}

void privateParams(mpz_t secret_key) {
    mpz_set_ui(secret_key, 3343);
}

void publicKey(mpz_t public_key, mpz_t p, mpz_t g, mpz_t secret_key) {
    mod_exp(public_key, g, secret_key, p);
    gmp_printf("[Public Key] %Zd\n", public_key);
}

void privateKey(mpz_t private_key, mpz_t p, mpz_t other_public_key, mpz_t secret_key) {
    mod_exp(private_key, other_public_key, secret_key, p);
    gmp_printf("[Private Key] %Zd\n", private_key);
}

void encrypt(char *plaintext, char **ciphertext, mpz_t private_key) {
    size_t length = strlen(plaintext);
    *ciphertext = malloc(length + 1);
    xor_encrypt(plaintext, *ciphertext, length, private_key);
}

void decrypt(char *ciphertext, char **decrypted, mpz_t private_key) {
    size_t length = strlen(ciphertext);
    *decrypted = malloc(length + 1);
    xor_decrypt(ciphertext, *decrypted, length, private_key);
}
