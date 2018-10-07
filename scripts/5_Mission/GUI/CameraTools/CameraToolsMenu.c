class CameraToolsMenu extends UIScriptedMenu
{
	protected vector														m_NextCamPosition;
	
	protected float															m_TimeRemaining;
	
	protected Camera														m_Camera1;
	protected Camera														m_Camera2;
	
	protected ref array<ref Param5<vector, vector, float, float, float>>	m_CameraLines;
	protected bool															m_IsPlaying;
	protected int															m_NextCameraIndex = 1;
	
	protected CTKeyframe													m_SelectedKeyframe;
	protected CTEvent														m_SelectedEvent;
	protected ref array<ref CTKeyframe>										m_Cameras;
	protected ref array<ref CTEvent>										m_Events;
	
	protected Widget														m_KeyframeContent;
	protected Widget														m_EventContent;
	
	protected XComboBoxWidget												m_InterpTypeSpeedCombo;
	protected XComboBoxWidget												m_InterpTypeCombo;
	protected EditBoxWidget													m_BlurEditWidget;
	
	protected ButtonWidget													m_Play;
	protected ButtonWidget													m_Stop;
	protected ButtonWidget													m_Save;
	protected ButtonWidget													m_Load;
	
	protected ButtonWidget													m_AddKeyframe;
	protected ButtonWidget													m_SaveKeyframe;
	protected ButtonWidget													m_DeleteKeyframe;
	protected ButtonWidget													m_ViewKeyframe;
	
	protected ButtonWidget													m_AddEvent;
	protected ButtonWidget													m_SaveEvent;
	protected ButtonWidget													m_DeleteEvent;
	protected ButtonWidget													m_ViewEvent;
	
	void CameraToolsMenu()
	{
		m_Cameras = new array<ref CTKeyframe>;
		m_Events = new array<ref CTEvent>;
		
		m_Camera1				= Camera.Cast( GetGame().CreateObject( "staticcamera", GetGame().GetPlayer().GetPosition(), true ) );
		m_Camera2				= Camera.Cast( GetGame().CreateObject( "staticcamera", GetGame().GetPlayer().GetPosition(), true ) );
	}
	
	void ~CameraToolsMenu()
	{
		GetGame().GetInput().ResetGameFocus();
		GetGame().GetInput().ChangeGameFocus(-1);
		GetGame().GetUIManager().ShowCursor(false);
		
		GetGame().ObjectDelete( m_Camera1 );
		GetGame().ObjectDelete( m_Camera2 );
		
		DeveloperFreeCamera.DisableFreeCamera( PlayerBase.Cast( GetGame().GetPlayer() ), false );
		
		MissionGameplay ms	= MissionGameplay.Cast( GetGame().GetMission() );
		ms.GetHud().Show( false );
	}
	
	override void OnShow()
	{
		if( !m_IsPlaying )
		{
			GetGame().GetInput().ChangeGameFocus(1);
			GetGame().GetUIManager().ShowCursor(true);
		}
	}
	
	override void OnHide()
	{
		if( !m_IsPlaying )
		{
			GetGame().GetInput().ResetGameFocus();
			GetGame().GetInput().ChangeGameFocus(-1);
			GetGame().GetUIManager().ShowCursor(false);
		}
	}
	
	void SelectKeyframe( CTKeyframe kf )
	{
		if( m_SelectedKeyframe )
			m_SelectedKeyframe.Unselect();
		
		m_SelectedKeyframe = kf;
		m_SelectedKeyframe.Select();
	}
	
	void SelectEvent( CTEvent ev )
	{
		m_SelectedEvent = ev;
	}
	
	override Widget Init()
	{
		layoutRoot				= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/camera_tools/camera_tools.layout" );
		
		m_KeyframeContent		= layoutRoot.FindAnyWidget( "keyframe_list_content" );
		m_EventContent			= layoutRoot.FindAnyWidget( "events_list_content" );
		m_InterpTypeCombo		= XComboBoxWidget.Cast( layoutRoot.FindAnyWidget( "interp_type" ) );
		m_InterpTypeSpeedCombo	= XComboBoxWidget.Cast( layoutRoot.FindAnyWidget( "interp_speed_type" ) );
		m_BlurEditWidget		= EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "blur_value" ) );
		m_AddKeyframe			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "add_keyframe" ) );
		m_AddEvent				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "add_event" ) );
		
		m_Play					= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "play" ) );
		m_Stop					= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "stop" ) );
		m_Save					= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "save" ) );
		m_Load					= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "load" ) );
		
		m_SaveKeyframe			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "save_keyframe" ) );
		m_DeleteKeyframe		= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "delete_keyframe" ) );
		m_ViewKeyframe			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "snap_to_keyframe" ) );
		m_SaveEvent				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "save_event" ) );
		m_DeleteEvent			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "delete_event" ) );
		m_ViewEvent				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "snap_to_event" ) );
		
		MissionGameplay ms		= MissionGameplay.Cast( GetGame().GetMission() );
		ms.GetHud().Show( false );
		DeveloperFreeCamera.EnableFreeCameraSceneEditor( PlayerBase.Cast( GetGame().GetPlayer() ) );
		
		m_BlurEditWidget.SetText( "1.0" );
		return layoutRoot;
	}
	
	void LoadData()
	{
		if( !FileExist( "CameraTools" ) )
			MakeDirectory( "CameraTools" );
		
		m_Cameras.Clear();
		m_Events.Clear();
		
		ref CTSaveStructure save_data = new CTSaveStructure();
		JsonFileLoader<ref CTSaveStructure>.JsonLoadFile( "CameraTools/keyframe_data.json", save_data );
		
		m_InterpTypeCombo.SetCurrentItem( save_data.m_InterpType );
		m_InterpTypeSpeedCombo.SetCurrentItem( save_data.m_InterpSpeed );
		SetBlurValue( save_data.m_BlurValue );
		
		foreach( Param5<vector, vector, float, float, float> transform : save_data.m_Cameras )
		{
			ref CTKeyframe kf = new CTKeyframe( m_Cameras.Count(), transform.param1, transform.param2, transform.param3, transform.param4, transform.param5, GetTotalTime( m_Cameras.Count() ), m_KeyframeContent, this );
			kf.SetInterpTime( transform.param3 );
			m_Cameras.Insert( kf );
		}
		
		foreach( Param5<vector, vector, float, string, string> event_t : save_data.m_Events )
		{
			ref CTEvent ev = new CTEvent( m_Events.Count(), event_t.param1, event_t.param2, m_EventContent, event_t.param4, this );
			ev.SetEventTime( event_t.param3 );
			m_Events.Insert( ev );
		}
	}
	
	void SaveData()
	{
		if( !FileExist( "CameraTools" ) )
			MakeDirectory( "CameraTools" );
		ref CTSaveStructure save_data	= new CTSaveStructure();
		save_data.m_InterpType			= m_InterpTypeCombo.GetCurrentItem();
		save_data.m_InterpSpeed			= m_InterpTypeSpeedCombo.GetCurrentItem();
		save_data.m_BlurValue			= GetBlurValue();
		
		foreach( CTKeyframe keyframe : m_Cameras )
		{
			ref Param5<vector, vector, float, float, float> transform = new Param5<vector, vector, float, float, float>( keyframe.GetPosition(), keyframe.GetOrientation(), keyframe.GetInterpTime(), keyframe.GetFOV(), keyframe.GetDOF() );
			save_data.m_Cameras.Insert( transform );
		}
		
		foreach( CTEvent event_t : m_Events )
		{
			ref Param5<vector, vector, float, string, string> ev = new Param5<vector, vector, float, string, string>( event_t.GetPosition(), event_t.GetRotation(), event_t.GetEventTime(), event_t.GetEventObjectType(), event_t.GetEventType() );
			save_data.m_Events.Insert( ev );
		}
		
		JsonFileLoader<ref CTSaveStructure>.JsonSaveFile( "CameraTools/keyframe_data.json", save_data );
	}
	
	float GetTotalTime( int index )
	{
		float time;
		index = Math.Clamp( index, 0, m_Cameras.Count() );
		for( int i = 0; i < index; i++ )
		{
			time += m_Cameras.Get( i ).GetInterpTime();
		}
		return time;
	}
	
	void AddKeyframe()
	{
		if( Camera.GetCurrentCamera() )
		{
			vector pos = Camera.GetCurrentCamera().GetWorldPosition();
			vector orient = Camera.GetCurrentCamera().GetOrientation();
			ref CTKeyframe kf = new CTKeyframe( m_Cameras.Count(), pos, orient, 1, 70, 50, GetTotalTime( m_Cameras.Count() ), m_KeyframeContent, this );
			m_Cameras.Insert( kf );
			SelectKeyframe( kf );
		}
	}
	
	void SaveKeyframe()
	{
		if( m_SelectedKeyframe && Camera.GetCurrentCamera() )
		{
			m_SelectedKeyframe.SetPosition( Camera.GetCurrentCamera().GetWorldPosition() );
			m_SelectedKeyframe.SetOrientation( Camera.GetCurrentCamera().GetOrientation() );
		}
	}
	
	void DeleteKeyframe()
	{
		if( m_SelectedKeyframe )
		{
			int index = m_Cameras.Find( m_SelectedKeyframe );
			m_Cameras.Remove( index );
			index = Math.Clamp( index, 0, m_Cameras.Count() - 1 );
			if( m_Cameras.Count() > 0 )
			{
				SelectKeyframe( m_Cameras.Get( index ) );
			}
		}
	}
	
	void ViewKeyframe()
	{
		if( m_SelectedKeyframe && Camera.GetCurrentCamera() )
		{
			Camera.GetCurrentCamera().SetPosition( m_SelectedKeyframe.GetPosition() );
			Camera.GetCurrentCamera().SetOrientation( m_SelectedKeyframe.GetOrientation() );
		}
	}
	
	void AddEvent()
	{
		if( Camera.GetCurrentCamera() )
		{
			vector pos = GetMousePosInScene();
			ref CTEvent ev = new CTEvent( m_Events.Count(), pos, "0 0 0", m_EventContent, GetGame().CreateRandomPlayer(), this );
			m_Events.Insert( ev );
			
			//SelectEvent( ev );
		}
	}
	
	void SaveEvent()
	{
		
	}
	
	void DeleteEvent()
	{
		
	}
	
	void ViewEvent()
	{
		
	}
	
	void Play()
	{
		if( m_Cameras.Count() > 1 )
		{
			layoutRoot.Show( false );
			GetGame().GetUIManager().ShowCursor( false );
			m_CameraLines = new array<ref Param5<vector, vector, float, float, float>>;
			foreach( CTKeyframe keyframe : m_Cameras )
			{
				ref Param5<vector, vector, float, float, float> transform = new Param5<vector, vector, float, float, float>( keyframe.GetPosition(), keyframe.GetOrientation(), keyframe.GetInterpTime(), keyframe.GetFOV(), keyframe.GetDOF() );
				m_CameraLines.Insert( transform );
			}
			
			DeveloperFreeCamera.DisableFreeCamera( PlayerBase.Cast( GetGame().GetPlayer() ), false );
			
			vector pos				= m_CameraLines[0].param1;
			
			m_TimeRemaining			= SetCameraData( m_CameraLines[0], m_CameraLines[1] );
			
			m_NextCamPosition		= m_Camera2.GetPosition();
			
			m_Camera1.SetActive( true );
			m_Camera1.InterpolateTo( m_Camera2, m_TimeRemaining, m_InterpTypeCombo.GetCurrentItem() );
			
			m_NextCameraIndex		= 1;
			m_IsPlaying				= true;
		}
	}
	
	void Stop()
	{
		layoutRoot.Show( true );
		GetGame().GetUIManager().ShowCursor( true );
		m_NextCameraIndex = 1;
		m_IsPlaying = false;
	}
	
	vector GetMousePosInScene()
	{
		vector dir = GetGame().GetPointerDirection();
		vector from = FreeDebugCamera.GetInstance().GetPosition(); 
		vector to = from + ( dir * 10000 );   
		vector contact_pos;
		vector contact_dir;
		int contact_component;
		
		if ( DayZPhysics.RaycastRV(from, to, contact_pos, contact_dir, contact_component) )   
		{
			return contact_pos;
		}
		
		return "0 0 0";
	}
	
	override void Update( float timeslice )
	{
		if ( m_CameraLines && m_IsPlaying )
		{
			m_TimeRemaining -= timeslice;
			if( !Camera.GetCurrentCamera() || !m_Camera1 || !m_Camera2 )
			{
				return;
			}
			
			if( m_NextCameraIndex + 1 == m_CameraLines.Count() )
			{
				Stop();
				DeveloperFreeCamera.EnableFreeCameraSceneEditor( PlayerBase.Cast( GetGame().GetPlayer() ) );
				return;
			}
			
			if( Camera.GetCurrentCamera().GetPosition() == m_NextCamPosition )
			{
				m_TimeRemaining			= SetCameraData( m_CameraLines[m_NextCameraIndex], m_CameraLines[m_NextCameraIndex + 1] );
				
				m_NextCamPosition		= m_CameraLines[m_NextCameraIndex + 1].param1;
				
				m_Camera1.SetActive( true );
				m_Camera1.InterpolateTo( m_Camera2, m_TimeRemaining, m_InterpTypeCombo.GetCurrentItem() );
				m_NextCameraIndex++;
			}
		}
		else
		{
			if( m_Camera1.GetPosition() == m_NextCamPosition )
			{
				DeveloperFreeCamera.EnableFreeCameraSceneEditor( PlayerBase.Cast( GetGame().GetPlayer() ) );
			}
			
			for( int i = 0; i < ( m_Cameras.Count() - 1 ); i++ )
			{
				vector pos0 = m_Cameras.Get( i ).GetPosition();
				vector pos1 = m_Cameras.Get( i + 1 ).GetPosition();
				Debug.DrawArrow( pos0, pos1, 2, COLOR_RED, ShapeFlags.ONCE );
			}
			
			if( m_SelectedEvent )
			{
				if( !g_Game.IsLeftCtrlDown() )
					m_SelectedEvent.SetPosition( GetMousePosInScene() );
				else
				{
					vector from	= m_SelectedEvent.GetPosition();
					vector to	= GetMousePosInScene();
					Debug.DrawLine( from, to, 0xFFFFFFFF, ShapeFlags.ONCE );
					DbgUI.Text( vector.Direction( from,to ).Normalized().ToString() );
					m_SelectedEvent.SetRotation( vector.Direction( from,to ).Normalized() );
				}
					
			}
			
			foreach( CTEvent ev : m_Events )
			{
				ev.Update();
			}
		}
	}
	
	float SetCameraData( Param5<vector, vector, float, float, float> transform1, Param5<vector, vector, float, float, float> transform2 )
	{
		float int_value			= transform1.param3;
		if( m_InterpTypeSpeedCombo.GetCurrentItem() == 1 )
		{
			int_value = vector.Distance( transform1.param1, transform2.param1 ) / int_value;
		}
		
		m_Camera1.SetPosition( transform1.param1 );
		m_Camera1.SetOrientation( transform1.param2 );
		m_Camera1.SetFOV( transform1.param4 * Math.DEG2RAD );
		m_Camera1.SetFocus( transform1.param5, GetBlurValue() );
		
		m_Camera2.SetPosition( transform2.param1 );
		m_Camera2.SetOrientation( transform2.param2 );
		m_Camera2.SetFOV( transform2.param4 * Math.DEG2RAD );
		m_Camera2.SetFocus( transform2.param5, GetBlurValue() );
		
		return int_value;
	}
	
	void SetBlurValue( float blur )
	{
		m_BlurEditWidget.SetText( blur.ToString() );
	}
	
	float GetBlurValue()
	{
		return m_BlurEditWidget.GetText().ToFloat();
	}
	
	override bool OnMouseButtonDown( Widget w, int x, int y, int button )
	{
		if( w == layoutRoot && button == MouseState.RIGHT && !m_IsPlaying )
		{
			GetGame().GetInput().ResetGameFocus();
			GetGame().GetInput().ChangeGameFocus(-1);
			GetGame().GetUIManager().ShowCursor(false);
			SetFocus( layoutRoot );
			return true;
		}
		else if( !IsFocusable( w ) )
		{
			SetFocus( layoutRoot );
		}
		return false;
	}
	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		if( w == layoutRoot && button == MouseState.LEFT )
		{
			SelectEvent( null );
			return true;
		}
		return false;
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if( button == MouseState.LEFT )
		{
			if( w == m_SaveKeyframe )
			{
				SaveKeyframe();
				return true;
			}
			else if ( w == m_DeleteKeyframe )
			{
				DeleteKeyframe();
				return true;
			}
			else if ( w == m_ViewKeyframe )
			{
				ViewKeyframe();
				return true;
			}
			else if ( w == m_SaveEvent )
			{
				SaveEvent();
				return true;
			}
			else if ( w == m_DeleteEvent )
			{
				DeleteEvent();
				return true;
			}
			else if ( w == m_ViewEvent )
			{
				ViewEvent();
				return true;
			}
			else if ( w == m_AddKeyframe )
			{
				AddKeyframe();
				return true;
			}
			else if ( w == m_AddEvent )
			{
				AddEvent();
				return true;
			}
			else if ( w == m_Play )
			{
				Play();
				return true;
			}
			else if ( w == m_Stop )
			{
				Stop();
				return true;
			}
			else if ( w == m_Save )
			{
				SaveData();
				return true;
			}
			else if ( w == m_Load )
			{
				LoadData();
				return true;
			}
		}
		return false;
	}
	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		return true;
	}
	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		return true;
	}
	
	override bool OnFocus( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorRed( w );
			return true;
		}
		return false;
	}
	
	override bool OnFocusLost( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorWhite( w, null );
			return true;
		}
		return false;
	}
	
	bool IsFocusable( Widget w )
	{
		return ( w == m_SaveKeyframe  || w == m_DeleteKeyframe || w == m_ViewKeyframe || w == m_SaveEvent || w == m_DeleteEvent || w == m_ViewEvent || w == m_AddKeyframe || w == m_AddEvent || w == m_Play || w == m_Stop || w == m_Save || w == m_Load || w == m_InterpTypeCombo );
	}
	
	override bool OnKeyPress(Widget w, int x, int y, int key)
	{
		if( key == KeyCode.KC_SPACE )
		{
			if( !m_IsPlaying )
			{
				//Play();
				return true;
			}
			else
			{
				Stop();
				return true;
			}
			return false;
		}
		else if( g_Game.IsLeftCtrlDown() && key == KeyCode.KC_S )
		{
			SaveKeyframe();
			return true;
		}
		else if( g_Game.IsLeftCtrlDown() && key == KeyCode.KC_Q )
		{
			AddKeyframe();
			return true;
		}
		else if( g_Game.IsLeftCtrlDown() && key == KeyCode.KC_E )
		{
			DeleteKeyframe();
			return true;
		}
		return false;
	}
	
	//Coloring functions (Until WidgetStyles are useful)
	void ColorRed( Widget w )
	{
		//SetFocus( w );
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetTextColor( ARGB( 255, 255, 0, 0 ) );
			button.SetAlpha( 0.9 );
		}
	}
	
	void ColorWhite( Widget w, Widget enterW )
	{
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetTextColor( ARGB( 255, 255, 255, 255 ) );
			button.SetAlpha( 0.75 );
		}
	}
}