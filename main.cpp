// TIVIDY GUI - Phase 1 skeleton, all in one file. The side-by-side code report walks through it.
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

// PART 1: THE GUI'S OWN DATA

// Copies text into a char buffer and always ends it with '\0' (strncpy alone may not).
static void copyText(char* dest, size_t size, const char* src) {
    std::strncpy(dest, src, size - 1);
    dest[size - 1] = '\0';
}

// ---- Handler tab data
// Everything the Handler tab edits. ImGui text boxes need fixed-size char arrays, not std::string.
struct HandlerData {
    char handlerNames[3][64];
    char levelNames[9][64];

    char recipientEmail[128];

    HandlerData() {
        copyText(handlerNames[0], 64, "Workforce");
        copyText(handlerNames[1], 64, "Middle management");
        copyText(handlerNames[2], 64, "Top management");
        for (int i = 0; i < 9; i++) {
            std::snprintf(levelNames[i], 64, "Level %d", i + 1);
        }
        recipientEmail[0] = '\0';
    }
};

// Builds the drop-down text like "4 - Team lead". Only the number is stored.
static std::string levelLabel(const HandlerData& h, int level) {
    if (level < 1) level = 1;
    if (level > 9) level = 9;
    char number[8];
    std::snprintf(number, sizeof(number), "%d", level);
    std::string name = h.levelNames[level - 1];
    return std::string(number) + " - " + (name.empty() ? "(unnamed)" : name);
}

// ---- Task tab data
// One row of the task list. A group is an entry with isGroup = true whose children hold more entries (a nested list).
struct TaskEntry {
    int  uid;
    char name[64];
    char deadline[32];
    int  deptLevel;
    bool isGroup;
    int  duration;
    // Children of a group (a struct holding a vector of itself is officially C++17 but works in MinGW's g++).
    std::vector<TaskEntry> children;

    TaskEntry(bool group, int id)
        : uid(id), deptLevel(1), isGroup(group), duration(1) {
        name[0] = '\0';
        deadline[0] = '\0';
    }
};

// Everything the Task tab edits: the workflow's name and deadline, and the ordered list of entries.
struct TaskData {
    char workflowName[64];
    char workflowDeadline[32];
    std::vector<TaskEntry> entries;
    int nextUid;
    TaskData() : nextUid(1) {
        copyText(workflowName, sizeof(workflowName), "Workflow");
        workflowDeadline[0] = '\0';
    }
};

// ---- Output tab data
// The lines shown in the console. scrollToBottom asks it to jump to the newest line.
struct OutputData {
    std::vector<std::string> lines;
    bool scrollToBottom;
    OutputData() : scrollToBottom(false) {}
};

// PART 2: ONE DRAW FUNCTION PER TAB

// ---- Handler Definition tab
// Draws the Handler Definition tab. These names are GUI-only labels; the backend only sees level numbers.
static void drawHandlerTab(HandlerData& data) {
    ImGui::Text("Departments");ImGui::Separator();

    const char* handlerLetters[3] = { "A", "B", "C" };
    for (int i = 0; i < 3; i++) {
        // PushID gives each loop pass its own ID so widgets with the same label don't clash.
        ImGui::PushID(i);

        char label[32];
        std::snprintf(label, sizeof(label), "Handler %s##name", handlerLetters[i]);
        // A text box that edits the char buffer in place. Text after ## is hidden but part of the widget's ID.
        ImGui::InputText(label, data.handlerNames[i], sizeof(data.handlerNames[i]));

        ImGui::SameLine();
        ImGui::TextDisabled("handles levels %d-%d", 1 + 3 * i, 3 + 3 * i);

        ImGui::PopID();
    }

    ImGui::Spacing();
    ImGui::Text("Department level names");
    ImGui::TextDisabled("These labels appear in the Task Builder's level drop-down.");
    ImGui::Separator();

    for (int i = 0; i < 9; i++) {
        ImGui::PushID(100 + i);
        char label[32];
        std::snprintf(label, sizeof(label), "Level %d##level", i + 1);
        ImGui::InputText(label, data.levelNames[i], sizeof(data.levelNames[i]));
        ImGui::PopID();
    }

    ImGui::Spacing();
    ImGui::Text("Email service");
    ImGui::Separator();

    ImGui::InputText("Recipient email", data.recipientEmail, sizeof(data.recipientEmail));
    // Button() is true only on the frame it is clicked. Placeholder for now; later this calls the facade.
    if(ImGui::Button("submit email")) {
        printf("[Phase 1] Email service: would send to %s\n", data.recipientEmail);

    }
}

// ---- Task Builder tab

// Draws one entry (and a group's children). Returns true if Remove was pressed, so the CALLER erases it (erasing during a loop is unsafe).
static bool drawEntry(TaskEntry& entry, const HandlerData& handlers, int& nextUid) {
    bool removeMe = false;

    // Each entry gets its own ID scope.
    ImGui::PushID(entry.uid);

    std::string title = entry.isGroup ? "[Group] " : "[Task] ";
    title += (entry.name[0] == '\0') ? "(unnamed)" : entry.name;
    // Only the text after ### is hashed, so the node keeps its ID (and stays open) while the name is typed.
    title += "###node";

    // TreeNodeEx returns true while open; TreePop() must then be called.
    if (ImGui::TreeNodeEx(title.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {

        ImGui::InputText("Name",     entry.name,     sizeof(entry.name));
        ImGui::InputText("Deadline", entry.deadline, sizeof(entry.deadline));

        std::string preview = levelLabel(handlers, entry.deptLevel);
        // Drop-down: shows the level labels but stores only the number 1-9.
        if (ImGui::BeginCombo("Department level", preview.c_str())) {
            for (int level = 1; level <= 9; level++) {
                bool selected = (entry.deptLevel == level);
                std::string label = levelLabel(handlers, level);
                if (ImGui::Selectable(label.c_str(), selected)) {
                    entry.deptLevel = level;
                }
                if (selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        // Tasks have a duration; groups hold children instead.
        if (!entry.isGroup) {
            ImGui::InputInt("Duration", &entry.duration);
            if (entry.duration < 0) entry.duration = 0;
        } else {
            int removeIndex = -1;
            // Recursion: draw each child with this same function, so groups can nest to any depth.
            for (size_t i = 0; i < entry.children.size(); i++) {
                if (drawEntry(entry.children[i], handlers, nextUid)) {
                    removeIndex = static_cast<int>(i);
                }
            }
            // Erase after the loop, never during it.
            if (removeIndex >= 0) {
                entry.children.erase(entry.children.begin() + removeIndex);
            }

            // push_back adds at the end, so the list order is the workflow order.
            if (ImGui::Button("Add task to group")) {
                entry.children.push_back(TaskEntry(false, nextUid++));
            }
            ImGui::SameLine();
            if (ImGui::Button("Add group to group")) {
                entry.children.push_back(TaskEntry(true, nextUid++));
            }
        }

        // Removing a group removes its children too (they live inside it).
        if (ImGui::Button("Remove")) {
            removeMe = true;
        }

        ImGui::TreePop();
    }

    ImGui::PopID();
    return removeMe;
}

// Draws the Task Builder tab: the workflow inputs, then the top-level list.
static void drawTaskTab(TaskData& data, const HandlerData& handlers) {
    ImGui::Text("Workflow");
    ImGui::InputText("Workflow name", data.workflowName, sizeof(data.workflowName));
    ImGui::InputText("Workflow deadline", data.workflowDeadline, sizeof(data.workflowDeadline));
    ImGui::Separator();

    ImGui::Text("Tasks run in the order they appear here.");

    if (ImGui::Button("Add task")) {
        data.entries.push_back(TaskEntry(false, data.nextUid++));
    }
    ImGui::SameLine();
    if (ImGui::Button("Add group")) {
        data.entries.push_back(TaskEntry(true, data.nextUid++));
    }
    ImGui::Separator();

    int removeIndex = -1;
    for (size_t i = 0; i < data.entries.size(); i++) {
        if (drawEntry(data.entries[i], handlers, data.nextUid)) {
            removeIndex = static_cast<int>(i);
        }
    }
    if (removeIndex >= 0) {
        data.entries.erase(data.entries.begin() + removeIndex);
    }
}

// ---- Run and Output tab
// Draws the Run and Output tab: a scrolling console and the Run button.
static void drawOutputTab(OutputData& data) {
    // Reserve one row at the bottom for the button; the console fills the rest.
    float footerHeight = ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y;

    // A child window is a box with its own scrolling. EndChild() is always called.
    ImGui::BeginChild("Console", ImVec2(0, -footerHeight), true);
    for (size_t i = 0; i < data.lines.size(); i++) {
        ImGui::TextUnformatted(data.lines[i].c_str());
    }
    if (data.scrollToBottom) {
        ImGui::SetScrollHereY(1.0f);
        data.scrollToBottom = false;
    }
    ImGui::EndChild();

    // Phase 1 placeholder: only writes a line to the console.
    if (ImGui::Button("Run Workflow")) {
        data.lines.push_back("[Phase 1] Run clicked - not connected to the backend yet.");
        data.scrollToBottom = true;
    }
}

// PART 3: THE FUNCTION THE MAIN LOOP CALLS EVERY FRAME
// Called every frame. Describes the whole window; ImGui draws it.
static void drawTividyGui() {
    // static = created once and kept between frames; otherwise the input would reset every frame.
    static HandlerData handlerData;
    static TaskData    taskData;
    static OutputData  outputData;

    // One window that fills the application window, with no title bar.
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("TIVIDY", NULL,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove     | ImGuiWindowFlags_NoCollapse);

    // For Begin... calls: draw the contents and call End... only if Begin... returned true. (Begin and BeginChild always get their End.)
    if (ImGui::BeginTabBar("MainTabs")) {

        if (ImGui::BeginTabItem("Handler Definition")) {
            drawHandlerTab(handlerData);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Task Builder")) {
            drawTaskTab(taskData, handlerData);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Run and Output")) {
            drawOutputTab(outputData);
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

// PART 4: MAIN - THE WINDOW AND THE LOOP
// Opens the GLFW window, starts ImGui, and runs the frame loop.
int main() {
    if (!glfwInit()) {
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(1280, 720, "TIVIDY", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // One pass of this loop is one frame.
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start a new ImGui frame, describe the UI, then render it.
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        drawTividyGui();

        // Turn the described UI into drawing data and put it on screen.
        ImGui::Render();
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Clean up in the reverse order of setup.
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}