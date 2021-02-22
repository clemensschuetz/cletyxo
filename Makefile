include kernel/Makefile

KERNELBINDIR = kernel/bin
KERNELDIR = kernel


dist: clean actual-dist-do-not-call-in-console

actual-dist-do-not-call-in-console:
	@echo !-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= MAKING DIRECTORIES... -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-!
	mkdir dist
	mkdir dist/bare
	mkdir dist/min
	mkdir dist/std
	@echo !-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= MAKING KERNEL DIRECTORIES -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-!
	make pfb-extern-call

setup: actual-dist-do-not-call-in-console 

clean:
	@echo !-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= CLEANING DIST FILES -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-!
	rm --dir --recursive dist
