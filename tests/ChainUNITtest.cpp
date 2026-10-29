#include "doctest.h"
#include <iostream>
#include <sstream>
#include <string>
#include "../SimpleTask.h"
#include "../ConcreteHandlerA.h"
#include "../ConcreteHandlerB.h"
#include "../ConcreteHandlerC.h"

static std::string run(Handler& h, Task* t, int number) {
    std::ostringstream captured;
    std::streambuf* old = std::cout.rdbuf(captured.rdbuf());
    h.handle(t, number);
    std::cout.rdbuf(old);
    return captured.str();
}

static bool has(const std::string& s, const std::string& part) {
    return s.find(part) != std::string::npos;
}

TEST_CASE("Handler canHandle ranges") {
    ConcreteHandlerA a;
    ConcreteHandlerB b;
    ConcreteHandlerC c;

    CHECK(!a.canHandle(0));
    CHECK(a.canHandle(1));
    CHECK(a.canHandle(3));
    CHECK(!a.canHandle(4));
    CHECK(b.canHandle(4));
    CHECK(b.canHandle(6));
    CHECK(!b.canHandle(3));
    CHECK(!b.canHandle(7));
    CHECK(c.canHandle(7));
    CHECK(c.canHandle(9));
    CHECK(!c.canHandle(10));
}

TEST_CASE("Chain routes each category to the right handler") {
    ConcreteHandlerA a;
    ConcreteHandlerB b;
    ConcreteHandlerC c;
    a.setNext(&b);
    b.setNext(&c);

    for (int i = 1; i <= 9; ++i) {
        SimpleTask t(std::to_string(i) + ".Category", "d", i);
        std::string out = run(a, &t, t.getDepartmentLevel());
        char expected = (i <= 3) ? 'A' : (i <= 6) ? 'B' : 'C';
        std::string tag = std::string("Handler ") + expected;
        bool ok = has(out, tag) && has(out, t.getName());
        CHECK_MESSAGE(ok, "category " << i << " handled by " << tag);
    }

    SimpleTask t5("5.Category", "d", 5);
    std::string out5 = run(a, &t5, 5);
    CHECK(!has(out5, "Handler A"));
    CHECK(has(out5, "Handler B"));
    CHECK(!has(out5, "Handler C"));
}

TEST_CASE("Chain: unhandled categories") {
    ConcreteHandlerA a;
    ConcreteHandlerB b;
    ConcreteHandlerC c;
    a.setNext(&b);
    b.setNext(&c);

    SimpleTask t0("Category", "d", 1);
    SimpleTask t10("10.Category", "d", 1);
    SimpleTask t1("1.Category", "d", 1);

    CHECK(has(run(a, &t0, t0.getDepartmentLevel()), "No handler"));
    CHECK(has(run(a, &t10, t10.getDepartmentLevel()), "No handler"));
    CHECK(has(run(b, &t1, 1), "No handler"));
}

TEST_CASE("Lone handler passes nothing on") {
    ConcreteHandlerA lone;
    SimpleTask t8("8.Category", "d", 8);
    SimpleTask t1("1.Category", "d", 1);

    CHECK(has(run(lone, &t8, 8), "No handler"));
    CHECK(has(run(lone, &t1, 1), "Handler A"));
}