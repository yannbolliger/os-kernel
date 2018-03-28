
/**
 * MinUnit - minimal test running framework
 */

#include <stdio.h>

extern int tests_run;

#define FAIL(message) printf("\nFailure in %s() line %d '%s'.\n", __func__, __LINE__, message)

#define _assert_message(message, test) do { if (!(test)) { FAIL(message); return 1; } } while (0)
#define _assert(test) _assert_message("", test)

#define _verify(test) do { int r = test(); tests_run++; if (r) return r; } while (0)
