#define _XOPEN_SOURCE 600

#include <atf-c.h>
#include <errno.h>
#include <fenv.h>
#include <float.h>	/* {DBL, FLT, LDBL}_EPSILON */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "subr_errhandling.h"
#include "subr_fpcmp.h"

struct tentry {
	double x;       /* Input */
	double y;       /* atanh output */
} ttable[] = {
	{ -0.98300989232637628307667866650414052,
	  -2.37986992196954893929767067278821100 },
	{ -0.93795424898798655428310379243227805,
	  -1.7207580761868919248593834902581755 },
	{ -0.90754290910008541327841884063081372,
	  -1.5134133015589968609850002058768177 },
	{ -0.89079494571633285488294737703184465,
	  -1.4257626394670719955257042052316218 },
	{ -0.76057170821908298483708926114590930,
	  -0.9975699527469023948566691662021247 },
	{ -0.73335742125965470881844377036313750,
	  -0.9359532037446405241792406177285530 },
	{ -0.52389063619319038553852199342777172,
	  -0.58168719445266030837254668121517959 },
	{ -0.49838535456068611112400215137805343,
	  -0.54715559534548800464496695456363696 },
	{ -0.48829736053540054351220866384208473,
	  -0.53382218023646149496866298849155007 },
	{ -0.48705538328395755775388385705972506,
	  -0.53219265551042131996569566891445690 },
	{ -0.40344939306589659055623103854924576,
	  -0.42776212987885710258225960127913916 },
	{ -0.24115458260162595521110553880896054,
	  -0.24599962493532628767292766263464512 },
	{ -0.18555955752640138992616655169948673,
	  -0.18773441938634736485741882089245788 },
	{ -0.091100188763111399099015307087983021,
	  -0.091353472110973789529236073371555984 },
	{ -0.081781410067753687417819848928514231,
	  -0.081964468679878609137985989520512606 },
	{ 0.020150485387821232146994946402178894,
	  0.020153213367331524349792953010876194 },
	{ 0.028521589047554500035542342940029560,
	  0.028529326748580325607408958316905855 },
	{ 0.053241861330743800811119787708728195,
	  0.053292255229572029704954365523078291 },
	{ 0.33705832445750471083363599418296737,
	  0.35077009186860941041706061859555510 },
	{ 0.35386502010204390741572611015872132,
	  0.36985516480181245386861128819459199 },
	{ 0.35615311908780661181233056359131479,
	  0.37247322326550873385549122705480026 },
	{ 0.38162606978234529638354081099349941,
	  0.40196152818346316268808726338299147 },
	{ 0.38933877474598097730597095339945536,
	  0.41102043270259701031864207205602373 },
	{ 0.41482610709858955879685835641047535,
	  0.44142642706584553746802165717602550 },
	{ 0.44507156291777543150319796393460400,
	  0.47853748029616026509503644079739418 },
	{ 0.45491722166848173045717620976783009,
	  0.49088330784333418461042845911369160 },
	{ 0.71435938678497438441845709767980417,
	  0.8960301654633007233257311033715028 },
	{ 0.78385973515370566916879385199344026,
	  1.0553035575179309009237057891139416 },
	{ 0.95279341373011296727512391386006435,
	  1.861241361934322130503477108270478 },
	{ 0.96868157745702157460107716135244785,
	  2.070456421806768804999778045642674 }
};

/*
 * Test case 1 -- Basic functionality
 */
ATF_TC(test_atanh1);
ATF_TC_HEAD(test_atanh1, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Basic functionality based on a set of good (x, atanh(x)) pairs");
}
ATF_TC_BODY(test_atanh1, tc)
{
	size_t i, N;

	N = sizeof(ttable) / sizeof(ttable[0]);
	for (i = 0; i < N; i++)
		ATF_CHECK(fpcmp_equal(atanh(ttable[i].x), ttable[i].y));
}

/*
 * Test case 2 -- Edge cases
 */
ATF_TC(test_atanh2);
ATF_TC_HEAD(test_atanh2, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check some edge cases");
}
ATF_TC_BODY(test_atanh2, tc)
{
	/* If x is NaN, a NaN shall be returned */
#ifdef  NAN
	ATF_CHECK(fpcmp_equal(atanh(NAN), NAN));
#endif

	/* If x is +-0, x shall be returned */
	ATF_CHECK(fpcmp_equal(atanh( 0.0),  0.0));
	ATF_CHECK(fpcmp_equal(atanh(-0.0), -0.0));
}

/*
 * Test case 3 -- Errors
 */
#define	NAN_IF_AVAIL 0
#define TYPE_DBL  1
#define TYPE_FLT  2
#define TYPE_LDBL 3

struct t3entry {
	int type;
	long double x;
	long double y;
	int error;
	int except;
} t3table[] = {
	/*
	 * If x is +-1, a pole error shall occur, and atanh(), atanhf(), and
	 * atanhl() shall return the value of the macro HUGE_VAL, HUGE_VALF,
	 * and HUGE_VALL, respectively, with the same sign as the correct
	 * value of the function.
	 */
#ifdef	HUGE_VAL
	{ TYPE_DBL,  1.0,  HUGE_VAL, ERANGE, FE_INVALID },
	{ TYPE_DBL, -1.0, -HUGE_VAL, ERANGE, FE_INVALID },
#endif
#ifdef	HUGE_VALF
	{ TYPE_FLT,  1.0,  HUGE_VALF, ERANGE, FE_INVALID },
	{ TYPE_FLT, -1.0, -HUGE_VALF, ERANGE, FE_INVALID },
#endif
#ifdef	HUGE_VALL
	{ TYPE_LDBL,  1.0,  HUGE_VALL, ERANGE, FE_INVALID },
	{ TYPE_LDBL, -1.0, -HUGE_VALL, ERANGE, FE_INVALID},
#endif

	/*
	 * For finite |x|>1, a domain error shall occur, and either a NaN
	 * (if supported), or an implementation-defined value shall be returned.
	 */
	{ TYPE_DBL,   1.0 +  DBL_EPSILON, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_FLT,   1.0 +  FLT_EPSILON, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_LDBL,  1.0 + LDBL_EPSILON, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_DBL,  -1.0 -  DBL_EPSILON, NAN_IF_AVAIL, ERANGE, FE_INVALID },
        { TYPE_FLT,  -1.0 -  FLT_EPSILON, NAN_IF_AVAIL, ERANGE, FE_INVALID },
        { TYPE_LDBL, -1.0 - LDBL_EPSILON, NAN_IF_AVAIL, ERANGE, FE_INVALID },

	/*
	 * If x is +-Inf, a domain error shall occur, and either a NaN
	 * (if supported), or an implementation-defined value shall be returned.
	 */
#ifdef	INFINITY
	{ TYPE_DBL,  INFINITY, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_DBL, -INFINITY, NAN_IF_AVAIL, ERANGE, FE_INVALID },

	{ TYPE_FLT,  INFINITY, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_FLT, -INFINITY, NAN_IF_AVAIL, ERANGE, FE_INVALID },

	{ TYPE_LDBL,  INFINITY, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_LDBL, -INFINITY, NAN_IF_AVAIL, ERANGE, FE_INVALID },
#endif
#ifdef  HUGE_VAL
	{ TYPE_DBL,  HUGE_VAL, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_DBL, -HUGE_VAL, NAN_IF_AVAIL, ERANGE, FE_INVALID },

	{ TYPE_FLT,  HUGE_VAL, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_FLT, -HUGE_VAL, NAN_IF_AVAIL, ERANGE, FE_INVALID },

	{ TYPE_LDBL,  HUGE_VAL, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_LDBL, -HUGE_VAL, NAN_IF_AVAIL, ERANGE, FE_INVALID },
#endif
#ifdef  HUGE_VALF
	{ TYPE_DBL,  HUGE_VALF, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_DBL, -HUGE_VALF, NAN_IF_AVAIL, ERANGE, FE_INVALID },

	{ TYPE_FLT,  HUGE_VALF, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_FLT, -HUGE_VALF, NAN_IF_AVAIL, ERANGE, FE_INVALID },

	{ TYPE_LDBL,  HUGE_VALF, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_LDBL, -HUGE_VALF, NAN_IF_AVAIL, ERANGE, FE_INVALID },
#endif
#ifdef  HUGE_VALL
	{ TYPE_DBL,  HUGE_VALL, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_DBL, -HUGE_VALL, NAN_IF_AVAIL, ERANGE, FE_INVALID },

	{ TYPE_FLT,  HUGE_VALL, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_FLT, -HUGE_VALL, NAN_IF_AVAIL, ERANGE, FE_INVALID },

	{ TYPE_LDBL,  HUGE_VALL, NAN_IF_AVAIL, ERANGE, FE_INVALID },
	{ TYPE_LDBL, -HUGE_VALL, NAN_IF_AVAIL, ERANGE, FE_INVALID }
#endif
};

ATF_TC(test_atanh3);
ATF_TC_HEAD(test_atanh3, tc)
{
	atf_tc_set_md_var(tc,
	    "descr",
	    "Check if errors are generated where due");
}
ATF_TC_BODY(test_atanh3, tc)
{
	long double ldy;
	int haserrexcept;
	int haserrno;
	size_t i, N;

	/* We can't proceed if there's no way to detect errors */
	query_errhandling(&haserrexcept, &haserrno);
	ATF_REQUIRE(haserrexcept || haserrno);

	N = sizeof(t3table) / sizeof(t3table[0]);
	ATF_REQUIRE(N > 0);
	for (i = 0; i < N; i++) {
		/* Initialize error handling */
		clear_exceptions();
		clear_errno();

		/* Override NAN if avalaible */
#ifdef	NAN
		if (t3table[i].y == NAN_IF_AVAIL)
			t3table[i].y = NAN;
#endif

		switch(t3table[i].type) {
		case TYPE_DBL:
			ldy = atanhl(t3table[i].x);
			break;
		case TYPE_FLT:
			ldy = atanhf(t3table[i].x);
			break;
		case TYPE_LDBL:
			ldy = atanhl(t3table[i].x);
			break;
		default:
			/* XXX abort */
			exit(1);		   
		}

		/* Check return value */
		if (t3table[i].y != NAN_IF_AVAIL) {
			ATF_CHECK(fpcmp_equal(ldy, t3table[i].y));
		}

		/* Check errno */
		ATF_CHECK(iserrno_equalto(t3table[i].error));

		/* Check floating-point exception */
		ATF_CHECK(raised_exceptions(t3table[i].except));
	}
}

/* Add test cases to test program */
ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, test_atanh1);
	ATF_TP_ADD_TC(tp, test_atanh2);
	ATF_TP_ADD_TC(tp, test_atanh3);

	return atf_no_error();
}
