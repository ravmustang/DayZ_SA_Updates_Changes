class ActionDialCombinationLockCB : ActionContinuousBaseCB
{
	private const float REPEAT_AFTER_SEC = 0.5;
	
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousRepeat(REPEAT_AFTER_SEC);
	}
}

class ActionDialCombinationLock: ActionContinuousBase
{
	void ActionDialCombinationLock()
	{
		m_CallbackClass = ActionDialCombinationLockCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_TUNE;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_ITEM_TUNE;		
		m_MessageStartFail = "I have failed the tunning.";
		m_MessageStart = "I have started the tunning.";
		m_MessageFail = "I have failed the tunning.";
		m_MessageCancel = "I have stopped the tunning.";
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_LOW;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTNone;
		m_ConditionItem = new CCINonRuined;
	}
	
	override bool HasProneException()
	{
		return true;
	}
	
	override int GetType()
	{
		return AT_DIAL_COMBINATION_LOCK;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		string combination_lock_text;
		
		if ( player )
		{
			ConstructionActionData construction_action_data = player.GetConstructionActionData();
			combination_lock_text = construction_action_data.GetDialNumberText();
		}		
		
		return "#dial_combination_lock" + " " + combination_lock_text;
	}

	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if ( item.IsInherited( CombinationLock ) )
		{
			ConstructionActionData construction_action_data = player.GetConstructionActionData();
			construction_action_data.SetCombinationLock( CombinationLock.Cast( item ) );
	
			return true;
		}
		
		return false;
	}

	override void OnFinishProgressServer( ActionData action_data )
	{	
		//set dialed number
		ConstructionActionData construction_action_data = action_data.m_Player.GetConstructionActionData();
		CombinationLock combination_lock =  construction_action_data.GetCombinationLock();
		combination_lock.DialNextNumber( construction_action_data.GetDialIndex() );
	}
}