
enum EBiosJoinType
{
	INVITE,
	ACTIVITY,
};

class BiosUser
{
	proto native string GetName();
	proto native string GetUid();
	proto native BiosClientServices GetClientServices();
};

class BiosUserManager
{
	//! Gets the initiatior of the title
	/*!
		@return BiosUser the initiator. May be NULL.
	*/
	proto native BiosUser GetTitleInitiator();
	
	//! Gets the currently present list of users
	/*!
		Fills in the array.
		Expected errors:
			BAD_PARAMETER -	user_list is NULL, 
	
		@return EBiosError indicating if the operation is done.
	*/
	proto native EBiosError GetUserList(ref array<ref BiosUser> user_list);
	
	//! Handle user selection
	/*!
		Xbox: If one user is available, or the title initiator is valid,
		otherwise displays the user picker.
	*/
	EBiosError PickUser()
	{
		ref array<ref BiosUser> user_list = new array<ref BiosUser>;
		GetUserList(user_list);
		if( user_list.Count() == 1 )
		{
			SelectUser( user_list.Get( 0 ) );
			g_Game.SelectUser();
			return EBiosError.OK;
		}
		else
		{
			return PickUserAsync();
		}
	}
	
	//! Display a system dependant account picket
	/*!
		Xbox: The async result is returned in the OnUserPicked callback.
	
		@return EBiosError indicating if the async operation is pending.
	*/
	proto native EBiosError PickUserAsync();
	
	
	//! Parse the join data from from command line parameters.
	/*!
		The async result is returned in the OnJoin callback.
		The OnJoin callback is called also during runtime when a join is accepted.
		Expected errors:
			BAD_PARAMETER -	join_data could not be parsed, 
	
		@param join_data the startup join data from command line parameters.
		@return EBiosError indicating if the async operation is pending.
	*/
	proto native EBiosError ParseJoinAsync(string join_data);
	
	//! Informs the engine about the current selected user.
	/*!
		Must be called to ensure proper authentication etc.
	
		@param user the user to select.
	*/
	proto native void SelectUser(BiosUser user);
	
	//! Returns the currently selected user.
	/*!
		@return BiosUser the selected user. May be NULL.
	*/
	proto native BiosUser GetSelectedUser();
	
	//! Call async function to get database ID
	/*!
		@return EBiosError indicating if the async operation is pending. If active user is not set, then return NOT_FOUND
	*/
	proto native EBiosError GetUserDatabaseIdAsync();
	
	//! Callback function.
	/*!
		@param dbID user database ID. If something went wrong, then it is empty string.
		@param error indicating correct state.
	*/
	void OnUserDatabaseId(string dbID, EBiosError error)
	{
		if( !OnlineServices.ErrorCaught( error ) )
		{
			g_Game.SetDatabaseID( dbID );
		}
	}
	
	//! Callback function.
	/*!
		@param user picked user. NULL on fail.
		@param error error indicating success or fail of the async operation.
	*/
	void OnUserPicked(BiosUser user, EBiosError error)
	{
		if( user || !OnlineServices.ErrorCaught( error ) )
		{
			SelectUser( user );
			if( GetGame().GetMission() )
				GetGame().GetMission().Reset();
		}
		g_Game.SelectUser();
	}
	
	//! Callback function.
	/*!
		Called when a new user signed in
	
		@param user the user that signed in. Cannot be NULL.
	*/
	void OnSignedIn(BiosUser user)
	{
		
	}
	
	//! Callback function.
	/*!
		Called when a new user signed out
	
		@param user the user that signed out. Cannot be NULL.
	*/
	void OnSignedOut(BiosUser user)
	{
		if( user == GetSelectedUser() || !GetSelectedUser() )
		{
			SelectUser( null );
			
			GetGame().GetUIManager().CloseAllSubmenus();
			GetGame().GetMission().AbortMission();
			
			GetGame().GetInput().ResetActiveGamepad();
			
			g_Game.SetGameState( DayZGameState.MAIN_MENU );
			g_Game.SetLoadState( DayZLoadState.MAIN_MENU_START );
			g_Game.SelectUser();
		}
	}
	
	//! Callback function.
	/*!
		Called when a join is parsed or when a runtime join is accepted from the system UI.
	
		@param type the type of join. Undefined on error.
		@param joiner the user that is joining. NULL on error.
		@param handle join handle for a session. Empty on error.
		@param joinee uid of the user that is being joined. Empty on error.
		@param error indicating if parsing failed. OK if not a result of ParseJoinAsync.
	*/
	void OnJoin(EBiosJoinType type, BiosUser joiner, string handle, string joinee, EBiosError error) 
	{
		if( !OnlineServices.ErrorCaught( error ) )
		{
			BiosUserManager user_manager = GetGame().GetUserManager();
			user_manager.SelectUser( joiner );
			
			OnlineServices.SetSessionHandle( handle );
			
			g_Game.SetGameState( DayZGameState.JOIN );
			g_Game.SetLoadState( DayZLoadState.JOIN_START );
			g_Game.SelectUser();
		}
	}
	
	//! Callback function.
	/*!
		Called when display info of a signed in user changed.
	
		@param user the user affected by the change.
	*/
	void OnGameNameChanged(BiosUser user)
	{
		if( user == GetSelectedUser() )
		{
			g_Game.SetPlayerName( user.GetName() );
			#ifdef PLATFORM_CONSOLE
				g_Game.SetPlayerGameName( user.GetName() );
			#endif
			if( GetGame().GetUIManager().GetMenu() )
			{
				GetGame().GetUIManager().GetMenu().Refresh();
			}
		}
	}
	
	BiosUser GetUser( string user_id )
	{
		array<ref BiosUser> user_list = new array<ref BiosUser>;
		GetUserList( user_list );
		foreach( BiosUser user : user_list )
		{
			if( user.GetUid() == user_id )
			{
				return user;
			}
		}
		return null;
	}
};
