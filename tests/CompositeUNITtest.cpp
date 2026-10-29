#include "doctest.h"
#include <iostream>
#include <string>
#include "../SimpleTask.h"
#include "../TaskGroup.h"

TEST_CASE("SimpleTask basics") {
    SimpleTask leaf("3.Category", "2026-12-01", 30);
    CHECK(leaf.getName() == "3.Category");
    CHECK(leaf.getDeadline() == "2026-12-01");
    CHECK(leaf.getDepartmentLevel() == 3);
    CHECK(leaf.isLeaf());
    CHECK(leaf.totalTime() == 30);
    CHECK(leaf.getChildren().empty());

    SUBCASE("add and remove do nothing on a leaf") {
        SimpleTask other("1.Category", "d", 5);
        leaf.add(&other);
        CHECK(leaf.getChildren().empty());
        leaf.remove(&other);
        CHECK(leaf.totalTime() == 30);
    }
}

TEST_CASE("deptLevel parsing") {
    SimpleTask nine("9.Category", "d", 1);
    SimpleTask twelve("12.Category", "d", 1);
    SimpleTask none("Category", "d", 1);
    CHECK(nine.getDepartmentLevel() == 9);
    CHECK(twelve.getDepartmentLevel() == 12); 
    CHECK(none.getDepartmentLevel() == 0); 
}

TEST_CASE("TaskGroup composite behaviour") {
    TaskGroup group("0.Project", "2026-12-31");
    CHECK(!group.isLeaf());
    CHECK(group.totalTime() == 0);
    CHECK(group.getChildren().empty());

    group.add(new SimpleTask("1.Category", "d", 10));
    group.add(new SimpleTask("2.Category", "d", 20));
    Task* third = new SimpleTask("3.Category", "d", 30);
    group.add(third);
    group.add(nullptr);
    CHECK(group.getChildren().size() == 3);
    CHECK(group.totalTime() == 60);
    TaskGroup* sub = new TaskGroup("4.Sub", "d");
    sub->add(new SimpleTask("4.Category", "d", 40));
    sub->add(new SimpleTask("5.Category", "d", 50));
    group.add(sub);
    CHECK(sub->totalTime() == 90);
    CHECK(group.totalTime() == 150);
    CHECK(group.getChildren().size() == 4);

    group.remove(third);
    CHECK(group.getChildren().size() == 3);
    CHECK(group.totalTime() == 120);
    delete third;
    group.execute();
}