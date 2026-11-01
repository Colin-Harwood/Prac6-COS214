
#include "doctest.h"
#include "../Director.h"
#include "../SimpleTask.h"
#include "../SimpleTaskBuilder.h"
#include "../TaskGroup.h"
#include "../TaskGroupBuilder.h"
#include "../TaskInput.h"

class TaskState {};

TEST_CASE("Director builds a nested task group") {
    TaskState state;

    TaskInput root;
    root.name = "0.Project";
    root.deadline = "2026-12-31";
    root.deptLevel = 8;
    root.state = &state;
    root.isGroup = true;

    TaskInput leaf;
    leaf.name = "2.Leaf";
    leaf.deadline = "2026-12-01";
    leaf.deptLevel = 6;
    leaf.state = &state;
    leaf.duration = 15;

    TaskInput nestedGroup;
    nestedGroup.name = "1.Group";
    nestedGroup.deadline = "2026-12-15";
    nestedGroup.isGroup = true;

    TaskInput nestedLeaf;
    nestedLeaf.name = "3.NestedLeaf";
    nestedLeaf.deadline = "2026-12-10";
    nestedLeaf.duration = 25;
    nestedGroup.children.push_back(nestedLeaf);

    root.children.push_back(leaf);
    root.children.push_back(nestedGroup);

    TaskGroupBuilder builder;
    Director director(&root, &builder);
    director.constructTask();

    TaskGroup* result = builder.getResult();
    const std::vector<Task*> children = result->getChildren();
    REQUIRE(children.size() == 2);
    CHECK(result->getName() == "0.Project");
    CHECK(result->getDepartmentLevel() == 8);
    CHECK(result->totalTime() == 40);
    CHECK(children[0]->isLeaf());
    CHECK(children[0]->getName() == "2.Leaf");
    CHECK(children[0]->getDepartmentLevel() == 6);
    CHECK(children[0]->totalTime() == 15);
    CHECK(!children[1]->isLeaf());
    REQUIRE(children[1]->getChildren().size() == 1);
    CHECK(children[1]->getChildren()[0]->totalTime() == 25);

    // The placeholder lets the Director pass a non-null state pointer, but Task exposes no state getter.
    delete result;
}

TEST_CASE("Director builds a standalone simple task") {
    TaskInput input;
    input.name = "4.Standalone";
    input.deadline = "2026-11-01";
    input.deptLevel = 9;
    input.duration = 7;

    SimpleTaskBuilder builder;
    Director director(&input, &builder);
    director.constructTask();

    SimpleTask* result = builder.getResult();
    CHECK(result->getName() == "4.Standalone");
    CHECK(result->getDeadline() == "2026-11-01");
    CHECK(result->getDepartmentLevel() == 9);
    CHECK(result->totalTime() == 7);
    delete result;
}
