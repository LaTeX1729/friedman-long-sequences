CC ?= gcc
CFLAGS ?= -O2
BIN = bin/lfs bin/lfs2 bin/rle bin/sweep bin/special bin/macro
all: $(BIN)
bin/%: src/%.c src/core.h
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $<
# Re-verify the stored sequences (the brute-force checks take a few minutes).
verify: all
	bin/lfs2 check 4 data/m4_1310741.txt
	bin/lfs  check 2 4 data/m4_1310741.txt
	bin/lfs2 check 4 data/m4_245779.txt
	python3 scripts/to_special.py data/m4_245779.txt /tmp/special_check.txt 0
small: all
	for n in 1 2 3; do bin/lfs2 search $$n 60; done
clean:
	rm -rf bin
.PHONY: all verify small clean
