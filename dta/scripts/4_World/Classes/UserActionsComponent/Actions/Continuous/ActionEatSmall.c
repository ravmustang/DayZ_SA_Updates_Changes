class ActionEatSmallCB : ActionEatCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousQuantityEdible(UAQuantityConsumed.EAT_SMALL,UATimeSpent.DEFAULT);
	}
};

class ActionEatSmall: ActionEat
{
	void ActionEatSmall()
	{
		m_CallbackClass = ActionEatSmallCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EAT;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_EAT;
	}
};