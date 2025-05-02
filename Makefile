build_folder := build

main: main.cpp glad.c
	mkdir -p $(build_folder)
	g++ glad.c main.cpp -o $(build_folder)/main -Iinclude/ -lglfw -lGL -lpthread -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon

clean:
	rm -rf $(build_folder)