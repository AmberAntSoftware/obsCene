#ifndef OBC_H_INCLUDED
#define OBC_H_INCLUDED


#ifndef NULL
    #define NULL 0
#endif // NULL

#ifndef OBC_MEMORY_FAILURE
    #define OBC_MEMORY_FAILURE 1
#endif // NULL

typedef enum OBC_ERROR_ENUM{
    OBC_NO_OP = 0,
    OBC_SUCCESS,
    OBC_FAILURE
}OBC_ERROR_ENUM;


#endif // OBC_H_INCLUDED
