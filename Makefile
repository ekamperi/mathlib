MAKE = make
SUBDIRS = math fenv complex

.PHONY: all
all:
	@for i in $(SUBDIRS); 			\
	do					\
		(cd $$i; $(MAKE) all);		\
	done

.PHONY: run
run:
	@for i in $(SUBDIRS); 			\
	do 					\
		(cd $$i; $(MAKE) run);		\
	done

.PHONY: clean
clean:
	@for i in $(SUBDIRS); 			\
	do					\
		(cd $$i; $(MAKE) clean);	\
	done

.PHONY: git-clean
git-clean:
	@for i in $(SUBDIRS); 			\
	do					\
		(cd $$i; $(MAKE) git-clean);	\
	done

################################################################################
#				Report generation			       #
################################################################################
TMPFILES= tmp.xml tmp2.xml tmp3.xml tmp.html tmp2.html

.PHONY: run-html
run-html:
	@echo "*** This may take a while, depending on the value of      ***"
	@echo "*** 'iterations' configuration variable, in your Atffile  ***"
	@echo "*** and the speed of your computer.                       ***"
	@echo
	@rm -f $(TMPFILES)
	atf-run $(SUBDIRS) | atf-report -o xml:tmp.xml
	@cat tmp.xml | col > tmp2.xml
	@sed 's/http:\/\/www.NetBSD.org\/XML\/atf/\./' tmp2.xml > tmp3.xml
	@xsltproc ./tests-results.xsl tmp3.xml > tmp.html
	@sed 's/tests-results.css/\.\.\/\.\.\/\.\.\/\.\.\/css\/tests-results.css/' \
		tmp.html > scripts/results.html
	@rm -f $(TMPFILES)
