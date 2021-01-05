#if defined INC_SAMP_ANTIAIRBREAK
    #endinput
#endif
#define INC_SAMP_ANTIAIRBREAK

#include <a_samp>
#tryinclude <Pawn.RakNet>

#if !defined PAWNRAKNET_INC_
    #error "[SAMP-ANTI-AIRBREAK] Pawn.RakNet is missing. You can download it from https://github.com/katursis/Pawn.RakNet"
#endif

static
        Float:__scy_ab_POS_X[MAX_PLAYERS],
        Float:__scy_ab_POS_Y[MAX_PLAYERS],
        Float:__scy_ab_POS_Z[MAX_PLAYERS],
        __scy_ab__LAST_CALL[MAX_PLAYERS];

// Callbacks
forward OnPlayerAirBreak(playerid);

public OnPlayerDisconnect(playerid, reason)
{
    __scy_ab_POS_X[playerid] =
    __scy_ab_POS_Y[playerid] =
    __scy_ab_POS_Z[playerid] = 0.0;
    __scy_ab__LAST_CALL[playerid] = 0;

    #if defined AntiAirbreak_OnPlayerDisconnect
        return AntiAirbreak_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect AntiAirbreak_OnPlayerDisconnect
#if defined AntiAirbreak_OnPlayerDisconnect
    forward AntiAirbreak_OnPlayerDisconnect(playerid, reason);
#endif

public OnIncomingPacket(playerid, packetid, BitStream:bs)
{
    if (packetid == 207)
    {
        new onFootData[PR_OnFootSync];
        BS_IgnoreBits(bs, 8);
        BS_ReadOnFootSync(bs, onFootData);

        new Float:player_speed;
        player_speed = floatsqroot(floatpower(floatabs(onFootData[PR_velocity][0]), 2.0) +
        floatpower(floatabs(onFootData[PR_velocity][1]), 2.0) +
        floatpower(floatabs(onFootData[PR_velocity][2]), 2.0)) * 179.28625; // Thanks to whoever made this

        if(player_speed < 27.0)
        {
            if(__scy_ab_POS_X[playerid] != 0.0)
            {
                if(onFootData[PR_surfingVehicleId] == 0) // Hey there, having problems? Add some immunity to the player after setting his position here :)
                {
                    if(onFootData[PR_position][0] > __scy_ab_POS_X[playerid] + 1.3 || onFootData[PR_position][1] > __scy_ab_POS_Y[playerid] + 1.3 || onFootData[PR_position][2] > __scy_ab_POS_Z[playerid] + 1.3
                    || onFootData[PR_position][0] < __scy_ab_POS_X[playerid] - 1.3 || onFootData[PR_position][1] < __scy_ab_POS_Y[playerid] - 1.3
                    || onFootData[PR_position][2] < __scy_ab_POS_Z[playerid] - 1.3)
                    {
                        if((gettime() - __scy_ab__LAST_CALL[playerid]) < 2 && __scy_ab__LAST_CALL[playerid] != 0)
                            CallLocalFunction("OnPlayerAirBreak", "i", playerid);

                        __scy_ab__LAST_CALL[playerid] = gettime();
                    }
                }
            }
        }

        __scy_ab_POS_X[playerid] = onFootData[PR_position][0];
        __scy_ab_POS_Y[playerid] = onFootData[PR_position][1];
        __scy_ab_POS_Z[playerid] = onFootData[PR_position][2];
    }
    
    #if defined __scy_ab_OnIncomingPacket
        return __scy_ab_OnIncomingPacket(playerid, packetid, bs);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnIncomingPacket
    #undef OnIncomingPacket
#else
    #define _ALS_OnIncomingPacket
#endif

#define OnIncomingPacket __scy_ab_OnIncomingPacket

#if defined __scy_ab_OnIncomingPacket
    forward __scy_ab_OnIncomingPacket(playerid, packetid, BitStream:bs);
#endif