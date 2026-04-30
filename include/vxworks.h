#pragma once

typedef int (*FUNCPTR)();

typedef int STATUS;

#define VX_OK		    (0)
#define VX_ERROR		(-1)

#define NO_WAIT         (0)
#define WAIT_FOREVER    (-1)

#include <stdbool.h>
