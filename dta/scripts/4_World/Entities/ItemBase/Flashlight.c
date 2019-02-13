class Flashlight extends ItemBase
{
	ScriptedLightBase m_Light;
	
	override void OnWorkStart()
	{
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() ) // Client side
		{
			vector pos = Vector(0.03,0,0);
			vector ori = Vector(90,0,0);
			m_Light = ScriptedLightBase.CreateLight(FlashlightLight, pos, 60);
			m_Light.AttachOn(this, pos, ori);
		}
	}

	override void OnWorkStop()
	{
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() ) // Client side
		{
			if (m_Light)
				m_Light.FadeOut(90);
			
			m_Light = NULL;
		}
	}
}