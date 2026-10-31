#include "doctest.h"
#include "../Target.h"
#include "../VMimeEmailAdapter.h"
#include "../VMimeMailer.h"
#include "../SimpleTask.h"
#include "../TaskGroup.h"
#include "../LogQueue.h"
#include "../TaskSnapshotManager.h"
#include "../ExecuteCommand.h"
#include "../LogCommand.h"
#include <string>

/**
 * Spy subclass of VMimeMailer (Adaptee) to intercept Adapter calls offline
 * during automated unit testing without requiring live internet/SMTP credentials.
 */
class SpyVMimeMailer : public VMimeMailer {
public:
    bool connectCalled = false;
    bool sendCalled = false;
    std::string capturedHost;
    int capturedPort = 0;
    std::string capturedFrom;
    std::string capturedTo;
    std::string capturedSubject;
    std::string capturedBody;

    SpyVMimeMailer() : VMimeMailer("smtp.gmail.com", 587) {}

    bool connectSMTP(std::string host, int port) override {
        connectCalled = true;
        capturedHost = host;
        capturedPort = port;
        return true; // Simulate successful SMTP handshake
    }

    bool sendMimeMessage(std::string from, std::string to, std::string subject, std::string body) override {
        sendCalled = true;
        capturedFrom = from;
        capturedTo = to;
        capturedSubject = subject;
        capturedBody = body;
        return true; // Simulate successful MIME transmission
    }
};

TEST_CASE("Adapter Pattern: Simulated GUI Email Input and Task Completion Notification") {
    SpyVMimeMailer spyMailer;
    Target* emailAdapter = new VMimeEmailAdapter(&spyMailer, "noreply@tividy.org");

    // 1. Simulate the user typing their email into the GUI text box
    emailAdapter->setRecipientEmail("student@tuks.co.za");
    CHECK(emailAdapter->getRecipientEmail() == "student@tuks.co.za");

    // 2. Create a sample leaf task and complete it
    SimpleTask task("2_DatabaseMigration", "2026-11-05", 45);
    task.start();
    task.complete();

    // 3. Trigger notification using the convenience overload (uses GUI-stored email)
    emailAdapter->notifyTaskComplete(&task);

    // 4. Verify the Adapter properly translated the Task into VMimeMailer calls
    CHECK(spyMailer.connectCalled == true);
    CHECK(spyMailer.capturedHost == "smtp.gmail.com");
    CHECK(spyMailer.capturedPort == 587);
    CHECK(spyMailer.sendCalled == true);
    CHECK(spyMailer.capturedFrom == "noreply@tividy.org");
    CHECK(spyMailer.capturedTo == "student@tuks.co.za");
    CHECK(spyMailer.capturedSubject.find("2_DatabaseMigration") != std::string::npos);
    CHECK(spyMailer.capturedSubject.find("Dept 2") != std::string::npos);
    CHECK(spyMailer.capturedBody.find("2026-11-05") != std::string::npos);
    CHECK(spyMailer.capturedBody.find("Finished Status  : Yes") != std::string::npos);

    delete emailAdapter;
}

TEST_CASE("Adapter Pattern: State Change and Project Completion Audit Log") {
    SpyVMimeMailer spyMailer;
    VMimeEmailAdapter adapter(&spyMailer, "noreply@tividy.org", "user@example.com");

    SimpleTask task("5_SecurityAudit", "2026-11-10", 30);
    task.start();
    task.timeout();

    // Test state change alert (e.g., TimedOut / Rejected / Undone)
    adapter.notifyStateChange(&task, "TIMED OUT");
    CHECK(spyMailer.sendCalled == true);
    CHECK(spyMailer.capturedSubject.find("Alert (TIMED OUT)") != std::string::npos);
    CHECK(spyMailer.capturedBody.find("Event Triggered: TIMED OUT") != std::string::npos);

    // Test project completion audit log email
    spyMailer.sendCalled = false;
    TaskGroup project("0_ReleaseProject", "2026-11-15");
    adapter.notifyProjectComplete(&project, "EXECUTED: 2_DatabaseMigration\nWORKFLOW CLOSED: 0_ReleaseProject");

    CHECK(spyMailer.sendCalled == true);
    CHECK(spyMailer.capturedSubject == "[TIVIDY] Project Completed: 0_ReleaseProject");
    CHECK(spyMailer.capturedBody.find("EXECUTED: 2_DatabaseMigration") != std::string::npos);
}