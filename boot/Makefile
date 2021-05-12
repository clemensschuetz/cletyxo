DOWNLOAD ?= false

all: clean do

do:
ifeq ($(DOWNLOAD), true)
	git clone https://github.com/Absurdponcho/gnu-efi extern
endif
	cp main.c extern/bootloader/main.c
	$(MAKE) -C extern
	$(MAKE) -C extern bootloader
	cp extern/x86_64/bootloader/main.efi ../build

clean:
ifeq ($DOWNLOAD), true)
	rm -rf extern
endif