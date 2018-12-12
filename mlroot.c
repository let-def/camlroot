#include "mlroot.h"
#include <string.h>
#include <caml/alloc.h>
#include <caml/memory.h>

static void validate_abort()
{

}

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

static void validate_root(value *root)
{
  (void)root;
}

static void validate_block(value *root)
{
  (void)root;
}

static void validate_block_field(value *root, size_t index)
{
  (void)root;
}

static void validate_block_tag(value *root, tag_t tag)
{
  (void)root;
}

static void validate_block_tag_field(value *root, tag_t tag, size_t index)
{
  (void)root;
}

static void validate_out_of_ocaml_heap(const void *ptr)
{

}

bool mlroot_value_is_int(value *root)
{
  validate_root(root);
  return Is_long(*root);
}

tag_t mlroot_block_tag(value *root)
{
  validate_block(root);
  return Tag_val(*root);
}

mlsize_t mlroot_block_size(value *root)
{
  validate_block(root);
  return Wosize_val(*root);
}

void mlroot_alloc(value *dst, mlsize_t size, tag_t tag)
{
  validate_root(dst);
  *dst = caml_alloc(size, tag);
}

void mlroot_get_field(value *dst, value *src, int index)
{
  validate_root(dst);
  validate_block_field(src, index);
  *dst = Field(*src, index);
}

void mlroot_set_field(value *dst, int index, value *src)
{
  validate_block_field(dst, index);
  validate_root(src);
  Store_field(*dst, index, *src);
}

/* Primitive types conversions */

long mlroot_long_val(value *root)
{
  validate_root(root);
  return Long_val(*root);
}

void mlroot_val_long(value *dst, long v)
{
  validate_root(dst);
  *dst = v;
}

bool mlroot_bool_val(value *root)
{
  validate_root(root);
  return Bool_val(*root);
}

void mlroot_val_bool(value *root, bool b)
{
  validate_root(root);
  *root = Val_bool(b);
}

double mlroot_double_val(value *src)
{
  validate_root(src);
  return Double_val(*src);
}

void mlroot_val_double(value* src, double d)
{
  validate_root(src);
  *src = caml_copy_double(d);
}

void mlroot_doubles_alloc(value *dst, size_t len)
{
  validate_root(dst);
  *dst = caml_alloc_float_array(len);
}

size_t mlroot_doubles_length(value *src)
{
  validate_root(src);
  return Wosize_val(*src) / Double_wosize;
}

double mlroot_doubles_get(value *src, int index)
{
  validate_root(src);
  return Double_field(*src, index);
}

void mlroot_doubles_set(value *src, int index, double v)
{
  validate_block_tag(src, Double_array_tag);
  Store_double_field(*src, index, v);
}

char *mlroot_string_alloc(value *root, size_t len)
{
  validate_root(root);
  *root = caml_alloc_string(len);
  return String_val(*root);
}

size_t mlroot_string_length(value *root)
{
  validate_block_tag(root, String_tag);
  return caml_string_length(*root);
}

const char *mlroot_string_val(value *root)
{
  validate_block_tag(root, String_tag);
  return String_val(*root);
}

void mlroot_string_copy(value *root, const char *src)
{
  validate_root(root);
  validate_out_of_ocaml_heap(src);

  size_t len = strlen(src);
  *root = caml_alloc_string(len);
  memcpy(String_val(*root), src, len);
}

void mlroot_string_init(value *root, const char *src, size_t len)
{
  validate_root(root);
  validate_out_of_ocaml_heap(src);

  *root = caml_alloc_string(len);
  memcpy(String_val(*root), src, len);
}

char  *mlroot_bytes_alloc(value *root, size_t len)
{
  validate_root(root);
  *root = caml_alloc_string(len);
  return String_val(*root);
}

size_t mlroot_bytes_length(value *root)
{
  validate_block_tag(root, String_tag);
  return caml_string_length(*root);
}

char  *mlroot_bytes_val(value *root)
{
  validate_block_tag(root, String_tag);
  return String_val(*root);
}


void mlroot_bytes_copy(value *root, const char *src)
{
  validate_root(root);
  validate_out_of_ocaml_heap(src);

  size_t len = strlen(src);
  *root = caml_alloc_string(len);
  memcpy(String_val(*root), src, len);

}

void mlroot_bytes_init(value *root, const char *src, size_t len)
{
  validate_root(root);
  validate_out_of_ocaml_heap(src);

  *root = caml_alloc_string(len);
  memcpy(String_val(*root), src, len);
}
