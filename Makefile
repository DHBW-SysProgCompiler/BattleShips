format:
	find . -name '*.[ch]' -exec clang-format --style=file -i {} +;

run:
	qemu-system-arm -M microbit -device loader,file=build-cortex-m0/testApp.elf -nographic -s -serial mon:stdio;

init_mod:
	git submodule update --init --recursive;

doxy:
	cd doc; doxygen Doxyfile; 

open_doxy:
	./doc/doc_website/html/index.html;
