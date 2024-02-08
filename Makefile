format:
	find . -name '*.[ch]' -exec clang-format --style=LLVM -i {} +;

init_mod:
	git submodule update --init --recursive;

doc:
	cd doc; doxygen Doxyfile; 

open_doc:
	./doc/doc_website/html/index.html