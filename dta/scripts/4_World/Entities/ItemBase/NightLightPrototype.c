class NightLightPrototype extends ItemBase
{
	PlayerNightLight m_Light;
	float m_Timer = 0;
	
	void NightLightPrototype()
	{
		
	}
	
	override void OnWorkStart()
	{
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() ) // Client side
		{
			vector pos = GetGame().GetCurrentCameraPosition();
			m_Light = PlayerNightLight.Cast( ScriptedLightBase.CreateLight(PlayerNightLight, pos) );
		}
	}
	
	override void OnWorkStop()
	{
		if (m_Light)
			m_Light.SetEnabled(false);
	}
}