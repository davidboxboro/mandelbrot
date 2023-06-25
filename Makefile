src_files = main.cpp displayer.cpp util.cpp
ex_file = main

sfml_dir = /usr/local/Cellar/sfml/2.5.1_2
sfml_lib_dir = $(sfml_dir)/lib
sfml_include_dir = $(sfml_dir)/include
sfml_flags = -I$(sfml_include_dir) -L$(sfml_lib_dir) -lsfml-graphics -lsfml-window -lsfml-system

compiler = /usr/local/Cellar/llvm/16.0.6/bin/clang++

opt_flags = -Ofast -march=native -mtune=native

all: clean build

clean:
	rm -f $(ex_file)

build:
	$(compiler) $(opt_flags) -fopenmp -std=c++20 $(src_files) -o $(ex_file) $(sfml_flags)

run: all
	./$(ex_file)

