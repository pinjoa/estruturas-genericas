/**
 * @file hash_known_algorithms.c
 * @author João Pinto (pinjoa@gmail.com)
 * @brief this set of functions were created and adapted from the
 * code available on this URL https://www.partow.net/programming/hashfunctions/#StringHashing
 *
 * @version 0.1
 * @date 2021-05-16
 *
 * @copyright Copyright (c) 2021, João Carlos Pinto
 *
 */

#include "hash_known_algorithms.h"

/**
00 - RS Hash Function
A simple hash function from Robert Sedgwicks Algorithms in C book. I've added some simple optimizations to the algorithm in order to speed up its hashing process.
*/
unsigned int RSHash(const char *str, unsigned int length)
{
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;
    unsigned int i = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = hash * a + (*str);
        a = a * b;
    }

    return hash;
}

/**
01 - JS Hash Function
A bitwise hash function written by Justin Sobel
*/
unsigned int JSHash(const char *str, unsigned int length)
{
    unsigned int hash = 1315423911;
    unsigned int i = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash ^= ((hash << 5) + (*str) + (hash >> 2));
    }

    return hash;
}

/**
02 - PJW Hash Function
This hash algorithm is based on work by Peter J. Weinberger of Renaissance Technologies. The book Compilers (Principles, Techniques and Tools) by Aho, Sethi and Ulman, recommends the use of hash functions that employ the hashing methodology found in this particular algorithm.
*/
unsigned int PJWHash(const char *str, unsigned int length)
{
    const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    const unsigned int ThreeQuarters = (unsigned int)((BitsInUnsignedInt * 3) / 4);
    const unsigned int OneEighth = (unsigned int)(BitsInUnsignedInt / 8);
    const unsigned int HighBits = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
    unsigned int hash = 0;
    unsigned int test = 0;
    unsigned int i = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = (hash << OneEighth) + (*str);

        if ((test = hash & HighBits) != 0)
        {
            hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

    return hash;
}

/**
03 - ELF Hash Function
Similar to the PJW Hash function, but tweaked for 32-bit processors. It is a widley used hash function on UNIX based systems.
*/
unsigned int ELFHash(const char *str, unsigned int length)
{
    unsigned int hash = 0;
    unsigned int x = 0;
    unsigned int i = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = (hash << 4) + (*str);

        if ((x = hash & 0xF0000000L) != 0)
        {
            hash ^= (x >> 24);
        }

        hash &= ~x;
    }

    return hash;
}

/**
04 - BKDR Hash Function
This hash function comes from Brian Kernighan and Dennis Ritchie's book "The C Programming Language". It is a simple hash function using a strange set of possible seeds which all constitute a pattern of 31....31...31 etc, it seems to be very similar to the DJB hash function.
*/
unsigned int BKDRHash(const char *str, unsigned int length)
{
    unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
    unsigned int hash = 0;
    unsigned int i = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = (hash * seed) + (*str);
    }

    return hash;
}

/**
05 - SDBM Hash Function
This is the algorithm of choice which is used in the open source SDBM project. The hash function seems to have a good over-all distribution for many different data sets. It seems to work well in situations where there is a high variance in the MSBs of the elements in a data set.
*/
unsigned int SDBMHash(const char *str, unsigned int length)
{
    unsigned int hash = 0;
    unsigned int i = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = (*str) + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

/**
06 - DJB Hash Function
An algorithm produced by Professor Daniel J. Bernstein and shown first to the world on the usenet newsgroup comp.lang.c. It is one of the most efficient hash functions ever published.
*/
unsigned int DJBHash(const char *str, unsigned int length)
{
    unsigned int hash = 5381;
    unsigned int i = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = ((hash << 5) + hash) + (*str);
    }

    return hash;
}

/**
07 - DEK Hash Function
An algorithm proposed by Donald E. Knuth in The Art Of Computer Programming Volume 3, under the topic of sorting and search chapter 6.4.
*/
unsigned int DEKHash(const char *str, unsigned int length)
{
    unsigned int hash = length;
    unsigned int i = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
    }

    return hash;
}

/**
08 - AP Hash Function
An algorithm produced by me Arash Partow. I took ideas from all of the above hash functions making a hybrid rotative and additive hash function algorithm. There isn't any real mathematical analysis explaining why one should use this hash function instead of the others described above other than the fact that I tired to resemble the design as close as possible to a simple LFSR. An empirical result which demonstrated the distributive abilities of the hash algorithm was obtained using a hash-table with 100003 buckets, hashing The Project Gutenberg Etext of Webster's Unabridged Dictionary, the longest encountered chain length was 7, the average chain length was 2, the number of empty buckets was 4579.
*/
unsigned int APHash(const char *str, unsigned int length)
{
    unsigned int hash = 0xAAAAAAAA;
    unsigned int i = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (*str) * (hash >> 3)) : (~((hash << 11) + ((*str) ^ (hash >> 5))));
    }

    return hash;
}
