CXX = g++
# Evaluates whether vmime is already installed via pkg-config
HAS_VMIME := $(shell pkg-config --exists vmime 2>/dev/null && echo 1 || echo 0)

CXXFLAGS = -std=c++11 -Wall -Wextra -Iinclude -Itests -g $(shell pkg-config --cflags vmime 2>/dev/null)
LDFLAGS  = $(shell pkg-config --libs vmime 2>/dev/null)

ALL_SRC     = $(wildcard *.cpp)
TEST_SRC    = $(wildcard tests/*.cpp)
SRC_NO_MAIN = $(filter-out main.cpp, $(ALL_SRC))
TEST_TARGET = unit_tests

all: check-deps $(TEST_TARGET)

# Automatically installs pkg-config and libvmime-dev ONLY if missing on the machine
check-deps:
ifeq ($(HAS_VMIME), 0)
	@echo "[Setup] Missing VMime or pkg-config. Installing once..."
	sudo apt-get update && sudo apt-get install -y pkg-config libvmime-dev
endif

$(TEST_TARGET): $(SRC_NO_MAIN) $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $(TEST_TARGET) $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(TARGET)

# =============================================================================
# GUI (Dear ImGui + GLFW + OpenGL)         Linux / WSL, same apt style as above
#
#   make install-gui-deps   install the packages (run once)
#   make gui                build the GUI program
#   make run-gui            build if needed, then start it
#   make clean-gui          delete the GUI program
# =============================================================================
GUI_TARGET   = tividy_gui

# Packages: ImGui, GLFW (the window), OpenGL headers, and pkg-config.
GUI_PKGS     = libimgui-dev libglfw3-dev libgl1-mesa-dev pkg-config

# The apt ImGui package ships the core as a library (libimgui.a) and the
# GLFW / OpenGL3 "backend" sources only as examples, so we compile those two.
IMGUI_BACKENDS_DIR = /usr/share/doc/libimgui-dev/examples/backends
GUI_BACKENDS = $(IMGUI_BACKENDS_DIR)/imgui_impl_glfw.cpp $(IMGUI_BACKENDS_DIR)/imgui_impl_opengl3.cpp

GUI_CXXFLAGS = -std=c++11 -Wall -Wextra -Iinclude -I/usr/include/imgui -I/usr/include/imgui/backends $(shell pkg-config --cflags imgui glfw3 2>/dev/null) $(shell pkg-config --cflags vmime 2>/dev/null)
GUI_LIBS     = $(shell pkg-config --libs imgui glfw3 2>/dev/null) -lGL -ldl $(LDFLAGS)

# The GUI calls the facade, so it is built together with the backend files
# ($(SRC_NO_MAIN)) and linked against VMime ($(LDFLAGS) above).

install-gui-deps:
	sudo apt-get update && sudo apt-get install -y $(GUI_PKGS)

gui: $(GUI_TARGET)

$(GUI_TARGET): main.cpp $(SRC_NO_MAIN)
	@test -f $(IMGUI_BACKENDS_DIR)/imgui_impl_glfw.cpp || { echo "Dear ImGui is not installed. Run: make install-gui-deps"; exit 1; }
	$(CXX) $(GUI_CXXFLAGS) main.cpp $(SRC_NO_MAIN) $(GUI_BACKENDS) -o $@ $(GUI_LIBS)

run-gui: $(GUI_TARGET)
	./$(GUI_TARGET)

clean-gui:
	rm -f $(GUI_TARGET)

.PHONY: install-gui-deps gui run-gui clean-gui