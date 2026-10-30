#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>

// #include "Facade.h" // Uncomment when linking your backend

int main() {
    // 1. Initialize GLFW and Window
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Workflow System", NULL, NULL);
    if (!window) return -1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // 2. Initialize Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // 3. Setup Backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // 4. Main Rendering Loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // --- GUI SKELETON ---
        ImGui::Begin("Workflow Dashboard");
        
        if (ImGui::BeginTabBar("Navigation")) {
            if (ImGui::BeginTabItem("Handler Definition")) {
                ImGui::Text("Phase 2: Chain of Responsibility form goes here.");
                // TODO: Email input and Handler category selection
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Task Builder")) {
                ImGui::Text("Phase 3: Task Builder & Tree UI goes here.");
                // TODO: Task creation and collapsing trees
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Output")) {
                ImGui::Text("Phase 4: Run button and Console logs go here.");
                // TODO: Command log display
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
        // --------------------

        // Render to screen
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.15f, 0.15f, 0.15f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // 5. Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}