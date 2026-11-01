#include "Target.h"
#include "VMimeEmailAdapter.h"
#include "VMimeMailer.h"
#include "SimpleTask.h"
#include "TaskGroup.h"
#include <iostream>

int main() {
    // 1. Create the Adaptee (VMimeMailer) pointing to Gmail's STARTTLS port
    VMimeMailer mailer("smtp.gmail.com", 587);
    
    // Replace the second string with your 16-character Google App Password (no spaces)
    mailer.setCredentials("litekd18@gmail.com", "rebctkwhltiemkcy");

    // 2. Create the Adapter (VMimeEmailAdapter) wrapping the mailer
    Target* emailAdapter = new VMimeEmailAdapter(&mailer, "litekd18@gmail.com");

    // 3. Simulate what the GUI text box will do when a user types their email
    emailAdapter->setRecipientEmail("motsemmemotheo@gmail.com");

    // 4. Create a real SimpleTask, run its State transitions, and send the email
    SimpleTask task("1_AdapterLiveTest", "2026-11-05", 45);
    task.start();
    task.complete();

    std::cout << "Attempting to send live Task Complete email via Gmail SMTP..." << std::endl;
    emailAdapter->notifyTaskComplete(&task);

    // 5. Optional: Also test a State Change alert (e.g., TimedOut)
    SimpleTask urgentTask("4_ServerDeployment", "2026-11-06", 20);
    urgentTask.start();
    urgentTask.timeout();

    std::cout << "Attempting to send live State Change Alert email..." << std::endl;
    emailAdapter->notifyStateChange(&urgentTask, "TIMED OUT");

    delete emailAdapter;
    return 0;
}