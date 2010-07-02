#ifndef	__GEN_H__
#define	__GEN_H__

struct fentry {
	const char *f_name;
	size_t f_narg;
	union {
		int (*fp1)(double x);
		int (*fp2)(double x, double y);
	} f_u;
	double (*f_libm)();
	int (*f_mpfr)();
};

extern const struct fentry ftable[];
extern const int fsize;

const struct fentry *getfunctionbyname(const char *fname);
void printfunctions(void);

#endif	/* !__GEN_H__ */
