class CAContinuousRepeat : CAContinuousBase
{
	protected float				m_TimeElpased;
	protected float				m_TotalTimeElpased;
	protected float				m_TimeToComplete;
	protected float				m_DefaultTimeToComplete;
	protected ref Param1<float>	m_SpentUnits;
	
	void CAContinuousRepeat( float time_to_complete_action )
	{
		m_DefaultTimeToComplete = time_to_complete_action;
	}
	
	override void Setup( ActionData action_data )
	{
		m_TimeElpased = 0;
		if ( !m_SpentUnits )
		{ 
			m_SpentUnits = new Param1<float>(0);
		}
		else
		{	
			m_SpentUnits.param1 = 0;
		}

		m_TimeToComplete = action_data.m_Player.GetSoftSkillManager().SubtractSpecialtyBonus( m_DefaultTimeToComplete, m_Action.GetSpecialtyWeight(), true);		
	}
	
	override int Execute( ActionData action_data )
	{
		if ( !action_data.m_Player )
		{
			return UA_ERROR;
		}

		if ( m_TimeElpased < m_TimeToComplete )
		{
			m_TimeElpased += action_data.m_Player.GetDeltaT();
			m_TotalTimeElpased += action_data.m_Player.GetDeltaT();
			return UA_PROCESSING;
		}
		else
		{
			m_SpentUnits.param1 = m_TimeElpased;
			SetACData(m_SpentUnits);
			m_TimeElpased = 0;
			return UA_REPEAT;
		}
	}
	
	override int Cancel( ActionData action_data )
	{
		if ( m_SpentUnits )
		{
			m_SpentUnits.param1 = m_TotalTimeElpased;
			SetACData(m_SpentUnits);
		}
		return UA_CANCEL;
	}
	
	override float GetProgress()
	{	
		//float progress = m_TimeElpased/m_TimeToComplete;
		return  m_TimeElpased/m_TimeToComplete;
	}
};