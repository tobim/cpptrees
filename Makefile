
SUBDIRS := $(wildcard build/*)
SUBDIRS_CLEAN := $(foreach  s,$(SUBDIRS),$(s)-c)

all: $(SUBDIRS)
clean: $(SUBDIRS_CLEAN)

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ -j

$(SUBDIRS_CLEAN):
	$(MAKE) -C $(patsubst %-c,%,$@) clean

print-%:
	@echo '$*=$($*)'
