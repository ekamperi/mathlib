#include <math.h>
#include "mytypes.h"

#define	my_fpclassify(x)						\
	(fpclassify(x) == FP_INFINITE 				?	\
	    MY_FP_INFINITE  : fpclassify(x) == FP_NAN		?	\
	    MY_FP_NAN	    : fpclassify(x) == FP_NORMAL	?	\
	    MY_FP_NORMAL    : fpclassify(x) == FP_SUBNORMAL	?	\
	    MY_FP_SUBNORMAL : fpclassify(x) == FP_ZERO		?	\
	    MY_FP_ZERO	    : 1)
