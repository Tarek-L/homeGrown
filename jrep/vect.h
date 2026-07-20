#pragma once
#include <stdlib.h>

typedef struct {
  size_t size;
  size_t capacity;
} vect_header;

void *VECT() {
  vect_header *header = malloc(sizeof(vect_header));
  if (!header) {
    exit(EXIT_FAILURE);
  }
  header->size = 0;
  header->capacity = 0;
  return (void *)(header + 1);
}

#define __GET_VECT_HEADER(v) ((vect_header *)(v) - 1)

#define VECT_PUSH(v, d)                                                        \
  do {                                                                         \
    if (__GET_VECT_HEADER(v)->size == __GET_VECT_HEADER(v)->capacity) {        \
      if (__GET_VECT_HEADER(v)->capacity == 0) {                                  \
        __GET_VECT_HEADER(v)->capacity += 2;                                   \
      } else {                                                                 \
        __GET_VECT_HEADER(v)->capacity *= 2;                                   \
      }                                                                        \
      vect_header *header =                                                    \
          realloc(__GET_VECT_HEADER(v),                                        \
                  sizeof(vect_header) +                                        \
                      (__GET_VECT_HEADER(v)->capacity * sizeof(*v)));          \
      if (!header) {                                                           \
        exit(EXIT_FAILURE);                                                    \
      }                                                                        \
      v = (void *)(header + 1);                                                \
    }                                                                          \
      v[__GET_VECT_HEADER(v)->size++] = d;                                     \
  } while (0)

#define VECT_LEN(v) __GET_VECT_HEADER(v)->size
#define VECT_FREE(v)                                                           \
  do {                                                                         \
    free(__GET_VECT_HEADER(v));                                                \
    (v) = NULL;                                                                \
  } while (0)
