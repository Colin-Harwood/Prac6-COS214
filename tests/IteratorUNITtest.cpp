#include "doctest.h"

#include <memory>
#include <string>
#include <vector>

#include "../DepartmentTaskIterator.h"
#include "../SimpleTask.h"
#include "../TaskGroup.h"
#include "../TaskIterator.h"

namespace {

std::vector<std::string> collectNames(TaskIterator& iterator) {
    std::vector<std::string> names;
    for (iterator.first(); !iterator.isDone(); iterator.next()) {
        Task* task = iterator.currentItem();
        REQUIRE(task != nullptr);
        names.push_back(task->getName());
    }
    CHECK(iterator.currentItem() == nullptr);
    return names;
}

}

TEST_CASE("Iterator: null and empty roots are immediately exhausted") {
    SUBCASE("null root") {
        DepartmentTaskIterator iterator(nullptr, 1);
        CHECK(iterator.isDone());
        CHECK(iterator.currentItem() == nullptr);
        CHECK_NOTHROW(iterator.next());
        CHECK(iterator.isDone());
        CHECK(iterator.allCompleted());
    }

    SUBCASE("empty task group") {
        TaskGroup empty("0.Empty", "d");
        DepartmentTaskIterator iterator(&empty, 1);
        CHECK(iterator.isDone());
        CHECK(iterator.currentItem() == nullptr);
        CHECK(iterator.allCompleted());
    }
}

TEST_CASE("Iterator: leaf factory returns an iterator with safe end and reset behaviour") {
    SimpleTask matching("2.Match", "d", 10);
    std::unique_ptr<TaskIterator> iterator(matching.createIterator(2));

    REQUIRE(iterator != nullptr);
    CHECK_FALSE(iterator->isDone());
    CHECK(iterator->currentItem() == &matching);

    iterator->next();
    CHECK(iterator->isDone());
    CHECK(iterator->currentItem() == nullptr);

    CHECK_NOTHROW(iterator->next());
    CHECK(iterator->isDone());

    iterator->first();
    CHECK_FALSE(iterator->isDone());
    CHECK(iterator->currentItem() == &matching);
}

TEST_CASE("Iterator: leaf factory can start exhausted for a different department") {
    SimpleTask task("1.One", "d", 10);
    std::unique_ptr<TaskIterator> iterator(task.createIterator(2));

    REQUIRE(iterator != nullptr);
    CHECK(iterator->isDone());
    CHECK(iterator->currentItem() == nullptr);
}

TEST_CASE("Iterator: nested groups are traversed depth-first while only matching leaves are returned") {
    TaskGroup root("0.Project", "d");
    root.add(new SimpleTask("2.First", "d", 10));

    TaskGroup* sameDepartmentGroup = new TaskGroup("2.Group", "d");
    sameDepartmentGroup->add(new SimpleTask("1.OtherDepartment", "d", 10));
    sameDepartmentGroup->add(new SimpleTask("2.Second", "d", 10));

    TaskGroup* nestedGroup = new TaskGroup("0.Nested", "d");
    nestedGroup->add(new SimpleTask("2.Third", "d", 10));
    sameDepartmentGroup->add(nestedGroup);
    root.add(sameDepartmentGroup);

    root.add(new SimpleTask("3.OtherDepartment", "d", 10));
    root.add(new SimpleTask("2.Fourth", "d", 10));

    std::unique_ptr<TaskIterator> iterator(root.createIterator(2));
    REQUIRE(iterator != nullptr);

    const std::vector<std::string> expected = {
        "2.First", "2.Second", "2.Third", "2.Fourth"
    };
    CHECK(collectNames(*iterator) == expected);

    CHECK(collectNames(*iterator) == expected);
}

TEST_CASE("Iterator: finished leaves are skipped and unfinished states remain eligible") {
    TaskGroup root("0.Project", "d");

    SimpleTask* completed = new SimpleTask("2.Completed", "d", 10);
    completed->start();
    completed->complete();
    root.add(completed);

    SimpleTask* cancelled = new SimpleTask("2.Cancelled", "d", 10);
    cancelled->cancel();
    root.add(cancelled);

    root.add(new SimpleTask("2.Pending", "d", 10));

    SimpleTask* inProgress = new SimpleTask("2.InProgress", "d", 10);
    inProgress->start();
    root.add(inProgress);

    SimpleTask* rejected = new SimpleTask("2.Rejected", "d", 10);
    rejected->start();
    rejected->reject();
    root.add(rejected);

    SimpleTask* timedOut = new SimpleTask("2.TimedOut", "d", 10);
    timedOut->start();
    timedOut->timeout();
    root.add(timedOut);

    SimpleTask* otherDepartment = new SimpleTask("1.Completed", "d", 10);
    otherDepartment->start();
    otherDepartment->complete();
    root.add(otherDepartment);

    DepartmentTaskIterator iterator(&root, 2);
    const std::vector<std::string> expected = {
        "2.Pending", "2.InProgress", "2.Rejected", "2.TimedOut"
    };
    CHECK(collectNames(iterator) == expected);
}

TEST_CASE("Iterator: no eligible unfinished leaves produces an empty traversal") {
    TaskGroup root("0.Project", "d");
    SimpleTask* completed = new SimpleTask("2.Completed", "d", 10);
    completed->start();
    completed->complete();
    root.add(completed);
    root.add(new SimpleTask("1.Pending", "d", 10));

    DepartmentTaskIterator iterator(&root, 2);
    CHECK(iterator.isDone());
    CHECK(iterator.currentItem() == nullptr);
    CHECK(collectNames(iterator).empty());
}

TEST_CASE("Iterator: allCompleted checks leaves across the whole tree, not only the target department") {
    TaskGroup root("0.Project", "d");
    SimpleTask* completed = new SimpleTask("1.Completed", "d", 10);
    completed->start();
    completed->complete();
    root.add(completed);

    TaskGroup* nested = new TaskGroup("2.Nested", "d");
    SimpleTask* pending = new SimpleTask("3.Pending", "d", 10);
    nested->add(pending);
    root.add(nested);

    DepartmentTaskIterator iterator(&root, 1);
    CHECK_FALSE(iterator.allCompleted());

    pending->cancel();
    CHECK(iterator.allCompleted());
}