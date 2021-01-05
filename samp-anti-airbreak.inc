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
        Float:__scy_ab_POS_X,
        Float:__scy_ab_POS_Y,
        Float:__scy_ab_POS_Z,
        __scy_ab__LAST_CALL,
        bool:__scy_ab__HANDLER_REGISTERED;

// Internal callbacks used with Pawn.RakNet's RegHandler
forward AntiAirbreak_OnFootSync(playerid, packetid, BitStream:bs);

// Callbacks
forward OnPlayerAirBreak(playerid);

public OnPlayerDisconnect(playerid, reason)
{
    __scy_ab_POS_X = 0.0, __scy_ab_POS_Y = 0.0, __scy_ab_POS_Z = 0.0;
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

#if defined FILTERSCRIPT
public OnFilterScriptInit()
#else
public OnGameModeInit()
#endif
{
    if(!__scy_ab__HANDLER_REGISTERED)
    {
        PR_RegHandler(209, "AntiAirbreak_OnFootSync", PR_INCOMING_PACKET);
        __scy_ab__HANDLER_REGISTERED = true;
    }

    #if defined FILTERSCRIPT
        #if defined AntiAirbreak_OnFilterScriptInit
            return AntiAirbreak_OnFilterScriptInit();
        #else
            return 1;
        #endif
    #else
        #if defined AntiAirbreak_OnGameModeInit
            return AntiAirbreak_OnGameModeInit();
        #else
            return 1;
        #endif
    #endif
}

#if defined FILTERSCRIPT
    #if defined _ALS_OnFilterScriptInit
        #undef OnFilterScriptInit
    #else
        #define _ALS_OnFilterScriptInit
    #endif
    #define OnFilterScriptInit AntiAirbreak_OnFilterScriptInit
    #if defined AntiAirbreak_OnFilterScriptInit
        forward AntiAirbreak_OnFilterScriptInit();
    #endif
#else
    #if defined _ALS_OnGameModeInit
        #undef OnGameModeInit
    #else
        #define _ALS_OnGameModeInit
    #endif
    #define OnGameModeInit AntiAirbreak_OnGameModeInit
    #if defined AntiAirbreak_OnGameModeInit
        forward AntiAirbreak_OnGameModeInit();
    #endif
#endif

public AntiAirbreak_OnFootSync(playerid, packetid, BitStream:bs)
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
        if(__scy_ab_POS_X != 0.0)
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

    return 1;
}