class Rangefinder extends ItemOptics
{
	static const float RANGEFINDER_MAX_DISTANCE = 913.4856; //TODO adjust maximal distance to match real life rangefinder
	
	protected ref Timer 				m_Timer;
	protected PlayerBase 				m_Player;
	protected TextWidget				m_RangeText;
	
	void Rangefinder()
	{
	}
	
	void SetPlayer( PlayerBase player )
	{
		m_Player = player;
	}
	
	PlayerBase GetPlayer()
	{
		return m_Player;
	}
	
	// How frequently the measurement should be taken
	float GetMeasurementUpdateInterval()
	{
		return 0.5;
	}
	
	override void OnWorkStart()
	{
		if( GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			StartPeriodicMeasurement();
			m_RangeText = TextWidget.Cast( GetGame().GetWorkspace().CreateWidgets( "gui/layouts/gameplay/rangefinder_hud.layout" ) );
		}
	}
	
	override void OnWorkStop()
	{
		if( GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			StopPeriodicMeasurement();
			delete m_RangeText;
		}
	}
	
	void StartPeriodicMeasurement()
	{
		if( !m_Timer )
		{
			m_Timer = new Timer;
		}
		
		m_Timer.Run( GetMeasurementUpdateInterval(), this, "DoMeasurement", null, true );
	}
	
	void StopPeriodicMeasurement()
	{
		if( m_Timer )
		{
			m_Timer.Stop();
		}
	}
	
	// Measures the distance and returns result in formated string
	void DoMeasurement()
	{
		PlayerBase player = GetPlayer();
		
		if ( player )
		{
			vector 		from 			= GetGame().GetCurrentCameraPosition();
			vector 		to 				= from + (GetGame().GetCurrentCameraDirection() * RANGEFINDER_MAX_DISTANCE);
			vector 		contact_pos;
			vector 		contact_dir;
			int 		contactComponent;
			
			DayZPhysics.RaycastRV( from, to, contact_pos, contact_dir, contactComponent, NULL , NULL, player, false, false, ObjIntersectIFire);
			
			// Generate result
			float dist = vector.Distance( from, contact_pos );
			dist = Math.Round(dist);
			
			if (dist < RANGEFINDER_MAX_DISTANCE)
			{
				if( dist < 10 )
					m_RangeText.SetText( "00" + dist.ToString() );
				else if( dist < 100 )
					m_RangeText.SetText( "0" + dist.ToString() );
				else
					m_RangeText.SetText( dist.ToString() );
			}
			else
			{
				m_RangeText.SetText( "- - -" );
			}
		}
	}
}