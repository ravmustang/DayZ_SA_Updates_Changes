
/** @file */

// -------------------------------------------------------------------------
class Hive
{
	proto native void InitOnline( string ceSetup, string host = "" );
	proto native void InitOffline();
	proto native void InitSandbox();

	proto native void SetShardID( string shard );
	proto native void SetEnviroment( string env );

	proto native void CharacterSave( Man player );
	proto native void CharacterKill( Man player );
	proto native void CharacterExit( Man player );

	proto native void CallUpdater( string content );

};

proto native Hive CreateHive();
proto native void DestroyHive();
proto native Hive GetHive();
