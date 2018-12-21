class ActionFeedPurificationTablets: ActionForceConsume
{
	override int GetType()
	{
		return AT_FEED_PURIFICATION_TABLETS;
	}

	override void ApplyModifiers( ActionData action_data )
	{
		PlayerBase ntarget = PlayerBase.Cast( action_data.m_Target.GetObject() );
		//ntarget.m_ModifiersManager.ActivateModifier(eModifiers.MDF_CHEMICALPOISON);
	}
};