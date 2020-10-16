# obsCene
Minature C library designed for high performance containers, and better arbitrary coding.

Currently a work in progress.

Due to a lack of sufficiently powerful generics, the bytecode is not quite optimal, as a size member must be present; otherwise attempts are made to be as optimal as possible.

Why is this faster? Using logarithmic memory acessing and not indiviually malloc'ing memory, both good for the prefetcher and cache memory alignments, a massive performance boost can be given. This also speeds up the freeing process, as only one free -- `OBC_free[Structure]` -- call needs to be made to free the entire container's data.



Error checking is possible through a combination of NULL pointer checks, illegal bounds NULL_INDEX, and explicit error enumerations OBC_ERROR_FAILURE, but is not required but can make a program even more safe to use.


Easy Start Guide
==

All library calls start with OBC_

There are two ways to make a structure:
--

• System allocator (malloc) `OBC_new[Container]` will return a double pointer. 
 This is because the structure is initilized with the byte size of the object it will contain.
 This also allows it to be assigned to a double type and be accessed in DMA
 
• Initialization `OBC_init[Container]` will initialize the structure in a given memory address.
  The double pointer access pattern is gotten through the access function {under refactoring}

All containers have an iteration syntax in the form of `OBC_[Container]iter(args...)`
This will allow easy access touch all approach, and will have future accessors to cut off iterations


Code Sample Explicit
```
typedef struct Point{
    size_t x;
    size_t y;
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

    OBC_ListIterator unit;
    OBC_ListForEach(points, &unit){
        printf("Point: %u, %u\n", (*points)[unit.iter].x, (*points)[unit.iter].y);
    }

    OBC_freeList(points);
}
```


Code Sample Implicit
```
typedef struct Point{
    size_t x;
    size_t y;
}Point;

void doList(){

    Point **points = OBC_newQueue(sizeof(Point));
    srand(time(NULL));

    Point point;
    unsigned int i;
    for(i = 0; i < 32; i++){
        point.x = rand();
        point.y = rand();
        OBC_QueueAdd(points, &point);
    }

    OBC_QueueIterator unit;
    OBC_QueueForEach(points, &unit){
        printf("Point: %u, %u\n", points[obc(unit.iter)].x, points[obc(unit.iter)].y);
    }

    OBC_freeQueue(points);
}
```
