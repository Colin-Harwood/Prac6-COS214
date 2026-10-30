CXX = g++
CXXFLAGS = -std=c++11 -Wall -I./ImGui -I./ImGui/backends
LIBS = -lGL -lglfw

# ImGui core and backend files updated to match your folder name
IMGUI_SRC = ImGui/imgui.cpp ImGui/imgui_draw.cpp ImGui/imgui_tables.cpp ImGui/imgui_widgets.cpp \
            ImGui/backends/imgui_impl_glfw.cpp ImGui/backends/imgui_impl_opengl3.cpp

# Your workflow system files
PROJECT_SRC = main.cpp # Add Facade.cpp, Builder.cpp, etc. here

SOURCES = $(IMGUI_SRC) $(PROJECT_SRC)
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = workflow_gui

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)