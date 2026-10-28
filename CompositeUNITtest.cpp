#include <iostream>
#include <string>
#include "SimpleTask.h"
#include "TaskGroup.h"

static int passed = 0;
static int failed = 0;

#define CHECK(cond, msg) \
    do { \
        if (cond) { ++passed; std::cout << "[PASS] " << msg << std::endl; } \
        else      { ++failed; std::cout << "[FAIL] " << msg << std::endl; } \
    } while (0)

int main() {
    //SimpleTask
    SimpleTask leaf("3.Category", "2026-12-01", 30);
    CHECK(leaf.getName() == "3.Category", "leaf getName");
    CHECK(leaf.getDeadline() == "2026-12-01", "leaf getDeadline");
    CHECK(leaf.getDepartmentLevel() == 3, "leaf deptLevel read from name");
    CHECK(leaf.isLeaf(), "leaf isLeaf is true");
    CHECK(leaf.totalTime() == 30, "leaf totalTime equals duration");
    CHECK(leaf.getChildren().empty(), "leaf has no children");

    //deptLevel parsing
    SimpleTask nine("9.Category", "d", 1);
    SimpleTask twelve("12.Category", "d", 1);
    SimpleTask none("Category", "d", 1);
    CHECK(nine.getDepartmentLevel() == 9, "deptLevel 9");
    CHECK(twelve.getDepartmentLevel() == 12, "deptLevel 12 (multi digit)");
    CHECK(none.getDepartmentLevel() == 0, "deptLevel 0 when no number");

    //Leaf ignores add/remove
    SimpleTask other("1.Category", "d", 5);
    leaf.add(&other);
    CHECK(leaf.getChildren().empty(), "leaf add does nothing");
    leaf.remove(&other);
    CHECK(leaf.totalTime() == 30, "leaf remove does nothing");

    //Group
    TaskGroup group("0.Project", "2026-12-31");
    CHECK(!group.isLeaf(), "group isLeaf is false");
    CHECK(group.totalTime() == 0, "empty group totalTime is 0");
    CHECK(group.getChildren().empty(), "empty group has no children");
    group.add(new SimpleTask("1.Category", "d", 10));
    group.add(new SimpleTask("2.Category", "d", 20));
    Task* third = new SimpleTask("3.Category", "d", 30);
    group.add(third);
    group.add(nullptr);
    CHECK(group.getChildren().size() == 3, "group has 3 children (nullptr ignored)");
    CHECK(group.totalTime() == 60, "group totalTime is the sum of leaves");

    //Nested group
    TaskGroup* sub = new TaskGroup("4.Sub", "d");
    sub->add(new SimpleTask("4.Category", "d", 40));
    sub->add(new SimpleTask("5.Category", "d", 50));
    group.add(sub);
    CHECK(sub->totalTime() == 90, "sub group totalTime");
    CHECK(group.totalTime() == 150, "nested totalTime goes through sub group");
    CHECK(group.getChildren().size() == 4, "group now has 4 children");

    //Remove
    group.remove(third);
    CHECK(group.getChildren().size() == 3, "remove detaches the child");
    CHECK(group.totalTime() == 120, "totalTime updated after remove");
    delete third;

    //Execute
    std::cout << "--- execute output ---" << std::endl;
    group.execute();
    std::cout << "----------------------" << std::endl;
    CHECK(true, "group execute ran");
    std::cout << "\nPassed: " << passed << "  Failed: " << failed << std::endl;
    return failed == 0 ? 0 : 1;
}