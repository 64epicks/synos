X86DIR=$(ARCHDIR)/X86
include $(X86DIR)/.config

DEFINE_VALS += -D_X86_

IMG_LINK+=$(X86DIR)/initIMG.ld
ARCHOBJECTS += $(X86DIR)/cpuid.o

ifeq ($(PRINTF_FALLBACK), TRUE)
DEFINE_VALS += -DPRINTF_FALLBACK
endif
ifeq ($(LOAD_SYSTEM), MULTIBOOT2)
ARCHOBJECTS += $(X86DIR)/multiboot2.o
ARCHOBJECTS += $(X86DIR)/boot/multiboot2.o
else
ifeq ($(LOAD_SYSTEM), MULTIBOOT)
ARCHOBJECTS += $(X86DIR)/multiboot.o
ARCHOBJECTS += $(X86DIR)/boot/multiboot.o
endif
endif
ifeq ($(ENABLE_MEMSTACK), TRUE)
DEFINE_VALS += -DMEMSTACK_ENABLE
endif
ifdef SYSCALL_INT
DEFINE_VALS += -DSYSCALL_INT=$(SYSCALL_INT)
endif

X86: $(ARCHOBJECTS)
	@echo -e "$(SECTIONC)[arch] $(LINKC)Linking arch_x86 object files -> arch/arch.o$(NC)"
	@$(LINK) $(LINK_ARCH) -r $(ARCHOBJECTS) -o $(SOURCE_DIR)/arch/arch.o

$(X86DIR)/boot/%.o: $(X86DIR)/boot/%.asm
	@echo -e "$(SECTIONC)[arch] $(COMPILEC)Compiling ASM object $@$(NC)"
	@$(ASM) $(ASM_FLAGS) -i $(X86DIR)/boot/ $< -o $@

$(X86DIR)/%.o: $(X86DIR)/%.asm
	@echo -e "$(SECTIONC)[arch] $(COMPILEC)Compiling ASM object $@$(NC)"
	@$(ASM) $(ASM_FLAGS) $< -o $@

$(X86DIR)/%.o: $(X86DIR)/%.c
	@echo -e "$(SECTIONC)[arch] $(COMPILEC)Compiling C object $@$(NC)"
	@$(CC) $(CC_FLAGS) $< -o $@

.PHONY: grub
grub:
	@echo -e "$(SECTIONC)[install]$(INFOC) Installing synos along with GRUB to iso file $(INSTALL_DRIVE)...$(NC)"
	@mkdir -p $(ROOT_DIR)/boot/grub
	@cp conf/GRUB2.cfg.default $(ROOT_DIR)/boot/grub/grub.cfg
	@cp $(INITIMG_PREFIX)/synos $(ROOT_DIR)/boot/synos
	@grub-mkrescue -o synos.iso $(ROOT_DIR)
	@rm -rf $(ROOT_DIR)
	@echo -e "$(SECTIONC)[install]$(INFOC) $(INSTALL_DRIVE) created!$(NC)"
.PHONY: qemu
qemu:
	@echo -e "$(SECTIONC)[qemu]$(INFOC) Starting qemu with command: qemu-system-x86_64 -cpu qemu64 -display gtk -smp 2 -m 2G -vga std -cdrom $(INSTALL_DRIVE) -gdb tcp::9000 -S -monitor stdio $(QEMU_ARGS)$(NC)"
	@qemu-system-i386 -cpu qemu32 -display gtk -smp 2 -m 2G -vga std -cdrom $(INSTALL_DRIVE) -gdb tcp::9000 -S -monitor stdio
.PHONY: install
install:
	@echo -e "$(SECTIONC)[install]$(INFOC) Installing synos to $(INSTALL_DIR)/synos ...$(NC)"
	@cp $(ROOT_DIR)/boot/synos $(INSTALL_DIR)/synos
	@chmod 644 $(INSTALL_DIR)/synos
	@echo -e "$(SECTIONC)[install]$(INFOC) Done!$(NC)"