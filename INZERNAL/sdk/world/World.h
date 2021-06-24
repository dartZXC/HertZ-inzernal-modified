#pragma once
#include <core/utils.h>
#include <sdk/world/WorldTileMap.h>
#include <sdk/world/WorldObjectMap.h>

#pragma pack(push, 1)
GTClass World { //TODO: literally load offline worlds with World::LoadFromMem, would be kinda epic
   public:
    void* vtable;
    byte unk1;
    byte unk2;
    short version;
    char pad1[4]; //TODO: find out what this is
    WorldTileMap tilemap;
    WorldObjectMap worldobjectmap;
    std::string name;
    //padding
    //world object map
};
#pragma pack(pop)