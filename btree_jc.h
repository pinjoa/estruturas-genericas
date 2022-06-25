/**
 * @file btree_jc.h
 * @author João Pinto (pinjoa@gmail.com)
 * @brief este interface implementa as estruturas e partilha todas as funções e procedimentos de gestão de uma árvore de utilização genérica
 * @version 0.1
 * @date 2021-05-02
 *
 * @copyright Copyright (c) 2021, João Carlos Pinto
 *
 */


#ifndef INC_09_AED2_V1_BTREE_JC_H
#define INC_09_AED2_V1_BTREE_JC_H

#include <stdbool.h>

/**
 * @brief estrutura principal e básica da árvore genérica
 */
typedef struct btreenode BTreeNode;
struct btreenode {
    void *key;
    BTreeNode *left, *right;
};

/**
 * @brief tipo de assinatura da função comparadora
 */
typedef int (*TfuncComparaBTreeNodoKey)(void*, void*);

/**
 * @brief tipo de assinatura do procedimento para libertar o nodo da árvore
 */
typedef void (*TdestroyBTreeNodoKey)(void*);

/**
 * @brief tipo de assinatura do procedimento para iterar a árvore
 */
typedef void (*TtraverseBTreeProc)(void*, void*);

/**
 * @brief estrutura BTree, contém a definição da lista e personalização dos "comportamentos"
 */
typedef struct btree BTree;
struct btree {
    BTreeNode *root;                        /**< raiz da árvore. */
    BTreeNode *lastSearchMatch;             /**< apontador para o nodo encontrado na última pesquisa. */
    void *lastSearchMatchKey;               /**< apontador para a chave do nodo encontrado na última pesquisa. */
    TfuncComparaBTreeNodoKey comparadorKey; /**< função utilizada para comparar. */
    TdestroyBTreeNodoKey destruidorKey;     /**< procedimento responsável por libertar a memória ocupada pelo nodo na memória. */
};

/**
 * @brief estrutura de cada item individual para determinar 7 nodos para inicializar uma árvore
 */
typedef struct arraysetenodositem ArraySeteNodosItem;
struct arraysetenodositem {
    int posicao;    /**< posição numérica da lista a otimizar. */
    void *dataKey;  /**< apontador para a chave a indexar. */
};

/**
 * @brief constante para os 7 nodos a recolher de uma lista
 */
#define BTREESETENODOS 7 // os sete pontos para tentar criar a árvore balanceada

/**
 * @brief estrutura com os 7 elementos de uma lista para inicializar a árvore
 * NOTA: pretende-se com esta estrutura a criação de uma árvore "quase" balanceada para reduzir ao máximo as iterações nas pesquisas
 * as posições calculadas da lista são: 12%, 25%, 37%, 50%, 62%, 75%, 87%
 */
typedef struct arraystartsetenodos ArrayStartSeteNodos;
struct arraystartsetenodos {
    ArraySeteNodosItem lista[BTREESETENODOS];   /**< array com os 7 dados necessários para criar uma árvore "quase" balanceada. */
};

BTree *newBTree();
BTree *destroyBTree(BTree *lista);
bool insert_BTreeNode(BTree *lista, void *key);
bool searchBTreeByKey(BTree *lista, void *key);
int calcBTreeDepth(BTree *lista);
int calcBTreeSize(BTree *lista);
void traverseBTree(BTree *lista, TtraverseBTreeProc procIterar, void *ctxExterno);
void fakeDestroyBTreeNodeKey(void *key);
ArrayStartSeteNodos *newArrayStartSeteNodos(int listsize);

#endif //INC_09_AED2_V1_BTREE_JC_H
