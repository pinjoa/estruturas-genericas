/**
 * @file dblist_jc.h
 * @author João Pinto (pinjoa@gmail.com)
 * @brief Interface com a estrutura de dados para uma lista duplamente ligada e respetivas funções e procedimentos para
 * implementação de uma lista duplamente ligada sem o tipo de dados definido.
 * @version 0.1
 * @date 2021-04-12
 *
 * @copyright Copyright (c) 2021, João Carlos Pinto
 *
 */

#ifndef INC_01_AED2_V0_DBLIST_JC_H
#define INC_01_AED2_V0_DBLIST_JC_H

/**
 * @brief estrutura genérica para lista duplamente ligada
 *
 */
typedef struct intNodoListaDBLGenerico NodoDBLGenerico;
struct intNodoListaDBLGenerico
{
    void *dadosPtr;            /**< apontador genérico para os dados associados a este nodo. */
    NodoDBLGenerico *previous; /**< apontador para o nodo anterior. */
    NodoDBLGenerico *next;     /**< apontador para o próximo nodo. */
};

/**
 * @brief tipo de ordem da lista DBL genérica.
 * por enquanto tem apenas dois tipos de ordem de dados...
 *
 */
typedef enum tipoOrdemDBLGenerica
{
    NO, /**< não tem ordem. */
    O1  /**< ordem um utiliza tipo de comparação standard (-1, 0, 1). */
} TipoOrdemDBLGenerica;

/**
 * @brief flags para permitir dados repetidos na lista DBL genérica
 *
 */
typedef enum tipoDeDadosDBLGenerica
{
    REPETIDOS, /**< permite dados repetidos. */
    UNICOS     /**< deve aceitar apenas dados únicos. */
} TipoDeDadosDBLGenerica;

typedef enum tipoResultadoOperacaoDBLGenerica
{
    NOACTION,      /**< nada feito ou nada a fazer. */
    OK,            /**< tudo OK. */
    ERRO,          /**< ERRO na operação. */
    DUPLICADO,     /**< nodo DUPLICADO. */
    VAZIO,         /**< lista VAZIA. */
    POSENCONTRADO, /**< posicao ENCONTRADO. */
    ISSMALLER,     /**< é menor. */
    ISBIGGER,      /**< é maior. */
    ENDLIST,       /**< fim da lista. */
    NOOPTION       /**< sem opção. */
} TipoResultadoOperacaoDBLGenerica;

/**
 * @brief flags para controlar o cliclo iterativo da lista DBL genérica
 *
 */
typedef enum tipoResultadoIterarDBLGenerica
{
    PARAR,    /**< deve parar o ciclo iterativo. */
    CONTINUAR /**< continuar o ciclo iterativo. */
} TipoResultadoIterarDBLGenerica;

/**
 * @brief identificação da assinatura tipo para função de comparação de um nodo:
 * primeiro paramatro é o apontador para o nodo,
 * segundo parametro é o apontador para a informação a comparar.
 *
 * @param void* apontador dos dados do nodo
 * @param void* apontador dos dados extra para processar os dados de outrocontexto de informação
 * @return PARAR ou CONTINUAR.
 */
typedef TipoResultadoIterarDBLGenerica (*TfuncIterarDBLNodo)(void *, void *);

/**
 * @brief identificação da assinatura tipo para função de comparação de um nodo:
 * primeiro paramatro é o apontador para o nodo,
 * segundo parametro é o apontador para a informação a comparar.
 *
 * @param void* apontador dos dados do nodo
 * @param void* apontador dos dados para comparar
 * @return inteiro como resultado da comparação standard: -1(=menor), 0(=igual) e 1(=maior).
 */
typedef int (*TfuncComparaDBLNodo)(void *, void *);

/**
 * @brief identificação da assinatura tipo para procedimento responsável por libertar memória reservada para os dados do nodo da lista.
 *
 * @param void* apontador para os dados do nodo
 */
typedef void (*TdestroyDBLNodo)(void *);

/**
 * @brief identificação da assinatura tipo para procedimento responsável por imprimir os dados do nodo da lista.
 *
 * @param void* apontador para os dados do nodo
 */
typedef void (*TprintDBLNodo)(void *);

/**
 * @brief tipo de dados da configuração da lista genérica
 *
 */
typedef struct intCfgDBLGenerica CfgDBLGenerica;
struct intCfgDBLGenerica
{
    int id;                                      /**< ID caso seja necessário identificar a lista. */
    char *nome;                                  /**< nome a associar a esta lista. */
    int totalItems;                              /**< total de itens na lista. */
    TipoResultadoOperacaoDBLGenerica lastResult; /**< resultado da última operação. */
    TipoOrdemDBLGenerica tipoOrdemDados;         /**< tipo de ordem da lista. */
    TipoDeDadosDBLGenerica tipoDados;            /**< dados REPETIDOS ou ÚNICOS. */
    TfuncComparaDBLNodo comparador;              /**< é o endereço da função de comparação. */
    TdestroyDBLNodo destroyNodo;                 /**< é o endereço do procedimento para destruir os dados do nodo. */
    TprintDBLNodo printNodo;                     /**< é o endereço do procedimento para imprimir os dados do nodo. */
    NodoDBLGenerico *n0d0;                       /**< apontador para o primeiro nodo, é o "nodo mágico". */
    NodoDBLGenerico *lastSearchMatch;            /**< apontador para o último nodo encontrado. */
    NodoDBLGenerico *lastModified;               /**< apontador para o nodo inserido/modificado na lista. */
    NodoDBLGenerico *lastIteration;              /**< apontador para o último nodo iterado na lista. */
};

// espaço reservado para exportar as assinaturas do ficheiro "dblist_jc.c"
CfgDBLGenerica *newListaDBLGenerica(int id);
CfgDBLGenerica *newListaDBLGenericaNome(int id, char *nome);
CfgDBLGenerica *destroyListaDBLGenerica(CfgDBLGenerica *dbl);
CfgDBLGenerica *insertNodoDBLGenerica(CfgDBLGenerica *dbl, void *dados, NodoDBLGenerico *lastNearestRecord);
void *insertNodoDBLGenericaGetDataPtr(CfgDBLGenerica *dbl, void *dados, NodoDBLGenerico *lastNearestRecord);
CfgDBLGenerica *searchNodoDBLGenerica(CfgDBLGenerica *dbl, void *dados);
CfgDBLGenerica *removeNodoDBLGenerica(CfgDBLGenerica *dbl, NodoDBLGenerico *nodo);
void iterarListaDBLGenerica(CfgDBLGenerica *dbl, TfuncIterarDBLNodo func, void *ctx);
void iterarReverseListaDBLGenerica(CfgDBLGenerica *dbl, TfuncIterarDBLNodo func, void *ctx);
void iterarListaPrintDBLGenerica(CfgDBLGenerica *dbl);
void iterarReverseListaPrintDBLGenerica(CfgDBLGenerica *dbl);
NodoDBLGenerico *posicionaPrimeiroNodoDBL(CfgDBLGenerica *dbl, void *dados);
NodoDBLGenerico *posicionaProximoNodoDBL(CfgDBLGenerica *dbl, NodoDBLGenerico *anterior, void *dados);
NodoDBLGenerico *posicionaAnteriorNodoDBL(CfgDBLGenerica *dbl, NodoDBLGenerico *seguinte, void *dados);
int fake_comparadorNodo(void *a, void *b);
void fake_printNodo(void *ptr);

#endif // INC_01_AED2_V0_DBLIST_JC_H
