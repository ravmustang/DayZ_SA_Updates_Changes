class OwnPointLight extends EntityLightSource
{
	void OwnPointLight()
	{
		SetEventMask(EntityEvent.FRAME);

		SetLightType(LightSourceType.PointLight);
		SetVisibleDuringDaylight(true);
	}
	
	override void EOnFrame(IEntity other, float timeSlice)
	{
		float val = Math.AbsFloat(Math.Sin(m_Timer * Math.PI2));		
		SetDiffuseColor(val, 0, 0);

		m_Timer += timeSlice;
	}
	
	float m_Timer = 0;
}