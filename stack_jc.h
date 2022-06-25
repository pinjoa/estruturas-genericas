/**
 * @file stack_jc.h
 * @author João Pinto (pinjoa@gmail.com)
 * @brief interface da implementação de uma stack sem o tipo de dados definido.
 * Portanto pode ser utilizado para qualquer tipo de dados.
 * @version 0.1
 * @date 2021-04-26
 *
 * @copyright Copyright (c) 2021, João Carlos Pinto
 *
 */


#ifndef INC_09_AED2_V1_STACK_JC_H
#define INC_09_AED2_V1_STACK_JC_H

/**
 * @brief estrutura necessária para implementar uma stack genérica
 */
typedef struct stackgenericastruct StackGenerica;
struct stackgenericastruct {
    void *data;             /**< apontador para os dados armazenados na stack. */
    StackGenerica *prev;    /**< apontador para "anterior", i.e. o próximo nodo da stack. */
};

StackGenerica *stackNew();
StackGenerica *stackPush(StackGenerica *stack, void *data);
StackGenerica *stackPop(StackGenerica *stack, void destroyData(void*));
void *stackTop(StackGenerica *stack);
StackGenerica *stackDestroy(StackGenerica *stack, void destroyData(void*));
void stackFakeDestroyData(void *data);

#endif //INC_09_AED2_V1_STACK_JC_H
