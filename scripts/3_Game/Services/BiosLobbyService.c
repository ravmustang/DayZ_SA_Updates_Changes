// Script File

enum ESortType
{
	HOST,		
	TIME,
	POPULATION,
	SLOTS,
	PING
	FAVORITE,
	PASSWORDED
};

enum ESortOrder
{
	ASCENDING,		
	DESCENDING,		
};


//! GetServersResultRow the output structure of the GetServers operation that represents one game server
class GetServersResultRow
{
	string	m_Id;
	int		m_Priority;
	string	m_Name;
	string	m_Description;
	string	m_HostIp;
	int		m_HostPort;
	bool	m_Invisible;
	bool	m_Official;
	bool	m_Modded;
	int		m_ModeId;
	bool	m_AntiCheat;
	int		m_RegionId;
	int		m_MinPlayers;
	int		m_MaxPlayers;
	int		m_FreeSlots;
	int		m_CurrentNumberPlayers;
	string	m_GameVersion;
	bool	m_IsPasswordProtected;
	string	m_CreatedAt;
	string	m_UpdatedAt;

};


typedef array<ref GetServersResultRow> GetServersResultRowArray;

class GetFirstServerWithEmptySlotResult
{
	GetServersResult m_Result;
};

//! GetServersInput the input structure of the GetServers operation
class GetFirstServerWithEmptySlotInput
{
	bool		m_Official;
	string		m_GameVersion;
	int			m_RegionId;

	bool		m_UseOfficial;
	bool		m_UseGameVersion;
	bool		m_UseRegionId;
		
	void SetOfficial( bool Official )
	{
		m_Official					= Official;
		m_UseOfficial				= true;
	}
	
	void SetGameVersion( string GameVersion )
	{
		m_GameVersion				= GameVersion;
		m_UseGameVersion			= true;
	}
	
	void SetRegionId( int RegionId )
	{
		m_RegionId					= RegionId;
		m_UseRegionId				= true;
	}
};

//! GetServersResult the output structure of the GetServers operation
class GetServersResult
{
	int								m_Page;
	int								m_Pages;
	ref GetServersResultRowArray	m_Results;
};

//! GetServersInput the input structure of the GetServers operation
class GetServersInput
{
	bool		m_AntiCheat;
	int			m_RowsPerPage;
	string		m_SortBy; //na of property
	ESortOrder	m_SortOrder;
	string		m_Name;
	string		m_GameType;
	int			m_Platform; // 1 - PC, 2 - xbox, 3 - PSN
	int			m_ModeId;
	int			m_Page;
	string		m_GameVersion;
	bool		m_Official;
	bool		m_Joinable;
	bool		m_IsModded;
	int			m_MinPlayers; // minimum 1
	int			m_MaxPlayers; // maximum 99
	bool		m_IsPasswordProtected;
	int			m_RegionId; // 1 - Americas, 2 - Europe, 3 - Asia
	int			m_Priority;
	int 		m_FreeSlotsMin;
	int 		m_FreeSlotsMax;
	string		m_HostIp;
	int			m_HostPort;

	bool m_UseAntiCheat;
	bool m_UseName;
	bool m_UseGameType;
	bool m_UseModeId;
	bool m_UseGameVersion;
	bool m_UseOfficial;
	bool m_UseJoinable;
	bool m_UseIsModded;
	bool m_UseMinPlayers; 
	bool m_UseMaxPlayers; 
	bool m_UseIsPasswordProtected;
	bool m_UseRegionId; 
	bool m_UsePriority;
	bool m_UseFreeSlotsMin;
	bool m_UseFreeSlotsMax;
	bool m_UseHostIp;
	bool m_UseHostPort;
	
	void SetAntiCheatFilter( bool anti_cheat )
	{
		m_AntiCheat					= anti_cheat;
		m_UseAntiCheat				= true;
	}
	
	void SetNameFilter( string name )
	{
		m_Name						= name;
		m_UseName					= true;
	}
	
	void SetGameTypeFilter( string game_type )
	{
		m_GameType					= game_type;
		m_UseGameType				= true;
	}
	
	void SetModeIdFilter( int mode_id )
	{
		m_ModeId					= mode_id;
		m_UseModeId					= true;
	}
	
	void SetGameVersionFilter( string game_version )
	{
		m_GameVersion				= game_version;
		m_UseGameVersion			= true;
	}
	
	void SetOfficialFilter( bool official )
	{
		m_Official					= official;
		m_UseOfficial				= true;
	}
	
	void SetJoinableFilter( bool joinable )
	{
		m_Joinable					= joinable;
		m_UseJoinable				= true;
	}
	
	void SetIsModdedFilter( bool is_modded )
	{
		m_IsModded					= is_modded;
		m_UseIsModded				= true;
	}
	
	void SetMinPlayersFilter( int min_players )
	{
		m_MinPlayers				= min_players;
		m_UseMinPlayers				= true;
	}
	
	void SetMaxPlayersFilter( int max_players )
	{
		m_MaxPlayers				= max_players;
		m_UseMaxPlayers				= true;
	}
	
	void SetIsPasswordProtectedFilter( bool password_protected )
	{
		m_IsPasswordProtected		= password_protected;
		m_UseIsPasswordProtected	= true;
	}
	
	void SetRegionIdFilter( int region )
	{
		m_RegionId					= region;
		m_UseRegionId				= true;
	}
	
	void SetPriorityFilter( int priority )
	{
		m_Priority					= priority;
		m_UsePriority				= true;
	}
	
	void SetFreeSlotsMin( int freeSlotsMin )
	{
		m_FreeSlotsMin = freeSlotsMin;
		m_UseFreeSlotsMin = true;
	}
	
	void SetFreeSlotsMax( int freeSlotsMax )
	{
		m_FreeSlotsMax = freeSlotsMax;
		m_UseFreeSlotsMax = true;
	}
	
	void SetHostIp( string hostIp )
	{
		m_HostIp = hostIp;
		m_UseHostIp = true;
	}
	
	void SetHostPort( int hostPort )
	{
		m_HostPort = hostPort;
		m_UseHostPort = true;
	}
};

class BiosLobbyService
{
	//! Query game servers
	/*!
		The async result is returned in the OnDoneAsync callback.
		Expected errors:
			BAD_PARAMETER -	if atleast one of the permissions is not supported on the current platform, 
							or atleast one of the input arrays is NULL.
			
		@param inputValues input structure
		
	*/
	proto native EBiosError GetServers(GetServersInput inputValues);

	//! First 1st free server
	/*!
		The async result is returned in the OnGetFirstServerWithEmptySlot callback.
				
	*/
	proto native EBiosError GetFirstServerWithEmptySlot(GetFirstServerWithEmptySlotInput inputValues);

	//! Async callback for GetServers
	/*!
		@param result_list result object
		@param error error indicating success or fail of the async operation.
		@param response for debugging - this is the data returned by the server; or an empty string ;)
	*/
	void OnDoneAsync(ref GetServersResult result_list, EBiosError error, string response)
	{
		OnlineServices.OnLoadServersAsync( result_list, error, response );
	}
	
	void OnGetFirstServerWithEmptySlot(ref GetFirstServerWithEmptySlotResult result_list, EBiosError error)
	{
		OnlineServices.OnAutoConnectToEmptyServer( result_list, error );
	}
};
