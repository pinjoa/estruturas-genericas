/**
 * @file hashtable_jc.c
 * @author João Pinto (pinjoa@gmail.com)
 * @brief Implementação de uma lista duplamente ligada sem o tipo de dados definido.
 * Portanto pode ser utilizado como lista genérica para qualquer tipo de dados.
 * @version 0.1
 * @date 2021-04-14
 *
 * @copyright Copyright (c) 2021, João Carlos Pinto
 *
 */

#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include "hashtable_jc.h"
#include "lib_jc.h"

int fakeHashFunc(void *d, void *ctx)
{
    // é sempre zero...
    return 0;
}

void fakeHashDestroy(void *d)
{
    // nadaaa a fazer aquiiii...
}

int htLengthRow(NodoHashTable *nodo, int *colisoes)
{
    int c = 0;
    (*colisoes) = 0;
    while (nodo)
    {
        c++;
        (*colisoes) += nodo->count;
        nodo = nodo->next;
    }
    return c;
}

/**
 * @brief função para inserir elemento na lista da hashtable
 *
 * @param nodo
 * @param data
 * @return NodoHashTable*
 */
NodoHashTable *htHeadInsertNodo(NodoHashTable *nodo, void *data)
{
    NodoHashTable *cell = (NodoHashTable *)malloc(sizeof(NodoHashTable));
    assert(cell);
    cell->next = nodo;
    cell->data = data;
    cell->count = 0;
    return cell;
}

/**
 * @brief função para destruir a hashtable na "horizontal"
 *
 * @param nodo
 * @param dd
 * @return NodoHashTable*
 */
NodoHashTable *htFreeHashTableRow(NodoHashTable *nodo, TfuncHashTableDestroyData dd)
{
    NodoHashTable *ptr = nodo;
    while (nodo)
    {
        dd(nodo->data);
        ptr = nodo->next;
        free(nodo);
        nodo = ptr;
    }
    return NULL;
}

/**
 * @brief função para destruir a hashtable na "vertical"
 *
 * @param ht
 * @return HashTableCFG*
 */
HashTableCFG *destroyHashTable(HashTableCFG *ht)
{
    assert(ht);
    for (int i = 0; i < ht->M; i++)
    {
        ht->hashtable[i] = htFreeHashTableRow(ht->hashtable[i], ht->destroy);
    }
    free(ht);
    return NULL;
}

/**
 * @brief função para calcular os dados estatisticos
 *
 * @param ht
 */
void htStatsCalc(HashTableCFG *ht)
{
    assert(ht);
    int colisoes = 0;
    ht->EmptyRow = 0;
    ht->StatsMax = htLengthRow(ht->hashtable[0], &colisoes);
    if (ht->StatsMax == 0)
        ht->EmptyRow++;
    ht->StatsMin = ht->StatsMax;
    ht->ColisionsMax = colisoes;
    ht->ColisionsMin = ht->ColisionsMax;
    for (int i = 1; i < ht->M; i++)
    {
        int l = htLengthRow(ht->hashtable[i], &colisoes);
        if (l == 0)
            ht->EmptyRow++;
        if (l > ht->StatsMax)
            ht->StatsMax = l;
        if (l < ht->StatsMin)
            ht->StatsMin = l;
        if (colisoes > ht->ColisionsMax)
            ht->ColisionsMax = colisoes;
        if (colisoes < ht->ColisionsMin)
            ht->ColisionsMin = colisoes;
    }
}

// uso interno, não exportar!!!!!
bool htExistStringColision(HashTableCFG *ht, char *v, bool registar)
{
    NodoHashTable *nodo = ht->hashtable[ht->hash(v, ht)];
    ht->lastFound = NULL;
    int cmp = -2; // tem que ser diferente de "0"
    while (nodo && (cmp = strcmp(ht->getString(nodo->data), v)) != 0)
    {
        nodo = nodo->next;
    }
    if (nodo && cmp == 0)
    {
        ht->lastFound = nodo;
        if (registar)
            nodo->count++;
    }
    return nodo ? true : false;
}

/**
 * @brief função para inserir dados na hashtable
 *
 * @param ht
 * @param data
 * @return true
 * @return false
 */
bool htInsertData(HashTableCFG *ht, void *data)
{
    assert(ht);
    assert(ht->hash);
    char *tmpstr = ht->getString(data);
    if (!htExistStringColision(ht, tmpstr, true))
    {
        int pos = ht->hash(ht->getString(data), ht);
        ht->hashtable[pos] = htHeadInsertNodo(ht->hashtable[pos], data);
        ht->nextDataID++;
        return true;
    }
    return false;
}

/**
 * @brief função para verificar se existe uma string na hashtable
 *
 * @param ht
 * @param v
 * @return true
 * @return false
 */
bool htExistString(HashTableCFG *ht, char *v)
{
    assert(ht);
    assert(ht->hash);
    return htExistStringColision(ht, v, false);
}

/**
 * @brief função para inicializar uma hashtable
 *
 * @param m
 * @param fh
 * @param dd
 * @param gs
 * @return HashTableCFG*
 */
HashTableCFG *newHashTable(int m, TfuncHashTableHashFunc fh, TfuncHashTableDestroyData dd, TfuncHashTableGetString gs)
{
    HashTableCFG *novo = (HashTableCFG *)malloc(sizeof(HashTableCFG));
    assert(novo);
    novo->M = getNearestPrimeNumber(m); // procura o número primo mais próximo para reduzir o número de colisões na HT
    novo->StatsMax = novo->StatsMin = novo->ColisionsMax = novo->ColisionsMin = novo->EmptyRow = 0;
    novo->nextDataID = 1;
    novo->hash = fh;
    novo->getString = gs;
    novo->destroy = dd;
    novo->lastFound = NULL;
    novo->hashtable = calloc(m, sizeof(NodoHashTable));
    assert(novo->hashtable);
    return novo;
}
