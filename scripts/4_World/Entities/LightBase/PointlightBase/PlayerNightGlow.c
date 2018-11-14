// Quick prototype. Very WIP.

class PlayerNightGlow extends PointlightBase
{
	float m_Timer = 0;
	
	void PlayerNightGlow()
	{
		SetEventMask(EntityEvent.FRAME); // use this in your child objects if you want to enable EOnFrame event

		SetVisibleDuringDaylight(false);
		SetRadius( 3 );
		SetCastShadow(true); // Currently light doesn't work when this is FALSE!!!
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
		SetBrightness(1.0);
		SetDiffuseColor(0.5, 0.5, 1);
		
		vector pos = GetGame().GetCurrentCameraPosition();
		vector dir = GetGame().GetCurrentCameraDirection();
		
		//pos = pos + dir/2;
		//SetSpotLightAngle(200);
		dir[0] = dir[0]*Math.RAD2DEG;
		dir[1] = dir[1]*Math.RAD2DEG;
		dir[2] = dir[2]*Math.RAD2DEG;
		pos = pos + Vector(0, 0.44, 0);
		
		SetPosition(pos);
		//SetOrientation(dir); // CURRENTLY DOESN'T WORK! 
		
		m_Timer += timeSlice;
	}
}