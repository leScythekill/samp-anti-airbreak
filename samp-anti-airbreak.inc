#if defined INC_SAMP_ANTIAIRBREAK
    #endinput
#endif

#define INC_SAMP_ANTIAIRBREAK

#tryinclude <Pawn.RakNet>

#if !defined PAWNRAKNET_INC_
    #error "[SAMP-ANTI-AIRBREAK] Pawn.RakNet is missing."
#endif

#if PAWNRAKNET_INCLUDE_VERSION < 141
    #error "[SAMP-ANTI-AIRBREAK] Please update Pawn.RakNet."
#endif

static 
        Float:__scy_ab_POS_X,
        Float:__scy_ab_POS_Y,
        Float:__scy_ab_POS_Z,
        __scy_ab__LAST_CALL;

public OnPlayerDisconnect(playerid)
{
    __scy_ab_POS_X = 0, __scy_ab_POS_Y = 0, __scy_ab_POS_Z = 0;
    #if defined AntiAirbreak_OnPlayerDisconnect
        return AntiAirbreak_OnPlayerDisconnect(playerid);
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
    forward AntiAirbreak_OnPlayerDisconnect(playerid);
#endif

public OnIncomingPacket(playerid, packetid, BitStream:bs)
{
    if (packetid == 207) // OnFootSync
    {

        new onFootData[PR_OnFootSync];
        BS_IgnoreBits(bs, 8);
        BS_ReadOnFootSync(bs, onFootData);

        new Float:player_speed;
        player_speed = floatsqroot(floatpower(floatabs(onFootData[PR_velocity][0]), 2.0) +
        floatpower(floatabs(onFootData[PR_velocity][1]), 2.0) +
        floatpower(floatabs(onFootData[PR_velocity][2]), 2.0)) * 179.28625; // Thanks to whoever made this

        if(player_speed < 27)
        {
            if(__scy_ab_POS_X != 0)
            {
                if(onFootData[PR_surfingVehicleId] == 0) // Hey there, having problems? Add some immunity to the player after setting his position here :)
                {
                    if(onFootData[PR_position][0] > __scy_ab_POS_X + 1.3 || onFootData[PR_position][1] > __scy_ab_POS_Y + 1.3 || onFootData[PR_position][2] > __scy_ab_POS_Z + 1.3
                    || onFootData[PR_position][0] < __scy_ab_POS_X - 1.3 || onFootData[PR_position][1] < __scy_ab_POS_Y - 1.3
                    || onFootData[PR_position][2] < __scy_ab_POS_Z - 1.3)
                    {
                        if((gettime() - __scy_ab__LAST_CALL) < 2 && __scy_ab__LAST_CALL != 0) OnPlayerAirBreak(playerid);
                        __scy_ab__LAST_CALL = gettime();
                    }
                }
            }
        }

        __scy_ab_POS_X = onFootData[PR_position][0];
        __scy_ab_POS_Y = onFootData[PR_position][1];
        __scy_ab_POS_Z = onFootData[PR_position][2];

    }
}

forward OnPlayerAirBreak(playerid);