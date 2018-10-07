class Rangefinder extends ItemOptics
{
	const float RANGEFINDER_MAX_DISTANCE = 900; //TODO adjust maximal distance to match RL rangefinder
	
	protected ref Timer 				m_timer;
	protected ref Param1<string> 		m_MessageParam;
	protected PlayerBase 				m_Player;
	
	void Rangefinder()
	{
	}
	
	void SetPlayer(PlayerBase player)
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
		return 0.1;
	}
	
	override void OnWorkStart()
	{
		if ( GetGame().IsClient()  ||  !GetGame().IsMultiplayer())
		{
			StartPeriodicMeasurement();
		}
	}
	
	override void OnWorkStop()
	{
		if ( GetGame().IsClient()  ||  !GetGame().IsMultiplayer())
		{
			StopPeriodicMeasurement();
		}
	}
	
	void StartPeriodicMeasurement()
	{
		if ( m_timer == null )
		{
			m_timer = new Timer;

					}
		if ( m_MessageParam == null ) 	m_MessageParam = new Param1<string>("");
		
		m_timer.Run(GetMeasurementUpdateInterval(),this,"DoMeasurement",NULL,true);
	}
	
	void StopPeriodicMeasurement()
	{
		if ( m_timer )
		{
			m_timer.Stop();
		}
	}
	
	// Measures the distance and returns result in formated string
	void DoMeasurement()
	{
		PlayerBase player = GetPlayer();
		
		if (player)
		{
			const float	MAX_RANGE 		= 9999;
			float 		energy_needed 	= GetCompEM().GetEnergyUsage() * GetMeasurementUpdateInterval();
			vector 		from 			= GetGame().GetCurrentCameraPosition();
			vector 		to 				= from + (GetGame().GetCurrentCameraDirection() * MAX_RANGE);
			vector 		contact_pos;
			vector 		contact_dir;
			int 		contactComponent;
			
			DayZPhysics.RaycastRV( from, to, contact_pos, contact_dir, contactComponent, NULL , NULL, player, false, false, ObjIntersectIFire);
			
			// Generate result
			float dist = vector.Distance( from, contact_pos );
			dist = Math.Round(dist);
			
			if (dist < RANGEFINDER_MAX_DISTANCE)
			{
				m_MessageParam.param1 = "#range_finder_distance" + ": " + dist.ToString() + " #meters";
			}
			else
			{
				m_MessageParam.param1 = "#range_finder_too_far";
			}
		
			player.MessageAction(m_MessageParam.param1);
		}
	}
}