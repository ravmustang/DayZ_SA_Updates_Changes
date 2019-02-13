class Headtorch_ColorBase extends Clothing
{
	ScriptedLightBase m_Light;
	
	override void OnWorkStart()
	{
		SetPilotLight(true); // Temporal solution for scripted lights not attaching on Headtorch
		
		/*
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() ) // Client side
		{
			vector pos = Vector(0,0,0.3);
			vector ori = Vector(90,0,0);
			m_Light = CreateHeadtorchLight();
			m_Light.AttachOn(this, pos, ori);
		}
		*/
	}

	ScriptedLightBase CreateHeadtorchLight()
	{
		return ScriptedLightBase.CreateLight(HeadtorchLight, "0 0 0", 60);
	}
	
	override void OnWorkStop()
	{
		SetPilotLight(false);
		/*
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() ) // Client side
		{
			if (m_Light)
				m_Light.FadeOut(90);
			
			m_Light = NULL;
		}*/
	}
};