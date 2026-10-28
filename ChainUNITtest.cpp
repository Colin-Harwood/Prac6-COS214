#include <iostream>
#include <sstream>
#include <string>
#include "SimpleTask.h"
#include "ConcreteHandlerA.h"
#include "ConcreteHandlerB.h"
#include "ConcreteHandlerC.h"

static int passed = 0;
static int failed = 0;

#define CHECK(cond, msg) \
    do { \
        if (cond){++passed; std::cout << "[PASS] " << msg << std::endl; } \
        else{++failed; std::cout << "[FAIL] " << msg << std::endl; } \
    } while (0)
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

int main() {
    ConcreteHandlerA a;
    ConcreteHandlerB b;
    ConcreteHandlerC c;
    a.setNext(&b);
    b.setNext(&c);

    CHECK(!a.canHandle(0), "A cannot handle 0");
    CHECK(a.canHandle(1) && a.canHandle(3), "A handles 1 and 3");
    CHECK(!a.canHandle(4), "A cannot handle 4");
    CHECK(b.canHandle(4) && b.canHandle(6), "B handles 4 and 6");
    CHECK(!b.canHandle(3) && !b.canHandle(7), "B cannot handle 3 or 7");
    CHECK(c.canHandle(7) && c.canHandle(9), "C handles 7 and 9");
    CHECK(!c.canHandle(10), "C cannot handle 10");
    for (int i = 1; i <= 9; ++i) {
        SimpleTask t(std::to_string(i) + ".Category", "d", i);
        std::string out = run(a, &t, t.getDepartmentLevel());
        char expected = (i <= 3) ? 'A' : (i <= 6) ? 'B' : 'C';
        std::string tag = std::string("Handler ") + expected;
        CHECK(has(out, tag) && has(out, t.getName()),"category " + std::to_string(i) + " handled by " + tag);
    }

    SimpleTask t5("5.Category", "d", 5);
    std::string out5 = run(a, &t5, 5);
    CHECK(!has(out5, "Handler A") && has(out5, "Handler B") && !has(out5, "Handler C"),"category 5 handled only by B");

    SimpleTask t0("Category", "d", 1);   
    SimpleTask t10("10.Category", "d", 1);
    CHECK(has(run(a, &t0, t0.getDepartmentLevel()), "No handler"), "category 0 has no handler");
    CHECK(has(run(a, &t10, t10.getDepartmentLevel()), "No handler"), "category 10 has no handler");

    SimpleTask t1("1.Category", "d", 1);
    CHECK(has(run(b, &t1, 1), "No handler"), "starting at B, category 1 is not handled");

    ConcreteHandlerA lone;
    SimpleTask t8("8.Category", "d", 8);
    CHECK(has(run(lone, &t8, 8), "No handler"), "lone handler passes nothing on");
    CHECK(has(run(lone, &t1, 1), "Handler A"), "lone handler still handles its own range");

    std::cout << "\nPassed: " << passed << "  Failed: " << failed << std::endl;
    return failed == 0 ? 0 : 1;
}