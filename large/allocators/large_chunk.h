#ifndef LARGE_CHUNK_H_INCLUDED
#define LARGE_CHUNK_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../obc.h"
#include "../../containers/ray.h"

typedef struct OBC_LG_Alloc{

    OBC_Ray links;
    OBC_Ray sizes;

}OBC_LG_Alloc;


#endif // LARGE_CHUNK_H_INCLUDED
