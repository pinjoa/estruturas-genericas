/**
 * @file btree_jc.c
 * @author João Pinto (pinjoa@gmail.com)
 * @brief este módulo implementa uma árvore de utilização genérica
 * @version 0.1
 * @date 2021-05-02
 *
 * @copyright Copyright (c) 2021, João Carlos Pinto
 *
 */

#include <malloc.h>
#include <assert.h>
#include "btree_jc.h"

/**
 * @brief função responsável por criar um novo node (NOTA: é uma função interna)
 * @param key
 * @return novo node
 */
BTreeNode *newBTreeNode(void *key) {
    BTreeNode *novo=(BTreeNode*)malloc(sizeof(BTreeNode));
    assert(novo);
    novo->key=key;
    novo->left=novo->right=NULL;
    return novo;
}

/**
 * @brief função responsável por libertar memória de um node (NOTA: é uma função interna)
 * @param node
 * @param destroyer
 * @return NULL
 */
BTreeNode *destroyBTreeNode(BTreeNode *node, TdestroyBTreeNodoKey destroyer) {
    assert(node);
    destroyer(node->key);
    free(node);
    return NULL;
}

/**
 * @brief procedimento recursivo responsável por destruir a árvore (NOTA: é um procedimento interno)
 * @param node
 * @param destroyer
 */
void destroyBTree_rec(BTreeNode *node, TdestroyBTreeNodoKey destroyer) {
    if (node) {
        destroyBTree_rec(node->left, destroyer);
        destroyBTree_rec(node->right, destroyer);
        destroyBTreeNode(node, destroyer);
    }
}

/**
 * função responsável por inserir um nodo na árvore
 * @param lista
 * @param key
 * @return True/False
 */
bool insert_BTreeNode(BTree *lista, void *key) {
    assert(lista);
    bool resultado=false;
    lista->lastSearchMatch=NULL;
    lista->lastSearchMatchKey=NULL;
    if (!lista->root) {
        // inserir na cabeça da lista porque está vazia
        lista->root=newBTreeNode(key);
        resultado=true;
    } else {
        // garantir que o "comparador" está atribuido
        assert(lista->comparadorKey);
        BTreeNode *aux=lista->root;
        int r=0;
        while (aux) {
            // aproveitar o resultado da comparação para usar no "else"
            if ( (r=lista->comparadorKey(key, aux->key))<0 ) {
                if (!aux->left) {
                    // inserir no local livre
                    aux->left=newBTreeNode(key);
                    resultado=true;
                    // sair do ciclo...
                    aux=NULL;
                } else {
                    // continuar a procurar
                    aux=aux->left;
                }
            } else if (r>0) {
                if (!aux->right) {
                    // inserir no local livre
                    aux->right=newBTreeNode(key);
                    resultado=true;
                    // sair do ciclo...
                    aux=NULL;
                } else {
                    // continuar a procurar
                    aux=aux->right;
                }
            } else {
                // é igual e não se aceitam chaves iguais...
                // preservar "lastSearchMatch"...
                lista->lastSearchMatch=aux;
                lista->lastSearchMatchKey=aux->key;
                aux=NULL;
            }
        }
    }
    return resultado;
}

/**
 * @brief função de pesquisa de uma chave na árvore (NOTA: é uma função interna)
 * @param root
 * @param key
 * @param comparadorKey
 * @return
 */
BTreeNode *searchBTreeNode(BTreeNode *root, void *key, TfuncComparaBTreeNodoKey comparadorKey) {
    if (root) {
        int r=comparadorKey(key, root->key);
        if (r<0)
            return searchBTreeNode(root->left, key, comparadorKey);
        else if (r>0)
            return searchBTreeNode(root->right, key, comparadorKey);
        else
            return root;
    } else {
        return NULL;
    }
}

/**
 * @brief é uma função de pesquisa de uma key na árvore (NOTA: é uma função interna) o resultado é enviado pela variável "lastSearchMatch"
 * @param lista
 * @param key
 * @return True/False
 */
bool searchBTreeByKey(BTree *lista, void *key) {
    assert(lista);
    // garantir que o "comparador" está atribuido
    assert(lista->comparadorKey);
    bool resultado=false;
    lista->lastSearchMatch=NULL;
    lista->lastSearchMatchKey=NULL;
    BTreeNode *aux=searchBTreeNode(lista->root, key, lista->comparadorKey);
    if (aux) {
        lista->lastSearchMatch=aux;
        lista->lastSearchMatchKey=aux->key;
        resultado=true;
    }
    return resultado;
}

/**
 * @brief função responsável por criar e inicializar uma lista
 * @return
 */
BTree *newBTree() {
    BTree *novo=(BTree*)malloc(sizeof(BTree));
    assert(novo);
    novo->root=NULL;
    novo->lastSearchMatch=NULL;
    novo->lastSearchMatchKey=NULL;
    // este valor é necessário atribuir no código que utilizar a lista
    novo->comparadorKey=NULL;
    // assume-se inicialmente que a lista não liberta memória do "key" porque a BTree é um indexador de apoio
    novo->destruidorKey=&fakeDestroyBTreeNodeKey;
    return novo;
}

/**
 * @brief é uma função responsável por libertar e destruir a lista completa da memória
 * @param lista
 * @return
 */
BTree *destroyBTree(BTree *lista) {
    assert(lista);
    destroyBTree_rec(lista->root, lista->destruidorKey);
    return NULL;
}

void fakeDestroyBTreeNodeKey(void *key) {
    // é fake! portanto nada a fazer aqui... ;-)
}

/**
 * @brief esta função recursiva calcula a profundidade da árvore (NOTA: esta função é interna)
 * @param node
 * @return total de níveis
 */
int calcBTreeNodeDepth(BTreeNode *node) {
    int resultado=0;
    if (node) {
        int dLeft=calcBTreeNodeDepth(node->left);
        int dRight=calcBTreeNodeDepth(node->right);
        resultado = (dRight>dLeft) ? (1+dRight) : (1+dLeft);
    }
    return resultado;
}

/**
 * @brief esta função calcula a profundidade da árvore
 * @param lista
 * @return total de níveis
 */
int calcBTreeDepth(BTree *lista) {
    assert(lista);
    return calcBTreeNodeDepth(lista->root);
}

/**
 * @brief esta função recursiva calcula o tamanho(nº de elementos) da árvore (NOTA: esta função é interna)
 * @param node
 * @return total de elementos
 */
int calcBTreeNodeSize(BTreeNode *node) {
    int resultado=0;
    if (node) {
        int dLeft=calcBTreeNodeSize(node->left);
        int dRight=calcBTreeNodeSize(node->right);
        resultado=1+dLeft+dRight;
    }
    return resultado;
}

/**
 * @brief esta função calcula o tamanho(nº de elementos) da árvore
 * @param lista
 * @return total de elementos
 */
int calcBTreeSize(BTree *lista) {
    assert(lista);
    return calcBTreeNodeSize(lista->root);
}

/**
 * @brief procedimento recursivo para iterar a árvore executando um procedimento com um contexto  (NOTA: este procedimento é interno)
 * @param node
 * @param procIterar
 * @param ctxExterno
 */
void traverseBTree_rec(BTreeNode* node, TtraverseBTreeProc procIterar, void *ctxExterno) {
    if (node) {
        procIterar(node->key, ctxExterno);
        traverseBTree_rec(node->left, procIterar, ctxExterno);
        traverseBTree_rec(node->right, procIterar, ctxExterno);
    }
}

/**
 * @brief procedimento para iterar a árvore executando um procedimento com um contexto
 * @param lista
 * @param procIterar
 * @param ctxExterno
 */
void traverseBTree(BTree *lista, TtraverseBTreeProc procIterar, void *ctxExterno) {
    traverseBTree_rec(lista->root, procIterar, ctxExterno);
}

ArrayStartSeteNodos *newArrayStartSeteNodos(int listsize) {
    ArrayStartSeteNodos *novo=(ArrayStartSeteNodos*)malloc(sizeof(ArrayStartSeteNodos));
    assert(novo);
    // TODO: retomar a este assunto para avaliar opções de otimização
    novo->lista[0].posicao=(int)(listsize*0.12f);
    novo->lista[0].dataKey=NULL;

    novo->lista[1].posicao=(int)(listsize*0.25f);
    novo->lista[1].dataKey=NULL;

    novo->lista[2].posicao=(int)(listsize*0.37f);
    novo->lista[2].dataKey=NULL;

    novo->lista[3].posicao=(int)(listsize*0.5f);
    novo->lista[3].dataKey=NULL;

    novo->lista[4].posicao=(int)(listsize*0.62f);
    novo->lista[4].dataKey=NULL;

    novo->lista[5].posicao=(int)(listsize*0.75f);
    novo->lista[5].dataKey=NULL;

    novo->lista[6].posicao=(int)(listsize*0.87f);
    novo->lista[6].dataKey=NULL;

    return novo;
}
