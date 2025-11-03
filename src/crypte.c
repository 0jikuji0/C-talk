#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// Fonction pour calculer (base^exposant) % mod (exponentiation modulaire)
uint64_t mod_exp(uint64_t base, uint64_t exposant, uint64_t mod) {
    uint64_t resultat = 1;
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

// Chiffrement par XOR avec une clé répétée
void xor_chiffrer(const unsigned char *message, unsigned char *chiffre, size_t longueur, uint64_t cle) {
    unsigned char *cle_bytes = (unsigned char *)&cle;
    printf("[xor_chiffrer] Clé en octets : ");
    for (size_t i = 0; i < sizeof(cle); i++) {
        printf("%02x ", cle_bytes[i]);
    }
    printf("\n");
    for (size_t i = 0; i < longueur; i++) {
        printf("[xor_chiffrer] message[%zu] = %02x, cle_bytes[%zu %% %zu] = %02x → XOR = %02x\n",
               i, message[i], i, sizeof(cle), cle_bytes[i % sizeof(cle)], message[i] ^ cle_bytes[i % sizeof(cle)]);
        chiffre[i] = message[i] ^ cle_bytes[i % sizeof(cle)];
    }
}

// Déchiffrement par XOR (identique au chiffrement)
void xor_dechiffrer(const unsigned char *chiffre, unsigned char *dechiffre, size_t longueur, uint64_t cle) {
    xor_chiffrer(chiffre, dechiffre, longueur, cle);
}

int main() {
    // 1. Paramètres publics pour Diffie-Hellman
    uint64_t p = 10000303;  // Nombre premier
    uint64_t g = 10002007;   // Base (générateur)

    // 2. Alice choisit un nombre secret a
    uint64_t a = 99999989;
    uint64_t A = mod_exp(g, a, p);
    printf("[Alice] Envoie A = %lu\n", A);

    // 3. Bob choisit un nombre secret b
    uint64_t b = 1000859;
    uint64_t B = mod_exp(g, b, p);
    printf("[Bob] Envoie B = %lu\n", B);

    // 4. Alice et Bob calculent la clé secrète commune
    uint64_t s_alice = mod_exp(B, a, p);
    uint64_t s_bob = mod_exp(A, b, p);

    if (s_alice != s_bob) {
        printf("Erreur : les clés ne correspondent pas.\n");
        return 1;
    }
    printf("[Alice et Bob] Clé secrète commune : %lu\n", s_alice);

    // 5. Message à chiffrer
    unsigned char message[] = "Bonjour Bob !";
    size_t longueur = strlen((char *)message);
    printf("[Alice] Message original : %s\n", message);

    // 6. Chiffrement du message avec la clé secrète
    unsigned char *chiffre = malloc(longueur);
    xor_chiffrer(message, chiffre, longueur, s_alice);

    printf("[Alice] Message chiffré (hex) : ");
    for (size_t i = 0; i < longueur; i++) {
        printf("%02x ", chiffre[i]);
    }
    printf("\n");

    // 7. Déchiffrement du message avec la clé secrète
    unsigned char *dechiffre = malloc(longueur);
    xor_dechiffrer(chiffre, dechiffre, longueur, s_bob);

    printf("[Bob] Message déchiffré : %s\n", dechiffre);

    // Libération de la mémoire
    free(chiffre);
    free(dechiffre);

    return 0;
}
