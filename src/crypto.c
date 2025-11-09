#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// Fonction pour calculer (base^exposant) % mod (exponentiation modulaire)
size_t mod_exp(size_t base, size_t exposant, size_t mod) {
    size_t resultat = 1;
    base = base % mod;
    printf("[mod_exp] Calcul de %lu^%lu %% %lu\n", base, exposant, mod);
    while (exposant > 0) {
        printf("[mod_exp] exposant = %lu (binaire : ", exposant);
        for (int i = 63; i >= 0; i--) printf("%lu", (exposant >> i) & 1);
        printf(")\n");
        if (exposant % 2 == 1) {
            printf("[mod_exp] Bit de poids faible = 1 → resultat = (resultat * base) %% mod\n");
            resultat = (resultat * base) % mod;
        } else {
            printf("[mod_exp] Bit de poids faible = 0 → pas de multiplication\n");
        }
        exposant = exposant >> 1;
        base = (base * base) % mod;
        printf("[mod_exp] Nouvelle base = %lu, nouvel exposant = %lu\n", base, exposant);
    }
    return resultat;
}


void xor_encrypt(const char *plaintext, char *ciphertext, size_t length, size_t key) {
    char *key_bytes = (char *)&key;
    printf("[xor_encrypt] Clé en octets : ");
    for (size_t i = 0; i < sizeof(key); i++) {
        printf("%02x ", key_bytes[i]);
    }
    printf("\n");
    for (size_t i = 0; i < length; i++) {
        printf("[xor_encrypt] plaintext[%zu] = %02x, key_bytes[%zu %% %zu] = %02x → XOR = %02x\n",
               i, plaintext[i], i, sizeof(key), key_bytes[i % sizeof(key)], plaintext[i] ^ key_bytes[i % sizeof(key)]);
        ciphertext[i] = plaintext[i] ^ key_bytes[i % sizeof(key)];
        printf("ciphertext[%zu] = %02x\n", i, ciphertext[i]);
    }
    ciphertext[length] = '\0';
}

void xor_decrypt(const char *ciphertext, char *decrypted, size_t length, size_t key) {
    xor_encrypt(ciphertext, decrypted, length, key);
}

void publicParams(size_t * p, size_t * g) {
    *p = 101;
    *g = 863;
}

void privateParams(size_t * secret_key) {
    *secret_key = 3343;
}

size_t publicKey(size_t p, size_t g, size_t secret_key) {
    size_t public_key = mod_exp(g, secret_key, p);
    printf("[Public Key] %lu\n", public_key);
    return public_key;
}

size_t privateKey(size_t p, size_t public_key, size_t secret_key) {
    size_t private_key = mod_exp(public_key, secret_key, p);
    printf("[Private Key] %lu\n", private_key);
    return private_key;
}

void encrypt(const char *plaintext, char **ciphertext, size_t private_key) {
    size_t length = strlen(plaintext);
    assert((*ciphertext = (char *)malloc(sizeof(char) * length + 1)) != NULL);
    xor_encrypt(plaintext, *ciphertext, length, private_key);
}

void decrypt(const char *ciphertext, char **decrypted, size_t private_key) {
    size_t length = strlen(ciphertext);
    assert((*decrypted = (char *)malloc(sizeof(char) * length + 1)) != NULL);
    xor_decrypt(ciphertext, *decrypted, length, private_key);
}

// int main() {


//     size_t p, g, secret_key, public_key, private_key;
//     publicParams(&p, &g);
//     privateParams(&secret_key);

//     // 2. Alice choisit un nombre secret a
//     // size_t A = mod_exp(g, secret_key, p);
//     // printf("[Alice] Envoie A = %lu\n", A);

//     public_key = publicKey(p, g, secret_key);

//     // 3. Bob choisit un nombre secret b
//     // size_t b = 1000859;
//     // size_t B = mod_exp(g, b, p);
//     // printf("[Bob] Envoie B = %lu\n", B);

//     // 4. Alice et Bob calculent la clé secrète commune
//     // size_t s_alice = mod_exp(B, secret_key, p);
//     // size_t s_bob = mod_exp(public_key, b, p);

//     private_key = privateKey(p, public_key, secret_key);

//     // if (s_alice != s_bob) {
//     //     printf("Erreur : les clés ne correspondent pas.\n");
//     //     return 1;
//     // }
//     // printf("[Alice et Bob] Clé secrète commune : %lu\n", s_alice);

//     // 5. Message à chiffrer
//     // unsigned char message[] = "Bonjour Bob !";
//     // size_t longueur = strlen((char *)message);
//     // printf("[Alice] Message original : %s\n", message);

//     // // 6. Chiffrement du message avec la clé secrète
//     // unsigned char *chiffre = malloc(longueur);
//     // xor_encrypt(message, chiffre, longueur, s_alice);

//     // printf("[Alice] Message chiffré (hex) : ");
//     // for (size_t i = 0; i < longueur; i++) {
//     //     printf("%02x ", chiffre[i]);
//     // }
//     // printf("\n");

//     // // 7. Déchiffrement du message avec la clé secrète
//     // unsigned char *dechiffre = malloc(longueur);
//     // xor_decrypt(chiffre, dechiffre, longueur, s_bob);

//     // printf("[Bob] Message déchiffré : %s\n", dechiffre);

//     // // Libération de la mémoire
//     // free(chiffre);
//     // free(dechiffre);

//     return 0;
// }

    // [xor_encrypt] Clé en octets : 25 00 00 00 00 00 00 00
    // [xor_encrypt] plaintext[0] = 63, key_bytes[0 % 8] = 25 → XOR = 46
    // ciphertext[0] = 46
    // [xor_encrypt] plaintext[1] = 6f, key_bytes[1 % 8] = 00 → XOR = 6f
    // ciphertext[1] = 6f
    // [xor_encrypt] plaintext[2] = 75, key_bytes[2 % 8] = 00 → XOR = 75
    // ciphertext[2] = 75
    // [xor_encrypt] plaintext[3] = 63, key_bytes[3 % 8] = 00 → XOR = 63
    // ciphertext[3] = 63
    // [xor_encrypt] plaintext[4] = 6f, key_bytes[4 % 8] = 00 → XOR = 6f
    // ciphertext[4] = 6f
    // [xor_encrypt] plaintext[5] = 75, key_bytes[5 % 8] = 00 → XOR = 75
    // ciphertext[5] = 75
    // Segmentation fault (core dumped)
//
    // 1d fffffff8 39 00 00 00 00 00
    // [xor_encrypt] plaintext[0] = 63, key_bytes[0 % 8] = 1d → XOR = 7e
    // ciphertext[0] = 7e
    // [xor_encrypt] plaintext[1] = 6f, key_bytes[1 % 8] = fffffff8 → XOR = ffffff97
    // ciphertext[1] = ffffff97
    // [xor_encrypt] plaintext[2] = 75, key_bytes[2 % 8] = 39 → XOR = 4c
    // ciphertext[2] = 4c
    // [xor_encrypt] plaintext[3] = 63, key_bytes[3 % 8] = 00 → XOR = 63
    // ciphertext[3] = 63
    // [xor_encrypt] plaintext[4] = 6f, key_bytes[4 % 8] = 00 → XOR = 6f
    // ciphertext[4] = 6f
    // [xor_encrypt] plaintext[5] = 75, key_bytes[5 % 8] = 00 → XOR = 75
    // ciphertext[5] = 75
    // Segmentation fault (core dumped)
