FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

# Install build tools, VMime, GLFW, OpenGL, and ImGui packages
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    make \
    curl \
    pkg-config \
    libvmime-dev \
    libimgui-dev \
    libglfw3-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libglew-dev \
    libx11-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /usr/share/doc/libimgui-dev/examples/backends \
             /usr/include/imgui/backends && \
    curl -sSL https://raw.githubusercontent.com/ocornut/imgui/v1.85/backends/imgui_impl_glfw.cpp \
         -o /usr/share/doc/libimgui-dev/examples/backends/imgui_impl_glfw.cpp && \
    curl -sSL https://raw.githubusercontent.com/ocornut/imgui/v1.85/backends/imgui_impl_opengl3.cpp \
         -o /usr/share/doc/libimgui-dev/examples/backends/imgui_impl_opengl3.cpp && \
    curl -sSL https://raw.githubusercontent.com/ocornut/imgui/v1.85/backends/imgui_impl_glfw.h \
         -o /usr/include/imgui/backends/imgui_impl_glfw.h && \
    curl -sSL https://raw.githubusercontent.com/ocornut/imgui/v1.85/backends/imgui_impl_opengl3.h \
         -o /usr/include/imgui/backends/imgui_impl_opengl3.h

WORKDIR /app

COPY . .

RUN HAS_VMIME=1 make unit_tests && make gui

FROM ubuntu:22.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    libvmime1 \
    libglfw3 \
    libgl1-mesa-glx \
    libglu1-mesa \
    libglew2.2 \
    libx11-6 \
    libxrandr2 \
    libxinerama1 \
    libxcursor1 \
    libxi6 \
    libstb0 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/tividy_gui  ./tividy_gui
COPY --from=builder /app/unit_tests  ./unit_tests

EXPOSE 587

ENV DISPLAY=:0
ENV WAYLAND_DISPLAY=wayland-0

CMD ["./tividy_gui"]
