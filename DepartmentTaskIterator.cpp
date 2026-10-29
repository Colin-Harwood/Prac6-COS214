#include "DepartmentTaskIterator.h"
#include "Task.h"

DepartmentTaskIterator::DepartmentTaskIterator(Task *root, int deptLevel)
    : root(root),
      flatTasks(),
      currentIndex(0),
      targetDepartment(deptLevel)
{
    flatComposite(this->root);
    first();
}

void DepartmentTaskIterator::flatComposite(Task *node)
{
    if (node == nullptr)
    {
        return;
    }

    flatTasks.push_back(node);

    if (!node->isLeaf())
    {
        std::vector<Task *> children = node->getChildren();
        for (std::size_t i = 0; i < children.size(); ++i)
        {
            flatComposite(children[i]);
        }
    }
}

bool DepartmentTaskIterator::isEligible(Task *t)
{
    if (t == nullptr)
    {
        return false;
    }
    return (t->getDepartmentLevel() == targetDepartment) && (!t->isFinished());
}

void DepartmentTaskIterator::first()
{
    currentIndex = 0;
    while (!isDone() && !isEligible(flatTasks[currentIndex]))
    {
        ++currentIndex;
    }
}

void DepartmentTaskIterator::next()
{
    if (!isDone())
    {
        ++currentIndex;
        while (!isDone() && !isEligible(flatTasks[currentIndex]))
        {
            ++currentIndex;
        }
    }
}

bool DepartmentTaskIterator::isDone()
{
    return currentIndex < 0 ||
           static_cast<std::size_t>(currentIndex) >= flatTasks.size();
}

Task *DepartmentTaskIterator::currentItem()
{
    return isDone() ? nullptr : flatTasks[currentIndex];
}

bool DepartmentTaskIterator::allCompleted()
{
    for (std::size_t i = 0; i < flatTasks.size(); ++i)
    {
        if (flatTasks[i] != nullptr && !flatTasks[i]->isFinished())
        {
            return false;
        }
    }
    return true;
}