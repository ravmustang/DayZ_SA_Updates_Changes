
class BiosSessionService
{
	protected int		m_GetSessionAttempts;
	string				m_CurrentHandle;
	
	//! Enter a gameplay session
	/*!
		The async result is returned in the OnEnterGameplaySession callback.
		Expected errors:
			LOGICAL - if the user is currently in an active gameplay session.
			
		@param session_address server IP address.
		@param session_port server port.	
		@return EBiosError indicating if the async operation is pending.
	*/
	proto native EBiosError	EnterGameplaySessionAsync(string session_address, int session_port);
	
	//! Leave a gameplay session
	/*!
		The async result is returned in the OnLeaveGameplaySession callback.
		If there is an unexpected error the state is cleared.
		Expected errors:
			ERR_NOT_FOUND - when attempting to leave a gameplay session the user is not part of.
			ERR_LOGICAL - when the user is not in a gameplay session.
			
		@param session_address server IP address.
		@param session_port server port.	
		@return EBiosError indicating if the async operation is pending.
	*/
	proto native EBiosError	LeaveGameplaySessionAsync(string session_address, int session_port);
	
	//! Gets a session from a join handle
	/*!
		The async result is returned in the OnGetGameplaySession, OnGetLobbySession or OnGetSessionError callback,
		dependinng on the type of session, or error.
		Expected errors:

		@param join_handle the parsed join handle.
		@return EBiosError indicating if the async operation is pending.
	*/
	void TryGetSession( string join_handle = "" )
	{
		if( join_handle != "" )
		{
			m_GetSessionAttempts	= 0;
			m_CurrentHandle			= join_handle;
			Print( m_CurrentHandle );
		}
		
		if( m_GetSessionAttempts < 10 )
			GetSessionAsync( m_CurrentHandle );
		else
			Error( "Cannot get session." ); //TODOPAL
	}
	
	//! Gets a session from a join handle
	/*!
		The async result is returned in the OnGetGameplaySession, OnGetLobbySession or OnGetSessionError callback,
		dependinng on the type of session, or error.
		Expected errors:

		@param join_handle the parsed join handle.
		@return EBiosError indicating if the async operation is pending.
	*/
	proto native EBiosError	GetSessionAsync(string join_handle);
	
	//! Sets the activity to a gameplay session
	/*!
		The async result is returned in the OnSetActivity callback.
		Expected errors:
			ERR_NOT_FOUND - when attempting to set a gameplay session activity the user is not part of.
			
		@param session_address server IP address.
		@param session_port server port.		
		@return EBiosError indicating if the async operation is pending.
	*/
	proto native EBiosError	SetGameplayActivityAsync(string session_address, int session_port);
	
	//! not implemented
	//proto native EBiosError	SetLobbyActivityAsync(...);
	
	//! Clears the current activity
	/*!
		The async result is returned in the OnClearActivity callback.
		Expected errors:
					
		@return EBiosError indicating if the async operation is pending.
	*/
	proto native EBiosError	ClearActivityAsync();
	
	//! Callback function
	/*!
		@param error error indicating success or fail of the async operation.
	*/
	void OnSetActivity(EBiosError error)
	{
		OnlineServices.ErrorCaught( error );
	}
	
	//! Callback function
	/*!
		@param error error indicating success or fail of the async operation.
	*/
	void OnClearActivity(EBiosError error)
	{
		string addr;
		int port;
		if( GetGame().GetHostAddress( addr, port ) )
		{
			LeaveGameplaySessionAsync( addr, port );
		}
	}
	
	//! Callback function
	/*!
		@param session_address server IP address.
		@param session_port server port.
	*/
	void OnGetGameplaySession(string session_address, int session_port)
	{	
		m_GetSessionAttempts = 0;
		g_Game.ConnectFromJoin( session_address, session_port );
	}
	
	//! //! Callback function, not implemented
	/*void OnGetLobbySession(...)
	{
	}*/
	
	//! Callback function
	/*!
		@param error error indicating fail of the async operation. Cannot be OK.
	*/
	void OnGetSessionError(EBiosError error)
	{
		OnlineServices.ErrorCaught( error );
		m_GetSessionAttempts++;
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( TryGetSession, 100, false, "" );
	}
	
	//! Callback function
	/*!
		@param session_address server IP address. Empty if failed.
		@param session_port server port. 0 if failed.
		@param error error indicating success or fail of the async operation.
	*/
	void OnEnterGameplaySession(string session_address, int session_port, EBiosError error)
	{
		if( !OnlineServices.ErrorCaught( error ) )
		{
			SetGameplayActivityAsync( session_address, session_port );
		}
	}
	
	//! Callback function
	/*!
		@param error error indicating success or fail of the async operation.
	*/
	void OnLeaveGameplaySession(EBiosError error)
	{
		if( !OnlineServices.ErrorCaught( error ) )
		{
			ClearActivityAsync();
		}
	}
};
