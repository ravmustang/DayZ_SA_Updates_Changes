class ScriptAnalytics
{
	static proto native void PlayerDeath(string characterId, int characterLifetime, string cause, string weaponName, float distance, vector position);
	static proto native void PlayerScoredKill(string characterId, string weaponName, int killDistance, vector positionKiller, vector positionVictim);
	static proto native void PlayerDisconnected(string characterId, string reason);
	static proto native void PlayerMeasures(string characterId, int timeInterval, int daytimeHour, vector positionStart, vector positionEnd, float distanceOnFoot);
};
