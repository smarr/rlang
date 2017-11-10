#include "rlang.h"

static
SEXP replace_na_(SEXP x, SEXP replacement, int start);

SEXP rlang_replace_na(SEXP x, SEXP replacement) {
  int n = r_length(x);
  int i = 0;

  switch(r_kind(x)) {
  case LGLSXP: {
    int* arr = LOGICAL(x);
    for (; i < n; ++i) {
      if (arr[i] == NA_LOGICAL)
        break;
    }
    break;
  }

  case INTSXP: {
    int* arr = INTEGER(x);
    for (; i < n; ++i) {
      if (arr[i] == NA_INTEGER)
        break;
    }
    break;
  }

  case REALSXP: {
    double* arr = REAL(x);
    for (; i < n; ++i) {
      if (ISNA(arr[i]))
        break;
    }
    break;
  }

  case STRSXP: {
    for (; i < n; ++i) {
      if (STRING_ELT(x, i) == NA_STRING)
        break;
    }
    break;
  }

  case CPLXSXP: {
    r_complex_t* arr = COMPLEX(x);

    for (; i < n; ++i) {
      if (ISNA(arr[i].r))
        break;
    }
    break;
  }

  default: {
    r_abort("Don't know how to handle object of type", Rf_type2char(r_kind(x)));
  }
  }

  if (i < n)
    return replace_na_(x, replacement, i);
  else
    return x;
}

static
SEXP replace_na_(SEXP x, SEXP replacement, int i) {
  KEEP(x = Rf_duplicate(x));
  int n = r_length(x);

  switch(r_kind(x)) {
  case LGLSXP: {
    int* arr = LOGICAL(x);
    int new_value = LOGICAL(replacement)[0];
    for (; i < n; ++i) {
      if (arr[i] == NA_LOGICAL)
        arr[i] = new_value;
    }
    break;
  }

  case INTSXP: {
    int* arr = INTEGER(x);
    int new_value = INTEGER(replacement)[0];
    for (; i < n; ++i) {
      if (arr[i] == NA_INTEGER)
        arr[i] = new_value;
    }
    break;
  }

  case REALSXP: {
    double* arr = REAL(x);
    double new_value = REAL(replacement)[0];
    for (; i < n; ++i) {
      if (ISNA(arr[i]))
        arr[i] = new_value;
    }
    break;
  }

  case STRSXP: {
    SEXP new_value = STRING_ELT(replacement, 0);
    for (; i < n; ++i) {
      if (STRING_ELT(x, i) == NA_STRING)
        SET_STRING_ELT(x, i, new_value);
    }
    break;
  }

  case CPLXSXP: {
    r_complex_t* arr = COMPLEX(x);
    r_complex_t new_value = COMPLEX(replacement)[0];

    for (; i < n; ++i) {
      if (ISNA(arr[i].r))
        arr[i] = new_value;
    }
    break;
  }

  default: {
    r_abort("Don't know how to handle object of type", Rf_type2char(r_kind(x)));
  }
  }

  FREE(1);
  return x;
}
