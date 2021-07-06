/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "AnticheatLanguage.h"
#include "AnticheatMgr.h"
#include "Chat.h"
#include "Config.h"
#include "Language.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptMgr.h"

class anticheat_commandscript : public CommandScript
{
public:
    anticheat_commandscript() : CommandScript("anticheat_commandscript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> anticheatCommandTable =
        {
            { "delete",         SEC_ADMINISTRATOR,  true,   &HandleAntiCheatDeleteCommand,  "" },
            { "jail",           SEC_GAMEMASTER,     false,  &HandleAnticheatJailCommand,    "" },
            { "warn",           SEC_GAMEMASTER,     true,   &HandleAnticheatWarnCommand,    "" }
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "anticheat",      SEC_GAMEMASTER,     true,   nullptr, "",  anticheatCommandTable },
        };

        return commandTable;
    }

    static bool HandleAnticheatWarnCommand(ChatHandler* handler, const char* args)
    {
        Player* pTarget = nullptr;

        std::string strCommand;

        char* command = strtok((char*)args, " ");

        if (command)
        {
            strCommand = command;
            normalizePlayerName(strCommand);

            pTarget = ObjectAccessor::FindPlayerByName(strCommand.c_str()); // get player by name
        }
        else
        {
            pTarget = handler->getSelectedPlayer();
        }

        if (!pTarget)
        {
            return false;
        }

        WorldPacket data;

        // need copy to prevent corruption by strtok call in LineFromMessage original string
        char* buf = strdup("The anticheat system has reported several times that you may be cheating. You will be monitored to confirm if this is accurate.");
        char* pos = buf;

        while (char* line = handler->LineFromMessage(pos))
        {
            handler->BuildChatPacket(data, CHAT_MSG_SYSTEM, LANG_UNIVERSAL, nullptr, nullptr, line);
            pTarget->GetSession()->SendPacket(&data);
        }

        free(buf);
        return true;
    }

    static bool HandleAnticheatJailCommand(ChatHandler* handler, const char* args)
    {
        Player* pTarget = nullptr;

        std::string strCommand;

        char* command = strtok((char*)args, " ");

        if (command)
        {
            strCommand = command;
            normalizePlayerName(strCommand);

            pTarget = ObjectAccessor::FindPlayerByName(strCommand.c_str()); // get player by name
        }
        else
        {
            pTarget = handler->getSelectedPlayer();
        }

        if (!pTarget)
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (pTarget == handler->GetSession()->GetPlayer())
        {
            return false;
        }

        // teleport both to jail.
        pTarget->TeleportTo(1,16226.5f,16403.6f,-64.5f,3.2f);
        handler->GetSession()->GetPlayer()->TeleportTo(1,16226.5f,16403.6f,-64.5f,3.2f);

        // the player should be already there, but no :(
        // pTarget->GetPosition(&loc);

        WorldLocation loc;
        loc = WorldLocation(1, 16226.5f, 16403.6f, -64.5f, 3.2f);
        pTarget->SetHomebind(loc, 876);
        return true;
    }

    static bool HandleAntiCheatDeleteCommand(ChatHandler* handler, const char* args)
    {
        std::string strCommand;

        char* command = strtok((char*)args, " "); // get entered name

        if (!command)
        {
            return true;
        }

        strCommand = command;

        if (strCommand.compare("deleteall") == 0)
        {
            sAnticheatMgr->DeleteCommand();
        }
        else
        {
            normalizePlayerName(strCommand);
            Player* player = ObjectAccessor::FindPlayerByName(strCommand.c_str()); // get player by name
            if (!player)
            {
                handler->PSendSysMessage("Player doesn't exist");
            }
            else
            {
                sAnticheatMgr->DeleteCommand(player->GetGUID());
            }
        }

        return true;
    }
};

void AddSC_anticheat_commandscript()
{
    new anticheat_commandscript();
}
