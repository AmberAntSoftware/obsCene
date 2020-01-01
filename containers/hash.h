#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include "OBC.h"

typedef size_t OBC_Hash;

OBC_Hash OBC_hash(const void *const data, const size_t sizeInBytes);
OBC_Hash OBC_hash2(const void *const data, const size_t sizeInBytes);

#endif // HASH_H_INCLUDED
