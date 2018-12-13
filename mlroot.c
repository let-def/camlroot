#include "mlroot.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <caml/address_class.h>
#include <caml/alloc.h>
#include <caml/memory.h>

static void mlroot_abort(const char *function,
                           const char *arg,
                           const char *reason)
{
  fprintf(stderr, "%s: %s %s\n", function, arg, reason);
  abort();
}

#define ABORT(arg, reason) mlroot_abort(__func__, #arg, reason)

static bool is_registered_root(value *root)
{
  struct caml__roots_block *lr;
  for (lr = caml_local_roots; lr != NULL; lr = lr->next) {
    for (intnat i = 0; i < lr->ntables; i++){
      for (intnat j = 0; j < lr->nitems; j++){
        if (root == &(lr->tables[i][j]))
          return true;
      }
    }
  }
  return false;
}

#ifdef DEBUG

#define VALIDATE_ROOT(r) \
  if (!is_registered_root(r)) ABORT(r, "is not a registered root")

#define VALIDATE_BLOCK(r) \
  VALIDATE_ROOT(r); \
  if (!Is_block(*r)) ABORT(r, "is not a block")

#define VALIDATE_BLOCK_FIELD(r, index) \
  VALIDATE_BLOCK(r); \
  if (Wosize_val(*r) <= index) ABORT(r, "is too small (access out of bound)")


#define VALIDATE_BLOCK_TAG(r, tag) \
  VALIDATE_BLOCK(r); \
  if (Tag_val(*r) != tag) ABORT(r, "has an invalid tag")

#define VALIDATE_BLOCK_TAG_FIELD(r, tag, field) \
  VALIDATE_BLOCK_TAG(r, tag, field); \
  if (Wosize_val(*r) <= index) ABORT(r, "is too small (access out of bound)")

#define VALIDATE_OUT_OF_HEAP(r) \
  if (Is_in_heap(r)) ABORT(r, "should be out of OCaml heap")

#else

#define VALIDATE_ROOT(r) (void)0
#define VALIDATE_BLOCK(r)(void)0
#define VALIDATE_BLOCK_FIELD(r, index) (void)0
#define VALIDATE_BLOCK_TAG(r, tag) (void)0
#define VALIDATE_BLOCK_TAG_FIELD(r, tag, field) (void)0
#define VALIDATE_OUT_OF_HEAP(r) (void)0

#endif

bool mlroot_value_is_int(value *root)
{
  VALIDATE_ROOT(root);
  return Is_long(*root);
}

tag_t mlroot_block_tag(value *root)
{
  VALIDATE_BLOCK(root);
  return Tag_val(*root);
}

mlsize_t mlroot_block_size(value *root)
{
  VALIDATE_BLOCK(root);
  return Wosize_val(*root);
}

void mlroot_alloc(value *dst, mlsize_t size, tag_t tag)
{
  VALIDATE_ROOT(dst);
  *dst = caml_alloc(size, tag);
}

void mlroot_get_field(value *dst, value *src, int index)
{
  VALIDATE_ROOT(dst);
  VALIDATE_BLOCK_FIELD(src, index);
  *dst = Field(*src, index);
}

void mlroot_set_field(value *dst, int index, value *src)
{
  VALIDATE_BLOCK_FIELD(dst, index);
  VALIDATE_ROOT(src);
  Store_field(*dst, index, *src);
}

/* Primitive types conversions */

long mlroot_long_val(value *root)
{
  VALIDATE_ROOT(root);
  return Long_val(*root);
}

void mlroot_val_long(value *dst, long v)
{
  VALIDATE_ROOT(dst);
  *dst = v;
}

bool mlroot_bool_val(value *root)
{
  VALIDATE_ROOT(root);
  return Bool_val(*root);
}

void mlroot_val_bool(value *root, bool b)
{
  VALIDATE_ROOT(root);
  *root = Val_bool(b);
}

double mlroot_double_val(value *src)
{
  VALIDATE_ROOT(src);
  return Double_val(*src);
}

void mlroot_val_double(value* src, double d)
{
  VALIDATE_ROOT(src);
  *src = caml_copy_double(d);
}

void mlroot_doubles_alloc(value *dst, size_t len)
{
  VALIDATE_ROOT(dst);
  *dst = caml_alloc_float_array(len);
}

size_t mlroot_doubles_length(value *src)
{
  VALIDATE_ROOT(src);
  return Wosize_val(*src) / Double_wosize;
}

double mlroot_doubles_get(value *src, int index)
{
  VALIDATE_ROOT(src);
  return Double_field(*src, index);
}

void mlroot_doubles_set(value *src, int index, double v)
{
  VALIDATE_BLOCK_TAG(src, Double_array_tag);
  Store_double_field(*src, index, v);
}

char *mlroot_string_alloc(value *root, size_t len)
{
  VALIDATE_ROOT(root);
  *root = caml_alloc_string(len);
  return String_val(*root);
}

size_t mlroot_string_length(value *root)
{
  VALIDATE_BLOCK_TAG(root, String_tag);
  return caml_string_length(*root);
}

const char *mlroot_string_val(value *root)
{
  VALIDATE_BLOCK_TAG(root, String_tag);
  return String_val(*root);
}

void mlroot_string_copy(value *root, const char *src)
{
  VALIDATE_ROOT(root);
  VALIDATE_OUT_OF_HEAP(src);

  size_t len = strlen(src);
  *root = caml_alloc_string(len);
  memcpy(String_val(*root), src, len);
}

void mlroot_string_init(value *root, const char *src, size_t len)
{
  VALIDATE_ROOT(root);
  VALIDATE_OUT_OF_HEAP(src);

  *root = caml_alloc_string(len);
  memcpy(String_val(*root), src, len);
}

char  *mlroot_bytes_alloc(value *root, size_t len)
{
  VALIDATE_ROOT(root);
  *root = caml_alloc_string(len);
  return String_val(*root);
}

size_t mlroot_bytes_length(value *root)
{
  VALIDATE_BLOCK_TAG(root, String_tag);
  return caml_string_length(*root);
}

char  *mlroot_bytes_val(value *root)
{
  VALIDATE_BLOCK_TAG(root, String_tag);
  return String_val(*root);
}


void mlroot_bytes_copy(value *root, const char *src)
{
  VALIDATE_ROOT(root);
  VALIDATE_OUT_OF_HEAP(src);

  size_t len = strlen(src);
  *root = caml_alloc_string(len);
  memcpy(String_val(*root), src, len);

}

void mlroot_bytes_init(value *root, const char *src, size_t len)
{
  VALIDATE_ROOT(root);
  VALIDATE_OUT_OF_HEAP(src);

  *root = caml_alloc_string(len);
  memcpy(String_val(*root), src, len);
}
