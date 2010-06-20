CC99=gcc -std=c99
INCLUDE=-Iinclude -I/usr/opt/include -L/usr/opt/lib
CFLAGS=-Wall -W -Wformat-nonliteral -Wcast-align -Wpointer-arith 		\
	-Wbad-function-cast -Wmissing-prototypes -Wstrict-prototypes 		\
	-Wmissing-declarations -Winline -Wundef -Wnested-externs -Wcast-qual 	\
	-Wshadow -Wwrite-strings -Wno-unused-parameter 				\
	-Wswitch -Wbad-function-cast -g $(INCLUDE)
LIBS=-lm -latf-c

obj/%.o: %.c
	$(CC99) -c -o $@ $< $(CFLAGS)


all:	t_acos t_acosh t_asin t_asinh t_atan t_atan2 t_atanh t_cbrt t_ceil \
	t_constants t_copysign t_cos t_erf t_exp t_exp2 t_expm1 t_fabs t_fdim t_float \
	t_floor t_fmax t_fpclassify t_fpmacros t_mac t_tgamma t_hypot      \
	t_ldexp t_nextafter t_signbit t_sin t_sqrt t_trig_ident t_tanh     \
	t_trunc t_types

_DEPS_T_ACOS = t_acos.o subr_atf.o subr_fpcmp.o
 DEPS_T_ACOS = $(_DEPS_T_ACOS:%=obj/%)
t_acos: $(DEPS_T_ACOS)
	$(CC99) -o t_acos $(DEPS_T_ACOS) $(CFLAGS) $(LIBS)


_DEPS_T_ACOSH = t_acosh.o subr_fpcmp.o
 DEPS_T_ACOSH = $(_DEPS_T_ACOSH:%=obj/%)
t_acosh: $(DEPS_T_ACOSH)
	$(CC99) -o t_acosh $(DEPS_T_ACOSH) $(CFLAGS) $(LIBS)


_DEPS_T_ASIN = t_asin.o subr_atf.o subr_fpcmp.o
 DEPS_T_ASIN = $(_DEPS_T_ASIN:%=obj/%)
t_asin: $(DEPS_T_ASIN)
	$(CC99) -o t_asin $(DEPS_T_ASIN) $(CFLAGS) $(LIBS)


_DEPS_T_ASINH = t_asinh.o subr_fpcmp.o
 DEPS_T_ASINH = $(_DEPS_T_ASINH:%=obj/%)
t_asinh: $(DEPS_T_ASINH)
	$(CC99) -o t_asinh $(DEPS_T_ASINH) $(CFLAGS) $(LIBS)


_DEPS_T_ATAN = t_atan.o subr_atf.o subr_fpcmp.o
 DEPS_T_ATAN = $(_DEPS_T_ATAN:%=obj/%)
t_atan:	$(DEPS_T_ATAN)
	$(CC99) -o t_atan $(DEPS_T_ATAN) $(CFLAGS) $(LIBS)


_DEPS_T_ATAN2 = t_atan2.o subr_atf.o subr_fpcmp.o
 DEPS_T_ATAN2 = $(_DEPS_T_ATAN2:%=obj/%)
t_atan2: $(DEPS_T_ATAN2)
	$(CC99) -o t_atan2 $(DEPS_T_ATAN2) $(CFLAGS) $(LIBS)


_DEPS_T_ATANH = t_atanh.o subr_atf.o subr_errhandling.o subr_fpcmp.o
 DEPS_T_ATANH = $(_DEPS_T_ATANH:%=obj/%)
t_atanh: $(DEPS_T_ATANH)
	$(CC99) -o t_atanh $(DEPS_T_ATANH) $(CFLAGS) $(LIBS)


_DEPS_T_CBRT = t_cbrt.o subr_atf.o subr_fpcmp.o
 DEPS_T_CBRT = $(_DEPS_T_CBRT:%=obj/%)
t_cbrt: $(DEPS_T_CBRT)
	$(CC99) -o t_cbrt $(DEPS_T_CBRT) $(CFLAGS) $(LIBS)


_DEPS_T_CEIL = t_ceil.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_CEIL= $(_DEPS_T_CEIL:%=obj/%)
t_ceil: $(DEPS_T_CEIL)
	$(CC99) -o t_ceil $(DEPS_T_CEIL) $(CFLAGS) $(LIBS)


_DEPS_T_CONSTANTS = t_constants.o
 DEPS_T_CONSTANTS = $(_DEPS_T_CONSTANTS:%=obj/%)
t_constants: $(DEPS_T_CONSTANTS)
	$(CC99) -o t_constants $(DEPS_T_CONSTANTS) $(CFLAGS) $(LIBS)


_DEPS_T_COPYSIGN = t_copysign.o subr_atf.o subr_random.o
 DEPS_T_COPYSIGN = $(_DEPS_T_COPYSIGN:%=obj/%)
t_copysign: $(DEPS_T_COPYSIGN)
	$(CC99) -o t_copysign $(DEPS_T_COPYSIGN) $(CFLAGS) $(LIBS)


_DEPS_T_COS = t_cos.o subr_atf.o subr_errhandling.o	\
	     subr_fpcmp.o subr_random.o
 DEPS_T_COS = $(_DEPS_T_COS:%=obj/%)
t_cos: $(DEPS_T_COS)
	$(CC99) -o t_cos $(DEPS_T_COS) $(CFLAGS) $(LIBS)


_DEPS_T_ERF = t_erf.o subr_atf.o subr_errhandling.o  \
             subr_fpcmp.o subr_random.o
 DEPS_T_ERF = $(_DEPS_T_ERF:%=obj/%)
t_erf: $(DEPS_T_ERF)
	$(CC99) -o t_erf $(DEPS_T_ERF) $(CFLAGS) $(LIBS)


_DEPS_T_EXP = t_exp.o subr_atf.o subr_errhandling.o  \
	      subr_fpcmp.o subr_random.o
 DEPS_T_EXP = $(_DEPS_T_EXP:%=obj/%)
t_exp: $(DEPS_T_EXP)
	$(CC99) -o t_erf $(DEPS_T_EXP) $(CFLAGS) $(LIBS)


_DEPS_T_EXP2 = t_exp2.o subr_atf.o subr_errhandling.o  \
	       subr_fpcmp.o subr_random.o
 DEPS_T_EXP2 = $(_DEPS_T_EXP2:%=obj/%)
t_exp2: $(DEPS_T_EXP2)
	$(CC99) -o t_exp2 $(DEPS_T_EXP2) $(CFLAGS) $(LIBS)


_DEPS_T_EXPM1 = t_expm1.o subr_atf.o subr_errhandling.o  \
		subr_fpcmp.o subr_random.o
 DEPS_T_EXPM1 = $(_DEPS_T_EXPM1:%=obj/%)
t_expm1: $(DEPS_T_EXPM1)
	$(CC99) -o t_erf $(DEPS_T_EXPM1) $(CFLAGS) $(LIBS)


_DEPS_T_FABS = t_fabs.o subr_fpcmp.o
 DEPS_T_FABS = $(_DEPS_T_FABS:%=obj/%)
t_fabs: $(DEPS_T_FABS)
	$(CC99) -o t_fabs $(DEPS_T_FABS) $(CFLAGS) $(LIBS)


_DEPS_T_FDIM = t_fdim.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_FDIM = $(_DEPS_T_FDIM:%=obj/%)
t_fdim: $(DEPS_T_FDIM)
	$(CC99) -o t_fdim $(DEPS_T_FDIM) $(CFLAGS) $(LIBS)


_DEPS_T_FMAX = t_fmax.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_FMAX = $(_DEPS_T_FMAX:%=obj/%)
t_fmax: $(DEPS_T_FMAX)
	$(CC99) -o t_fmax $(DEPS_T_FMAX) $(CFLAGS) $(LIBS)


_DEPS_T_FLOAT = t_float.o
 DEPS_T_FLOAT = $(_DEPS_T_FLOAT:%=obj/%)
t_float: $(DEPS_T_FLOAT)
	$(CC99) -o t_float $(DEPS_T_FLOAT) $(CFLAGS) $(LIBS)


_DEPS_T_FLOOR = t_floor.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_FLOOR = $(_DEPS_T_FLOOR:%=obj/%)
t_floor: $(DEPS_T_FLOOR)
	$(CC99) -o t_floor $(DEPS_T_FLOOR) $(CFLAGS) $(LIBS)


_DEPS_T_FPCLASSIFY = t_fpclassify.o
 DEPS_T_FPCLASSIFY = $(_DEPS_T_FPCLASSIFY:%=obj/%)
t_fpclassify: $(DEPS_T_FPCLASSIFY)
	$(CC99) -o t_fpclassify $(DEPS_T_FPCLASSIFY) $(CFLAGS) $(LIBS)


_DEPS_T_FPMACROS = t_fpmacros.o
 DEPS_T_FPMACROS = $(_DEPS_T_FPMACROS:%=obj/%)
t_fpmacros: $(DEPS_T_FPMACROS)
	$(CC99) -o t_fpmacros $(DEPS_T_FPMACROS) $(CFLAGS) $(LIBS)


_DEPS_T_HYPOT = t_hypot.o subr_errhandling.o subr_fpcmp.o
 DEPS_T_HYPOT = $(_DEPS_T_HYPOT:%=obj/%)
t_hypot: $(DEPS_T_HYPOT)
	$(CC99) -o t_hypot $(DEPS_T_HYPOT) $(CFLAGS) $(LIBS)


_DEPS_T_LDEXP = t_ldexp.o subr_fpcmp.o
 DEPS_T_LDEXP = $(_DEPS_T_LDEXP:%=obj/%)
t_ldexp: $(DEPS_T_LDEXP)
	$(CC99)	-o t_ldexp $(DEPS_T_LDEXP) $(CFLAGS) $(LIBS)


_DEPS_T_MAC = t_mac.o subr_atf.o subr_errhandling.o subr_random.o
 DEPS_T_MAC = $(_DEPS_T_MAC:%=obj/%)
t_mac: $(DEPS_T_MAC)
	$(CC99) -o t_mac $(DEPS_T_MAC) $(CFLAGS) $(LIBS)


_DEPS_T_NEXTAFTER = t_nextafter.o subr_atf.o subr_errhandling.o subr_random.o
 DEPS_T_NEXTAFTER = $(_DEPS_T_NEXTAFTER:%=obj/%)
t_nextafter: $(DEPS_T_NEXTAFTER)
	$(CC99) -o t_nextafter $(DEPS_T_NEXTAFTER) $(CFLAGS) $(LIBS)


_DEPS_T_SIGNBIT = t_signbit.o subr_atf.o subr_random.o
 DEPS_T_SIGNBIT = $(_DEPS_T_SIGNBIT:%=obj/%)
t_signbit: $(DEPS_T_SIGNBIT)
	$(CC99) -o t_signbit $(DEPS_T_SIGNBIT) $(CFLAGS) $(LIBS)


_DEPS_T_SIN = t_sin.o subr_atf.o subr_errhandling.o subr_fpcmp.o subr_random.o
 DEPS_T_SIN = $(_DEPS_T_SIN:%=obj/%)
t_sin: $(DEPS_T_SIN)
	$(CC99) -o t_sin $(DEPS_T_SIN) $(CFLAGS) $(LIBS)


_DEPS_T_SQRT = t_sqrt.o subr_atf.o subr_errhandling.o subr_fpcmp.o subr_random.o
 DEPS_T_SQRT = $(_DEPS_T_SQRT:%=obj/%)
t_sqrt: $(DEPS_T_SQRT)
	$(CC99) -o t_sqrt $(DEPS_T_SQRT) $(CFLAGS) $(LIBS)


_DEPS_T_TANH = t_tanh.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_TANH = $(_DEPS_T_TANH:%=obj/%)
t_tanh: $(DEPS_T_TANH)
	$(CC99) -o t_tanh $(DEPS_T_TANH) $(CFLAGS) $(LIBS)


_DEPS_T_TGAMMA = t_tgamma.o subr_atf.o subr_errhandling.o subr_fpcmp.o subr_random.o
 DEPS_T_TGAMMA = $(_DEPS_T_TGAMMA:%=obj/%)
t_tgamma: $(DEPS_T_TGAMMA)
	$(CC99) -o t_tgamma $(DEPS_T_TGAMMA) $(CFLAGS) $(LIBS)


_DEPS_T_TRIG_IDENT = t_trig_ident.o subr_atf.o subr_fpcmp.o subr_random.o
 DEPS_T_TRIG_IDENT = $(_DEPS_T_TRIG_IDENT:%=obj/%)
t_trig_ident: $(DEPS_T_TRIG_IDENT)
	$(CC99) -o t_trig_ident $(DEPS_T_TRIG_IDENT) $(CFLAGS) $(LIBS)


_DEPS_T_TRUNC = t_tgamma.o subr_atf.o subr_errhandling.o subr_fpcmp.o subr_random.o
 DEPS_T_TRUNC = $(_DEPS_T_TRUNC:%=obj/%)
t_trunc: $(DEPS_T_TRUNC)
	$(CC99) -o t_trunc $(DEPS_T_TRUNC) $(CFLAGS) $(LIBS)


_DEPS_T_TYPES = t_types.o
 DEPS_T_TYPES = $(_DEPS_T_TYPES:%=obj/%)
t_types: $(DEPS_T_TYPES)
	$(CC99) -o t_trunc $(DEPS_T_TYPES) $(CFLAGS) $(LIBS)


.PHONY: run
run:
	atf-run | atf-report

.PHONY: run-html
run-html:
	@echo "*** This may take awhile, depending on the value of       ***"
	@echo "*** 'iterations' configuration variable, in your Atffile. ***"
	@echo
	@rm -f tmp.xml tmp2.xml tmp3.xml
	@rm -f tmp.html tmp2.html
	atf-run . fenv | atf-report -o xml:tmp.xml
	@cat tmp.xml | col > tmp2.xml
	@sed 's/http:\/\/www.NetBSD.org\/XML\/atf/\./' tmp2.xml > tmp3.xml
	@xsltproc ./tests-results.xsl tmp3.xml > tmp.html
	@sed 's/tests-results.css/\.\.\/\.\.\/\.\.\/\.\.\/css\/tests-results.css/' tmp.html > results.html
	@rm -f tmp.xml tmp2.xml tmp3.xml
	@rm -f tmp.html

.PHONY: clean
clean:
	@rm -f obj/*.o *~ core
	@rm -f tmp.xml tmp2.xml tmp3.xml
	@rm -f tmp.html results.html

.PHONY: git-clean
git-clean:
	@git clean -fd > /dev/null
