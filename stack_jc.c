/**
 * @file stack_jc.c
 * @author João Pinto (pinjoa@gmail.com)
 * @brief Implementação de uma stack sem o tipo de dados definido.
 * Portanto pode ser utilizado para qualquer tipo de dados.
 * @version 0.1
 * @date 2021-04-26
 *
 * @copyright Copyright (c) 2021, João Carlos Pinto
 *
 */

#include <malloc.h>
#include <assert.h>
#include "stack_jc.h"

/**
 * @brief função responsável por criar uma stack
 *
 * @return StackGenerica*
 */
StackGenerica *stackNew()
{
    return NULL;
}

/**
 * @brief função responsável por adicionar elemento na stack
 *
 * @param stack
 * @param data
 * @return StackGenerica*
 */
StackGenerica *stackPush(StackGenerica *stack, void *data)
{
    StackGenerica *novo = (StackGenerica *)malloc(sizeof(StackGenerica));
    assert(novo);
    novo->data = data;
    if (stack)
    {
        novo->prev = stack;
    }
    else
    {
        novo->prev = NULL;
    }
    return novo;
}

/**
 * @brief função responsável por excluir um elemento no inicio da stack
 *
 * @param stack
 * @param destroyData
 * @return StackGenerica*
 */
StackGenerica *stackPop(StackGenerica *stack, void destroyData(void *))
{
    StackGenerica *resto = NULL;
    if (stack)
    {
        resto = stack->prev;
        destroyData(stack->data);
        free(stack);
    }
    return resto;
}

/**
 * @brief função para obter o elemento no inicio da stack
 *
 * @param stack
 * @return void*
 */
void *stackTop(StackGenerica *stack)
{
    if (stack)
    {
        return stack->data;
    }
    else
        return NULL;
}

/**
 * @brief função responsável por destruir e lebertar o espaço da stack
 *
 * @param stack
 * @param destroyData
 * @return StackGenerica*
 */
StackGenerica *stackDestroy(StackGenerica *stack, void destroyData(void *))
{
    StackGenerica *aux = NULL;
    while (stack)
    {
        aux = stack;
        stack = aux->prev;
        destroyData(aux->data);
        free(aux);
    }
    return stack;
}

/**
 * @brief função falsa, não destroi informação...
 *
 * @param data
 */
void stackFakeDestroyData(void *data)
{
    // não faz nada, é só para manter o funcionamento da stack...
}
