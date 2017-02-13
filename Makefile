
SUBDIRS := $(wildcard build/*)
SUBDIRS_CLEAN := $(foreach  s,$(SUBDIRS),$(s)-c)
SUBDIRS_TEST := $(foreach  s,$(SUBDIRS),$(s)-t)

all: $(SUBDIRS)
clean: $(SUBDIRS_CLEAN)
test: $(SUBDIRS_TEST)

.PHONY: $(SUBDIRS) $(SUBDIRS_CLEAN) $(SUBDIRS_TEST)
$(SUBDIRS):
	$(MAKE) -C $@ -j

$(SUBDIRS_CLEAN):
	$(MAKE) -C $(patsubst %-c,%,$@) clean

$(SUBDIRS_TEST):
	$(MAKE) -C $(patsubst %-t,%,$@) test

print-%:
	@echo '$*=$($*)'
