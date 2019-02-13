// Quick prototype. Very WIP.

class PlayerNightLight extends ScriptedLightBase
{
	float m_Timer;
	
	void PlayerNightLight()
	{
		SetEventMask(EntityEvent.FRAME); // use this in your child objects if you want to enable EOnFrame event
		SetLightType(LightSourceType.PointLight);

		SetVisibleDuringDaylight(false);
		SetRadiusClient( 4 );
		SetCastShadow(false);
		EnableSpecular(false);
		SetEnabled(true);
	}
	
	override void EOnFrame(IEntity other, float timeSlice)
	{
		float valS = Math.AbsFloat(Math.Sin(m_Timer * Math.PI2));
		float valC = Math.AbsFloat(Math.Cos(m_Timer * Math.PI2));
		float valT = Math.AbsFloat(Math.Tan(m_Timer * Math.PI2));
		//SetSpotLightAngle(1+valS);
		//SetDiffuseColor(valS, valC, valT);
		//SetAmbientColor(valS, valC, valT);
		SetBrightnessClient(1);
		SetDiffuseColor(0.5, 0.5, 1);
		
		vector pos = GetGame().GetCurrentCameraPosition();
		vector dir = GetGame().GetCurrentCameraDirection();
		
		//pos = pos + dir/2;
		//SetSpotLightAngle(200);
		dir[0] = dir[0]*Math.RAD2DEG;
		dir[1] = dir[1]*Math.RAD2DEG;
		dir[2] = dir[2]*Math.RAD2DEG;
		
		SetPosition(pos);
		//SetOrientation(dir); // CURRENTLY DOESN'T WORK! 
		
		m_Timer += timeSlice;
	}
}