<div align="center"><img width="375" src="https://i.imgur.com/8LPkEym.png">
    <h4>samp-anti-airbreak</h4>
</div>
 

# Description
Simple include that uses <a href="https://github.com/katursis/Pawn.RakNet">Pawn.RakNet</a> to read OnFoot packets and determine if the player is using AirBreak using simple math.

# Usage
Just include the library to your script and...

```c
public OnPlayerAirBreak(playerid)
{
    // whatever you want to do when the player is cheating...
	return 1;
}
```
# Dependencies
* <a href="https://github.com/katursis/Pawn.RakNet/releases/tag/1.4.1">Pawn.RakNet v141</a>

# 