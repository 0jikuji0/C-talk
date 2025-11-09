/*
 * =============================================================================
 * Fichier : crypto.h
 * Projet  : C-talk - Application de messagerie en C
 * ----------------------------------------------------------------------------
 * Description :
 *   Déclarations des fonctions et structures pour le module crypto.
 *   Gère le chiffrement et déchiffrement des messages.
 * ----------------------------------------------------------------------------
 * Auteur(s) : FortyTwo Dev
 * Date de création : 05/10/2025
 * Dernière modification : 05/10/2025
 * =============================================================================
 */

#ifndef C_TALK_CRYPTO_H
#define C_TALK_CRYPTO_H

#include <stdio.h>
#include <stdint.h>

void encrypt(const char *plaintext, char **ciphertext, uint64_t private_key);

void decrypt(const char *ciphertext, char **decrypted, uint64_t private_key);

void publicParams(uint64_t * p, uint64_t * g);
void privateParams(uint64_t * secret_key);

uint64_t publicKey(uint64_t p, uint64_t g, uint64_t secret_key);
uint64_t privateKey(uint64_t p, uint64_t public_key, uint64_t secret_key);

#endif // C_TALK_CRYPTO_H
