class ActionRaiseMegaphoneCB : ActionContinuousBaseCB
{
	private const float REPEAT_AFTER_SEC = -1.0;
	
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousRepeat( REPEAT_AFTER_SEC );
	}
}

class ActionRaiseMegaphone: ActionContinuousBase
{
	void ActionRaiseMegaphone()
	{
		m_CallbackClass = ActionRaiseMegaphoneCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_RAISEMEGAPHONE;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_RAISEMEGAPHONE;		
		m_MessageStartFail = "I have failed to raise the megaphone.";
		m_MessageStart = "I have raised the megaphone.";
		m_MessageFail = "I have failed to raise the megaphone.";
		m_MessageCancel = "I have stopped raising the megaphone.";
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
		return AT_RAISE_MEGAPHONE;
	}

	override bool HasProgress()
	{
		return false;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "Raise megaphone";
	}

	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if ( item.IsTransmitter() ) 
		{
			ItemMegaphone megaphone = ItemMegaphone.Cast( item );
			
			if ( megaphone ) 
			{
				return true;
			}
		}
		
		return false;
	}

	override void OnStartServer( ActionData action_data )
	{
		action_data.m_MainItem.GetCompEM().SwitchOn();
	}
		
	override void OnCancelServer( ActionData action_data )
	{
		action_data.m_MainItem.GetCompEM().SwitchOff();
	}
	
	override void OnCompleteServer( ActionData action_data )
	{
		action_data.m_MainItem.GetCompEM().SwitchOff();
	}	
}