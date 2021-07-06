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

#ifndef SC_ACMGR_H
#define SC_ACMGR_H

#include "AnticheatData.h"
#include "ObjectGuid.h"

class Player;
class Unit;

struct MovementInfo;

// GUID is the key.
typedef std::unordered_map<ObjectGuid, AnticheatData> AnticheatPlayersDataMap;

class AnticheatMgr
{
    AnticheatMgr();
    ~AnticheatMgr();

    public:
        static AnticheatMgr* instance();

        void SetExcludedMaps();
        void SetExcludedAreas();

        void HandlePlayerLoadFromDB(Player* player);
        void HandlePlayerLogout(Player* player);

        void DeleteCommand(ObjectGuid guid = ObjectGuid::Empty);

        void SetClientTimestamps(Player* player);
        void Update(Player* player, uint32 time);

        void SetSkipOnePacketForASH(Player* player, bool apply);
        void SetCanFlybyServer(Player* player, bool apply);
        void SetUnderACKmount(Player* player);
        void SetRootACKUpd(Player* player);
        void SetJumpingbyOpcode(Player* player, bool jump);

        void UpdateMovementInfo(Player* player, MovementInfo const& movementInfo);

        bool isMapDisabledForAC(uint32 mapid) const { return excludeACMapsId.count(mapid); }
        bool isAreaDisabledForAC(uint32 areaid) const { return excludeACAreasId.count(areaid); }

        bool HandleDoubleJump(Player* player, Unit* mover);
        bool CheckMovementInfo(Player* player, MovementInfo const& movementInfo, Unit* mover, bool jump);

        void ResetFallingData(Player* player);
        bool NoFallingDamage(Player* player, uint16 opcode);
        void HandleNoFallingDamage(Player* player, uint16 opcode);
        void SetSuccessfullyLanded(Player* player);

    private:
        AnticheatPlayersDataMap m_Players;

        std::unordered_set<uint32> excludeACMapsId;
        std::unordered_set<uint32> excludeACAreasId;
};

#define sAnticheatMgr AnticheatMgr::instance()

#endif
