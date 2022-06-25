/**
 * @file lib_jc.h
 * @author João Pinto (pinjoa@gmail.com)
 * @brief biblioteca genérica de funções úteis para projetos e exercicios
 * @version 0.4
 * @date 2021-04-10
 *
 * @copyright Copyright (c) 2021, João Carlos Pinto
 *
 */

#include <stdbool.h>

#ifndef interface_lib_jc_h
#define interface_lib_jc_h

bool isPrimeNumber(int n);
int getNextPrimeNumber(int n);
int getNearestPrimeNumber(int n);

#endif // interface_lib_jc_h
