class NightLightPrototype extends ItemBase
{
	PlayerNightGlow m_Light;
	float m_Timer = 0;
	
	void NightLightPrototype()
	{
		
	}
	
	override void OnWorkStart()
	{
		vector pos = GetGame().GetCurrentCameraPosition();
		m_Light = PlayerNightGlow.Cast( GetGame().CreateObject("PlayerNightGlow", pos, true) );
	}
	
	override void OnWorkStop()
	{
		if (m_Light)
			m_Light.SetEnabled(false);
	}
}