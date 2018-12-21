class ActionForceDrinkCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousQuantityEdible(UAQuantityConsumed.DRINK,UATimeSpent.DEFAULT);
	}
};

class ActionForceDrink: ActionForceConsume
{
	void ActionForceDrink()
	{
		m_CallbackClass = ActionForceDrinkCB;
		m_MessageStartFail = "There's nothing left.";
		m_MessageStart = "I started forcing target drink.";
		m_MessageStartTarget = "Player started forcing you drink.";
		m_MessageSuccess = "I finished force drinking.";
		m_MessageSuccessTarget = "Player finished sforce drinking you.";
		m_MessageCancelTarget = "Player stopped force drinking you.";
		m_MessageFail = "Player moved too far away.";
		m_MessageCancel = "I stopped forcing target to drink.";	
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_EMPTY_VESSEL;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_WATER;
	}
		
	override string GetText()
	{
		return "#give_drink";
	}
};
	
	