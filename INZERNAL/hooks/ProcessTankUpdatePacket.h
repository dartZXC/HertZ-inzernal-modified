#pragma once
#include <core/globals.h>
#include <core/gt.h>
#include <sdk/sdk.h>
#include <string>
#include "core/discord_webhook.h"
//#include <menu/menu.h>

/*
    {
    "content": "Webhooks are a low-effort way to post messages to channels in Discord. They do not require a bot user or authentication to use. https://discordapp.com/developers/docs/resources/webhook"
    }

*/

class ProcessTankUpdatePacketHook {
   public:
    static void Execute(GameLogic* logic, GameUpdatePacket* packet) {
        static auto orig = decltype(&hooks::ProcessTankUpdatePacket)(hookmgr->orig(sig::processtankupdatepacket));

        //just let the ghosts not even exist
        if (opt::cheat::antighost && packet->type == PACKET_NPC && (packet->npctype == 1 || packet->npctype == 7 || packet->npctype == 12))
            return;

        if (logging::packetview == false) { // View packets
            if (true)
                printf("===================================================\n");
            utils::printc("96", "[RECV]Raw Packet: %d [%s]\n\n", packet->type, gt::get_type_string(packet->type).c_str());
            variantlist_t varlist{};
            if (packet->type == PACKET_CALL_FUNCTION) {
                if (varlist.serialize_from_mem(utils::get_extended(packet))) {
                    auto content = varlist.print();
                    printf("Packet Info:\n%s\n", content.c_str());
                    printf("\n");
                }
            }
            else {
                printf("Packet Info: \n");
                if (packet->item != NULL) {
                    std::cout << "item: " << packet->item << std::endl;
                }
                if (packet->item_id != NULL) {
                    std::cout << "item_id: " << packet->item_id << std::endl;
                }
                if (packet->item_count != NULL && packet->item_count != 4294967295) {
                    std::cout << "item_count: " << packet->item_count << std::endl;
                }
                if (packet->gained_item_count != NULL) {
                    std::cout << "gained_item_count: " << packet->gained_item_count << std::endl;
                }
                if (packet->lost_item_count != NULL) {
                    std::cout << "lost_item_count: " << packet->lost_item_count << std::endl;
                }
                if (packet->objtype != NULL) {
                    std::cout << "objtype: " << packet->objtype << std::endl;
                }
                if (packet->punchid != NULL) {
                    std::cout << "punchid: " << packet->punchid << std::endl;
                }
                if (packet->pos_x != NULL) {
                    std::cout << "pos_x: " << packet->pos_x << std::endl;
                }
                if (packet->pos_y != NULL) {
                    std::cout << "pos_y: " << packet->pos_y << std::endl;
                }
                if (packet->int_x != NULL && packet->int_x != 4294967295) {
                    std::cout << "int_x: " << packet->int_x << std::endl;
                }
                if (packet->int_y != NULL && packet->int_y != 4294967295) {
                    std::cout << "int_y: " << packet->int_y << std::endl;
                }
                if (packet->build_range != NULL) {
                    std::cout << "build_range: " << packet->build_range << std::endl;
                }
                if (packet->punch_range != NULL) {
                    std::cout << "punch_range: " << packet->punch_range << std::endl;
                }
                if (packet->tiles_length != NULL) {
                    std::cout << "tiles_length: " << packet->tiles_length << std::endl;
                }
                if (packet->animation_type != NULL) {
                    std::cout << "animation_type: " << packet->animation_type << std::endl;
                }
                if (packet->flags != NULL) {
                    std::cout << "flags: " << packet->flags << std::endl;
                }
                printf("****************\nGameLogic:\n");
                if (logic->local != NULL) {
                    std::cout << "local: " << logic->local << std::endl;
                }
                if (logic->playeritems != NULL) {
                    std::cout << "playeritems: " << logic->playeritems << std::endl;
                }
                printf("\n");
                if (logic->world != NULL) {
                    std::cout << "world size: x: " << logic->GetTileMap()->size.x << " | y: " << logic->GetTileMap()->size.y << std::endl;
                    std::cout << "tile dmg: " << (int)logic->GetTileMap()->GetTileSafe(57, 14)->health_or_damage << std::endl;
                }
            }
        }
        auto local = sdk::GetGameLogic()->GetLocalPlayer();
        switch (packet->type) {
            case PACKET_CALL_FUNCTION: {
                if (logging::enabled && logging::console & logging::callfunction) {
                    variantlist_t varlist{};
                    auto extended = utils::get_extended(packet);
                    if (varlist.serialize_from_mem(extended)) {
                        auto content = varlist.print();
                        if (content.length() < 4096)
                            printf("%s\n", content.c_str());
                    }
                }
                bool test = true;
                if (test) {
                    variantlist_t varlist{};
                    auto extended = utils::get_extended(packet);
                    if (varlist.serialize_from_mem(extended)) {
                        auto head = varlist.get(0);
                        if (head.get_type() == variant_t::vartype_t::TYPE_STRING && head.get_string().find("onShowCaptcha") != -1) {
                            auto captcha = varlist.get(1).get_string();
                            gt::solve_captcha(captcha);
                            return;
                        }
                        if (head.get_type() == variant_t::vartype_t::TYPE_STRING && head.get_string().find("OnDialogRequest") != -1) {
                            auto content = varlist.get(1).get_string();
                            if (content.find("set_default_color|`o") != -1) {
                                auto content12 = varlist.get(1).get_string();
                                if (content.find("end_dialog|captcha_submit||Submit|") != -1) {
                                    gt::solve_captcha(content12);
                                    return;
                                }
                            }
                            if (content.find("set_default_color|`o") != -1) {
                                if (content.find("add_label_with_icon|big|`wThe Growtopia Gazette``|left|5016|") != -1) {
                                    std::string editedstring =
                                        "\nadd_label_with_icon|big|Nothing lol|left|162|"
                                        "\nadd_spacer|small"
                                        "\nadd_label_with_icon|small|`4Private Inzernal|left|2|"
                                        "\nadd_label_with_icon|small|`5Not Gonna Share + sell|left|8|"
                                        "\nadd_label_with_icon|small|`9Subscribe To HertZxD|left|822|"
                                        "\nadd_quick_exit|"
                                        "\nend_dialog|end|Cancel|Okay|";
                                    varlist.get(1).set(editedstring);
                                    variantlist_t liste{ "OnDialogRequest" };
                                    liste[1] = editedstring;
                                    gt::send_varlist_self(liste);
                                    return;
                                }
                                if (content.find("What would you like to write on this sign?") != -1) {
                                    std::string tileX = content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex") - 1);
                                    std::string tileY = content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley") - 1);
                                    std::string packet = "action|dialog_return\ndialog_name|sign_edit\ntilex|" + tileX + "|\ntiley|" + tileY + "|\nsign_text|`4HertZxD (sigh)";
                                    gt::send(2, packet);
                                    return;
                                }
                            }    
                        }
                        if (head.get_type() == variant_t::vartype_t::TYPE_STRING && head.get_string().find("OnConsoleMessage") != -1) {
                            auto content = varlist.get(1).get_string();
                            /*
                            if (content.find("[SB]_") != -1) {
                                //CP:_OID:_CT:[W]_ `6<`wHZID42``>`` `$s``
                                utils::replace(content,"`","");
                                DiscordWebhookSender(content);
                            }
                            if (content.find("CP:0_PL:4_OID:_CT:[BC]_") != -1) {
                                utils::replace(content, "`", "");
                                DiscordWebhookSender(content);
                            }
                            if (content.find("CP:0_PL:4_OID:_CT:[CB]_") != -1) {
                                utils::replace(content, "`", "");
                                DiscordWebhookSender(content);
                            }
                            */
                        }
                        //`7[```wHZID3`` spun the wheel and got `436``!`7]``
                    }
                }

            } break;
            case PACKET_SEND_INVENTORY_STATE: {
                variantlist_t varlist{};
                auto extended = utils::get_extended(packet);
                break;
            }
            case PACKET_STATE: {

                break;
            }
            case PACKET_SET_CHARACTER_STATE: {
                if (!local)
                    break;

                
                if (packet->netid == local->netid) {
                    if (opt::cheat::antighost && global::state.gravity < packet->gravity_out && global::state.speed > packet->speed_out)
                        return;

                    global::state.copy_from_packet(packet);
                }
            } break;
        }

        orig(logic, packet);
    }
};