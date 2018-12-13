#include "mlroot.h"
#include <caml/alloc.h>
#include <caml/memory.h>

CAMLprim
value mk_pair_caml(value a, value b)
{
  CAMLparam2(a, b);
  CAMLlocal1(result);
  result = caml_alloc(2, 0);
  Field(result, 0) = a;
  Field(result, 1) = b;
  CAMLreturn(result);
}

CAMLprim
value mk_pair_caml_slow(value a, value b)
{
  CAMLparam2(a, b);
  CAMLlocal1(result);
  result = caml_alloc(2, 0);
  Store_field(result, 0, a);
  Store_field(result, 1, b);
  CAMLreturn(result);
}

CAMLprim
value mk_pair_mlroot(value a, value b)
{
  CAMLparam2(a, b);
  CAMLlocal1(result);
  mlroot_alloc(&result, 2, 0);
  mlroot_set_field(&result, 0, &a);
  mlroot_set_field(&result, 1, &b);
  CAMLreturn(result);
}
