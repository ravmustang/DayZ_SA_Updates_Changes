class DamageDealtEffect
{
	const float ALPHA_MIN = 0;
	const float ALPHA_MAX = 1;
	float m_HitDuration;
	float m_BreakPoint;
	float m_TimeActive;
	
	void DamageDealtEffect()
	{
		m_HitDuration = 0.10;
		m_BreakPoint = 0.05;
		//Print("=====================================================");
		//Print("=====================================================");
	}
	
	void ~DamageDealtEffect()
	{
		PPEffects.HitEffect(0);
		PPEffects.UpdateColor();
	}
	
	void Update(float deltatime)
	{
		float value;
		
		if( m_TimeActive <= m_BreakPoint )
		{
			value = Math.InverseLerp(0, m_BreakPoint, m_TimeActive);
		}
		else
		{
			float tmp_value = Math.InverseLerp(m_BreakPoint, m_HitDuration, m_TimeActive);
			value = 1 - tmp_value;
		}
		m_TimeActive += deltatime;
		value = Math.Clamp(value,0,1);
		float val = Math.Lerp(ALPHA_MIN, ALPHA_MAX, value);
		//Print(val);
		PPEffects.HitEffect(val);
		if(m_TimeActive >= m_HitDuration)
		{
			delete this;
		}
	}
	
}