class ActionTakeFireplaceFromBarrel: ActionInteractBase
{
	void ActionTakeFireplaceFromBarrel()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageSuccess = "I took the fireplace.";
	}

	override int GetType()
	{
		return AT_TAKE_FIREPLACE_BARREL;
	}

	override string GetText()
	{
		return "Take fireplace";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object target_object = target.GetObject();
		
		if ( target_object )
		{
			BarrelHoles_ColorBase fireplace_barrel = BarrelHoles_ColorBase.Cast( target_object );
			
			//check barrel fireplace state
			if ( fireplace_barrel.IsOpened() && !fireplace_barrel.HasAshes() && !fireplace_barrel.IsBurning() )
			{
				//check cargo and attachments
				if ( fireplace_barrel.IsCargoEmpty() && fireplace_barrel.GetInventory().AttachmentCount() > 0 )
				{
					return true;
				}
			}
		}

		return false;
	}

	override void OnCompleteServer( ActionData action_data )
	{
		Object target_object = action_data.m_Target.GetObject();
		BarrelHoles_ColorBase fireplace_barrel = BarrelHoles_ColorBase.Cast( target_object );
		
		MiscGameplayFunctions.TurnItemIntoItemEx(action_data.m_Player, new TurnItemIntoItemLambda(fireplace_barrel, "Fireplace", action_data.m_Player));
	}
}