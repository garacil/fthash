/*
Copyright (c) 2019, Germán Luis Aracil Boned
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdlib.h> /* malloc, calloc */
#include <string.h> /* strcmp */

#ifndef FTHASH
#define FTHASH

#define FTHASH_VERSION 1.0

/* Define integer typedef */
#if defined(_WIN32)
#if defined(_MSC_VER) && _MSC_VER >= 1600
#include <stdint.h>
#elif defined(__WATCOMC__) || defined(__MINGW32__) || defined(__CYGWIN__)
#include <stdint.h>
#else
typedef unsigned long long uint64_t;
typedef unsigned long uint32_t;
typedef unsigned char uint8_t;
#endif
#elif defined(__GNUC__) && !defined(__VXWORKS__)
#include <stdint.h>
#else
typedef unsigned long long uint64_t;
typedef unsigned long uint32_t;
typedef unsigned char uint8_t;
#endif

/* Index definition */

typedef void *_LPTR_T[];

/* Hash table definition */

typedef struct
{
  uint8_t index_size;        /* Index size in bits: 1 to 32 bits (hash's mask)*/
  uint32_t index_mask;       /* Index mask for apply to hash and get index position*/
  uint32_t index_collisions; /* How many collisions have */
  uint64_t itsize;           /* Size of index's structure */
  uint32_t buckets;          /* Current used buckets to store data */
  uint64_t btsize;           /* Current size of all buckets */
  void *i;                   /* Pointer to first position of index's buffer */
} HASHTABLE_T;

/* Bucket definition */

typedef struct
{
  void *key;
  void *data;
  char flag;
  void *next;
} BUCKET_T;

/* Macro for hash calculation */

#define _HASH_L4()                                   \
  uint16_t *_klli = _k;                              \
  *_hp = 0x811c9dc5;                                 \
                                                     \
  while (*_klli)                                     \
  {                                                  \
    *_hp = (*_hp ^ (uint16_t)*_klli++) * 0x01000193; \
  }

/* Macro: initialize table */

#define HASH_TABLE_INIT(ht, is, out)                             \
  do                                                             \
  {                                                              \
    HASHTABLE_T *_ht = (ht);                                     \
    _ht->index_size = (is);                                      \
    uint8_t _ibc;                                                \
                                                                 \
    (out) = 0;                                                   \
    _ht->index_mask = 1;                                         \
    for (_ibc = 1; _ibc < _ht->index_size; _ibc++)               \
      _ht->index_mask = (_ht->index_mask << 1) + 1;              \
    if (!(_ht->i = calloc(_ht->index_mask + 1, sizeof(void *)))) \
    {                                                            \
      (out) = 1;                                                 \
      break;                                                     \
    }                                                            \
    _ht->itsize = sizeof(void *) * (_ht->index_mask);            \
    _ht->buckets = 0;                                            \
    _ht->btsize = 0;                                             \
    _ht->index_collisions = 0;                                   \
  } while (0)

/* Macro to remove table & free memory */

#define HASH_TABLE_FREE(ht) \
  HASHTABLE_T *_ht = (ht);  \
                            \
  ;

/* Macro: store data */

#define HASH_TABLE_ADD(ht, k, d, f, out)              \
  do                                                  \
  {                                                   \
    HASHTABLE_T *_ht = (ht);                          \
    void *_k = (k);                                   \
    void *_d = (d);                                   \
    uint32_t _h;                                      \
    uint32_t *_hp;                                    \
    uint32_t _hi;                                     \
    _LPTR_T *_i;                                      \
    BUCKET_T *_b;                                     \
    BUCKET_T *_bn;                                    \
                                                      \
    (out) = 0;                                        \
    _hp = &_h;                                        \
    _HASH_L4();                                       \
    _hi = *_hp & _ht->index_mask;                     \
    _i = _ht->i;                                      \
    if (!(*_i)[_hi])                                  \
    {                                                 \
      if (!(_b = malloc(sizeof(BUCKET_T))))           \
      {                                               \
        (out) = 1;                                    \
        break;                                        \
      }                                               \
      _b->key = _k;                                   \
      _b->data = _d;                                  \
      _b->flag = (f);                                 \
      (*_i)[_hi] = _b;                                \
      _ht->buckets++;                                 \
      _ht->btsize = _ht->btsize + sizeof(BUCKET_T);   \
    }                                                 \
    else                                              \
    {                                                 \
      _b = (*_i)[_hi];                                \
      _bn = NULL;                                     \
      while (_b)                                      \
      {                                               \
        if (strcmp((char *)_b->key, (char *)_k) == 0) \
        {                                             \
          _bn = NULL;                                 \
          _b->data = _d;                              \
          _b->flag = (f);                             \
          break;                                      \
        }                                             \
        _bn = _b;                                     \
        _b = _b->next;                                \
      }                                               \
      if (_bn)                                        \
      {                                               \
        if (!(_b = malloc(sizeof(BUCKET_T))))         \
        {                                             \
          (out) = 1;                                  \
          break;                                      \
        }                                             \
        _b->key = _k;                                 \
        _b->data = _d;                                \
        _b->flag = (f);                               \
        _b->next = (*_i)[_hi];                        \
        (*_i)[_hi] = _b;                              \
        _ht->buckets++;                               \
        _ht->btsize = _ht->btsize + sizeof(BUCKET_T); \
        _ht->index_collisions++;                      \
      }                                               \
    }                                                 \
  } while (0)

/* Macro: read data */

#define HASH_TABLE_GET(ht, k, d, f)                 \
  do                                                \
  {                                                 \
    HASHTABLE_T *_ht = (ht);                        \
    void *_k = (k);                                 \
    uint32_t _h;                                    \
    uint32_t *_hp;                                  \
    uint32_t _hi;                                   \
    _LPTR_T *_i;                                    \
    BUCKET_T *_b;                                   \
                                                    \
    (d) = NULL;                                     \
    (f) = 0;                                        \
    _hp = &_h;                                      \
    _HASH_L4();                                     \
    _hi = *_hp & _ht->index_mask;                   \
    _i = _ht->i;                                    \
    if (!(*_i)[_hi])                                \
      break;                                        \
    _b = (*_i)[_hi];                                \
    while (_b)                                      \
    {                                               \
      if (strcmp((char *)_b->key, (char *)_k) == 0) \
      {                                             \
        (d) = _b->data;                             \
        (f) = _b->flag;                             \
        break;                                      \
      }                                             \
      else                                          \
        _b = _b->next;                              \
    }                                               \
  } while (0)

/* Macro: remove data */

#define HASH_TABLE_ERA(ht, k, out)                      \
  do                                                    \
  {                                                     \
    HASHTABLE_T *_ht = (ht);                            \
    void *_k = (k);                                     \
    uint32_t _h;                                        \
    uint32_t *_hp;                                      \
    uint32_t _hi;                                       \
    _LPTR_T *_i;                                        \
    BUCKET_T *_b;                                       \
    BUCKET_T *_bn;                                      \
                                                        \
    (out) = 0;                                          \
    _hp = &_h;                                          \
    _HASH_L4();                                         \
    _hi = *_hp & _ht->index_mask;                       \
    _i = _ht->i;                                        \
    if (!(*_i)[_hi])                                    \
      break;                                            \
    _b = (*_i)[_hi];                                    \
    if (strcmp((char *)_b->key, (char *)_k) == 0)       \
    {                                                   \
      if (_b->next)                                     \
      {                                                 \
        (*_i)[_hi] = _b->next;                          \
        _ht->index_collisions--;                        \
      }                                                 \
      else                                              \
        (*_i)[_hi] = NULL;                              \
      free(_b);                                         \
      _ht->buckets--;                                   \
      _ht->btsize = _ht->btsize - sizeof(BUCKET_T);     \
      (out) = 1;                                        \
      break;                                            \
    }                                                   \
    else                                                \
    {                                                   \
      _bn = _b;                                         \
      _b = _b->next;                                    \
      while (_b)                                        \
      {                                                 \
        if (strcmp((char *)_b->key, (char *)_k) == 0)   \
        {                                               \
          _bn->next = _b->next;                         \
          free(_b);                                     \
          _ht->index_collisions--;                      \
          _ht->buckets--;                               \
          _ht->btsize = _ht->btsize - sizeof(BUCKET_T); \
          (out) = 1;                                    \
          break;                                        \
        }                                               \
        else                                            \
        {                                               \
          _bn = _b;                                     \
          _b = _b->next;                                \
        }                                               \
      }                                                 \
    }                                                   \
  } while (0)

#endif /* FTHASH */
