class ActionWringClothesCB : ActionContinuousBaseCB
{
	protected const float QUANTITY_WRINGLED_PER_SECOND = 0.02;
	
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousWringClothes(QUANTITY_WRINGLED_PER_SECOND, UATimeSpent.WASH_HANDS);
	}
};

class ActionWringClothes: ActionContinuousBase
{
	void ActionWringClothes()
	{
		m_CallbackClass = ActionWringClothesCB;
		m_MessageStartFail = "It's ruined.";
		m_MessageStart = "I have started wringing clothes.";
		m_MessageSuccess = "I have finished wringing clothes.";
		m_MessageFail = "Player moved and stopped wringing clothes.";
		m_MessageCancel = "I stopped wringing clothes.";
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_WRING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	override int GetType()
	{
		return AT_WRING_CLOTHES;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "Wring the clothes";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (player.IsInWater()) return false;

		//! wet+ items (so they will stay damp after wringing)
		if ( item && item.GetWet() >= 0.25 && item.GetWet() <= item.GetWetMax())
		{
			return true;	
		}
		else
		{	
			return false;		
		}
	}	
	
	override void OnExecuteServer( ActionData action_data )
	{
		Param1<float> nacdata = Param1<float>.Cast( action_data.m_ActionComponent.GetACData() );		
		float delta = nacdata.param1;
		action_data.m_MainItem.AddWet( -delta );
	}

	override void OnCompleteServer( ActionData action_data )
	{
		Param1<float> nacdata = Param1<float>.Cast( action_data.m_ActionComponent.GetACData() );		
		float delta = nacdata.param1;
		action_data.m_MainItem.AddWet( -delta );
	}	
};