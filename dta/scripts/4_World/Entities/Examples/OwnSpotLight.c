class OwnSpotLight extends EntityLightSource
{
	void OwnSpotLight()
	{
		SetEventMask(EntityEvent.FRAME);
		
		SetLightType(LightSourceType.SpotLight);
		SetVisibleDuringDaylight(true);
	}

	override void EOnFrame(IEntity other, float timeSlice)
	{
		float val = Math.AbsFloat(Math.Sin(m_Timer * Math.PI2));		
		SetDiffuseColor(val, val, 0.0);
		SetBrightness(val * 100);
		
		m_Timer += timeSlice;
	}
	
	float m_Timer = 0;
}