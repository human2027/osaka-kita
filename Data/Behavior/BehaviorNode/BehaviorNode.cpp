#include "BehaviorNode.h"
#include "Blackboard.h"
#include "AIPlayer.h"

BTStatus SelectorNode::Tick(AIPlayer& ai, const Blackboard& bb)
{
    for (auto& c : children)
    {
        if (c->Tick(ai, bb) == BTStatus::Success)
            return BTStatus::Success;
    }
    return BTStatus::Failure;
}

BTStatus SequenceNode::Tick(AIPlayer& ai, const Blackboard& bb)
{
    for (auto& c : children)
    {
        if (c->Tick(ai, bb) == BTStatus::Failure)
            return BTStatus::Failure;
    }
    return BTStatus::Success;
}
