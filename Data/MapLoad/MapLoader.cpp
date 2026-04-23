#include "MapLoader.h"
#include "InitialValue.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "ItemType.h"

static std::string Trim(const std::string& s)
{
    std::string t = s;

    // UTF-8 BOM ‘Î‰ž
    if (t.size() >= 3 &&
        static_cast<unsigned char>(t[0]) == 0xEF &&
        static_cast<unsigned char>(t[1]) == 0xBB &&
        static_cast<unsigned char>(t[2]) == 0xBF)
    {
        t.erase(0, 3);
    }

    const char* ws = " \t\r\n\"";
    const size_t begin = t.find_first_not_of(ws);
    if (begin == std::string::npos) return "";

    const size_t end = t.find_last_not_of(ws);
    return t.substr(begin, end - begin + 1);
}

static bool TryParseInt(const std::string& s, int& out)
{
    const std::string t = Trim(s);
    if (t.empty()) return false;

    try
    {
        size_t pos = 0;
        int v = std::stoi(t, &pos);
        if (pos != t.size()) return false;

        out = v;
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool MapLoader::LoadCSV(const std::string& path, std::vector<Tile>& outTiles)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cout << "CSV‚ðŠJ‚¯‚Ü‚¹‚ñ: " << path << "\n";
        return false;
    }

    outTiles.clear();

    std::string line;
    int lineNo = 0;

    while (std::getline(file, line))
    {
        ++lineNo;

        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string typeStr;
        std::string itemStr;

        std::getline(ss, typeStr, ',');
        std::getline(ss, itemStr, ',');

        typeStr = Trim(typeStr);
        itemStr = Trim(itemStr);

        if (typeStr.empty()) continue;

        int typeNum = 0;
        if (!TryParseInt(typeStr, typeNum))
        {
            std::cout << "type ‚Ì•ÏŠ·Ž¸”s: line=" << lineNo
                << " value=[" << typeStr << "]\n";
            continue;
        }

        Tile tile{};
        tile.type = static_cast<TileType>(typeNum);
        tile.itemType = ItemType::Item_none;
        tile.value = 0;

        if (!itemStr.empty())
        {
            int itemNum = 0;
            if (!TryParseInt(itemStr, itemNum))
            {
                std::cout << "item ‚Ì•ÏŠ·Ž¸”s: line=" << lineNo
                    << " value=[" << itemStr << "]\n";
                continue;
            }

            tile.itemType = static_cast<ItemType>(itemNum);
        }

        switch (tile.type)
        {
        case TileType::Damage:
            tile.value = Tile_Damage_Value;
            tile.itemType = ItemType::Item_none;
            break;

        case TileType::Heal:
            tile.value = Tile_Heal_Value;
            tile.itemType = ItemType::Item_none;
            break;

        case TileType::Item:
            tile.value = 0;
            break;

        case TileType::Event:
        case TileType::Normal:
        default:
            tile.value = 0;
            tile.itemType = ItemType::Item_none;
            break;
        }

        outTiles.push_back(tile);
    }

    return true;
}