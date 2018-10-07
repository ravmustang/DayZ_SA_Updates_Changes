class CTEvent extends CTObjectFollower
{
	protected int				m_Index;
	protected float				m_EventTime;
	protected string			m_EventType;
	
	protected Widget			m_Root;
	
	void CTEvent( int index, vector pos, vector orient, Widget root, string type, CameraToolsMenu parent )
	{
		m_Root				= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/camera_tools/keyframe_entry.layout", root );
		m_FollowerRoot		= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/camera_tools/event_tracker.layout", null );
		m_FollowerButton	= m_FollowerRoot.FindAnyWidget( "IconPanel" );
		m_Index				= index;
		m_EventTime			= 0.0;
		m_EventType			= "";
		m_Position			= pos;
		m_Orientation		= orient;
		m_Menu				= parent;
		
		m_FollowerRoot.SetHandler( this );
		CreateFollowedObject( type );
	}
	
	void ~CTEvent()
	{
		delete m_Root;
	}
	
	void SetEventTime( float time )
	{
		m_EventTime = time;
	}
	
	void SetEventType( string type )
	{
		m_EventType = type;
	}
	
	float GetEventTime()
	{
		return m_EventTime;
	}
	
	string GetEventType()
	{
		return m_EventType;
	}
	
	string GetEventObjectType()
	{
		if( m_FollowedObject )
		{
			return m_FollowedObject.GetType();
		}
		return "";
	}
}