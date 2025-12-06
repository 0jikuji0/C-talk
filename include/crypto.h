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
 * Dernière modification : 1/12/2025
 * =============================================================================
 */

#ifndef C_TALK_CRYPTO_H
#define C_TALK_CRYPTO_H

#include <stdio.h>
#include <stdint.h>
#include <gmp.h>

void encrypt(const char *plaintext, char **ciphertext, const mpz_t private_key);

void decrypt(const char *ciphertext, char **decrypted, const mpz_t private_key);

void publicParams(mpz_t p, mpz_t g);
void privateParams(mpz_t secret_key);

void publicKey(mpz_t public_key, const mpz_t p, const mpz_t g, const mpz_t secret_key);
void privateKey(mpz_t private_key, const mpz_t p, const mpz_t other_public_key, const mpz_t secret_key);

#endif // C_TALK_CRYPTO_H
