/**Edited by AmberCadence**/
/* Copyright (C) 1991-2019 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Written by Douglas C. Schmidt (schmidt@ics.uci.edu).
   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* If you consider tuning this algorithm, you should consult first:
   Engineering a sort function; Jon Bentley and M. Douglas McIlroy;
   Software - Practice and Experience; Vol. 23 (11), 1249-1265, 1993.  */

#include "obsceneqsort.h"
#include "obscene.h"


//#include <alloca.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
/* Byte-wise swap two items of size SIZE. */
#define SWAP(a, b, size)                                                      \
  do                                                                              \
    {                                                                              \
      size_t __size = (size);                                                      \
      char *__a = (a), *__b = (b);                                              \
      do                                                                      \
        {                                                                      \
          char __tmp = *__a;                                                      \
          *__a++ = *__b;                                                      \
          *__b++ = __tmp;                                                      \
        } while (--__size > 0);                                                      \
    } while (0)
/* Discontinue quicksort algorithm when partition gets below this size.
   This particular magic number was chosen to work best on a Sun 4/260. */
#define MAX_THRESH 4
/* Stack node declarations used to store unfulfilled partition obligations. */
typedef struct
  {
    char *lo;
    char *hi;
  } stack_node;
/* The next 4 #defines implement a very fast in-line stack abstraction. */
/* The stack needs log (total_elements) entries (we could even subtract
   log(MAX_THRESH)).  Since total_elements has type size_t, we get as
   upper bound for log (total_elements):
   bits per byte (CHAR_BIT) * sizeof(size_t).  */
#define STACK_SIZE        (CHAR_BIT * sizeof (size_t))
#define PUSH(low, high)        ((void) ((top->lo = (low)), (top->hi = (high)), ++top))
#define        POP(low, high)        ((void) (--top, (low = top->lo), (high = top->hi)))
#define        STACK_NOT_EMPTY        (stack < top)
/* Order size using quicksort.  This implementation incorporates
   four optimizations discussed in Sedgewick:
   1. Non-recursive, using an explicit stack of pointer that store the
      next array partition to sort.  To save time, this maximum amount
      of space required to store an array of SIZE_MAX is allocated on the
      stack.  Assuming a 32-bit (64 bit) integer for size_t, this needs
      only 32 * sizeof(stack_node) == 256 bytes (for 64 bit: 1024 bytes).
      Pretty cheap, actually.
   2. Chose the pivot element using a median-of-three decision tree.
      This reduces the probability of selecting a bad pivot value and
      eliminates certain extraneous comparisons.
   3. Only quicksorts TOTAL_ELEMS / MAX_THRESH partitions, leaving
      insertion sort to order the MAX_THRESH items within each partition.
      This is a big win, since insertion sort is faster for small, mostly
      sorted array segments.
   4. The larger of the two sub-partitions is always pushed onto the
      stack first, with the algorithm then concentrating on the
      smaller partition.  This *guarantees* no more than log (total_elems)
      stack size is needed (actually O(1) in this case)!  */
void OBC_quicksort (void *const pbase, size_t total_elems, size_t size, int OBC_TYPE)
{
  /**AC**/

  char truth = 0;
  //other values already supplied and comparator is a constant <

  /******/

  char *base_ptr = (char *) pbase;
  const size_t max_thresh = MAX_THRESH * size;
  if (total_elems == 0)
    /* Avoid lossage with unsigned arithmetic below.  */
    return;
  if (total_elems > MAX_THRESH)
    {
      char *lo = base_ptr;
      char *hi = &lo[size * (total_elems - 1)];
      stack_node stack[STACK_SIZE];
      stack_node *top = stack;
      PUSH (NULL, NULL);
      while (STACK_NOT_EMPTY)
        {
          char *left_ptr;
          char *right_ptr;
          /* Select median value from among LO, MID, and HI. Rearrange
             LO and HI so the three values are sorted. This lowers the
             probability of picking a pathological pivot value and
             skips a comparison for both the LEFT_PTR and RIGHT_PTR in
             the while loops. */
          char *mid = lo + size * ((hi - lo) / size >> 1);
          /**
          if ((*cmp) ((void *) mid, (void *) lo, arg) < 0)
            SWAP (mid, lo, size);
          **/
          COMPARE__DUMP_FULL_SWITCH(OBC_TYPE,OBC_ACTION_COMPARE_L_, CMP0 ,truth,mid,lo);
          if(truth){
            SWAP (mid, lo, size);
          }
          /**
          if ((*cmp) ((void *) hi, (void *) mid, arg) < 0)
            SWAP (mid, hi, size);
          **/
          COMPARE__DUMP_FULL_SWITCH(OBC_TYPE,OBC_ACTION_COMPARE_L_,CMP1,truth,hi,mid);
          if(truth){
            SWAP (mid, hi, size);
          }
          else
            goto jump_over;
          /**
          if ((*cmp) ((void *) mid, (void *) lo, arg) < 0)
            SWAP (mid, lo, size);
          **/
          COMPARE__DUMP_FULL_SWITCH(OBC_TYPE,OBC_ACTION_COMPARE_L_,CMP2,truth,mid,lo);
          if(truth){
            SWAP (mid, lo, size);
          }
        jump_over:;
          left_ptr  = lo + size;
          right_ptr = hi - size;
          /* Here's the famous ``collapse the walls'' section of quicksort.
             Gotta like those tight inner loops!  They are the main reason
             that this algorithm runs much faster than others. */
          do
            {
              /**
              while ((*cmp) ((void *) left_ptr, (void *) mid, arg) < 0)
                left_ptr += size;
              while ((*cmp) ((void *) mid, (void *) right_ptr, arg) < 0)
                right_ptr -= size;
              **/
              goto PIPE_SKIP_1;
              do{
                left_ptr += size;
                PIPE_SKIP_1:
                COMPARE__DUMP_FULL_SWITCH(OBC_TYPE,OBC_ACTION_COMPARE_L_,CMP3,truth,left_ptr,mid);
              }while(truth);
              goto PIPE_SKIP_2;
              do{
                right_ptr -= size;
                PIPE_SKIP_2:
                COMPARE__DUMP_FULL_SWITCH(OBC_TYPE,OBC_ACTION_COMPARE_L_,CMP4,truth,mid,right_ptr);
              }while(truth);

              if (left_ptr < right_ptr)
                {
                  SWAP (left_ptr, right_ptr, size);
                  if (mid == left_ptr)
                    mid = right_ptr;
                  else if (mid == right_ptr)
                    mid = left_ptr;
                  left_ptr += size;
                  right_ptr -= size;
                }
              else if (left_ptr == right_ptr)
                {
                  left_ptr += size;
                  right_ptr -= size;
                  break;
                }
            }
          while (left_ptr <= right_ptr);
          /* Set up pointers for next iteration.  First determine whether
             left and right partitions are below the threshold size.  If so,
             ignore one or both.  Otherwise, push the larger partition's
             bounds on the stack and continue sorting the smaller one. */
          if ((size_t) (right_ptr - lo) <= max_thresh)
            {
              if ((size_t) (hi - left_ptr) <= max_thresh)
                /* Ignore both small partitions. */
                POP (lo, hi);
              else
                /* Ignore small left partition. */
                lo = left_ptr;
            }
          else if ((size_t) (hi - left_ptr) <= max_thresh)
            /* Ignore small right partition. */
            hi = right_ptr;
          else if ((right_ptr - lo) > (hi - left_ptr))
            {
              /* Push larger left partition indices. */
              PUSH (lo, right_ptr);
              lo = left_ptr;
            }
          else
            {
              /* Push larger right partition indices. */
              PUSH (left_ptr, hi);
              hi = right_ptr;
            }
        }
    }
  /* Once the BASE_PTR array is partially sorted by quicksort the rest
     is completely sorted using insertion sort, since this is efficient
     for partitions below MAX_THRESH size. BASE_PTR points to the beginning
     of the array to sort, and END_PTR points at the very last element in
     the array (*not* one beyond it!). */
#define min(x, y) ((x) < (y) ? (x) : (y))
  {
    char *const end_ptr = &base_ptr[size * (total_elems - 1)];
    char *tmp_ptr = base_ptr;
    char *thresh = min(end_ptr, base_ptr + max_thresh);
    char *run_ptr;
    /* Find smallest element in first threshold and place it at the
       array's beginning.  This is the smallest array element,
       and the operation speeds up insertion sort's inner loop. */
    for (run_ptr = tmp_ptr + size; run_ptr <= thresh; run_ptr += size)
      /**
      if ((*cmp) ((void *) run_ptr, (void *) tmp_ptr, arg) < 0)
        tmp_ptr = run_ptr;
      **/
    {
        COMPARE__DUMP_FULL_SWITCH(OBC_TYPE,OBC_ACTION_COMPARE_L_,CMP5,truth,run_ptr,tmp_ptr);
        if(truth){
            tmp_ptr = run_ptr;
        }
    }
    if (tmp_ptr != base_ptr)
      SWAP (tmp_ptr, base_ptr, size);
    /* Insertion sort, running from left-hand-side up to right-hand-side.  */
    run_ptr = base_ptr + size;
    while ((run_ptr += size) <= end_ptr)
      {
        tmp_ptr = run_ptr - size;
        /**
        while ((*cmp) ((void *) run_ptr, (void *) tmp_ptr, arg) < 0)
          tmp_ptr -= size;
        **/
        goto PIPE_SKIP_4;
        do{
          tmp_ptr -= size;
          PIPE_SKIP_4:
          COMPARE__DUMP_FULL_SWITCH(OBC_TYPE,OBC_ACTION_COMPARE_L_,CMP6,truth,run_ptr,tmp_ptr);
        }while(truth);
        tmp_ptr += size;
        if (tmp_ptr != run_ptr)
          {
            char *trav;
            trav = run_ptr + size;
            while (--trav >= run_ptr)
              {
                char c = *trav;
                char *hi, *lo;
                for (hi = lo = trav; (lo -= size) >= tmp_ptr; hi = lo)
                  *hi = *lo;
                *hi = c;
              }
          }
      }
  }
}

/**
unsigned long long int funcCalls;
unsigned long long int heapSorts;
void heapify(unsigned char *arr, unsigned long long int length, unsigned long long int i)
{
    unsigned long long int largest = i; // Initialize largest as root
    unsigned long long int l = 2*i + 1; // left = 2*i + 1
    unsigned long long int r = 2*i + 2; // right = 2*i + 2

    funcCalls++;

    // If left child is larger than root
    if (l < length && arr[l] > arr[largest])
        largest = l;

    // If right child is larger than largest so far
    if (r < length && arr[r] > arr[largest])
        largest = r;

    // If largest is not root
    if (largest != i)
    {
        SWAP(&arr[i], &arr[largest],sizeof(unsigned char));

        // Recursively heapify the affected sub-tree
        heapify(arr, length, largest);
    }
}

// main function to do heap sort
void heapSort(unsigned char *arr, unsigned long long int length)
{

    funcCalls++;

    unsigned long long int i;

    // Build heap (rearrange array)
    for (i = length / 2 - 1; i >= 0; i--)
        heapify(arr, length, i);

    // One by one extract an element from heap
    for (i=length-1; i>=0; i--)
    {
        // Move current root to end
        SWAP(&arr[0], &arr[i], sizeof(unsigned char));

        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

unsigned long long int partition(unsigned char *arr, unsigned long long int length)
{

    funcCalls++;

    OBC_ENUM type = OBC_ACTION_COMPARE_UCHAR_;
    char truth;
    /* Select median value from among LO, MID, and HI. Rearrange
                 LO and HI so the three values are sorted. This lowers the
                 probability of picking a pathological pivot value and
                 skips a comparison for both the LEFT_PTR and RIGHT_PTR in
                 the while loops. * /
    unsigned char *mid = arr+(length/2);
    /**
    if ((*cmp) ((void *) mid, (void *) lo, arg) < 0)
      SWAP (mid, lo, size);
    ** /
    COMPARE__DUMP_FULL_SWITCH(type,OBC_ACTION_COMPARE_L_, CMP0,truth,mid,arr);
    if(truth)
    {
        SWAP (mid, arr, sizeof(unsigned char));
    }
    /**
    if ((*cmp) ((void *) hi, (void *) mid, arg) < 0)
      SWAP (mid, hi, size);
    ** /
    COMPARE__DUMP_FULL_SWITCH(type,OBC_ACTION_COMPARE_L_,CMP1,truth,arr+length-1,mid);
    if(truth)
    {
        SWAP (mid, arr+length-1, sizeof(unsigned char));
    }
    else
        goto jump_over;
    /**
    if ((*cmp) ((void *) mid, (void *) lo, arg) < 0)
      SWAP (mid, lo, size);
    ** /
    COMPARE__DUMP_FULL_SWITCH(type,OBC_ACTION_COMPARE_L_,CMP2,truth,mid,arr);
    if(truth)
    {
        SWAP (mid, arr, sizeof(unsigned char));
    }
    jump_over:
    return (unsigned long long int)(mid-arr);

}

void introsort(unsigned char *arr, unsigned long long int length, int maxdepth){
    unsigned long long int p = partition(arr,length);  // assume this function does pivot selection, p is the final position of the pivot

    p = length/2;

    funcCalls++;

    if (length <= 1){
        return;
    }else if (maxdepth == 0){
        heapSorts++;
        heapSort(arr,length);
        return;
    }else{
        introsort(arr,p, maxdepth - 1);
        introsort(arr+p+1,(length)-p, maxdepth - 1);
    }
}

void OBC_introSortFunc(unsigned char *arr, unsigned long long int length){
    int maxdepth = (int)(log((double)length)/log(2));
    printf("MAX DEPTH: %i\n",maxdepth);
    funcCalls = 0;
    heapSorts = 0;
    introsort(arr, length, maxdepth);

    printf("FUNC CALLS: %llu\n",funcCalls);
    printf("HEAP SORTS: %llu\n",heapSorts);
}**/


/*
void heapSort(unsigned char *arr, unsigned long long int length);
void heapify(unsigned char *arr, unsigned long long int length, unsigned long long int i);
void swapValue(unsigned char *a, unsigned char *b);
void InsertionSort(unsigned char arr[], unsigned char *begin, unsigned char *end);
int* Partition(unsigned char *arr, unsigned long long int low, unsigned long long int high);
unsigned char *MedianOfThree(unsigned char * a, unsigned char * b, unsigned char * c);


unsigned long long int funcCalls;
unsigned long long int heapSorts;
void heapify(unsigned char *arr, unsigned long long int length, unsigned long long int i)
{
    unsigned long long int largest = i; // Initialize largest as root
    unsigned long long int l = 2*i + 1; // left = 2*i + 1
    unsigned long long int r = 2*i + 2; // right = 2*i + 2

    funcCalls++;

    // If left child is larger than root
    if (l < length && largest < length){
        printf("LEFT_ %llu: %i\n",l, arr[l]);
        printf("LARGE %llu: %i\n",largest,arr[largest]);
        if(arr[l] > arr[largest])
        largest = l;
    }

    // If right child is larger than largest so far
    if (r < length && arr[r] > arr[largest])
        largest = r;

    // If largest is not root
    if (largest != i)
    {
        SWAP(arr +i, arr + largest,sizeof(unsigned char));

        // Recursively heapify the affected sub-tree
        heapify(arr, length, largest);
    }
}
void heapSort(unsigned char *arr, unsigned long long int length)
{

    funcCalls++;

    unsigned long long int i = 0;

    // Build heap (rearrange array)
    for (i = length / 2 - 1; i >= 0; i--)
        heapify(arr, length-1, i);

    // One by one extract an element from heap
    for (i=length-1; i>=0; i--)
    {
        // Move current root to end
        SWAP(arr, arr+i, sizeof(unsigned char));

        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

// A utility function to swap the values pointed by
// the two pointers
void swapValue(unsigned char *a, unsigned char *b)
{
    unsigned char *temp = a;
    a = b;
    b = temp;
    return;
}

//* Function to sort an array using insertion sort* /
void InsertionSort(unsigned char arr[], unsigned char *begin, unsigned char *end)
{
    // Get the left and the right index of the subarray
    // to be sorted
    unsigned long long int left = begin - arr;
    unsigned long long int right = end - arr;
unsigned long long int i;
    for (i = left+1; i <= right; i++)
    {
        unsigned char key = arr[i];
        unsigned long long int j = i-1;

       //* Move elements of arr[0..i-1], that are
        //  greater than key, to one position ahead
        //  of their current position * /
        while (j >= left && arr[j] > key)
        {
            arr[j+1] = arr[j];
            j = j-1;
        }
        arr[j+1] = key;
   }

   return;
}

// A function to partition the array and return
// the partition point
int* Partition(unsigned char *arr, unsigned long long int low, unsigned long long int high)
{
    unsigned char pivot = arr[high];    // pivot
    unsigned long long int i = (low - 1);  // Index of smaller element
unsigned long long int j;
    for (j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] <= pivot)
        {
            // increment index of smaller element
            i++;

            swapValue(arr+i , arr+j );
        }
    }
    swapValue(arr+i + 1, arr + high);
    return arr + i + 1;
}

// A function that find the middle of the
// values pointed by the pointers a, b, c
// and return that pointer
unsigned char *MedianOfThree(unsigned char * a, unsigned char * b, unsigned char * c)
{
    if (*a < *b && *b < *c)
        return (b);

    if (*a < *c && *c <= *b)
        return (c);

    if (*b <= *a && *a < *c)
        return (a);

    if (*b < *c && *c <= *a)
        return (c);

    if (*c <= *a && *a < *b)
        return (a);

    if (*c <= *b && *b <= *a)
        return (b);
}

// A Utility function to perform intro sort
void IntrosortUtil(unsigned char arr[], unsigned char * begin,
                  unsigned char * end, int depthLimit)
{
    // Count the number of elements
    unsigned long long int size = (unsigned long long int)(end - begin);

      // If partition size is low then do insertion sort
    if (size < 16)
    {
        InsertionSort(arr, begin, end);
        return;
    }

    // If the depth is zero use heapsort
    if (depthLimit == 0)
    {
        //make_heap(begin, end+1);
        //sort_heap(begin, end+1);
        if(begin < end)
        heapSort(begin,size);

        return;
    }

    // Else use a median-of-three concept to
    // find a good pivot
    unsigned char * pivot = MedianOfThree(begin, begin+size/2, end);

    // Swap the values pointed by the two pointers
    ///swapValue(pivot, end);

   // Perform Quick Sort
    unsigned char * partitionPoint = Partition(arr, begin-arr, end-arr);
    IntrosortUtil(arr, begin, partitionPoint-1, depthLimit - 1);
    IntrosortUtil(arr, partitionPoint + 1, end, depthLimit - 1);

    return;
}

//* Implementation of introsort* /
void OBC_Introsort2(unsigned char arr[], unsigned char *begin, unsigned char *end)
{
    int depthLimit = 2 * (log(end-begin)/log(2));

    // Perform a recursive Introsort
    IntrosortUtil(arr, begin, end, depthLimit);

      return;
}
//*/







/**
// To heapify a subtree rooted with node i which is
// an index in arr[]. n is size of heap
void heapify(int arr[], int n, int i)
{
    int largest = i; // Initialize largest as root
    int l = 2*i + 1; // left = 2*i + 1
    int r = 2*i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (l < n && arr[l] > arr[largest])
        largest = l;

    // If right child is larger than largest so far
    if (r < n && arr[r] > arr[largest])
        largest = r;

    // If largest is not root
    if (largest != i)
    {
        SWAP(arr +i, arr+largest,sizeof(int));

        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

// main function to do heap sort
void heapSort(int arr[], int n)
{int i;
    // Build heap (rearrange array)
    for (i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // One by one extract an element from heap
    for (i=n-1; i>=0; i--)
    {
        // Move current root to end
        SWAP(arr, arr+i, sizeof(int));

        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}


// A utility function to swap the values pointed by
// the two pointers
void swapValue(int *a, int *b)
{
    int *temp = a;
    a = b;
    b = temp;
    return;
}

//* Function to sort an array using insertion sort* /
void InsertionSort(int arr[], int *begin, int *end)
{
    // Get the left and the right index of the subarray
    // to be sorted
    int left = begin - arr;
    int right = end - arr;
int i;
    for (i = left+1; i <= right; i++)
    {
        int key = arr[i];
        int j = i-1;

        //* Move elements of arr[0..i-1], that are
        //  greater than key, to one position ahead
        //  of their current position * /
        while (j >= left && arr[j] > key)
        {
            arr[j+1] = arr[j];
            j = j-1;
        }
        arr[j+1] = key;
   }

   return;
}

// A function to partition the array and return
// the partition point
int* Partition(int arr[], int low, int high)
{
    int pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element
int j;
    for ( j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] <= pivot)
        {
            // increment index of smaller element
            i++;

            SWAP(arr+i, arr+j,sizeof(int));
        }
    }
    SWAP(arr+ i + 1, arr+high,sizeof(int));
    return (arr + i + 1);
}


// A function that find the middle of the
// values pointed by the pointers a, b, c
// and return that pointer
int *MedianOfThree(int * a, int * b, int * c)
{
    if (*a < *b && *b < *c)
        return (b);

    if (*a < *c && *c <= *b)
        return (c);

    if (*b <= *a && *a < *c)
        return (a);

    if (*b < *c && *c <= *a)
        return (c);

    if (*c <= *a && *a < *b)
        return (a);

    if (*c <= *b && *b <= *a)
        return (b);
}

// A Utility function to perform intro sort
void IntrosortUtil(int arr[], int * begin,
                  int * end, int depthLimit)
{
    // Count the number of elements
    int size = end - begin;

      // If partition size is low then do insertion sort
    if (size < 16)
    {
        InsertionSort(arr, begin, end);
        return;
    }

    // If the depth is zero use heapsort
    if (depthLimit == 0)
    {
        //make_heap(begin, end+1);
        //sort_heap(begin, end+1);
        heapSort(begin, size);
        return;
    }

    // Else use a median-of-three concept to
    // find a good pivot
    int * pivot = MedianOfThree(begin, begin+size/2, end);

    // Swap the values pointed by the two pointers
    swapValue(pivot, end);

   // Perform Quick Sort
    int * partitionPoint = Partition(arr, begin-arr, end-arr);
    IntrosortUtil(arr, begin, partitionPoint-1, depthLimit - 1);
    IntrosortUtil(arr, partitionPoint + 1, end, depthLimit - 1);

    return;
}

//* Implementation of introsort* /
void Introsort(int arr[], int *begin, int *end)
{
    int depthLimit = 2 * log(end-begin);

    // Perform a recursive Introsort
    IntrosortUtil(arr, begin, end, depthLimit);

      return;
}
//**/
