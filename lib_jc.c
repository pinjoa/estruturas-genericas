/**
 * @file lib_jc.c
 * @author João Pinto (pinjoa@gmail.com)
 * @brief biblioteca genérica de funções úteis para projetos e exercicios
 * algumas das funções foram criadas para responder a alguns trabalhos de AED1
 * @version 0.4
 * @date 2021-04-10
 *
 * @copyright Copyright (c) 2021, João Carlos Pinto
 *
 */
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "lib_jc.h"

/**
 * @brief retorna TRUE se "n" for um número primo (NOTA: novo algoritmo de teste)
 *
 * @param n
 * @return Boolean
 */
bool isPrimeNumber(int n)
{
    // testar os casos iniciais
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;
    // os numeros multiplos de 2 e 3 também podem ser descartados e depois já só falta testar a partir de 5
    if (n % 2 == 0 || n % 3 == 0)
        return false;
    // iniciar o teste a partir de 5 ...
    for (int i = 5; i * i <= n; i = i + 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return false;

    return true;
}

/**
 * @brief procura o próximo número primo depois de "n"
 * @param n
 * @return
 */
int getNextPrimeNumber(int n)
{
    if (n <= 1)
        return 2;
    int novo = n;
    bool found = false;
    while (!found)
    {
        novo++;
        if (isPrimeNumber(novo))
            found = true;
    }
    return novo;
}

/**
 * @brief procura o proximo numero primo caso "n" não se um número primo
 * @param n
 * @return
 */
int getNearestPrimeNumber(int n)
{
    if (isPrimeNumber(n))
        return n;
    return getNextPrimeNumber(n);
}
