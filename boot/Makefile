ARCH          = x86_64

BINDIR        = bin
IMGDIR        = img

HEADERS       = src/video.h src/memory.h src/rtl.h
OBJS          = src/main.o src/video.o src/memory.o src/rtl.o

EFIINC        = /usr/local/include/efi
EFIINCS       = -I$(EFIINC) -I$(EFIINC)/$(ARCH) -I$(EFIINC)/protocol
EFI_CRT_OBJS  = /usr/local/lib/crt0-efi-$(ARCH).o
EFI_LDS       = /usr/local/lib/elf_$(ARCH)_efi.lds

OVMF          = /usr/share/ovmf/OVMF.fd
QEMU_OPTS     = -net none -m 64 -device VGA #-S -s #-enable-kvm

CFLAGS        = $(EFIINCS) -xc -std=c11 -fno-stack-protector -fpic -fshort-wchar -mno-red-zone \
-Wall -Wno-incompatible-library-redeclaration -O2

ifeq ($(ARCH),x86_64)
  CFLAGS += -DHAVE_USE_MS_ABI
endif

LDFLAGS       = -nostdlib -znocombreloc -T $(EFI_LDS) -shared -Bsymbolic -L /usr/local/lib $(EFI_CRT_OBJS)

all: $(IMGDIR)/disk.img

run: $(IMGDIR)/disk.img
	qemu-system-x86_64 -cpu qemu64 -bios $(OVMF) -drive file=$(IMGDIR)/disk.img,if=ide $(QEMU_OPTS)

$(IMGDIR)/disk.img: $(IMGDIR)/data.img
	dd if=/dev/zero of=$@ bs=512 count=93750
	parted $@ -s -a minimal mklabel gpt
	parted $@ -s -a minimal mkpart EFI FAT16 2048s 93716s
	parted $@ -s -a minimal toggle 1 boot
	dd if=$(IMGDIR)/data.img of=$@ bs=512 count=91669 seek=2048 conv=notrunc

$(IMGDIR)/data.img: $(BINDIR)/boot.efi
	dd if=/dev/zero of=$@ bs=512 count=91669
	mformat -i $@ -h 32 -t 32 -n 64 -c 1
	mcopy -i $@ $< ::/

$(BINDIR)/boot.so: $(OBJS)
	ld.gold $(LDFLAGS) $(OBJS) -o $@ -lefi -lgnuefi

$(BINDIR)/%.efi: $(BINDIR)/%.so
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym  -j .rel -j .rela -j .reloc --target=efi-app-$(ARCH) $^ $@

%.o: %.c $(HEADERS)
	clang $(CFLAGS) -c -o $@ $<

