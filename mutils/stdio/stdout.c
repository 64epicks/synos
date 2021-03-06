/* This is free and unencumbered software released into the public domain. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h> /* for stdout */

#include "sysio_file.h"

/**
 * @date   2014-11-19
 * @author Arto Bendiken
 * @see    http://libc11.org/stdio/stdout.html
 */
static FILE __sysio_stdout = {
  .fd    = 1,
  .error = 0,
};

FILE* const stdout = &__sysio_stdout;