class ActionBuildOvenCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime( UATimeSpent.DEFAULT_CONSTRUCT );
	}
}

class ActionBuildOven: ActionContinuousBase
{
	void ActionBuildOven()
	{
		m_CallbackClass = ActionBuildOvenCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		
		m_MessageStartFail = "I cannot build an oven.";
		m_MessageSuccess = "I have build an oven.";
		m_MessageFail = "I cannot build an oven.";
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}
	
	override void CreateConditionComponents()  
	{	
		
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
		m_ConditionItem = new CCINotPresent;
	}

	override int GetType()
	{
		return AT_BUILD_OVEN;
	}
		
	override string GetText()
	{
		return "Build oven";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object target_object = target.GetObject();
			
		if ( target_object && target_object.IsFireplace() )
		{
			FireplaceBase fireplace_target = FireplaceBase.Cast( target_object );
			
			if ( fireplace_target.IsBaseFireplace() && fireplace_target.CanBuildOven() )
			{
				return true;
			}
		}
		
		return false;
	}
		
	override void OnCompleteServer( ActionData action_data )
	{	
		Object target_object = action_data.m_Target.GetObject();
		FireplaceBase fireplace_target = FireplaceBase.Cast( target_object );
		
		if ( fireplace_target.CanBuildOven() )
		{
			ItemBase attached_item = ItemBase.Cast( fireplace_target.GetAttachmentByType( fireplace_target.ATTACHMENT_STONES ) );
			int slot_id = attached_item.GetInventory().GetSlotId(0);
			fireplace_target.GetInventory().SetSlotLock( slot_id, true );
			
			//set oven state
			fireplace_target.SetOvenState( true );
			
			//add specialty to soft skills
			action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
		}
		else
		{
			SendMessageToClient( action_data.m_Player, fireplace_target.MESSAGE_CANNOT_BUILD_OVEN );
		}
	}
}