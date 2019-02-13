class PartyLight extends PointLightBase
{
	float m_Timer = 0;
	
	void PartyLight()
	{
		SetVisibleDuringDaylight(true);
		SetRadiusClient( 30 );
		SetCastShadow(true);
		SetLifetime(10000);
		SetFadeOutTime(5000);
	}
	
	override void OnFrameLightSource(IEntity other, float timeSlice)
	{
		float valS = Math.AbsFloat(Math.Sin(m_Timer * Math.PI2));
		float valC = Math.AbsFloat(Math.Cos(m_Timer * Math.PI2));
		float valT = Math.AbsFloat(Math.Tan(m_Timer * Math.PI2));
		SetDiffuseColor(valS, valC, valT);
		SetAmbientColor(valS, valC, valT);
		SetBrightnessClient(1.0);
		
		m_Timer += timeSlice;
	}
}