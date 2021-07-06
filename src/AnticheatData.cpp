#include "AnticheatData.h"
#include "AnticheatMgr.h"
#include "AnticheatLanguage.h"
#include "Config.h"
#include "Player.h"
#include "Vehicle.h"
#include "Realm.h"

#define DEFAULT_PLAYER_BOUNDING_RADIUS      0.388999998569489f     // player size, also currently used (correctly?) for any non Unit world objects

AnticheatData::AnticheatData(Player* player /* =nullptr*/) : m_owner(player)
{
    m_flyhackTimer = 0;
    if (sConfigMgr->GetOption<bool>("AntiCheats.FlyHack.Enabled", true))
    {
        m_flyhackTimer = sConfigMgr->GetOption<int32>("AntiCheats.FlyHackTimer", 1000);
    }

    m_mountTimer = 0;
    m_rootUpdTimer = 0;
    m_antiNoFallDmgTimer = 0;
    m_ACKmounted = false;
    m_rootUpd = false;
    m_skipOnePacketForASH = true;
    m_isjumping = false;
    m_canfly = false;
    m_antiNoFallDmg = false;
    m_antiNoFallDmgLastChance = false;
    m_walking = false;
    lastMoveClientTimestamp = 0;
    lastMoveServerTimestamp = 0;
}

AnticheatData::~AnticheatData()
{
}

void AnticheatData::Update(uint32 time)
{
    if (m_flyhackTimer > 0)
    {
        if (time >= m_flyhackTimer)
        {
            if (!CheckOnFlyHack() && sConfigMgr->GetOption<bool>("AntiCheats.FlyHack.Kick.Enabled", true))
            {
                m_owner->GetSession()->KickPlayer();
            }

            m_flyhackTimer = sConfigMgr->GetOption<int32>("AntiCheats.FlyHackTimer", 1000);
        }
        else
        {
            m_flyhackTimer -= time;
        }
    }

    if (m_ACKmounted && m_mountTimer > 0)
    {
        if (time >= m_mountTimer)
        {
            m_mountTimer = 0;
            m_ACKmounted = false;
        }
        else
        {
            m_mountTimer -= time;
        }
    }

    if (m_rootUpd && m_rootUpdTimer > 0)
    {
        if (time >= m_rootUpdTimer)
        {
            m_rootUpdTimer = 0;
            m_rootUpd = false;
        }
        else
        {
            m_rootUpdTimer -= time;
        }
    }

    if (m_antiNoFallDmg && m_antiNoFallDmgTimer > 0)
    {
        if (time >= m_antiNoFallDmgTimer)
        {
            m_antiNoFallDmgTimer = 0;
            m_antiNoFallDmg = false;
            m_antiNoFallDmgLastChance = true;
        }
        else
            m_antiNoFallDmgTimer -= time;
    }
}

void AnticheatData::SetUnderACKmount()
{
    m_mountTimer = 3000;
    m_ACKmounted = true;
}

void AnticheatData::SetRootACKUpd()
{
    float fabscount = fabs(float(GetLastMoveClientTimestamp()) - float(GetLastMoveServerTimestamp()));
    uint32 pinginthismoment = uint32(fabscount) / 1000000;
    m_rootUpdTimer = 1500 + pinginthismoment;
    m_rootUpd = true;
}

bool AnticheatData::CheckOnFlyHack()
{
    if (IsCanFlybyServer())
    {
        return true;
    }

    if ((m_owner->movespline->Initialized() && !m_owner->movespline->Finalized()) || m_owner->ToUnit()->IsFalling() || m_owner->IsFalling())
    {
        return true;
    }

    if (m_owner->m_mover != m_owner)
    {
        return true;
    }

    if (sAnticheatMgr->isMapDisabledForAC(m_owner->GetMapId()) || sAnticheatMgr->isAreaDisabledForAC(m_owner->GetAreaId()))
    {
        return true;
    }

    if (m_owner->IsFlying() && !m_owner->CanFly()) // kick flyhacks
    {
        LOG_INFO("anticheat", "PassiveAnticheat: FlyHack Detected for Account id : %u, Player %s (%s), Map: %d, Position: %s, MovementFlags: %d",
            m_owner->GetSession()->GetAccountId(), m_owner->GetName().c_str(), m_owner->GetGUID().ToString().c_str(), m_owner->GetMapId(),
            m_owner->GetPosition().ToString().c_str(), m_owner->GetUnitMovementFlags());
        LOG_INFO("anticheat", "Player::========================================================");
        LOG_INFO("anticheat", "Player IsFlying but CanFly is false");

        sWorld->SendGMText(LANG_GM_ANNOUNCE_AFH_CANFLYWRONG, m_owner->GetName().c_str());
        RecordAntiCheatLog(GetDescriptionACForLogs(1));
        return false;
    }

    if (m_owner->IsFlying() || m_owner->IsLevitating() || m_owner->IsInFlight())
    {
        return true;
    }

    if (m_owner->GetVehicle() || m_owner->GetVehicleKit())
    {
        return true;
    }

    if (m_owner->HasAuraType(SPELL_AURA_CONTROL_VEHICLE))
    {
        return true;
    }

    if (m_owner->GetTransport() || m_owner->HasUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT))
    {
        return true;
    }

    if (m_owner->HasUnitState(UNIT_STATE_IGNORE_ANTISPEEDHACK))
    {
        return true;
    }

    if (UnderACKmount())
    {
        return true;
    }

    if (IsSkipOnePacketForASH())
    {
        return true;
    }

    Position npos = m_owner->GetPosition();
    float pz = npos.GetPositionZ();
    if (!m_owner->IsInWater() && m_owner->HasUnitMovementFlag(MOVEMENTFLAG_SWIMMING))
    {
        LiquidData liquid_status;
        m_owner->GetMap()->getLiquidStatus(npos.GetPositionX(), npos.GetPositionY(), pz, MAP_ALL_LIQUIDS, &liquid_status, m_owner->GetCollisionHeight());

        float waterlevel = liquid_status.level; // water walking
        bool hovergaura = m_owner->HasAuraType(SPELL_AURA_WATER_WALK) || m_owner->HasAuraType(SPELL_AURA_HOVER);
        if (waterlevel > INVALID_HEIGHT && (pz - waterlevel) <= (hovergaura ? m_owner->GetCollisionHeight() + 1.5f : m_owner->GetCollisionHeight() + 1.5f))
        {
            return true;
        }

        LOG_INFO("anticheat", "PassiveAnticheat: FlyHack Detected for Account id : %u, Player %s (%s), Map: %d, Position: %s, MovementFlags: %d",
            m_owner->GetSession()->GetAccountId(), m_owner->GetName().c_str(), m_owner->GetGUID().ToString().c_str(), m_owner->GetMapId(),
            m_owner->GetPosition().ToString().c_str(), m_owner->GetUnitMovementFlags());
        LOG_INFO("anticheat", "Player::========================================================");
        LOG_INFO("anticheat", "Player has a MOVEMENTFLAG_SWIMMING, but not in water");

        sWorld->SendGMText(LANG_GM_ANNOUNCE_AFK_SWIMMING, m_owner->GetName().c_str());
        RecordAntiCheatLog(GetDescriptionACForLogs(2));
        return false;
    }
    else if (!m_owner->HasUnitMovementFlag(MOVEMENTFLAG_SWIMMING))
    {
        float z = m_owner->GetMap()->GetHeight(m_owner->GetPhaseMask(), npos.GetPositionX(), npos.GetPositionY(), pz + m_owner->GetCollisionHeight() + 0.5f, true, 50.0f); // smart flyhacks -> SimpleFly
        if (z > INVALID_HEIGHT)
        {
            float diff = pz - z;
            if (diff > 6.8f) // better calculate the second time for false situations, but not call GetHoverOffset everytime (economy resource)
            {
                LiquidData liquid_status;
                m_owner->GetMap()->getLiquidStatus(npos.GetPositionX(), npos.GetPositionY(), pz, MAP_ALL_LIQUIDS, &liquid_status, m_owner->GetCollisionHeight());

                float waterlevel = liquid_status.level; // water walking
                if (waterlevel > INVALID_HEIGHT && waterlevel + m_owner->GetCollisionHeight() > pz)
                {
                    return true;
                }

                float cx, cy, cz;
                m_owner->GetTheClosestPoint(cx, cy, cz, 0.5, pz, 6.8f); // first check
                if (pz - cz > 6.8f)
                {
                    m_owner->GetMap()->GetObjectHitPos(m_owner->GetPhaseMask(), m_owner->GetPositionX(), m_owner->GetPositionY(),
                        m_owner->GetPositionZ() + m_owner->GetCollisionHeight(), cx, cy, cz + m_owner->GetCollisionHeight(), cx, cy, cz, -m_owner->GetCollisionHeight());
                    if (pz - cz > 6.8f)
                    {
                        LOG_INFO("anticheat", "PassiveAnticheat: FlyHack Detected for Account id : %u, Player %s (%s), Map: %d, Position: %s, MovementFlags: %d",
                            m_owner->GetSession()->GetAccountId(), m_owner->GetName().c_str(), m_owner->GetGUID().ToString().c_str(), m_owner->GetMapId(),
                            m_owner->GetPosition().ToString().c_str(), m_owner->GetUnitMovementFlags());
                        LOG_INFO("anticheat", "Player::========================================================");
                        LOG_INFO("anticheat", "playerZ = %f", pz);
                        LOG_INFO("anticheat", "normalZ = %f", z);
                        LOG_INFO("anticheat", "checkz = %f", cz);
                        LOG_INFO("anticheat", "========================================================");
                        sWorld->SendGMText(LANG_GM_ANNOUNCE_AFH, m_owner->GetName().c_str());
                        RecordAntiCheatLog(GetDescriptionACForLogs(3, pz, z));
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

void AnticheatData::UpdateMovementInfo(MovementInfo const& movementInfo)
{
    SetLastMoveClientTimestamp(movementInfo.time);
    SetLastMoveServerTimestamp(World::GetGameTimeMS());
}

void AnticheatData::StartWaitingLandOrSwimOpcode()
{
    m_antiNoFallDmgTimer = 3000;
    m_antiNoFallDmg = true;
}

bool AnticheatData::CheckMovementInfo(MovementInfo const& movementInfo, Unit* mover, bool jump)
{
    if (CheckMovement(movementInfo, mover, jump))
    {
        UpdateMovementInfo(movementInfo);
        return true;
    }

    return false;
}

bool AnticheatData::CheckMovement(MovementInfo const& movementInfo, Unit* mover, bool jump)
{
    if (m_owner->movespline->Initialized() && !m_owner->movespline->Finalized())
    {
        return true;
    }

    if (sAnticheatMgr->isMapDisabledForAC(m_owner->GetMapId()) || sAnticheatMgr->isAreaDisabledForAC(m_owner->GetAreaId()))
    {
        return true;
    }

    if (sConfigMgr->GetOption<bool>("AntiCheats.FakeJumper.Enabled", true) && mover->IsFalling() && movementInfo.pos.GetPositionZ() > mover->GetPositionZ())
    {
        if (!IsJumpingbyOpcode() && !UnderACKmount() && !m_owner->IsFlying())
        {
            // fake jumper -> for example gagarin air mode with falling flag (like player jumping), but client can't sent a new coords when falling
            LOG_INFO("anticheat", "PassiveAnticheat: Fake jumper by Account id : %u, Player %s (%s), Map: %d, Position: %s, MovementFlags: %d",
                m_owner->GetSession()->GetAccountId(), m_owner->GetName().c_str(), m_owner->GetGUID().ToString().c_str(), m_owner->GetMapId(),
                m_owner->GetPosition().ToString().c_str(), m_owner->GetUnitMovementFlags());
            sWorld->SendGMText(LANG_GM_ANNOUNCE_JUMPER_FAKE, m_owner->GetName().c_str());
            RecordAntiCheatLog(GetDescriptionACForLogs(7));
            if (sConfigMgr->GetOption<bool>("AntiCheats.FakeJumper.Kick.Enabled", true))
            {
                return false;
            }
        }
    }

    if (sConfigMgr->GetOption<bool>("AntiCheats.FakeFlyingmode.Enabled", true) && !IsCanFlybyServer() && !UnderACKmount() &&
        movementInfo.HasMovementFlag(MOVEMENTFLAG_MASK_MOVING_FLY) && !m_owner->IsInWater())
    {
        LOG_INFO("anticheat", "PassiveAnticheat: Fake flying mode (using MOVEMENTFLAG_FLYING flag doesn't restricted) by Account id : %u, Player %s (%s), Map: %d, Position: %s, MovementFlags: %d",
            m_owner->GetSession()->GetAccountId(), m_owner->GetName().c_str(), m_owner->GetGUID().ToString().c_str(), m_owner->GetMapId(),
            m_owner->GetPosition().ToString().c_str(), m_owner->GetUnitMovementFlags());
        sWorld->SendGMText(LANG_GM_ANNOUNCE_JUMPER_FLYING, m_owner->GetName().c_str());
        RecordAntiCheatLog(GetDescriptionACForLogs(8));
        if (sConfigMgr->GetOption<bool>("AntiCheats.FakeFlyingmode.Kick.Enabled", true))
        {
            return false;
        }
    }

    if (!sConfigMgr->GetOption<bool>("AntiCheats.SpeedHack.Enabled", true))
    {
        return true;
    }

    uint32 oldctime = GetLastMoveClientTimestamp();
    if (oldctime)
    {
        if (m_owner->ToUnit()->IsFalling() || m_owner->IsInFlight())
        {
            return true;
        }

        bool vehicle = false;
        if (m_owner->GetVehicleKit() && m_owner->GetVehicleKit()->GetBase())
            vehicle = true;

        if (m_owner->GetVehicle())
        {
            return true;
        }

        if (m_owner->m_mover != m_owner)
        {
            return true;
        }

        if (!m_owner->IsControlledByPlayer())
        {
            return true;
        }

        if (m_owner->HasUnitState(UNIT_STATE_IGNORE_ANTISPEEDHACK))
        {
            return true;
        }

        if (IsSkipOnePacketForASH())
        {
            SetSkipOnePacketForASH(false);
            return true;
        }

        bool transportflag = (movementInfo.GetMovementFlags() & MOVEMENTFLAG_ONTRANSPORT) || m_owner->HasUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT);
        float x, y, z;
        Position npos;

        // Position coords for new point
        if (!transportflag)
            npos = movementInfo.pos;
        else
            npos = movementInfo.transport.pos;

        // Position coords for previous point (old)
        // Just CheckMovementInfo are calling before player change UnitMovementFlag MOVEMENTFLAG_ONTRANSPORT
        if (transportflag)
        {
            if (m_owner->GetTransOffsetX() == 0.f) // if it elevator or fist step - player can have zero this coord
                return true;

            x = m_owner->GetTransOffsetX();
            y = m_owner->GetTransOffsetY();
            z = m_owner->GetTransOffsetZ();
        }
        else
            m_owner->GetPosition(x, y, z);

        if (sConfigMgr->GetOption<bool>("AntiCheats.IgnoreControlMovement.Enabled", true))
        {
            if (m_owner->HasUnitState(UNIT_STATE_ROOT) && !UnderACKRootUpd())
            {
                bool unrestricted = npos.GetPositionX() != x || npos.GetPositionY() != y;
                if (unrestricted)
                {
                    LOG_INFO("anticheat", "CheckMovementInfo :  Ignore control Hack detected for Account id : %u, Player %s (%s), Position: %s, MovementFlags: %d",
                        m_owner->GetSession()->GetAccountId(), m_owner->GetName().c_str(), m_owner->GetGUID().ToString().c_str(),
                        m_owner->GetPosition().ToString().c_str(), m_owner->GetUnitMovementFlags());
                    sWorld->SendGMText(LANG_GM_ANNOUNCE_MOVE_UNDER_CONTROL, m_owner->GetSession()->GetAccountId(), m_owner->GetName().c_str());
                    RecordAntiCheatLog(GetDescriptionACForLogs(4));

                    if (sConfigMgr->GetOption<bool>("AntiCheats.MoveUnderControl.Kick.Enabled", true))
                    {
                        return false;
                    }
                }
            }
        }

        float flyspeed = 0.f;
        float distance, runspeed, difftime, normaldistance, delay, diffPacketdelay;
        uint32 ptime;
        std::string mapname = m_owner->GetMap()->GetMapName();

        // calculate distance - don't use func, because x,z can be offset transport coords
        distance = sqrt((npos.GetPositionY() - y) * (npos.GetPositionY() - y) + (npos.GetPositionX() - x) * (npos.GetPositionX() - x));

        if (!jump && !m_owner->CanFly() && !m_owner->isSwimming() && !transportflag && distance > 0.f)
        {
            float diffz = fabs(movementInfo.pos.GetPositionZ() - z);
            float tanangle = distance / diffz;

            if (movementInfo.pos.GetPositionZ() > z && diffz > 1.87f && tanangle < 0.57735026919f) // 30 degrees
            {
                LOG_INFO("anticheat", "PassiveAnticheat: Climb Hack detected for Account id : %u, Player %s (%s), Map: %d, Position: %s, MovementFlags: %d",
                    m_owner->GetSession()->GetAccountId(), m_owner->GetName().c_str(), m_owner->GetGUID().ToString().c_str(), m_owner->GetMapId(),
                    m_owner->GetPosition().ToString().c_str(), m_owner->GetUnitMovementFlags());

                sWorld->SendGMText(LANG_GM_ANNOUNCE_WALLCLIMB, m_owner->GetSession()->GetAccountId(), m_owner->GetName().c_str(), diffz, distance, tanangle, mapname.c_str(), m_owner->GetMapId(), x, y, z);
                RecordAntiCheatLog(GetDescriptionACForLogs(5, diffz, distance));

                if (sConfigMgr->GetOption<bool>("AntiCheats.Wallclimb.Kick.Enabled", true))
                {
                    return false;
                }
            }
        }

        uint32 oldstime = GetLastMoveServerTimestamp();
        uint32 stime = World::GetGameTimeMS();
        uint32 ping;
        ptime = movementInfo.time;

        if (!vehicle)
            runspeed = m_owner->GetSpeed(MOVE_RUN);
        else
            runspeed = m_owner->GetVehicleKit()->GetBase()->GetSpeed(MOVE_RUN);

        if (m_owner->isSwimming())
        {
            if (!vehicle)
                runspeed = m_owner->GetSpeed(MOVE_SWIM);
            else
                runspeed = m_owner->GetVehicleKit()->GetBase()->GetSpeed(MOVE_SWIM);
        }

        if (m_owner->IsFlying() || m_owner->CanFly())
        {
            if (!vehicle)
                flyspeed = m_owner->GetSpeed(MOVE_FLIGHT);
            else
                flyspeed = m_owner->GetVehicleKit()->GetBase()->GetSpeed(MOVE_FLIGHT);
        }

        if (flyspeed > runspeed)
            runspeed = flyspeed;

        delay = ptime - oldctime;
        diffPacketdelay = 10000000 - delay;

        if (oldctime > ptime)
        {
            LOG_INFO("anticheat", "oldctime > ptime");
            delay = 0;
        }
        diffPacketdelay = diffPacketdelay * 0.0000000001f;

        difftime = delay * 0.001f + diffPacketdelay;

        // if movetime faked and lower, difftime should be with "-"
        normaldistance = (runspeed * difftime) + 0.002f; // 0.002f a little safe temporary hack
        if (UnderACKmount())
            normaldistance += 20.0f;

        if (normaldistance - distance >= -1.5f)
            return true;

        ping = uint32(diffPacketdelay * 10000.f);

        LOG_INFO("anticheat", "PassiveAnticheat: SpeedHack Detected for Account id : %u, Player %s (%s), Map: %d, Position: %s, MovementFlags: %d",
            m_owner->GetSession()->GetAccountId(), m_owner->GetName().c_str(), m_owner->GetGUID().ToString().c_str(), m_owner->GetMapId(),
            m_owner->GetPosition().ToString().c_str(), m_owner->GetUnitMovementFlags());
        LOG_INFO("anticheat", "========================================================");
        LOG_INFO("anticheat", "oldX = %f", x);
        LOG_INFO("anticheat", "oldY = %f", y);
        LOG_INFO("anticheat", "newX = %f", npos.GetPositionX());
        LOG_INFO("anticheat", "newY = %f", npos.GetPositionY());
        LOG_INFO("anticheat", "packetdistance = %f", distance);
        LOG_INFO("anticheat", "available distance = %f", normaldistance);
        LOG_INFO("anticheat", "oldStime = %u", oldstime);
        LOG_INFO("anticheat", "oldCtime = %u", oldctime);
        LOG_INFO("anticheat", "serverTime = %u", stime);
        LOG_INFO("anticheat", "packetTime = %u", ptime);
        LOG_INFO("anticheat", "diff delay between old ptk and current pkt = %f", diffPacketdelay);
        LOG_INFO("anticheat", "FullDelay = %f", delay / 1000.f);
        LOG_INFO("anticheat", "difftime = %f", difftime);
        LOG_INFO("anticheat", "ping = %u", ping);
        LOG_INFO("anticheat", "========================================================");

        sWorld->SendGMText(LANG_GM_ANNOUNCE_ASH, m_owner->GetName().c_str(), normaldistance, distance);
        RecordAntiCheatLog(GetDescriptionACForLogs(0, distance, normaldistance));
    }
    else
    {
        return true;
    }

    if (sConfigMgr->GetOption<bool>("AntiCheats.SpeedHack.Kick.Enabled", true))
    {
        return false;
    }

    if (!HasWalkingFlag() && movementInfo.HasMovementFlag(MOVEMENTFLAG_WALKING))
    {
        SetWalkingFlag(true);
    }

    if (HasWalkingFlag() && !movementInfo.HasMovementFlag(MOVEMENTFLAG_WALKING))
    {
        SetWalkingFlag(false);
    }

    return true;
}

bool AnticheatData::HandleDoubleJump(Unit* mover)
{
    SetJumpingbyOpcode(true);
    SetUnderACKmount();

    if (sAnticheatMgr->isMapDisabledForAC(m_owner->GetMapId()) || sAnticheatMgr->isAreaDisabledForAC(m_owner->GetAreaId()))
    {
        return true;
    }

    if (mover->IsFalling())
    {
        LOG_INFO("anticheat", "PassiveAnticheat: Double jump by Account id : %u, Player %s (%s), Map: %d, Position: %s, MovementFlags: %d",
            m_owner->GetSession()->GetAccountId(), m_owner->GetName().c_str(), m_owner->GetGUID().ToString().c_str(),
            m_owner->GetPosition().ToString().c_str(), m_owner->GetUnitMovementFlags());

        sWorld->SendGMText(LANG_GM_ANNOUNCE_DOUBLE_JUMP, m_owner->GetName().c_str());

        RecordAntiCheatLog(GetDescriptionACForLogs(6));

        if (sConfigMgr->GetOption<bool>("AntiCheats.DoubleJump.Enabled", true))
        {
            return false;
        }
    }

    return true;
}

void AnticheatData::ResetFallingData()
{
    if (IsWaitingLandOrSwimOpcode())
        m_antiNoFallDmg = false;

    if (IsUnderLastChanceForLandOrSwimOpcode())
        m_antiNoFallDmgLastChance = false;
}

bool AnticheatData::NoFallingDamage(uint16 opcode)
{
    if (IsUnderLastChanceForLandOrSwimOpcode())
    {
        if (sAnticheatMgr->isMapDisabledForAC(m_owner->GetMapId()) || sAnticheatMgr->isAreaDisabledForAC(m_owner->GetAreaId()))
        {
            SetSuccessfullyLanded();
            return true;
        }

        bool checkNorm = false;
        switch (opcode)
        {
            case MSG_MOVE_FALL_LAND:
            case MSG_MOVE_START_SWIM:
                checkNorm = true;
                break;
        }

        if (IsCanFlybyServer())
            checkNorm = true;

        if (!checkNorm)
        {
            LOG_INFO("anticheat", "PassiveAnticheat: NoFallingDamage by Account id : %u, Player %s (%s), Map: %d, Position: %s, MovementFlags: %d",
                m_owner->GetSession()->GetAccountId(), m_owner->GetName().c_str(), m_owner->GetGUID().ToString().c_str(),
                m_owner->GetMapId(), m_owner->GetPosition().ToString().c_str(), m_owner->GetUnitMovementFlags());

            sWorld->SendGMText(LANG_GM_ANNOUNCE_NOFALLINGDMG, m_owner->GetSession()->GetAccountId(), m_owner->GetName().c_str());

            RecordAntiCheatLog(GetDescriptionACForLogs(9));

            if (sConfigMgr->GetOption<bool>("AntiCheats.NoFallingDmg.Kick.Enabled", false))
            {
                m_owner->GetSession()->KickPlayer("Kicked by anticheat::NoFallingDamage");
                return false;
            }
        }
        else
            SetSuccessfullyLanded();
    }

    return true;
}

void AnticheatData::RecordAntiCheatLog(std::string const& description)
{
    LoginDatabase.PExecute("INSERT INTO anticheat_logs (account, player, description, position, realmId) VALUES (%d, '%s', '%s', '%s', %d)",
        m_owner->GetSession()->GetAccountId(), m_owner->GetName().c_str(), description.c_str(), GetPositionACForLogs().c_str(), int32(realm.Id.Realm));
}

void AnticheatData::HandleNoFallingDamage(uint16 opcode)
{
    if (!IsCanFlybyServer())
    {
        bool checkNorm = false;
        switch (opcode)
        {
            case MSG_MOVE_FALL_LAND:
            case MSG_MOVE_START_SWIM:
                checkNorm = true;
                break;
        }

        if (!checkNorm && !IsWaitingLandOrSwimOpcode())
            StartWaitingLandOrSwimOpcode();
    }
}

std::string AnticheatData::GetDescriptionACForLogs(uint8 type, float param1, float param2) const
{
    std::ostringstream str;

    switch (type)
    {
        case 0: // ASH
        {
            str << "AntiSpeedHack: distance from packet = " << param1 << ", available distance = " << param2;
            break;
        }
        case 1: // AFH - IsFlying but CanFly is false
        {
            str << "AntiFlyHack: Player IsFlying but CanFly is false";
            break;
        }
        case 2: // AFH - Player has a MOVEMENTFLAG_SWIMMING, but not in water
        {
            str << "AntiFlyHack: Player has a MOVEMENTFLAG_SWIMMING, but not in water";
            break;
        }
        case 3: // AFH - just z checks (smaughack)
        {
            str << "AntiFlyHack: Player::CheckOnFlyHack : playerZ = " << param1 << ", but normalZ = " << param2;
            break;
        }
        case 4: // Ignore control Hack
        {
            str << "Ignore controll Hack detected";
            break;
        }
        case 5: // Climb-Hack
        {
            str << "Climb-Hack detected , diffZ = " << param1 << ", distance = " << param2;
            break;
        }
        case 6: // doublejumper
        {
            str << "Double-jump detected";
            break;
        }
        case 7: // fakejumper
        {
            str << "FakeJumper detected";
            break;
        }
        case 8: // fakeflying
        {
            str << "FakeFlying mode detected";
            break;
        }
        case 9: // NoFallingDmg
        {
            str << "NoFallingDamage mode detected";
            break;
        }
        default:
            break;
    }

    return str.str();
}

std::string AnticheatData::GetPositionACForLogs() const
{
    uint32 areaId = m_owner->GetAreaId();
    std::string areaName = "Unknown";
    std::string zoneName = "Unknown";
    if (AreaTableEntry const* area = sAreaTableStore.LookupEntry(areaId))
    {
        int locale = m_owner->GetSession()->GetSessionDbcLocale();
        areaName = area->area_name[locale];
        if (AreaTableEntry const* zone = sAreaTableStore.LookupEntry(area->zone))
            zoneName = zone->area_name[locale];
    }

    std::ostringstream str;
    str << "Map: " << m_owner->GetMapId() << " (" << (m_owner->FindMap() ? m_owner->FindMap()->GetMapName() : "Unknown") << ") Area: " << areaId << " (" << areaName.c_str() << ") Zone: " << zoneName.c_str() << " XYZ: " << m_owner->GetPositionX() << " " << m_owner->GetPositionY() << " " << m_owner->GetPositionZ();
    return str.str();
}
