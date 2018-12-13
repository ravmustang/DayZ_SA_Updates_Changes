class ActionConsumeCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousQuantityEdible(UAQuantityConsumed.DEFAULT,UATimeSpent.DEFAULT);
	}
};

class ActionConsume: ActionContinuousBase
{
	void ActionConsume()
	{
		m_CallbackClass = ActionConsumeCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EAT;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_EAT;
		m_MessageStartFail = "There's nothing left.";
		m_MessageStart = "I have started consuming.";
		m_MessageSuccess = "I have finished consuming.";
		m_MessageFail = "Player moved and consuming was canceled.";
		m_MessageCancel = "I stopped consuming.";
		//m_Animation = "EATTABLETS";		
	}
	
	override void CreateConditionComponents()  
	{		
		m_ConditionItem = new CCINotRuinedAndEmpty;
		m_ConditionTarget = new CCTSelf;
	}
	
	override bool HasProneException()
	{
		return true;
	}

	override int GetType()
	{
		return AT_CONSUME;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "#eat";
	}
	
	override void OnEndServer( ActionData action_data )
	{	
		if ( action_data.m_MainItem && action_data.m_MainItem.GetQuantity() <= 0.01 )
		{
			action_data.m_MainItem.SetQuantity(0);
		}
		
		EntityAI item = action_data.m_MainItem;
		PlayerBase player = action_data.m_Player;
		PluginTransmissionAgents plugin = PluginTransmissionAgents.Cast( GetPlugin(PluginTransmissionAgents) );
		plugin.TransmitAgents(player, item, AGT_UACTION_TO_ITEM);
	}
};