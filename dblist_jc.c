/**
 * @file dblist_jc.c
 * @author João Pinto (pinjoa@gmail.com)
 * @brief Implementação de uma lista duplamente ligada sem o tipo de dados definido.
 * Portanto pode ser utilizado como lista genérica para qualquer tipo de dados.
 * @version 0.1
 * @date 2021-04-12
 *
 * @copyright Copyright (c) 2021, João Carlos Pinto
 *
 */

#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "dblist_jc.h"

/**
 * @brief cria um novo nodo para a lista
 *
 * @param dados     apontador para os dados
 * @return  novo nodo
 */
NodoDBLGenerico *newNodoDBL(void *dados) {
    NodoDBLGenerico *novo=(NodoDBLGenerico*)malloc(sizeof(NodoDBLGenerico));
    assert(novo);
    novo->dadosPtr=dados;
    novo->next=NULL;
    novo->previous=NULL;
    return novo;
}

/**
 * @brief função responsável pela inserção do nodo na cabeça da lista sem ordenação dos dados.
 * NOTA: esta função é interna e não deve ser exportada!
 *
 * @return novo nodo inserido
 */
NodoDBLGenerico *headInsertNodoDBL(CfgDBLGenerica *dbl, void *dados) {
    NodoDBLGenerico *novo=newNodoDBL(dados);
    novo->next           =dbl->n0d0->next;
    novo->previous       =dbl->n0d0;
    novo->next->previous =novo;
    novo->previous->next =novo;
    dbl->totalItems++;
    dbl->lastModified    =novo;
    dbl->lastResult      =OK;
    return novo;
}

/**
 * @brief função responsável pela inserção ordenada de um nodo na lista.
 * NOTA: esta função é interna e não deve ser exportada!
 * @param dbl
 * @param dados
 * @param lastNearestRecord
 * @return NULL ou novo nodo inserido
 */
NodoDBLGenerico *insertOrdenadoNodoDBL(CfgDBLGenerica *dbl, void *dados, NodoDBLGenerico *lastNearestRecord) {
    NodoDBLGenerico *aux=dbl->n0d0->next;
    int r=-1;
    if (lastNearestRecord) {
        aux=lastNearestRecord;
    }
    dbl->lastResult=NOACTION;
    dbl->lastSearchMatch=NULL;
    dbl->lastModified=NULL;
    // iterar a lista para se posicionar... deve parar a pesquisa quando for igual ou maior...
    while (aux!=dbl->n0d0 && (r=dbl->comparador(aux->dadosPtr, dados))<0) {
        aux=aux->next;
    }
    // testar se é repetido e verificar se permite dados repetidos
    //if (aux!=dbl->n0d0 && (dbl->tipoDados==UNICOS && dbl->comparador(aux->dadosPtr, dados)==0)) {
    if (aux!=dbl->n0d0 && (dbl->tipoDados==UNICOS && r==0)) {
        dbl->lastResult=DUPLICADO;
        dbl->lastSearchMatch=aux;
        return NULL;
    }
    NodoDBLGenerico *novo=newNodoDBL(dados);
    novo->next          =aux;
    novo->previous      =aux->previous;
    novo->next->previous=novo;
    novo->previous->next=novo;
    dbl->totalItems++;
    dbl->lastModified   =novo;
    dbl->lastResult     =OK;
    return novo;
}

/**
 * @brief função responsável pela inserção de um nodo na lista.
 * NOTA: esta função verifica se a lista é ordenada e executa o método de inserção adequado!
 *
 * @param dbl
 * @param dados
 * @return configuração da lista
 */
CfgDBLGenerica *insertNodoDBLGenerica(CfgDBLGenerica *dbl, void *dados, NodoDBLGenerico *lastNearestRecord) {
    dbl->lastResult=NOACTION;
    dbl->lastSearchMatch=NULL;
    dbl->lastModified=NULL;
    switch (dbl->tipoOrdemDados) {
        case NO:
            // inserção normal no inicio da lista
            headInsertNodoDBL(dbl, dados);
            return dbl;
            break;
        case O1:
            // garantir que está configurada a função que faz as comparações de dados para inserção ordenada dos dados
            //assert(dbl->comparador);
            insertOrdenadoNodoDBL(dbl, dados, lastNearestRecord);
            return dbl;
            break;
        default:
            return dbl;
    }
}

/**
 * @brief função responsável pela inserção de um nodo na lista.
 * NOTA: esta função verifica se a lista é ordenada e executa o método de inserção adequado!
 *
 * @param dbl
 * @param dados
 * @return  NULL ou apontador dadosPtr
 */
void *insertNodoDBLGenericaGetDataPtr(CfgDBLGenerica *dbl, void *dados, NodoDBLGenerico *lastNearestRecord) {
    NodoDBLGenerico *aux=NULL;
    dbl->lastResult=NOACTION;
    dbl->lastSearchMatch=NULL;
    dbl->lastModified=NULL;
    switch (dbl->tipoOrdemDados) {
        case NO:
            // inserção normal no inicio da lista
            aux=headInsertNodoDBL(dbl, dados);
            break;
        case O1:
            // garantir que está configurada a função que faz as comparações de dados para inserção ordenada dos dados
            //assert(dbl->comparador);
            aux=insertOrdenadoNodoDBL(dbl, dados, lastNearestRecord);
            break;
        default:
            return NULL;
    }
    return aux ? aux->dadosPtr : NULL;
}

/**
 * @brief função responsável pesquisa de um nodo próximo do local para inserção ordenada de um nodo na lista.
 * NOTA: esta função é interna e não deve ser exportada!
 *
 * @return configuração da lista
 */
CfgDBLGenerica *searchNodoDBLGenerica(CfgDBLGenerica *dbl, void *dados) {
    int r=-1, iterou1vez=0;
    NodoDBLGenerico *aux=dbl->n0d0->next;
    dbl->lastResult=NOACTION;
    dbl->lastSearchMatch=NULL;
    // iterar a lista para se posicionar... deve parar a pesquisa quando for igual ou maior...
    while (aux!=dbl->n0d0 && (r=dbl->comparador(aux->dadosPtr, dados))<0) {
        iterou1vez++;
        aux=aux->next;
    }
    if (aux!=dbl->n0d0) {
        iterou1vez++;
    }
    if (iterou1vez>0) {
        // verificar e ajustar o resultado da comparação
        if (r<0) {
            dbl->lastResult=ISSMALLER;
        } else if (r>0) {
            dbl->lastResult=ISBIGGER;
        } else {
            dbl->lastResult=POSENCONTRADO;
        }
        dbl->lastSearchMatch=(r==0 ? aux : NULL);
    }
    return dbl;
}

/**
 * função responsável por posicionar no primeiro nodo da lista que satisfaça a condição.
 * ATENÇÃO: esta função utiliza a ordeação da lista e compara apenas "<", pára o ciclo quando for igual ou maior
 * @param dbl
 * @param dados
 * @return
 */
NodoDBLGenerico *posicionaPrimeiroNodoDBLordenado(CfgDBLGenerica *dbl, void *dados) {
    int r=-1, iterou1vez=0;
    NodoDBLGenerico *aux=dbl->n0d0->next;
    dbl->lastResult=NOACTION;
    dbl->lastSearchMatch=NULL;
    // iterar a lista para se posicionar... deve parar a pesquisa quando for igual...
    while (aux!=dbl->n0d0 && (r=dbl->comparador(aux->dadosPtr, dados))<0) {
        iterou1vez++;
        aux=aux->next;
    }
    if (aux!=dbl->n0d0) {
        iterou1vez++;
    }
    if (iterou1vez>0) {
        // verificar e ajustar o resultado da comparação
        if (r==0) {
            dbl->lastResult=POSENCONTRADO;
            dbl->lastSearchMatch=aux;
        }
    }
    return dbl->lastSearchMatch;
}

/**
 * função para posicionar no próximo nodo, enquanto for igual (i.e. enquanto a comparação==0)
 * ATENÇÃO: esta função utiliza apenas a ordem da lista e compara apenas "<=" pára quando for maior
 *
 * @param dbl
 * @param anterior
 * @param dados
 * @return
 */
NodoDBLGenerico *posicionaProximoNodoDBLordenado(CfgDBLGenerica *dbl, NodoDBLGenerico *anterior, void *dados) {
    assert(anterior);
    int iterou1vez=0, r=-2;
    dbl->lastSearchMatch=NULL;
    dbl->lastResult=NOACTION;
    NodoDBLGenerico *aux=anterior->next;
    // iterar a lista para se posicionar... deve parar a pesquisa quando for igual...
    while (aux!=dbl->n0d0 && (r=dbl->comparador(aux->dadosPtr, dados))<=0) {
        iterou1vez++;
        aux=aux->next;
    }
    if (aux) {
        if (r==0) {
            dbl->lastResult=POSENCONTRADO;
            dbl->lastSearchMatch=aux;
        } else {
            // verificar se chegou ao final da lista
            if (aux==dbl->n0d0) {
                dbl->lastResult=ENDLIST;
            }
            aux=NULL;
        }
    }
    return aux;
}

/**
 * função responsável por posicionar no primeiro nodo da lista que satisfaça a condição.
 * NOTA: esta função utiliza apenas a ordem da lista e compara apenas a igualdade, pode ter que percorrer toda a lista
 *
 * @param dbl
 * @param dados
 * @return
 */
NodoDBLGenerico *posicionaPrimeiroNodoDBLnooreder(CfgDBLGenerica *dbl, void *dados) {
    int r=-1, iterou1vez=0;
    NodoDBLGenerico *aux=dbl->n0d0->next;
    dbl->lastResult=NOACTION;
    dbl->lastSearchMatch=NULL;
    // iterar a lista para se posicionar... deve parar a pesquisa quando for igual...
    while (aux!=dbl->n0d0 && (r=dbl->comparador(aux->dadosPtr, dados))!=0) {
        iterou1vez++;
        aux=aux->next;
    }
    if (aux!=dbl->n0d0) {
        iterou1vez++;
    }
    if (iterou1vez>0) {
        // verificar e ajustar o resultado da comparação
        if (r==0) {
            dbl->lastResult=POSENCONTRADO;
            dbl->lastSearchMatch=aux;
        }
    }
    return dbl->lastSearchMatch;
}

/**
 * função responsável por posicionar no primeiro nodo da lista que satisfaça a condição.
 * ATENÇÃO: esta função verifica se a lista está ordenada e utiliza o algoritmo para cada contexto
 *
 * @param dbl
 * @param dados
 * @return
 */
NodoDBLGenerico *posicionaPrimeiroNodoDBL(CfgDBLGenerica *dbl, void *dados) {
    dbl->lastResult=NOACTION;
    dbl->lastSearchMatch=NULL;
    switch (dbl->tipoOrdemDados) {
        case NO:
            // utilizar a ordem de criação da lista
            return posicionaPrimeiroNodoDBLnooreder(dbl, dados);
            break;
        case O1:
            // utilizar a ordenação
            return posicionaPrimeiroNodoDBLordenado(dbl, dados);
            break;
        default:
            return dbl->lastSearchMatch;
    }
}

/**
 * função para posicionar no próximo nodo, enquanto for igual (i.e. enquanto a comparação==0)
 * ATENÇÃO: esta função utiliza apenas a ordem da lista e compara apenas a igualdade, pode ter que percorrer toda a lista
 *
 * @param dbl
 * @param anterior
 * @param dados
 * @return
 */
NodoDBLGenerico *posicionaProximoNodoDBLnooreder(CfgDBLGenerica *dbl, NodoDBLGenerico *anterior, void *dados) {
    assert(anterior);
    int iterou1vez=0, r=-2;
    dbl->lastSearchMatch=NULL;
    dbl->lastResult=NOACTION;
    NodoDBLGenerico *aux=anterior->next;
    // iterar a lista para se posicionar... deve parar a pesquisa quando for igual...
    while (aux!=dbl->n0d0 && (r=dbl->comparador(aux->dadosPtr, dados))!=0) {
        iterou1vez++;
        aux=aux->next;
    }
    if (aux) {
        if (r==0) {
            dbl->lastResult=POSENCONTRADO;
            dbl->lastSearchMatch=aux;
        } else {
            // verificar se chegou ao final da lista
            if (aux==dbl->n0d0) {
                dbl->lastResult=ENDLIST;
            }
            aux=NULL;
        }
    }
    return aux;
}

/**
 * função para posicionar no próximo nodo, enquanto for igual (i.e. enquanto a comparação==0)
 * ATENÇÃO: esta função verifica se a lista está ordenada e utiliza o algoritmo para cada contexto
 *
 * @param dbl
 * @param anterior
 * @param dados
 * @return
 */
NodoDBLGenerico *posicionaProximoNodoDBL(CfgDBLGenerica *dbl, NodoDBLGenerico *anterior, void *dados) {
    assert(anterior);
    dbl->lastResult=NOACTION;
    dbl->lastSearchMatch=NULL;
    switch (dbl->tipoOrdemDados) {
        case NO:
            // utilizar a ordem de criação da lista
            return posicionaProximoNodoDBLnooreder(dbl, anterior, dados);
            break;
        case O1:
            // utilizar a ordenação
            return posicionaProximoNodoDBLordenado(dbl, anterior, dados);
            break;
        default:
            return dbl->lastSearchMatch;
    }
}

/**
 * função para posicionar no nodo anterior, enquanto for igual (i.e. enquanto a comparação==0)
 * ATENÇÃO: esta função utiliza apenas a ordem da lista e compara apenas "<=" pára quando for maior
 *
 * @param dbl
 * @param anterior
 * @param dados
 * @return
 */
NodoDBLGenerico *posicionaAnteriorNodoDBLordenado(CfgDBLGenerica *dbl, NodoDBLGenerico *seguinte, void *dados) {
    assert(seguinte);
    int iterou1vez=0, r=-2;
    dbl->lastSearchMatch=NULL;
    dbl->lastResult=NOACTION;
    NodoDBLGenerico *aux=seguinte->previous;
    // iterar a lista para se posicionar... deve parar a pesquisa quando for igual...
    while (aux!=dbl->n0d0 && (r=dbl->comparador(aux->dadosPtr, dados))<=0) {
        iterou1vez++;
        aux=aux->previous;
    }
    if (aux) {
        if (r==0) {
            dbl->lastResult=POSENCONTRADO;
            dbl->lastSearchMatch=aux;
        } else {
            // verificar se chegou ao final da lista
            if (aux==dbl->n0d0) {
                dbl->lastResult=ENDLIST;
            }
            aux=NULL;
        }
    }
    return aux;
}

/**
 * função para posicionar no nodo anterior, enquanto for igual (i.e. enquanto a comparação==0)
 * ATENÇÃO: esta função utiliza apenas a ordem da lista e compara apenas a igualdade, pode ter que percorrer toda a lista
 *
 * @param dbl
 * @param anterior
 * @param dados
 * @return
 */
NodoDBLGenerico *posicionaAnteriorNodoDBLnooreder(CfgDBLGenerica *dbl, NodoDBLGenerico *seguinte, void *dados) {
    assert(seguinte);
    int iterou1vez=0, r=-2;
    dbl->lastSearchMatch=NULL;
    dbl->lastResult=NOACTION;
    NodoDBLGenerico *aux=seguinte->previous;
    // iterar a lista para se posicionar... deve parar a pesquisa quando for igual...
    while (aux!=dbl->n0d0 && (r=dbl->comparador(aux->dadosPtr, dados))!=0) {
        iterou1vez++;
        aux=aux->previous;
    }
    if (aux) {
        if (r==0) {
            dbl->lastResult=POSENCONTRADO;
            dbl->lastSearchMatch=aux;
        } else {
            // verificar se chegou ao final da lista
            if (aux==dbl->n0d0) {
                dbl->lastResult=ENDLIST;
            }
            aux=NULL;
        }
    }
    return aux;
}

/**
 * função para posicionar no nodo anterior, enquanto for igual (i.e. enquanto a comparação==0)
 * ATENÇÃO: esta função verifica se a lista está ordenada e utiliza o algoritmo para cada contexto
 *
 * @param dbl
 * @param anterior
 * @param dados
 * @return
 */
NodoDBLGenerico *posicionaAnteriorNodoDBL(CfgDBLGenerica *dbl, NodoDBLGenerico *seguinte, void *dados) {
    assert(seguinte);
    dbl->lastResult=NOACTION;
    dbl->lastSearchMatch=NULL;
    switch (dbl->tipoOrdemDados) {
        case NO:
            // utilizar a ordem de criação da lista
            return posicionaAnteriorNodoDBLnooreder(dbl, seguinte, dados);
            break;
        case O1:
            // utilizar a ordenação
            return posicionaAnteriorNodoDBLordenado(dbl, seguinte, dados);
            break;
        default:
            return dbl->lastSearchMatch;
    }
}

/**
 * @brief procedimento cuja função é iterar a lista chamando a função recebida como parametro
 * @param dbl   configuração da lista
 * @param func  função a executar em cada iteração
 * @param ctx   apontador de contexto a enviar para a função
 */
void iterarListaDBLGenerica(CfgDBLGenerica *dbl, TfuncIterarDBLNodo func, void *ctx) {
    assert(func);
    TipoResultadoIterarDBLGenerica r=CONTINUAR;
    NodoDBLGenerico *aux=dbl->n0d0->next;
    dbl->lastIteration=NULL;
    while (aux!=dbl->n0d0) {
        dbl->lastIteration=aux;
        r=func(aux->dadosPtr, ctx);
        if (r!=CONTINUAR) {
            break;
        }
        aux=aux->next;
    }
    dbl->lastResult=OK;
}

/**
 * @brief procedimento cuja função é iterar a lista por ordem inversa, chamando a função recebida como parametro
 * @param dbl   configuração da lista
 * @param func  função a executar em cada iteração
 * @param ctx   apontador de contexto a enviar para a função
 */
void iterarReverseListaDBLGenerica(CfgDBLGenerica *dbl, TfuncIterarDBLNodo func, void *ctx) {
    assert(func);
    TipoResultadoIterarDBLGenerica r=CONTINUAR;
    NodoDBLGenerico *aux=dbl->n0d0->previous;
    dbl->lastIteration=NULL;
    while (aux!=dbl->n0d0) {
        dbl->lastIteration=aux;
        r=func(aux->dadosPtr, ctx);
        if (r!=CONTINUAR) {
            break;
        }
        aux=aux->previous;
    }
    dbl->lastResult=OK;
}

/**
 * @brief procedimento cuja função é iterar a lista chamando a função printNodo
 * @param dbl   configuração da lista
 */
void iterarListaPrintDBLGenerica(CfgDBLGenerica *dbl) {
    //assert(dbl->printNodo);
    NodoDBLGenerico *aux=dbl->n0d0->next;
    dbl->lastIteration=NULL;
    while (aux!=dbl->n0d0) {
        dbl->lastIteration=aux;
        dbl->printNodo(aux->dadosPtr);
        aux=aux->next;
    }
    dbl->lastResult=OK;
}

/**
 * @brief procedimento cuja função é iterar a lista por ordem inversa, chamando a função printNodo
 * @param dbl   configuração da lista
 */
void iterarReverseListaPrintDBLGenerica(CfgDBLGenerica *dbl) {
    //assert(dbl->printNodo);
    NodoDBLGenerico *aux=dbl->n0d0->previous;
    dbl->lastIteration=NULL;
    while (aux!=dbl->n0d0) {
        dbl->lastIteration=aux;
        dbl->printNodo(aux->dadosPtr);
        aux=aux->previous;
    }
    dbl->lastResult=OK;
}

/**
 * @brief função para remover um nodo da lista
 * @param dbl   configuração da lista
 * @param nodo  nodo a remover da lista
 * @return      configuração da lista
 */
CfgDBLGenerica *removeNodoDBLGenerica(CfgDBLGenerica *dbl, NodoDBLGenerico *nodo) {
    // garantir que está configurado o procedimento para destruir os dados do nodo
    assert(dbl->destroyNodo);
    // certificar-se que não está a remover o "nodo mágico"
    if (nodo!=dbl->n0d0) {
        NodoDBLGenerico *aux=nodo;
        // atualizar apontadores para isolar o nodo da lista
        aux->previous->next=aux->next;
        aux->next->previous=aux->previous;
        // libertar espaço dos dados
        dbl->destroyNodo(aux->dadosPtr);
        // libertar o espaço do nodo
        free(aux);
        dbl->totalItems--;
        dbl->lastResult=OK;
    } else {
        dbl->lastResult=VAZIO;
    }
    dbl->lastIteration=NULL;
    dbl->lastSearchMatch=NULL;
    dbl->lastModified=NULL;
    return dbl;
}

/**
 * @brief função responsável pela criação do "nodo mágico" da lista duplamente ligada, a configuração do nodo zero é feita,
 * não é necessário fazer qualquer mudança neste nodo.
 * NOTA: esta função é interna e não deve ser exportada!
 *
 * @return novo nodo mágico
 */
NodoDBLGenerico *newNodoMagicoDBL() {
    NodoDBLGenerico *novo=(NodoDBLGenerico*)malloc(sizeof(NodoDBLGenerico));
    assert(novo);
    novo->dadosPtr=novo;
    novo->next=novo;
    novo->previous=novo;
    return novo;
}

/**
 * @brief esta função simula uma comparação, retorna sempre -1
 * @param a
 * @param b
 * @return -1
 */
int fake_comparadorNodo(void *a, void *b) {
    // é sempre menor... ;-)
    return -1;
}

/**
 * @brief este procedimento simula printNodo, é inútil o resultado.
 * recomendo que seja colocado um procedimento válido antes de utilizarmem produção.
 * @param ptr
 */
void fake_printNodo(void *ptr) {
    // este procedimento foi criado apenas para que todos os apontadores estivessem inicializados
    // o utilizador deverá substituir conforme o contexto...
}

/**
 * @brief função responsável pela criação da configuração simplificada de uma lista duplamente ligada genérica.
 * NOTA: esta função não inicializa o apontador "destroyNodo"
 * portanto a execução do programa pára no assert quando necessitar de executar este comportamento.
 *
 * @param id    identificador numérico da lista
 * @return  configuração da nova lista
 */
CfgDBLGenerica *newListaDBLGenerica(int id) {
    CfgDBLGenerica *novo=(CfgDBLGenerica*)malloc(sizeof(CfgDBLGenerica));
    assert(novo);
    novo->id=id;
    novo->totalItems=0;
    novo->tipoOrdemDados=NO;   /**< NO, valor por omissão. */
    novo->tipoDados=REPETIDOS; /**< REPETIDOS, valor por omissão. */
    novo->lastResult=NOACTION; /**< NOACTION, valor por omissão. */
    novo->comparador=&fake_comparadorNodo;     /**< fake_comparadorNodo, valor por omissão. */
    novo->destroyNodo=NULL;    /**< NULL, valor por omissão. */
    novo->printNodo=&fake_printNodo;      /**< fake_printNodo, valor por omissão. */
    novo->nome=NULL;
    novo->n0d0=newNodoMagicoDBL();
    novo->lastSearchMatch=NULL;
    novo->lastModified=NULL;
    novo->lastIteration=NULL;
    return novo;
}

/**
 * @brief função responsável pela criação da configuração simplificada de uma lista duplamente ligada genérica
 *
 * @param id    identificador numérico da lista
 * @param nome    texto para identificar a lista
 * @return  configuração da nova lista
 */
CfgDBLGenerica *newListaDBLGenericaNome(int id, char *nome) {
    CfgDBLGenerica *novo=newListaDBLGenerica(id);
    novo->nome=strdup(nome);
    return novo;
}

/**
 * @brief função resposável por remover todos os nodos e respetivos valores e destruir a própria lista e respetiva configuração.
 * @param dbl   configuração da lista
 * @return      NULL, lista vazia sem configuração
 */
CfgDBLGenerica *destroyListaDBLGenerica(CfgDBLGenerica *dbl) {
    assert(dbl->destroyNodo);
    NodoDBLGenerico *aux=dbl->n0d0->next;
    NodoDBLGenerico *tmp;
    // iterar a lista e limpar os dados e remover os nodos
    while (aux!=dbl->n0d0) {
        dbl->destroyNodo(aux->dadosPtr);
        tmp=aux->next;
        free(aux);
        aux=tmp;
    }
    // destroy n0d0 mágico
    free(dbl->n0d0);
    // destruir a CfgDBLGenerica
    free(dbl);
    return NULL;
}
