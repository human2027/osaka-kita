#include "ItemSystem.h"
#include "InitialValue.h"

bool ItemSystem::UseItem(CharacterBase& user)
{
    switch (user.GetHeldItem())
    {
    case ItemType::Item_heal:
        user.AddHP(Item_Recovery);
        user.ClearHeldItem();
        return true;

    case ItemType::Item_boost:
        user.ActivateBoost();
        user.ClearHeldItem();
        return true;

    case ItemType::Item_reverse:
        user.ActivateReverse();
        user.ClearHeldItem();
        return true;

    case ItemType::Item_none:
    default:
        return false;
    }
}