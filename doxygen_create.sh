#!/bin/bash

cd "$(dirname "$0")"
cd "./doc"

doxygen Doxyfile

if command -v google-chrome; then
  google-chrome "./doc_website/html/index.html"
else
  xdg-open "./doc_website/html/index.html"
fi