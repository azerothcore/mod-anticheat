/*
 * Copyright (C) MaNGOS, TrinityCore, AzerothCore
 *
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 */

#ifndef __ANTICHEAT_ACORE_LANGUAGE_H
#define __ANTICHEAT_ACORE_LANGUAGE_H

enum AnticheatAcoreStrings
{
    LANG_GM_ANNOUNCE_ASH                    = 30087,  // AntiSpeedHack
    LANG_GM_ANNOUNCE_AFH_CANFLYWRONG        = 30088,  // AntiFlyHack - flying without canfly
    LANG_GM_ANNOUNCE_AFK_SWIMMING           = 30089,  // AntiFlyHack - flying swimming not in water
    LANG_GM_ANNOUNCE_AFH                    = 30090,  // AntiFlyHack
    LANG_GM_ANNOUNCE_DOUBLE_JUMP            = 30091,  // Double jump (client can't sent second packet of jump (only hack))
    LANG_GM_ANNOUNCE_JUMPER_FAKE            = 30092,  // Gagarin and others can set falling flag and move up as jump
    LANG_GM_ANNOUNCE_JUMPER_FLYING          = 30093,  // Hitchhiker's Hack and others can set fly unrestricted flag
    LANG_GM_ANNOUNCE_WALLCLIMB              = 30094,  // WallClimb
    LANG_GM_ANNOUNCE_MOVE_UNDER_CONTROL     = 30095,  // Movement under Controll (not restricted)
    LANG_GM_ANNOUNCE_NOFALLINGDMG           = 30096   // Falling without opcode of land/swim (not restricted)
};

#endif
