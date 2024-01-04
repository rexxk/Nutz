# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  NutzLib_config = debug
  glfw_config = debug
  NutEditor_config = debug

else ifeq ($(config),release)
  NutzLib_config = release
  glfw_config = release
  NutEditor_config = release

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := NutzLib glfw NutEditor

.PHONY: all clean help $(PROJECTS) Core Dependencies Tools

all: $(PROJECTS)

Core: NutzLib

Dependencies: glfw

Tools: NutEditor

NutzLib:
ifneq (,$(NutzLib_config))
	@echo "==== Building NutzLib ($(NutzLib_config)) ===="
	@${MAKE} --no-print-directory -C NutzLib -f Makefile config=$(NutzLib_config)
endif

glfw:
ifneq (,$(glfw_config))
	@echo "==== Building glfw ($(glfw_config)) ===="
	@${MAKE} --no-print-directory -C NutzLib/vendor/glfw -f Makefile config=$(glfw_config)
endif

NutEditor: NutzLib glfw
ifneq (,$(NutEditor_config))
	@echo "==== Building NutEditor ($(NutEditor_config)) ===="
	@${MAKE} --no-print-directory -C NutEditor -f Makefile config=$(NutEditor_config)
endif

clean:
	@${MAKE} --no-print-directory -C NutzLib -f Makefile clean
	@${MAKE} --no-print-directory -C NutzLib/vendor/glfw -f Makefile clean
	@${MAKE} --no-print-directory -C NutEditor -f Makefile clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   NutzLib"
	@echo "   glfw"
	@echo "   NutEditor"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"