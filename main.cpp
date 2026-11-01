// TIVIDY GUI - all in one file. Phase 1 skeleton + backend connection (the GUI passes its data down to TividyFacade).
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstdlib>   // atoi
#include <cstring>
#include <fstream>   // reading the .env file
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "TividyFacade.h"   // the ONE backend class the GUI talks to (Facade pattern)
#include "VMimeEmailAdapter.h"   // the notifier the facade sends emails through
#include "VMimeMailer.h"         // does the actual SMTP sending

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

    char recipientEmail[128];   // what the user is typing
    std::string savedEmail;     // what they submitted; passed to the facade when Run is clicked
    std::string emailStatus;    // feedback line shown under the button

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

// PART 1b: TALKING TO THE BACKEND (everything goes through TividyFacade)

// The backend finds a task's department level from the START of its name, in the form <level>.<name>
// (for example "4.Review"). The GUI keeps the level and the plain name apart, so we join them here.
static std::string backendName(const TaskEntry& entry) {
    std::ostringstream out;
    out << entry.deptLevel << '.' << entry.name;
    return out.str();
}

// The facade prints with std::cout, and the mailer reports errors on std::cerr. This object points a stream
// (cout or cerr) at our own buffer for as long as it lives, then puts the old buffer back in its destructor
// (even if an exception is thrown).
class StreamCapture {
public:
    StreamCapture(std::ostream& stream, std::ostringstream& target)
        : stream(stream), old(stream.rdbuf(target.rdbuf())) {}
    ~StreamCapture() { stream.rdbuf(old); }
private:
    std::ostream& stream;
    std::streambuf* old;
    StreamCapture(const StreamCapture&);              // not copyable (C++11 style without "= delete" noise)
    StreamCapture& operator=(const StreamCapture&);
};

// Checks the user's entries before we send anything. Returns "" when fine, otherwise a message for the console.
static std::string validateEntries(const std::vector<TaskEntry>& entries) {
    for (size_t i = 0; i < entries.size(); i++) {
        if (entries[i].name[0] == '\0') {
            return "A task or group has no name. Give every one a name first.";
        }
        if (entries[i].isGroup) {
            if (entries[i].children.empty()) {
                return std::string("Group \"") + entries[i].name + "\" is empty. Add a task to it or remove it.";
            }
            std::string inner = validateEntries(entries[i].children);
            if (!inner.empty()) {
                return inner;
            }
        }
    }
    return "";
}

// Sends the entries to the facade in order. beginGroup/endGroup bracket a group's children,
// so the recursion mirrors the tree the user built. Returns false as soon as the facade refuses something.
static bool submitEntries(TividyFacade& facade, const std::vector<TaskEntry>& entries) {
    for (size_t i = 0; i < entries.size(); i++) {
        const TaskEntry& e = entries[i];
        if (e.isGroup) {
            if (!facade.beginGroup(backendName(e), e.deadline)) return false;
            if (!submitEntries(facade, e.children))             return false;
            if (!facade.endGroup())                             return false;
        } else {
            if (!facade.addTask(backendName(e), e.deadline, e.deptLevel, e.duration)) return false;
        }
    }
    return true;
}

// Adds text to the console, one console line per text line.
static void addConsoleText(OutputData& out, const std::string& text) {
    std::istringstream in(text);
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line[line.size() - 1] == '\r') line.erase(line.size() - 1);
        out.lines.push_back(line);
    }
    out.scrollToBottom = true;
}

// ---- Email settings from the .env file
// The SMTP settings (and the password) live in a file called .env next to the program, NOT in the source code,
// so they never end up in the repository. Copy .env.example to .env and fill it in.
struct EmailSettings {
    std::string host;
    int port;
    std::string sender;
    std::string username;   // optional: leave empty if the server needs no login
    std::string password;
    EmailSettings() : port(0) {}
};

// Removes spaces, tabs and line endings (\r from Windows files) from both ends of a string.
static std::string trim(const std::string& text) {
    const char* whitespace = " \t\r\n";
    size_t first = text.find_first_not_of(whitespace);
    if (first == std::string::npos) return "";
    size_t last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}

// Reads lines like KEY=value (blank lines and lines starting with # are ignored).
// Returns false, with a message in "problem", if the file is missing or a required setting is empty.
static bool loadEmailSettings(const std::string& path, EmailSettings& settings, std::string& problem) {
    std::ifstream file(path.c_str());
    if (!file) {
        problem = "could not open " + path + " (copy .env.example to .env and fill it in)";
        return false;
    }
    std::map<std::string, std::string> values;
    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;
        size_t equals = line.find('=');
        if (equals == std::string::npos) continue;
        std::string key = trim(line.substr(0, equals));
        std::string value = trim(line.substr(equals + 1));
        // Allow "value" or 'value' with quotes around it.
        if (value.size() >= 2 && (value[0] == '"' || value[0] == '\'') && value[value.size() - 1] == value[0]) {
            value = value.substr(1, value.size() - 2);
        }
        values[key] = value;
    }
    settings.host     = values["SMTP_HOST"];
    settings.port     = std::atoi(values["SMTP_PORT"].c_str());
    settings.sender   = values["SMTP_SENDER"];
    settings.username = values["SMTP_USERNAME"];
    settings.password = values["SMTP_PASSWORD"];
    if (settings.host.empty() || settings.port <= 0 || settings.sender.empty()) {
        problem = "SMTP_HOST, SMTP_PORT and SMTP_SENDER must be set in " + path;
        return false;
    }
    return true;
}

// Gives the facade a notifier so it can send emails while the workflow runs.
// The facade OWNS the notifier (it deletes it), but the adapter does NOT own the mailer, so the caller keeps
// the mailer alive for as long as the facade exists.
static void setUpEmail(TividyFacade& facade, std::unique_ptr<VMimeMailer>& mailer, const std::string& recipient) {
    if (recipient.empty()) {
        std::cout << "No recipient email submitted: no emails will be sent." << std::endl;
        return;
    }
    EmailSettings settings;
    std::string problem;
    if (!loadEmailSettings(".env", settings, problem)) {
        std::cout << "Emails are off: " << problem << "." << std::endl;
        return;
    }
    mailer.reset(new VMimeMailer(settings.host, settings.port));
    if (!settings.username.empty()) {
        mailer->setCredentials(settings.username, settings.password);
    }
    facade.setNotifier(new VMimeEmailAdapter(mailer.get(), settings.sender, ""));
    if (facade.setNotificationEmail(recipient)) {   // the GUI's recipient input goes down here
        std::cout << "Emails will be sent to " << recipient << "." << std::endl;
    } else {
        std::cout << "The facade did not accept the recipient email." << std::endl;
    }
}

// Builds the workflow from the Task tab and runs it. Called when "Run Workflow" is clicked.
// Note: this blocks the window until the workflow finishes (fine for small workflows).
static void runWorkflowFromGui(const TaskData& tasks, const std::string& recipientEmail, OutputData& out) {
    out.lines.push_back("=== Run ===");

    if (tasks.entries.empty()) {
        addConsoleText(out, "Nothing to run. Add at least one task on the Task Builder tab.");
        return;
    }
    std::string problem = validateEntries(tasks.entries);
    if (!problem.empty()) {
        addConsoleText(out, problem);
        return;
    }

    // The facade appends to workflow.log. Start from an empty file so the log shows only this run.
    std::remove("workflow.log");

    std::ostringstream captured;
    {
        StreamCapture captureOut(std::cout, captured);   // everything the facade prints now lands in "captured"
        StreamCapture captureErr(std::cerr, captured);   // ...and so do the mailer's error messages
        try {
            // Declared BEFORE the facade on purpose: variables are destroyed in reverse order, so the facade
            // (which uses the mailer through its notifier) is destroyed first and the mailer last.
            std::unique_ptr<VMimeMailer> mailer;
            // A fresh facade per run: no leftover tree, undo list or snapshots from an earlier run.
            std::unique_ptr<TividyFacade> facade(new TividyFacade());
            setUpEmail(*facade, mailer, recipientEmail);

            bool built = facade->beginWorkflow(tasks.workflowName, tasks.workflowDeadline)
                      && submitEntries(*facade, tasks.entries)
                      && facade->finishWorkflow();   // the Director/Builder create the real tasks here

            if (built) {
                facade->runWorkflow();
                std::cout << "--- Final state ---" << std::endl;
                facade->printWorkflow();
                std::cout << "--- Log ---" << std::endl;
                facade->showLog();   // flushes the log queue to workflow.log, then prints the file
            } else {
                std::cout << "The workflow could not be built." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }   // capture goes out of scope here: cout is back to normal

    addConsoleText(out, captured.str());
}

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
    // Button() is true only on the frame it is clicked.
    // This only remembers the address. The emails themselves are sent while the workflow runs.
    if (ImGui::Button("submit email")) {
        std::string email = data.recipientEmail;
        if (email.find('@') == std::string::npos) {
            data.emailStatus = "That does not look like an email address (missing @).";
        } else {
            data.savedEmail = email;
            data.emailStatus = "Saved: " + email + " (emails are sent when the workflow runs)";
        }
    }
    if (!data.emailStatus.empty()) {
        ImGui::TextDisabled("%s", data.emailStatus.c_str());
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

        // CharsNoBlank rejects spaces (typed or pasted): the backend cannot handle spaces in task names yet.
        ImGui::InputText("Name",     entry.name,     sizeof(entry.name), ImGuiInputTextFlags_CharsNoBlank);
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

        // Shows exactly what the backend will receive as the name: <level>.<name>
        if (entry.name[0] != '\0') {
            ImGui::TextDisabled("Backend name: %s", backendName(entry).c_str());
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
static void drawOutputTab(OutputData& data, const TaskData& tasks, const HandlerData& handlers) {
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

    if (ImGui::Button("Run Workflow")) {
        runWorkflowFromGui(tasks, handlers.savedEmail, data);
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear console")) {
        data.lines.clear();
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
            drawOutputTab(outputData, taskData, handlerData);
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