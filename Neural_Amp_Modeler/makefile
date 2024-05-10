

	# check if user is root
	user = $(shell whoami)
	ifeq ($(user),root)
	INSTALL_DIR = /usr/lib/lv2
	else 
	INSTALL_DIR = ~/.lv2
	endif


	# check LD version
	ifneq ($(shell xxd --version 2>&1 | head -n 1 | grep xxd),)
		USE_XXD = 1
	else ifneq ($(shell $(LD) --version 2>&1 | head -n 1 | grep LLD),)
		ifneq ($(shell uname -a | grep  x86_64), )
			LDEMULATION := elf_x86_64
		else ifneq ($(shell uname -a | grep amd64), )
			LDEMULATION := elf_x86_64
		else ifneq ($(shell uname -a | grep i386), )
			LDEMULATION := elf_i386
		endif
		USE_LDD = 1
	else ifneq ($(shell gold --version 2>&1 | head -n 1 | grep gold),)
		LD = gold
	endif


	NAME = Neural_Amp_Modeler
	space := $(subst ,, )
	EXEC_NAME := $(subst $(space),_,$(NAME))
	BUNDLE = $(EXEC_NAME)_ui.lv2
	IBUNDLE = neural_amp_modeler.lv2
	RESOURCES_DIR :=../resources/
	LIB_DIR := ../libxputty/libxputty/
	HEADER_DIR := $(LIB_DIR)include/
	UI_LIB:= $(LIB_DIR)libxputty.$(STATIC_LIB_EXT)
	STRIP ?= strip

	RESOURCES := $(wildcard $(RESOURCES_DIR)*.png)
	RESOURCES_OBJ := $(notdir $(patsubst %.png,%.o,$(RESOURCES)))
	RESOURCES_LIB := $(notdir $(patsubst %.png,%.$(STATIC_LIB_EXT),$(RESOURCES)))
	RESOURCE_EXTLD := $(notdir $(patsubst %.png,%_png,$(RESOURCES)))
	RESOURCEHEADER := xresources.h
	SVGRESOURCES := $(wildcard $(RESOURCES_DIR)*.svg)
	SVGRESOURCES_LIB := $(notdir $(patsubst %.svg,%.$(STATIC_LIB_EXT),$(SVGRESOURCES)))
	SVGRESOURCES_OBJ := $(notdir $(patsubst %.svg,%.o,$(SVGRESOURCES)))
	SVGRESOURCES_C := $(notdir $(patsubst %.svg,%.c,$(SVGRESOURCES)))
	SVGRESOURCE_EXTLD := $(notdir $(patsubst %.svg,%_svg,$(SVGRESOURCES)))

ifeq ($(TARGET), Linux)

	LDFLAGS += -fvisibility=hidden -I$(HEADER_DIR) -Wl,-Bstatic -L. $(UI_LIB) \
	-Wl,-Bdynamic `pkg-config --cflags --libs cairo x11 lv2` -pthread -lpthread \
	-shared -lm -fPIC -Wl,-z,noexecstack -Wl,--no-undefined -Wl,--gc-sections

	CFLAGS := -O2 -D_FORTIFY_SOURCE=2 -Wall -fstack-protector -fvisibility=hidden \
	-fdata-sections -Wl,--gc-sections -Wl,-z,relro,-z,now -Wl,--exclude-libs,ALL

else ifeq ($(TARGET), Windows)

	LDFLAGS += -I. -I$(HEADER_DIR) -static-libgcc -static-libstdc++ \
	`$(PKGCONFIG) $(PKGCONFIG_FLAGS) --cflags --libs cairo ` \
	-L. $(LIB_DIR)libxputty.$(STATIC_LIB_EXT) -shared -lm $(PAWPAW_LFLAGS)

	CFLAGS := -O2 -D_FORTIFY_SOURCE=2 -Wall -fstack-protector -fvisibility=hidden \
	-fdata-sections -Wl,--gc-sections -Wl,--exclude-libs,ALL

	TTLUPDATE = sed -i '/lv2:binary/ s/\.so/\.dll/ ' ../bin/$(BUNDLE)/manifest.ttl
	TTLUPDATEGUI = sed -i '/a guiext:X11UI/ s/X11UI/WindowsUI/ ; /lv2:binary/ s/\.so/\.dll/ ' ../bin/$(BUNDLE)/$(NAME)_ui.ttl

endif

.PHONY : all install uninstall

.NOTPARALLEL:

all: $(RESOURCEHEADER) $(EXEC_NAME)

	$(QUIET)mkdir -p ../bin/$(BUNDLE)
	$(QUIET)cp ./*.ttl ../bin/$(BUNDLE)
	$(QUIET)$(TTLUPDATE)
	$(QUIET)$(TTLUPDATEGUI)
	$(QUIET)cp ./*.$(LIB_EXT) ../bin/$(BUNDLE)
	@if [ -f ../bin/$(BUNDLE)/$(EXEC_NAME)_ui.$(LIB_EXT) ]; then \
		$(B_ECHO) "=================== DONE =======================$(reset)"; \
	else \
		$(R_ECHO) "sorry, build failed$(reset)"; \
	fi

$(RESOURCEHEADER): $(RESOURCES_OBJ)
	@rm -f $(RESOURCEHEADER)
	@for f in $(RESOURCE_EXTLD); do \
		echo 'EXTLD('$${f}')' >> $(RESOURCEHEADER) ; \
	done
	@for f in $(SVGRESOURCE_EXTLD); do \
		echo 'extern const char* '$${f}';' >> $(RESOURCEHEADER) ; \
	done

ifdef USE_XXD
$(RESOURCES_OBJ): $(RESOURCES)
	@#use this line to regenerate the *.c files from used images
	@#cd $(RESOURCES_DIR) && xxd -i $(patsubst %.o,%.png,$@) > $(patsubst %.o,%.c,$@)
	$(CC) -c $(RESOURCES_DIR)$(patsubst %.o,%.c,$@) -o $@
	$(AR) rcs $(patsubst %.o,%.$(STATIC_LIB_EXT),$@) $@
else ifdef USE_LDD
$(RESOURCES_OBJ): $(RESOURCES)
	cd $(RESOURCES_DIR) && $(LD) -r -b binary -m $(LDEMULATION) -z noexecstack $(patsubst %.o,%.png,$@) -o ../$@
	$(AR) rcs $(patsubst %.o,%.$(STATIC_LIB_EXT),$@) $@
	LDFLAGS += -DUSE_LD=1
else
$(RESOURCES_OBJ): $(RESOURCES)
	cd $(RESOURCES_DIR) && $(LD) -r -b binary -z noexecstack --strip-all $(patsubst %.o,%.png,$@) -o ../$@
	$(AR) rcs $(patsubst %.o,%.$(STATIC_LIB_EXT),$@) $@
	LDFLAGS += -DUSE_LD=1
endif

$(SVGRESOURCES_OBJ): $(SVGRESOURCES)
	@cd $(RESOURCES_DIR) && echo 'const char* $(patsubst %.o,%_svg,$@) = "'| tr -d '\r\n' > $(patsubst %.o,%.c,$@)
	@cd $(RESOURCES_DIR) && base64  $(patsubst %.o,%.svg,$@) | tr -d '\r\n' >> $(patsubst %.o,%.c,$@)
	@cd $(RESOURCES_DIR) && echo '";' >> $(patsubst %.o,%.c,$@)
	$(CC) -c $(RESOURCES_DIR)$(patsubst %.o,%.c,$@) -o $@
	$(AR) rcs $(patsubst %.o,%.$(STATIC_LIB_EXT),$@) $@

$(EXEC_NAME):$(RESOURCES_OBJ) $(SVGRESOURCES_OBJ)
	@$(B_ECHO) "Compiling $(EXEC_NAME)_ui.$(LIB_EXT) $(reset)"
	@# use this line when you include libxputty as submodule
	$(QUIET)$(CC) -DUSE_ATOM  $(CFLAGS) '$(NAME).c' -L. $(RESOURCES_LIB) -L. $(SVGRESOURCES_LIB) $(UI_LIB) -o '$(EXEC_NAME)_ui.$(LIB_EXT)' $(LDFLAGS) -I./ -I$(HEADER_DIR)
	@#$(CC) -DUSE_ATOM  $(CFLAGS) '$(NAME).c' -L. $(RESOURCES_LIB) -L. $(SVGRESOURCES_LIB) -o '$(EXEC_NAME)_ui.$(LIB_EXT)' $(LDFLAGS) -I./
	$(QUIET)$(STRIP) -s -x -X -R .comment -R .note.ABI-tag $(EXEC_NAME)_ui.$(LIB_EXT)

$(DESTDIR)$(INSTALL_DIR)/$(IBUNDLE):
	@$(error "Couldn't find neural_amp_modeler.lv2 install location")

install : | $(DESTDIR)$(INSTALL_DIR)/$(IBUNDLE)
ifneq ("$(wildcard ../bin/$(BUNDLE))","")
	@cp -r ../bin/$(BUNDLE)/* $(DESTDIR)$(INSTALL_DIR)/$(IBUNDLE)
	@$(B_ECHO) ". ., done$(reset)"
else
	@$(R_ECHO) ". ., you must build first$(reset)"
endif

uninstall :
	$(QUIET)rm $(DESTDIR)$(INSTALL_DIR)/$(IBUNDLE)/$(EXEC_NAME)_ui.$(LIB_EXT)
	$(QUIET)rm $(DESTDIR)$(INSTALL_DIR)/$(IBUNDLE)/manifest.ttl
	$(QUIET)rm $(DESTDIR)$(INSTALL_DIR)/$(IBUNDLE)/Neural_Amp_Modeler_ui.ttl
	$(QUIET)cp ./restore/manifest.ttl $(DESTDIR)$(INSTALL_DIR)/$(IBUNDLE)
	@$(B_ECHO) ". ., done$(reset)"

clean:
	$(QUIET)rm -f *.a  *.lib *.o *.so *.dll xresources.h
	$(QUIET)rm -rf ../bin

