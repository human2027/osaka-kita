#pragma once
#include <memory>
#include <vector>
#include "BlackBoard.h"
#include "AIPlayer.h"

enum class BTStatus
{
    Success,
    Failure
};

class BTNode
{
public:
    virtual ~BTNode() = default;
    virtual BTStatus Tick(AIPlayer& ai, const Blackboard& bb) = 0;
};

using BTNodePtr = std::shared_ptr<BTNode>;

//Composite: Selector
class SelectorNode : public BTNode
{
public:
    void AddChild(const BTNodePtr& n) { children.push_back(n); }
    BTStatus Tick(AIPlayer& ai, const Blackboard& bb) override;

private:
    std::vector<BTNodePtr> children;
};

//Composite: Sequence
class SequenceNode : public BTNode
{
public:
    void AddChild(const BTNodePtr& n) { children.push_back(n); }
    BTStatus Tick(AIPlayer& ai, const Blackboard& bb) override;

private:
    std::vector<BTNodePtr> children;
};
