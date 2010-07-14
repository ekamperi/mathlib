#ifndef __T_NEXTAFTER_H__
#define __T_NEXTAFTER_H__

/*******************************************************************************
 *				float					       *
 ******************************************************************************/
const struct
t1fentry {
	float x;       /* Input */
	float y;       /* Input */
	float z;       /* nextafterf() output */
} t1ftable[] = {
	{ 1.0,	2.0,			1.0 + FLT_EPSILON },
	{ 1.0,	1.0 + FLT_EPSILON,	1.0 + FLT_EPSILON },
#ifdef	INFINITY
	{ 1.0,	INFINITY,		1.0 + FLT_EPSILON },
#endif
#ifdef	HUGE_VALF
	{ 1.0,	HUGE_VALF,		1.0 + FLT_EPSILON },
#endif

	{ 1.0,	-2.0,			1.0 - FLT_EPSILON/2 },
	{ 1.0,	 1.0 - FLT_EPSILON,	1.0 - FLT_EPSILON/2 },
#ifdef	INFINITY
	{ 1.0,	-INFINITY,		1.0 - FLT_EPSILON/2 },
#endif
#ifdef	HUGE_VALF
	{ 1.0,	-HUGE_VALF,		1.0 - FLT_EPSILON/2 },
#endif
};

/*******************************************************************************
 *                              double					       *
 ******************************************************************************/
const struct
t1dentry {
	double x;	/* Input */
	double y;	/* Input */
	double z;	/* nextafter() output */
} t1dtable[] = {
	{ 1.0,  2.0,			1.0 + DBL_EPSILON },
	{ 1.0,  1.0 + DBL_EPSILON,	1.0 + DBL_EPSILON },
#ifdef  HUGE_VAL
	{ 1.0,  HUGE_VAL,		1.0 + DBL_EPSILON },
#endif

	{ 1.0,  -2.0,			1.0 - DBL_EPSILON/2 },
	{ 1.0,   1.0 - DBL_EPSILON,	1.0 - DBL_EPSILON/2 },
#ifdef  HUGE_VAL
	{ 1.0,  -HUGE_VAL,		1.0 - DBL_EPSILON/2 },
#endif
};

/*******************************************************************************
 *                              long double				       *
 ******************************************************************************/
const struct
t1ldentry {
	long double x;		/* Input */
	long double y;		/* Input */
	long double z;		/* nextafterl() output */
} t1ldtable[] = {
	{ 1.0,  2.0,			1.0 + LDBL_EPSILON },
	{ 1.0,  1.0 + LDBL_EPSILON,	1.0 + LDBL_EPSILON },
#ifdef  HUGE_VALL
	{ 1.0,  HUGE_VALL,		1.0 + LDBL_EPSILON },
#endif

	{ 1.0,  -2.0,                   1.0 - LDBL_EPSILON/2 },
	{ 1.0,   1.0 - DBL_EPSILON,	1.0 - LDBL_EPSILON/2 },
#ifdef  HUGE_VALL
	{ 1.0,  -HUGE_VALL,		1.0 - LDBL_EPSILON/2 },
#endif
};

#endif /* ! __T_NEXTAFTER_H__ */
