#ifndef HASHMAP_H_INCLUDED
#define HASHMAP_H_INCLUDED

#include "../../obc.h"
#include "../../containers/hashmap.h"
#include "../../testing/testing.h"

OBC_TESTING_ENUM OBC_TST_hashmapCorrectnes();
OBC_TESTING_ENUM OBC_TST_hashmapThroughput(int rounds);

#endif // HASHMAP_H_INCLUDED
