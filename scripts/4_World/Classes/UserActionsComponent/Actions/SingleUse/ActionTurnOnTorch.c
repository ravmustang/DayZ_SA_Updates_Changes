class ActionTurnOnTorch: ActionTurnOnWhileInHands
{
	void ActionTurnOnTorch()
	{
		m_MessageSuccess = "I ignitied torch.";
		m_MessageFail = "It's burnt out.";
		//m_Animation = "ignite";
	}

	override int GetType()
	{
		return AT_TURN_ON_TORCH;
	}

	override string GetText()
	{
		return "#ignite";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		if ( targetObject != NULL && targetObject.IsInherited(Torch) && item != NULL && !item.IsDamageDestroyed() )
		{
			Torch target_torch = Torch.Cast( targetObject );
			
			if ( target_torch.CanIgnite()  &&  !target_torch.GetCompEM().IsWorking() ) // This also checks damage
			{
				if ( item.IsKindOf( "Matchbox" ) && item.GetQuantity() > 0 )
				{
					return true;
				}
				else if ( item.IsInherited( Torch) )
				{
					Torch item_torch = Torch.Cast( item );
					
					if ( item_torch.GetCompEM().IsWorking() )
					{
						return true;
					}
				}
			}
		}
		
		return false;
	}

	override void OnStartServer( ActionData action_data )
	{
		if ( action_data.m_MainItem != NULL && action_data.m_MainItem.IsKindOf( "Matchbox" ) )
		{
			action_data.m_MainItem.PlaySound("matchboxStrike", 50);
		}
	}
	
	override void OnStartClient( ActionData action_data )
	{
		if ( action_data.m_MainItem != NULL && action_data.m_MainItem.IsKindOf( "Matchbox" ) )
		{
			action_data.m_MainItem.PlaySound("matchboxStrike", 50);
		}
	}

	override void OnCompleteServer( ActionData action_data )
	{
		Object targetObject = action_data.m_Target.GetObject();
		if ( targetObject != NULL && targetObject.IsInherited(Torch) )
		{
			if ( action_data.m_MainItem.IsKindOf( "Matchbox" ) )
			{
				action_data.m_MainItem.AddQuantity(-1,true);
			}
			
			Torch torch = Torch.Cast( targetObject );
			torch.Ignite();
		}
	}
};