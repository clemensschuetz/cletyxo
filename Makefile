ARCH ?= x86_64
INCLUDE_STDAPPS ?= true
BUILD_BOOT ?= false
DOWNLOAD_BOOT ?= false
DEFAULT_FONT ?= zap-light16.psf
NIGHTLY ?= false

ASM_FILES := $(shell find ./arch/$(ARCH) -type f -name '*.S')
ASM_OBJS := $(ASM_FILES:.c=.o)
LDS = linker.ld
LD  = ld
LDFLAGS = -T $(LDS) -static -Bsymbolic -nostdlib
MODE ?= debug
BUILD_DIR = build
FINAL_DIR = bin
HDD = cletyxo-$(ARCH).hdd
OVMFDIR = OVMFbin
CC = gcc
rwildcard =$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CFLAGS = "-ffreestanding -fshort-wchar -Wall -W -Werror -Wfatal-errors -Wpedantic"
CNIGHTLYFLAGS = $(CFLAGS) -DNIGHLTY

OUTNAME = cletyxo-$(ARCH)
OBJS = $(call rwildcard,$(BUILD_DIR),*.o)
OBJS += $(ASM_OBJS)



all: build link copy prepare make_img

run: build link copy prepare make_img qemu clean_qemu

build: build_bootloader build_projects build_arch

maybe_build_bootloader:
ifeq ($(BUILD_BOOT), true)
	$(MAKE) build_bootloader
endif

qemu:
	qemu-system-x86_64 -drive file=$(FINAL_DIR)/$(OUTNAME).img -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none

clean:
	@ rm -rf $(FINAL_DIR) || true
	@ rm -d $(FINAL_DIR) || true
	@ rm -rf $(BUILD_DIR) || true
	@ rm -d $(BUILD_DIR) || true
	@ mkdir $(FINAL_DIR) || true
	@ mkdir $(BUILD_DIR) || true
	@ $(MAKE) -C boot clean || true
	@ rm -rf thirdparty || true
	@ rm -d thirdparty || true

clean_qemu:
setup:
	@ mkdir $(FINAL_DIR) || true
	@ git clone https://github.com/echfs/echfs thirdparty/echfs || true
	@ mkdir $(BUILD_DIR) || true
	@ $(MAKE) BUILD_BOOT=true


prepare:
	@ cp config/startup.nsh $(BUILD_DIR)/startup.nsh
	@ cp fonts/$(DEFAULT_FONT) $(BUILD_DIR)/defaultfont.psf

make_iso:
	@ xorriso -as mkisofs -b $(BUILD_DIR)/limine-cd.bin \
			-no-emul-boot -boot-load-size 4 -boot-info-table \
			-eltorito-alt-boot -e $(BUILD_DIR)/limine-eltorito-efi.bin \
			-no-emul-boot $(BUILD_DIR) -o $(FINAL_DIR)/cletyxo-$(ARCH).iso
make_img:
	rm -rf $(FINAL_DIR) | true
	mkdir $(FINAL_DIR) | true
	dd if=/dev/zero of=$(FINAL_DIR)/$(OUTNAME).img bs=512 count=93750
	mformat -i $(FINAL_DIR)/$(OUTNAME).img -f 2880 ::
	mmd -i $(FINAL_DIR)/$(OUTNAME).img ::/EFI
	mmd -i $(FINAL_DIR)/$(OUTNAME).img ::/EFI/BOOT
	mcopy -i $(FINAL_DIR)/$(OUTNAME).img build/main.efi ::/EFI/BOOT
	mcopy -i $(FINAL_DIR)/$(OUTNAME).img $(BUILD_DIR)/startup.nsh ::
	mcopy -i $(FINAL_DIR)/$(OUTNAME).img $(BUILD_DIR)/defaultfont.psf ::
	mcopy -i $(FINAL_DIR)/$(OUTNAME).img $(BUILD_DIR)/cletyxo.elf ::

build_bootloader:
	@ $(MAKE) -C boot

build_projects:
	@ echo ""
	@ echo "-=-=-=-=-=-=-=-=-=-=-= ( BUILDING USING CARGO FOR ARCHITECTURE '$(ARCH)') =-=-=-=-=-=-=-=-=-=-=-"
	@ cargo build --target targets/$(ARCH)-cletyxo.json


package: build
#	Building for different Architectures
	@ $(MAKE)

build_arch:
#	make -C arch ARCH=$(ARCH)

link:
	@ echo $(OBJS)
	@ $(LD) $(LDFLAGS) -o $(BUILD_DIR)/cletyxo.elf $(OBJS) target/$(ARCH)-cletyxo/$(MODE)/libcletyxo.a
	@ rm $(OBJS) | true

copy:

# For CI (Github Actions)
ci:



.PHONY: all clean link ci clean build_bootloader build build_rust_projects build_arch
