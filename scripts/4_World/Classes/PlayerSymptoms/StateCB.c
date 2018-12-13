class SymptomCB extends HumanCommandActionCallback
{
	//int m_SymptomUID;
	float m_RunningTime;
	float m_StartingTime;
	PlayerBase m_Player;
	
	override void OnFinish(bool pCanceled)
	{
		if( m_Player && m_Player.GetSymptomManager())
		{
			m_Player.GetSymptomManager().OnAnimationFinished();
		}
	}
	
	
	void Init(float running_time, PlayerBase player)
	{
		EnableCancelCondition(true);
		m_RunningTime = running_time * 1000;
		m_StartingTime = GetGame().GetTime();
		m_Player = player;
	}
	
	bool CancelCondition()
	{
		if(m_RunningTime > 0 && (GetGame().GetTime() > m_StartingTime + m_RunningTime))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	override bool IsSymptomCallback()
	{
		return true;
	}
	
};
