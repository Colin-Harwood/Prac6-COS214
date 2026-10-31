#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

static void copyText(char* dest, size_t size, const char* src) {
    std::strncpy(dest, src, size - 1);
    dest[size - 1] = '\0';
}

//Handler tab data
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

static std::string levelLabel(const HandlerData& h, int level) {
    if (level < 1) level = 1;
    if (level > 9) level = 9;
    char number[8];
    std::snprintf(number, sizeof(number), "%d", level);
    std::string name = h.levelNames[level - 1];
    return std::string(number) + " - " + (name.empty() ? "(unnamed)" : name);
}

// Task tab data
struct TaskEntry {
    int  uid;
    char name[64];
    char deadline[32];
    int  deptLevel;
    bool isGroup;
    int  duration;
    bool manualApproval;
    std::vector<TaskEntry> children;

    TaskEntry(bool group, int id)
        : uid(id), deptLevel(1), isGroup(group), duration(1), manualApproval(false) {
        name[0] = '\0';
        deadline[0] = '\0';
    }
};

struct TaskData {
    std::vector<TaskEntry> entries;
    int nextUid;
    TaskData() : nextUid(1) {}
};

//Output tab data
struct OutputData {
    std::vector<std::string> lines;
    bool scrollToBottom;
    OutputData() : scrollToBottom(false) {}
};

// ONE DRAW FUNCTION PER TAB

//Handler Definition tab
static void drawHandlerTab(HandlerData& data) {
    ImGui::Text("Departments");ImGui::Separator();

    const char* handlerLetters[3] = { "A", "B", "C" };
    for (int i = 0; i < 3; i++) {
        ImGui::PushID(i);

        char label[32];
        std::snprintf(label, sizeof(label), "Handler %s##name", handlerLetters[i]);
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
    if(ImGui::Button("submit email")) {
        printf("[Phase 1] Email service: would send to %s\n", data.recipientEmail);

    }
}

//Task Builder tab

static bool drawEntry(TaskEntry& entry, const HandlerData& handlers, int& nextUid) {
    bool removeMe = false;

    ImGui::PushID(entry.uid);

    std::string title = entry.isGroup ? "[Group] " : "[Task] ";
    title += (entry.name[0] == '\0') ? "(unnamed)" : entry.name;
    title += "###node";

    if (ImGui::TreeNodeEx(title.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {

        ImGui::InputText("Name",     entry.name,     sizeof(entry.name));
        ImGui::InputText("Deadline", entry.deadline, sizeof(entry.deadline));

        std::string preview = levelLabel(handlers, entry.deptLevel);
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

        if (!entry.isGroup) {
            ImGui::InputInt("Duration", &entry.duration);
            if (entry.duration < 0) entry.duration = 0;
            ImGui::Checkbox("Requires manual approval", &entry.manualApproval);
        } else {
            int removeIndex = -1;
            for (size_t i = 0; i < entry.children.size(); i++) {
                if (drawEntry(entry.children[i], handlers, nextUid)) {
                    removeIndex = static_cast<int>(i);
                }
            }
            if (removeIndex >= 0) {
                entry.children.erase(entry.children.begin() + removeIndex);
            }

            if (ImGui::Button("Add task to group")) {
                entry.children.push_back(TaskEntry(false, nextUid++));
            }
            ImGui::SameLine();
            if (ImGui::Button("Add group to group")) {
                entry.children.push_back(TaskEntry(true, nextUid++));
            }
        }

        if (ImGui::Button("Remove")) {
            removeMe = true;
        }

        ImGui::TreePop();
    }

    ImGui::PopID();
    return removeMe;
}

static void drawTaskTab(TaskData& data, const HandlerData& handlers) {
    ImGui::Text("Tasks run in the order they appear here.");
    ImGui::Separator();

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

//Run and Output tab
static void drawOutputTab(OutputData& data) {
    float footerHeight = ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y;

    ImGui::BeginChild("Console", ImVec2(0, -footerHeight), true);
    for (size_t i = 0; i < data.lines.size(); i++) {
        ImGui::TextUnformatted(data.lines[i].c_str());
    }
    if (data.scrollToBottom) {
        ImGui::SetScrollHereY(1.0f);
        data.scrollToBottom = false;
    }
    ImGui::EndChild();

    if (ImGui::Button("Run Workflow")) {
        data.lines.push_back("[Phase 1] Run clicked - not connected to the backend yet.");
        data.scrollToBottom = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Approve")) {
        data.lines.push_back("[Phase 1] Approve clicked - no manual task is waiting yet.");
        data.scrollToBottom = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Reject")) {
        data.lines.push_back("[Phase 1] Reject clicked - no manual task is waiting yet.");
        data.scrollToBottom = true;
    }
}

//THE FUNCTION THE MAIN LOOP CALLS EVERY FRAME
static void drawTividyGui() {
    static HandlerData handlerData;
    static TaskData    taskData;
    static OutputData  outputData;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("TIVIDY", NULL,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove     | ImGuiWindowFlags_NoCollapse);

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

//THE WINDOW AND THE LOOP
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

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        drawTividyGui();

        ImGui::Render();
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}