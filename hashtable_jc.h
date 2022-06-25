/**
 * @file hashtable_jc.h
 * @author João Pinto (pinjoa@gmail.com)
 * @brief Implementação de uma lista duplamente ligada sem o tipo de dados definido.
 * Portanto pode ser utilizado como lista genérica para qualquer tipo de dados.
 * @version 0.1
 * @date 2021-04-14
 *
 * @copyright Copyright (c) 2021, João Carlos Pinto
 *
 */


#ifndef INC_14AED2HASH_HASH_JC_H
#define INC_14AED2HASH_HASH_JC_H

#include <stdbool.h>

typedef struct nodohashtable NodoHashTable;
struct nodohashtable {
    void *data;
    int count;
    NodoHashTable *next;
};

typedef int (*TfuncHashTableHashFunc)(void*, void*);
typedef void (*TfuncHashTableDestroyData)(void*);
typedef char *(*TfuncHashTableGetString)(void*);

typedef struct hashtablecfg HashTableCFG;
struct hashtablecfg {
    int M, StatsMax, StatsMin, ColisionsMax, ColisionsMin, EmptyRow;
    int nextDataID;
    NodoHashTable **hashtable;
    NodoHashTable *lastFound;
    TfuncHashTableHashFunc hash;
    TfuncHashTableGetString getString;
    TfuncHashTableDestroyData destroy;
};

int fakeHashFunc(void *d, void *ctx);
void fakeHashDestroy(void *d);

HashTableCFG *newHashTable(int m, TfuncHashTableHashFunc fh, TfuncHashTableDestroyData dd, TfuncHashTableGetString gs);
HashTableCFG *destroyHashTable(HashTableCFG *ht);

bool htInsertData(HashTableCFG *ht, void *data);
bool htExistString(HashTableCFG *ht, char *v);
void htStatsCalc(HashTableCFG *ht);

#endif //INC_14AED2HASH_HASH_JC_H
