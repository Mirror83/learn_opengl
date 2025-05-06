build_folder := build

$(build_folder)/main: main.cpp glad.c
	mkdir -p $(build_folder)
	g++ glad.c main.cpp -o $(build_folder)/main -Iinclude/ -lglfw -lGL -lpthread -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon
	
run: $(build_folder)/main
	./$(build_folder)/main

clean:
	rm -rf $(build_folder)