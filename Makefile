default_target: init run

format:
	find . -name '*.[ch]' -exec clang-format --style=file -i {} +;

compile:
	make -C build-cortex-m0;

run: compile
	qemu-system-arm -M microbit -device loader,file=build-cortex-m0/testApp.elf -nographic -s -serial mon:stdio;

init:
	git submodule update --init --recursive;
	cmake --preset arm-cortex-m0-unix;

doxy:
	cd doc; doxygen Doxyfile; 

open_doxy:
	./doc/doc_website/html/index.html;
