class ActionInjectEpinephrineTarget: ActionInjectTarget
{
	void ActionInjectEpinephrineTarget()
	{
		//m_Animation = "INJECTEPINENT";
		m_MessageStart = "Player started injecting you epinephrine.";
		m_MessageSuccess = "Player finished injecting you epinephrine.";
	}

	override int GetType()
	{
		return AT_INJECT_EPINEPHRINE_T;
	}
		
	override void ApplyModifiers( ActionData action_data )
	{
		PlayerBase ntarget = PlayerBase.Cast( action_data.m_Target.GetObject() );
		ntarget.GiveShock(100);
	}
};