#pragma once
#include <menu/menu.h>
#include <sdk/sdk.h>
#include <hooks/SendPacket.h>
#include <hooks/SendPacketRaw.h>

void menu::playerpog() {
    if (ImGui::Button("Ban All")) {
        for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
            std::string name = pair.second->name;
            std::string name2 = name.substr(2);
            utils::replace(name2, "``", "");
            std::string packet = "action|input\n|text|/ban " + name2;
            SendPacketHook::Execute(2, packet, sdk::GetPeer());
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Pull All")) {
        for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
            std::string name = pair.second->name;
            std::string name2 = name.substr(2);
            utils::replace(name2, "``", "");
            std::string packet = "action|input\n|text|/pull " + name2;
            SendPacketHook::Execute(2, packet, sdk::GetPeer());
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Kick All")) {
        for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
            std::string name = pair.second->name;
            std::string name2 = name.substr(2);
            utils::replace(name2, "``", "");
            std::string packet = "action|input\n|text|/kick " + name2;
            SendPacketHook::Execute(2, packet, sdk::GetPeer());
        }
    }
    ImGui::Text("Player List");
    if (sdk::GetGameLogic()->GetNetObjMgr()) {
        ImGui::Columns(6, "Player"); // 4-ways, with border
        static auto width = ImGui::GetColumnWidth(0);

        ImGui::SetColumnWidth(0, width * 0.65f);
        ImGui::SetColumnWidth(1, width * 0.65f);
        ImGui::SetColumnWidth(2, width * 1.20f);
        ImGui::SetColumnWidth(3, width * 0.65f);
        ImGui::SetColumnWidth(4, width * 0.65f);
        ImGui::SetColumnWidth(5, width * 1.55f);
        ImGui::Separator();
        ImGui::Text("NetID");
        ImGui::NextColumn();
        ImGui::Text("UserID");
        ImGui::NextColumn();
        ImGui::Text("Name");
        ImGui::NextColumn();
        ImGui::Text("IsMod");
        ImGui::NextColumn();
        ImGui::Text("IsSuperMod");
        ImGui::NextColumn();
        ImGui::Text("Position");
        ImGui::NextColumn();
        ImGui::Separator();
        for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
            std::string name = pair.second->name;
            std::string name2 = name.substr(2);
            utils::replace(name2, "``", "");
            ImGui::Text("%d", pair.first);
            ImGui::NextColumn();
            ImGui::Text("%d", pair.second->userid);
            ImGui::NextColumn();
            ImGui::Text(std::string(name2).c_str());
            ImGui::NextColumn();
            ImGui::Text("%d", pair.second->is_mod);
            ImGui::NextColumn();
            ImGui::Text("%d", pair.second->is_supermod);
            ImGui::NextColumn();
            ImGui::Text("%.0f, %.0f", pair.second->pos.x, pair.second->pos.y);
            ImGui::NextColumn(); 
            //ImGui::Text("NetID: %d, Name: %s\n", pair.first, pair.second->name.c_str());
        }
        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::Columns(2, "Commands");
        ImGui::Text("Name");
        ImGui::NextColumn();
        ImGui::Text("Command");
        ImGui::NextColumn();
        ImGui::Separator();
        for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
            std::string name = pair.second->name;
            std::string name2 = name.substr(2);
            utils::replace(name2, "``", "");
            /////////////////////////////////////////
            ImGui::Text(std::string(name2).c_str());
            /////////////////////////////////////////
            ImGui::SameLine();
            ImGui::NextColumn();
            if (ImGui::Button((std::string("Kick") + std::string("##") + name2).c_str())) {
                std::string packet = "action|input\n|text|/kick " + name2;
                SendPacketHook::Execute(2, packet, sdk::GetPeer());
            }
            ImGui::SameLine();
            if (ImGui::Button((std::string("Ban") + std::string("##") + name2).c_str())) {
                std::string packet = "action|input\n|text|/ban " + name2;
                SendPacketHook::Execute(2, packet, sdk::GetPeer());
            }
            ImGui::SameLine();
            if (ImGui::Button((std::string("Pull") + std::string("##") + name2).c_str())) {
                std::string packet = "action|input\n|text|/pull " + name2;
                SendPacketHook::Execute(2, packet, sdk::GetPeer());
            }
            ImGui::SameLine();
            if (ImGui::Button((std::string("Teleport") + std::string("##") + name2).c_str())) {
                auto local = sdk::GetGameLogic()->GetLocalPlayer();
                local->SetPos(pair.second->pos.x, pair.second->pos.y);
            }
            ImGui::NextColumn();

        }
       
    }
}