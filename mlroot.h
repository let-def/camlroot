#ifndef MLROOT_H
#define MLROOT_H

#include <stdbool.h>
#include <caml/mlvalues.h>

bool     mlroot_value_is_int   (value *root);
tag_t    mlroot_block_tag      (value *root);
mlsize_t mlroot_block_size     (value *root);

void     mlroot_alloc          (value *root, mlsize_t size, tag_t tag);
void     mlroot_get_field      (value *root, value *src, int index);
void     mlroot_set_field      (value *root, int index, value *v);

long     mlroot_get_long       (value *root);
void     mlroot_set_long       (value *root, long v);

bool     mlroot_get_bool       (value *root);
void     mlroot_set_bool       (value *root, bool b);

double   mlroot_get_double     (value *root);
void     mlroot_set_double     (value *root, double d);

void     mlroot_doubles_alloc  (value *root, size_t);
size_t   mlroot_doubles_length (value *root);
double   mlroot_doubles_get    (value *root, int index);
void     mlroot_doubles_set    (value *root, int index, double v);

const char *mlroot_get_string  (value *root);
char    *mlroot_string_alloc   (value *root, size_t);
size_t   mlroot_string_length  (value *root);
void     mlroot_string_copy    (value *dst, const char *src);
void     mlroot_string_init    (value *dst, const char *src, size_t length);

char    *mlroot_get_bytes      (value *root);
char    *mlroot_bytes_alloc    (value *root, size_t len);
size_t   mlroot_bytes_length   (value *root);
void     mlroot_bytes_copy     (value *dst, const char *src);
void     mlroot_bytes_init     (value *dst, const char *src, size_t length);

#endif /* MLROOT_H */
