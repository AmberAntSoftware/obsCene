# obsCene
Minature C library designed for high performance containers, and easier, faster prototyping.

Currently a work in progress.

Due to a lack of sufficiently powerful generics, the bytecode is not quite optimal, as a size member must be present; otherwise attempts are made to be as optimal as possible.

Why is this faster? Using logarithmic memory allocation and not indiviually allocating memory, which is both good for cache memory and prevent threads from blocking each other to allocate memory. This also speeds up the freeing process, as only one free -- `OBC_free[Structure]` -- call needs to be made to free the entire container's data assuming each unit stored in a container does not need to deallocate resources they own.


Error checking container initialization can be done through the OBC_ERROR_ENUM checks or the null pointer for `OBC_init[Container]` and `OBC_new[Container]` functions respectively. This allows for memory fault tolerance for low memory or memory allocation errors across a variety of malloc() implementations.

Error checking on memory allocations and container emplacement or copying into structures can be checked through the OBC_NULL_INDEX check as being an illegal position of memory.

Error checking is not required, but it can make fault tolerant programs.


Easy Start Guide
==

All library calls start with `OBC_`

There are two ways to make a structure:
--

• System allocator (malloc) `OBC_new[Container]` will return a double pointer. 
 This is because the structure is initilized with the byte size of the object it will contain.
 This also allows it to be assigned to a type of a double pointer and be accessed in DMA
 
• Initialization `OBC_init[Container]` will initialize the structure in a given memory address.
  The double pointer access pattern is gotten through the access function `OBC_[Container]GetDataPointer([Container] *container)`

All containers have an iteration syntax in the form of `OBC_[Container]forEach(args...)`
This will allow for an easy touch all approach, and varients will be supplied based on the container type.


Code Sample
```c
typedef struct Point{
    unsigned int x;
    unsigned int y;
}Point;

void doList(){

    Point **points = OBC_newList(sizeof(Point));
    srand(time(NULL));

    Point point;
    unsigned int i;
    for(i = 0; i < 32; i++){
        point.x = rand();
        point.y = rand();
        OBC_ListAdd(points, &point);
    }

    OBC_Iterator unit;
    OBC_ListForEach(points, &unit){
        /*
        * Manual dereference accessor: (*arr)[index]
        */
        printf("Point: %u, %u\n", (*points)[unit.iter].x, (*points)[unit.iter].y);
        /*
        * Subscript accessor: arr[obc(index)]
        */
        printf("Point: %u, %u\n", points[obc(unit.iter)].x, points[obc(unit.iter)].y);
    }
    
    OBC_freeList(points);
}
```


The 64bit, or LARGE Containers
==

All library calls start with `OBCL_` and mirror OBC_ except addressors can have more than the offset size and are structs of addressors.

These are all triple pointers based containers for chunked accessor patterns for limits on malloc() calls in various systems.

Code Sample
```c
typedef struct Point{
    unsigned int x;
    unsigned int y;
}Point;

void doList(){

    Point ***points = OBCL_newList(sizeof(Point));
    srand(time(NULL));

    Point point;
    unsigned int i;
    for(i = 0; i < 32; i++){
        point.x = rand();
        point.y = rand();
        OBCL_ListAdd(points, &point);
    }

    OBCL_Iterator unit;
    OBCL_ListForEach(points, &unit){
        /*
        * Subscript accessor: arr[OBC(index)]
        * capital OBC for large containers
        */
        printf("Point: %u, %u\n", points[OBC(unit.iter)].x, points[OBC(unit.iter)].y);
    }
    
    OBCL_freeList(points);
}```
