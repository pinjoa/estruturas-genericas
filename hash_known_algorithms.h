/**
 * @file hash_known_algorithms.h
 * @author João Pinto (pinjoa@gmail.com)
 * @brief this file export all functions headers from file "hash_known_algorithms.c"
 *
 * @version 0.1
 * @date 2021-05-16
 *
 * @copyright Copyright (c) 2021, João Carlos Pinto
 *
 */

#ifndef INC_14AED2HASH_HASH_KNOWN_ALGORITHMS_H
#define INC_14AED2HASH_HASH_KNOWN_ALGORITHMS_H

/**
00 - RS Hash Function
A simple hash function from Robert Sedgwicks Algorithms in C book. I've added some simple optimizations to the algorithm in order to speed up its hashing process.
*/
unsigned int RSHash(const char *str, unsigned int length);

/**
01 - JS Hash Function
A bitwise hash function written by Justin Sobel
*/
unsigned int JSHash(const char *str, unsigned int length);

/**
02 - PJW Hash Function
This hash algorithm is based on work by Peter J. Weinberger of Renaissance Technologies. The book Compilers (Principles, Techniques and Tools) by Aho, Sethi and Ulman, recommends the use of hash functions that employ the hashing methodology found in this particular algorithm.
*/
unsigned int PJWHash(const char *str, unsigned int length);

/**
03 - ELF Hash Function
Similar to the PJW Hash function, but tweaked for 32-bit processors. It is a widley used hash function on UNIX based systems.
*/
unsigned int ELFHash(const char *str, unsigned int length);

/**
04 - BKDR Hash Function
This hash function comes from Brian Kernighan and Dennis Ritchie's book "The C Programming Language". It is a simple hash function using a strange set of possible seeds which all constitute a pattern of 31....31...31 etc, it seems to be very similar to the DJB hash function.
*/
unsigned int BKDRHash(const char *str, unsigned int length);

/**
05 - SDBM Hash Function
This is the algorithm of choice which is used in the open source SDBM project. The hash function seems to have a good over-all distribution for many different data sets. It seems to work well in situations where there is a high variance in the MSBs of the elements in a data set.
*/
unsigned int SDBMHash(const char *str, unsigned int length);

/**
06 - DJB Hash Function
An algorithm produced by Professor Daniel J. Bernstein and shown first to the world on the usenet newsgroup comp.lang.c. It is one of the most efficient hash functions ever published.
*/
unsigned int DJBHash(const char *str, unsigned int length);

/**
07 - DEK Hash Function
An algorithm proposed by Donald E. Knuth in The Art Of Computer Programming Volume 3, under the topic of sorting and search chapter 6.4.
*/
unsigned int DEKHash(const char *str, unsigned int length);

/**
08 - AP Hash Function
An algorithm produced by me Arash Partow. I took ideas from all of the above hash functions making a hybrid rotative and additive hash function algorithm. There isn't any real mathematical analysis explaining why one should use this hash function instead of the others described above other than the fact that I tired to resemble the design as close as possible to a simple LFSR. An empirical result which demonstrated the distributive abilities of the hash algorithm was obtained using a hash-table with 100003 buckets, hashing The Project Gutenberg Etext of Webster's Unabridged Dictionary, the longest encountered chain length was 7, the average chain length was 2, the number of empty buckets was 4579.
*/
unsigned int APHash(const char *str, unsigned int length);

#endif // INC_14AED2HASH_HASH_KNOWN_ALGORITHMS_H
