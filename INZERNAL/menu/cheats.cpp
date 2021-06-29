#pragma once
#include <menu/menu.h>
#include <sdk/Consts.h>
#include <sdk/ResourceManager.h>
#include <sdk/sdk.h>
#include <hooks/SendPacket.h>
#include <hooks/SendPacketRaw.h>
#include "sdk/world/WorldCamera.h"
#include <d3d/DrawingLUL.h>
#include "core/globals.h"
#include "proton/Math/rtRect.h"
#include <hooks/ProcessTankUpdatePacket.h>



void log(std::string msg) {
    variantlist_t var{ "OnConsoleMessage" };
    var[1] = "`0[`^INZERNAL`0] " + msg;
    gt::send_varlist_self(var, -1, 0, true);
}
void menu::cheats_tab() {
     if (ImGui::BeginChild("###INFO", AUTOSIZEC(2), true, ImGuiWindowFlags_MenuBar)) { //info test
    auto logic = sdk::GetGameLogic();
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    auto locals = sdk::GetGameLogic()->GetNetObjMgr();
    auto tilemap = logic->GetTileMap();
    auto w2s = logic->GetWorldRenderer()->camera;

    auto draw = ImGui::GetBackgroundDrawList();
    if (local) {
        auto pos2f = local->GetPos();
        float tilex = pos2f.x / 32;
        float tiley = pos2f.y / 32;
        ImGui::Text("Position: %.0f,%.0f", pos2f.x, pos2f.y);
        ImGui::Text("Pos Tile: %.0f,%.0f", tilex, tiley);
        if (local->flags != 0) {
            printf("Flags %d \n", local->flags);
        }
        ImGui::Text("Facing Left: %d",local->facing_left);
        string test = std::to_string(local->skin_color);
        imwrap::inputstring("##test", &test, ImGuiInputTextFlags_None);
        Tile* tile = tilemap->GetTileSafe(local->GetPos() / 32);
        if (tile) {
            ImGui::Text("%d", tile->foreground);
            ImGui::Text("%d", tile->flags);
            ImGui::Text("%d", tile->collision_rect);
        }
    }
         ImGui::EndChild();
        ImGui::Spacing();
    }
    ImGui::Checkbox("ItemFilter", &opt::cheat::filterautocollect);
    ImGui::SameLine();
    ImGui::InputInt("##ITEMID", &opt::cheat::itemfilter);
    ImGui::Checkbox("Auto Collect", &opt::cheat::autocollect);
    ImGui::SameLine();
    ImGui::SliderInt("Range", &opt::cheat::range_val, 1, 10);
    ImGui::Checkbox("Item ESP", &opt::cheat::itemesp);
    ImGui::Checkbox("Player Esp", &opt::cheat::playeresp);
    ImGui::Checkbox("PathFinding No Lagback", &opt::cheat::PathFindinglol);
    static int StartFrom = 0;
    ImGui::Checkbox("Brute Force Door", &opt::cheat::bruteforcepassdoor);
    if (opt::cheat::bruteforcepassdoor) {
        static types::time time = std::chrono::system_clock::now();
        if (utils::run_at_interval(time, 0.025f)) {
            auto local = sdk::GetGameLogic()->GetLocalPlayer();
            if (local) {
                Tile* tile = tilemap->GetTileSafe(local->GetPos() / 32);
                if (tile) {
                    if (tile->foreground == 762 || tile->foreground == 8260) {
                        StartFrom++;
                        GameUpdatePacket packet{ 0 };
                        packet.type = PACKET_TILE_ACTIVATE_REQUEST;
                        packet.int_x = local->pos.x / 32;
                        packet.int_y = local->pos.y / 32;
                        SendPacketRawHook::Execute(4, &packet, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                        int tilex, tiley;
                        tilex = local->pos.x / 32;
                        tiley = local->pos.y / 32;
                        std::string packets = "action|dialog_return\ndialog_name|password_reply\ntilex|" + std::to_string(tilex) + "|\ntiley|" + std::to_string(tiley) +
                                              "|\npassword|" + std::to_string(StartFrom);
                        SendPacketHook::Execute(2, packets, sdk::GetPeer());
                    }
                    else {
                        log("`bThe Pass is about: `4" + std::to_string(StartFrom - 9));
                        opt::cheat::bruteforcepassdoor = false;
                        StartFrom = 0;
                    }
                }
            }
            else {
                opt::cheat::bruteforcepassdoor = false;
                StartFrom = 0;
            }
        }
    }
    ImGui::SameLine();
    ImGui::InputInt("##StartFrom", &StartFrom);
    ImGui::Checkbox("Auto Farm", &opt::cheat::autofarm);
    ImGui::SameLine();
    ImGui::InputInt("Item ID", &opt::cheat::itemid_val);
    ImGui::Checkbox("Random Value", &opt::cheat::randommodepacketstate);
    ImGui::Checkbox("Sum Packet_STATE", &opt::cheat::spamwater);
    if (!opt::cheat::crystalmode && !opt::cheat::randommodepacketstate) {
        ImGui::SameLine();
        ImGui::InputInt("Item_ID", &opt::cheat::watervalue);
    }
    if (ImGui::BeginChild("###Warp", AUTOSIZEC(2), true, ImGuiWindowFlags_MenuBar)) { // warp with button at menu
        ImGui::BeginMenuBar();
        ImGui::Text("Warp");
        ImGui::EndMenuBar();
        static char worlddname[25];
        size_t size = std::strlen(worlddname);
        ImGui::Text("World Name:");
        ImGui::SameLine();
        ImGui::InputText("", worlddname, IM_ARRAYSIZE(worlddname));
        ImGui::SameLine();
        if (ImGui::Button("Warp to World")) {
            gt::join_world(worlddname);
        }
        ImGui::SameLine();
        ImGui::Text(" %u / 24", size);
        ImGui::EndChild();
        ImGui::Spacing();
    }
    //ImGui::Checkbox("Aimbot(meme)", &opt::cheat::autopunchplayer);
    ImGui::Checkbox("Epilepsy", &opt::cheat::blinkcolor);
    ImGui::Checkbox("Test Path Finding", &opt::cheat::testmftiles);
    ImGui::Checkbox("Dance Move", &opt::cheat::dancemove);
    ImGui::Checkbox("Ghost", &opt::cheat::ghost);
    ImGui::Checkbox("Nazi LOL",&opt::cheat::nazismoke);
    ImGui::Checkbox("Only Crystal", &opt::cheat::crystalmode);
    static bool punchaura = false;
    ImGui::Checkbox("Punch Aura (if lag = ban)", &punchaura);       
    if (punchaura) {
        auto local = sdk::GetGameLogic()->GetLocalPlayer();
        if (local) {
            static types::time times = std::chrono::system_clock::now();
            static types::time timer = std::chrono::system_clock::now();
            auto locals = sdk::GetGameLogic()->GetNetObjMgr();
            //for (auto it = objmap->objects.begin(); it != objmap->objects.end(); ++it)
            auto it = sdk::GetGameLogic()->GetNetObjMgr()->players.begin();
            std::advance(it, rand() % sdk::GetGameLogic()->GetNetObjMgr()->players.size());
            
                if (it->second != local && utils::isInside(it->second->pos.x, it->second->pos.y, 5 * 32, local->pos.x, local->pos.y)) {
                    GameUpdatePacket packet{ 0 };
                    auto pos = local->GetPos();
                    int x = local->GetPos().x / 32;
                    int y = local->GetPos().y / 32;
                    packet.type = PACKET_STATE;
                    packet.int_data = 18;
                    packet.int_x = it->second->pos.x / 32;
                    packet.int_y = it->second->pos.y / 32;
                    packet.flags = 2560; //no enum rn so using raw values

                    //Punch: 2560
                    //Respawn: 2308   show

                    packet.pos_x = pos.x;
                    packet.pos_y = pos.y;
                    if (utils::run_at_interval(times, 0.050)) {
                        SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                    }
                }
        }
    }
   
    ImGui::Checkbox("No Name", &opt::cheat::noname);
    static bool showcmd = false;
    ImGui::Checkbox("Show Log", &showcmd);
    if (showcmd) {
        utils::attach_console();
    }
    //static int itemid = 5640;
    static int Packetsended = 1;
    static bool crashergtps = false;
    static bool onlyforgtps = false;
    ImGui::Checkbox("SuperBreak (GTPS)", &opt::cheat::superpunchbreak);
    ImGui::Checkbox("Sum LAGGER i think (GTPS)", &onlyforgtps);
    if (onlyforgtps) {
        static types::time times = std::chrono::system_clock::now();
        auto it = sdk::GetGameLogic()->GetNetObjMgr()->players.begin();
        std::advance(it, rand() % sdk::GetGameLogic()->GetNetObjMgr()->players.size());
        GameUpdatePacket packet{ 0 };
        auto pos = local->GetPos();
        int x = local->GetPos().x / 32;
        int y = local->GetPos().y / 32;
        packet.type = PACKET_STATE;
        packet.int_data = 822;
        packet.int_x = it->second->pos.x / 32;
        packet.int_y = it->second->pos.y / 32;
        packet.flags = 32 | 48 | 268435456 | 65536 | (1 << 11) | 32768 | 163840 | 2308; //no enum rn so using raw values

        //Punch: 2560
        //Respawn: 2308   show

        packet.pos_x = pos.x;
        packet.pos_y = pos.y;
        packet.vec_x = pos.x;
        packet.vec_y = pos.y;
        packet.vec2_x = 0;
        packet.vec2_y = 0;

        GameUpdatePacket packets{ 0 };
        packets.type = PACKET_SET_ICON_STATE;
        packets.flags = 8;
        for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
            packet.netid = pair.first;
            packet.int_x = utils::random(0, 2);
            if (utils::run_at_interval(times, 0.020))
            gt::send(&packets);
        }

        if (utils::run_at_interval(times, 0.020))
            SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);                
    }
    ImGui::Checkbox("Lag GTPS(not for all gtps)", &crashergtps);
    if (crashergtps && opt::custom_server_on) {
        if (local) {
            Packetsended++;
            GameUpdatePacket packet{ 0 };
            packet.type = PACKET_TILE_ACTIVATE_REQUEST;
            packet.int_x = local->pos.x / 32;
            packet.int_y = local->pos.y / 32;
            SendPacketRawHook::Execute(4, &packet, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
            int tilex, tiley;
            tilex = local->pos.x / 32;
            tiley = local->pos.y / 32;
            std::string packets = "action|dialog_return\ndialog_name|password_reply\ntilex|" + std::to_string(tilex) + "|\ntiley|" + std::to_string(tiley) +
                                  "|\npassword|" + std::to_string(Packetsended);
            SendPacketHook::Execute(2, packets, sdk::GetPeer());

            GameUpdatePacket spammed{ 0 };
            spammed.type = 3;
            spammed.netid = -1;
            spammed.pos_x = local->pos.x;
            spammed.pos_y = local->pos.y;
            spammed.int_x = local->pos.x / 32 + Packetsended;
            spammed.int_y = local->pos.y / 32 + Packetsended;
            
            SendPacketRawHook::Execute(4, &spammed, 56, 0, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
        }
        else {
            Packetsended = 0;
            crashergtps = false;
        }
        
    }
    ImGui::Text("Packet Sended: %d", Packetsended);
   
    //SELF TODO:
    //Add proper changelog in github projects
    //Red punch shit
    //Actually cleanup shit
    //Respond to stuff in discussions
    //TODO//TODO
    //TODO
    //TODO
    //TODO
    //TODO
    //TODO
    //TODO
    //TODO
    //TODO
    //TODO
    //TODO
    //TODO
    //TODO
    //TODO
    //TODO
    //TODO


    //commented for now - added too many things and things are pretty messy ATM
    /*   static types::time timer2 = std::chrono::system_clock::now();
    if (local && utils::run_at_interval(timer2, 0.1)) {
          GameUpdatePacket packet{ 0 };
          packet.type = PACKET_ON_STEP_ON_TILE_MOD;
          packet.int_data = 3728;
          packet.netid = 2;
          packet.int_x = INT_MAX;
          packet.int_y = INT_MIN;
          gt::send(&packet);
    }*/

    //if (sdk::GetGameLogic()->GetNetObjMgr()) {
    //    for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
    //        ImGui::Text("NetID: %d, Name: %s\n", pair.first, pair.second->name.c_str());
    //    }
    //}
    /*if (ImGui::Button("rape shit")) {
        auto pos = local->GetPos();
        GameUpdatePacket packet{ 0 };
        packet.type = PACKET_ON_STEP_ON_TILE_MOD;
        packet.int_data = 3728;
        packet.int_x = -2;
        packet.int_y = -2;
        gt::send(&packet);
    }
    if (ImGui::Button("Test")) {
        GameUpdatePacket packet{ 0 };
        packet.type = PACKET_SET_ICON_STATE;
        packet.netid = utils::random(INT_MIN, -4);
        packet.int_x = utils::random(INT_MIN, INT_MAX);
        packet.int_y = utils::random(INT_MIN, INT_MAX);
        packet.int_data = utils::random(INT_MIN, INT_MAX);
        packet.flags = utils::random(INT_MIN, INT_MAX);
        packet.item = utils::random(INT_MIN, INT_MAX);
        gt::send(&packet);
    }*/
    
    
    

    ImGui::Checkbox("Local building", &sdk::GetGameLogic()->local_building);

    
    //commented for now - added too many things and things are pretty messy ATM
    /*   static SurfaceAnim* surfaceanim = nullptr;
    if (!surfaceanim && global::app) {
        ResourceManager* mgr = (ResourceManager*)((uintptr_t)global::app + 2408);
        if (mgr)
            surfaceanim = mgr->GetSurfaceAnim("game/tiles_page4.rttex");
    }*/

    //static types::time time = std::chrono::system_clock::now();
    //if (local && utils::run_at_interval(time, 0.10f, true, 0.05f)) {
    //    auto pos = local->GetPos();
    //    GameUpdatePacket packet{ 0 };
    //    packet.type = PACKET_ON_STEP_ON_TILE_MOD;
    //    packet.int_data = 3728;
    //    //   packet.flags = utils::random(0, INT_MAX);
    //    // packet.item = utils::random(0, INT_MAX);
    //    packet.int_x = int(pos.x / 32.f) + utils::random(-2, 2);
    //    packet.int_y = int(pos.y / 32.f) + utils::random(-2, 2);
    //    gt::send(&packet);
    //    packet.int_x += utils::random(-1, 1);
    //    packet.int_y += utils::random(-1, 1);
    //    gt::send(&packet);

    //    packet.int_x += utils::random(-1, 1);
    //    packet.int_y += utils::random(-1, 1);
    //    gt::send(&packet);
    //}

    
    
    //commented for now - added too many things and things are pretty messy ATM
    //tldr texture drawing 
    //if (surfaceanim) {
    //    //shoutout to atipls for helping me find s_renderD3D9 offset initially,
    //    //and helping me with this in general, couldnt have done it without him
    //    constexpr static auto m_textures_offset = 0x67800;    //bgfx internal, very very small chance of changing
    //    static auto s_renderD3D9 = sigs::get(sig::s_renderd3d9);
    //    static auto m_textures = (TextureD3D9*)(s_renderD3D9 + m_textures_offset);

    //    static auto& texture = m_textures[surfaceanim->texture_id];

    //    int largest = 0;
    //    for (auto i = 0; m_textures[i].m_ptr; ++i)
    //        largest = i;

    //    const auto size = ImVec2(64.f, 64.f);
    //    //proof of concept image drawing
    //    ImGui::Image(ImTextureID(texture.m_ptr), size, ImVec2((size.x + size.x) / texture.m_width, 1 - (size.x / texture.m_width)),
    //        ImVec2(1 - ((size.y + size.y) / texture.m_height), size.y / texture.m_height));
    //}

    imwrap::prep_columns(6);
    imwrap::checkbox("TP on click", opt::cheat::tp_click, "Teleports to cursor position when you press ctrl + left click");
    bool dash = imwrap::checkbox("Dashing", opt::cheat::dash, "Allows you to double tap to dash in any direction");

    ImGui::NextColumn();
    imwrap::checkbox("Block SPR", opt::cheat::block_sendpacketraw, "Sendpacketraw. Basically full-on ghost, but a bit more crude than actual ghost.");
    bool charge = imwrap::checkbox("Jump charge", opt::cheat::jump_charge, "Switches your jumping mode to being charging, holding means higher jump");

    ImGui::NextColumn();
    imwrap::checkbox("Mod zoom", opt::cheat::mod_zoom, "Allows you to zoom as far out as you want to, like mods");
    bool cancel = imwrap::checkbox("Jump cancel", opt::cheat::jump_cancel, "You can tap W in air to instantly change your ascent to starting descent");

    ImGui::NextColumn();
    imwrap::checkbox("Dev zoom", opt::cheat::dev_zoom, "Same as mod zoom but allows you to place and build far away too.\nWhich can ban btw.");
    imwrap::checkbox("Free dialogs", opt::cheat::dialog_cheat,
        "When enabled, you can move, punch and so on while a dialog is opened.\nIf there is text in the dialog, then you cant move, but can still punch, etc.");

    ImGui::NextColumn();
    imwrap::checkbox("Antighost", opt::cheat::antighost,
        "Ignores ghost slimed effect.\nAlso now completely ignores any packets relating to these annoying fuckers.\nWell, that makes see ghosts useless with this, I "
        "guess?");
    imwrap::checkbox("Antipunch", opt::cheat::antipunch, "Simple: Same as having a punch jammer in worlds that don't have one.");

    ImGui::NextColumn();
    imwrap::checkbox("See ghosts", opt::cheat::see_ghosts, "Allows you to see ghosts as the name says\nDoesn't work with antighost.");
    imwrap::checkbox("See fruits", opt::cheat::see_fruits, "See how many fruits a tree will have before it has grown fully.");

    ImGui::Columns(1, nullptr, false);
    ImGui::PopStyleVar();
    
    if (ImGui::Button("Fake Respawn(Bannable)")) { // ReDesigned
        GameUpdatePacket packet{ 0 };
        packet.flags = 2308;
                             //Punch: 2560
                             //Respawn: 2308
        //got hit : 268435456
        gt::send(&packet);
        //gt::send_self(&packet, true);
   }
   ImGui::SameLine();
   if (ImGui::Button("Got Hit By Shit")) {
       GameUpdatePacket packet{ 0 };
       packet.flags = 268435456;
       //Punch: 2560
       //Respawn: 2308
       //got hit cactus : 268435456
       //mushroom : 32768
       //pinball : 163840
       gt::send(&packet);
       //gt::send_self(&packet, true);
   }
    ImGui::SameLine();
    if(ImGui::Button("Drop Dirt")) {
        std::string packetC = "action|drop\nitemID|2";
        SendPacketHook::Execute(2, packetC, sdk::GetPeer());
        std::string packetD = "action|dialog_return\ndialog_name|drop_item\nitemID|2\ncount|1\n";
        SendPacketHook::Execute(2, packetD, sdk::GetPeer());
        //DiscordWebhookSender("Test","Dropped Dirt");
    }
    ImGui::SameLine();
    if (ImGui::Button("Take All Items in 10 Block Far")) {
        auto draw = ImGui::GetBackgroundDrawList();
        auto logic = sdk::GetGameLogic();
        auto tilemap = logic->GetTileMap();
        auto pos2f = local->GetPos();
        auto objmap = logic->GetObjectMap();
        auto rend = sdk::GetGameLogic()->renderer;
        if (local) {
            static types::time times = std::chrono::system_clock::now();
            for (auto it = objmap->objects.begin(); it != objmap->objects.end(); ++it) {
                if (utils::isInside(it->pos.x, it->pos.y, opt::cheat::range_val * 32, pos2f.x, pos2f.y)) {
                    static types::time times = std::chrono::system_clock::now();
                    auto mf = it->pos;
                    float mf1 = mf.x;
                    float mf2 = mf.y;
                    //Tile* tile = tilemap->GetTileSafe(mf1 / 32, mf2 / 32);
                    if (opt::cheat::filterautocollect) {
                        if (it->item_id == opt::cheat::itemfilter) {
                            GameUpdatePacket packet{ 0 };
                            packet.pos_x = it->pos.x;
                            packet.pos_y = it->pos.y;
                            packet.type = 11;
                            packet.netid = -1;
                            packet.object_id = it->object_id;
                            SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                        }
                    }
                    else {
                        GameUpdatePacket packet{ 0 };
                        packet.pos_x = it->pos.x;
                        packet.pos_y = it->pos.y;
                        packet.type = 11;
                        packet.netid = -1;
                        packet.object_id = it->object_id;
                        SendPacketRawHook::Execute(4, &packet, 56, NULL, sdk::GetPeer(), ENET_PACKET_FLAG_RELIABLE);
                    }
                }
            }
        }
    }
    
    //deleted sendpacket here

    if (local && (dash || charge || cancel))
        local->SetCharacterMods(opt::cheat::dash, opt::cheat::jump_charge, opt::cheat::jump_cancel);

    imwrap::prep_columns(2);
    ImGui::Spacing();
    static int ix = C_PUNCH_RELOAD_TIME;
    if (ImGui::BeginChild("###cooldownchanger", AUTOSIZEC(2), true, ImGuiWindowFlags_MenuBar)) {
        ImGui::BeginMenuBar();
        ImGui::Text("Punch speed changer");
        ImGui::EndMenuBar();
        if (ImGui::Checkbox("Enabled###punch", &opt::cheat::punch_cooldown_on)) {
            if (!opt::cheat::punch_cooldown_on)
                consts::set_float(ix, consts::defs[ix]);
            else
                consts::set_float(ix, opt::cheat::punch_cooldown_val);
        }
        ImGui::SameLine();
        if (ImGui::SliderFloat("###punchc", &opt::cheat::punch_cooldown_val, 0.0f, 0.4f, "%0.2f") && opt::cheat::punch_cooldown_on)
            consts::set_float(ix, opt::cheat::punch_cooldown_val);

        ImGui::EndChild();
        ImGui::Spacing();
    }
    if (imwrap::fchanger("Gravity changer", opt::cheat::gravity_val, opt::cheat::gravity_on, -500.f, 2000.f) && local)
        local->gravity.set(global::state.gravity);

    static bool isConstOn = false;

    if (ImGui::BeginChild("###Constants", isConstOn ? AUTOSIZEC(6) : AUTOSIZEC(2), true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar)) {
        ImGui::BeginMenuBar();
        ImGui::Text("Game constants");
        ImGui::EndMenuBar();

        if (!isConstOn) {
            if (ImGui::Button("Expand"))
                isConstOn = true;
            ImGui::SameLine();
            ImGui::Text("Contains 21 different protected constants");
        }
        else {
            if (ImGui::Button("Close"))
                isConstOn = false;
            ImGui::SameLine();
            int i = 0;
            if (ImGui::Button("Reset all")) {
                consts::reset_all();

                if (opt::cheat::punch_cooldown_on) //dont reset this if we have the other option on for this
                    consts::set_float(C_PUNCH_RELOAD_TIME, opt::cheat::punch_cooldown_val);
            }
            ImGui::Spacing();
            ImGui::Spacing();

            imwrap::const_slider("Max fall speed", 0.f, 2000.f, "%.0f", i++, "Pretty simple");
            imwrap::const_slider("Float power", 0.f, 10000.f, "%.0f", i++, "Higher makes damage flying curve smaller");
            imwrap::const_slider("Wind speed", 0.f, 2000.0f, "%.0f", i++, "The speed winter wind and others push you");
            imwrap::const_slider("Wind speed (against)", 0.f, 200.0f, "%.0f", i++, "The resistance of going against winter wind");
            imwrap::const_slider("Coll. max divergence", 0.f, 128.0f, "%.0f", i++, "No clue what this does, no references, no effect");
            imwrap::const_slider("Player drag", -500.f, 2400.0f, "%.0f", i++, "Drag for walking. Low values allow 'moonwalk'.");
            imwrap::const_slider("Initial jump boost", -100.f, -1000.0f, "%.0f", i++, "Affects how high you can jump in a different way from gravity");
            imwrap::const_slider(
                "Control hurt mod", 0.0f, 1.5f, "%0.2f", i++, "Affects how well you are able to control your char when hurt. 1 = normal, 0 = no control.");
            imwrap::const_slider("Lava bounce [x]", -100.f, 1000.0f, "%.0f", i++, "Just how much you bounce from lava/etc horizontally");
            imwrap::const_slider("Lava bounce [y]", -100.f, 1400.0f, "%.0f", i++, "Just how much you bounce from lava/etc vertically");
            imwrap::const_slider("Trampoline bounce", -100.f, 1600.0f, "%.0f", i++, "Bounciness for mushroom/etc");
            imwrap::const_slider("Regular bounce", -100.f, 1600.0f, "%.0f", i++, "Bounciness for pinballs/etc");
            imwrap::const_slider("Def. punch knockback", -500.f, 500.0f, "%.0f", i++, "Does not seem to have much effect except for anims (clientside)");
            imwrap::const_slider("Parasol gravity mod", 0.f, 1.0f, "%0.2f", i++, "How much parasol and similar affect gravity");
            imwrap::const_slider("Water gravity mod", 0.f, 2.0f, "%0.2f", i++, "How much water affects gravity");
            imwrap::const_slider("Mars gravity mod", 0.f, 2.0f, "%0.2f", i++, "How much mars affects gravity");
            imwrap::const_slider("Engine speedhack", 0.f, 4000.0f, "%.0f", i++, "Game engine speedhack. Affects various things, some need re-enter to apply fully.");
            imwrap::const_slider("Ghost speed", 0.f, 1000.0f, "%.0f", i++, "When you have the /ghost playmod, not related to ghosts that slime you.");
            imwrap::const_slider(
                "Water lev. time", 0.f, 6000.0f, "%.0f", i++, "Probably the time that the clothes that keep you floating on water have any effect. Didn't test.");
            imwrap::const_slider("Punch cooldown", 0.f, 0.4f, "%0.2f", i++, "Changes punch cooldown, fast punch when small value");
            imwrap::const_slider("Hack report delay (ms)", 100.f, 30000.0f, "%.0f", i++, "For example cheat engine open report, etc. Shouldn't have much of any use");
        }
    }
    ImGui::EndChild();

    ImGui::NextColumn();
    ImGui::Spacing();
    if (imwrap::fchanger("Acceleration changer", opt::cheat::accel_val, opt::cheat::accel_on, -500.f, 10000.f) && local)
        local->accel.set(global::state.accel);
    if (imwrap::fchanger("Speed changer", opt::cheat::speed_val, opt::cheat::speed_on, 100.f, 1400.f) && local)
        local->speed.set(global::state.speed);
    if (imwrap::fchanger("Water Speed changer", opt::cheat::waterspeed_val, opt::cheat::waterspeed_on, 0.f, 1400.f) && local)
        local->water_speed = global::state.water_speed;

    ImGui::Columns(1, nullptr, false);
    ImGui::PopStyleVar();
    //dont worry bringing back instant stop and start later on without the hooks.
    
}

