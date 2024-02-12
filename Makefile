format:
	find . -name '*.[ch]' -exec clang-format --style=file -i {} +;

init_mod:
	git submodule update --init --recursive;

doxy:
	cd doc; doxygen Doxyfile; 

open_doxy:
	./doc/doc_website/html/index.html
