#ifndef OBC_H_INCLUDED
#define OBC_H_INCLUDED


#ifndef NULL
    #define NULL 0
#endif // NULL

#ifndef OBC_NULL_INDEX
    #define OBC_NULL_INDEX (~((size_t)0))
#endif // NULL

#ifndef OBC_MEMORY_FAILURE
    #define OBC_MEMORY_FAILURE 1
#endif // NULL

typedef enum OBC_ERROR_ENUM{
    OBC_ERROR_NO_OP = 0,
    OBC_ERROR_SUCCESS,
    OBC_ERROR_FAILURE
}OBC_ERROR_ENUM;

typedef struct OBC_ErrorPointer{
    OBC_ERROR_ENUM errorValue;
    void *realValue;
}OBC_ErrorPointer;


#endif // OBC_H_INCLUDED
