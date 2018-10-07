class ActionViewOptics : ActionContinuousBase
{
	void ActionViewOptics()
	{
		m_CallbackClass = ActionRaiseAndViewCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_LOOKOPTICS;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_LOOKOPTICS;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	override int GetType()
	{
		return AT_VIEW_OPTICS;
	}
		
	override string GetText()
	{
		return "Use optics";
	}
	
	override bool HasProgress()
	{
		return false;
	}

	override bool HasTarget()
	{
		return false;
	}

	override bool HasProneException()
	{
		return true;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ItemOptics optic;
		if( Class.CastTo(optic, item) && !optic.IsInOptics() )
		{
			/*if ( !CanWork(optic) )
			{
				return false;
			}*/
			return true;
		}
		return false;
	}
	
	override bool ActionConditionContinue( ActionData action_data )
	{
		/*if ( !CanWork(ItemOptics.Cast(action_data.m_MainItem)) )
		{
			return false;
		}*/
		return true;
	}
	
	override void OnExecuteClient( ActionData action_data )
	{
		ItemOptics optic;
		if( Class.CastTo(optic, action_data.m_MainItem) )
		{
			if (!optic.IsInOptics())
			{
				EnterOptics(optic, action_data.m_Player);
			}
			else
			{
				ExitOptics(optic, action_data.m_Player);
			}
		}
	}
	
	override void OnExecuteServer( ActionData action_data )
	{
		ItemOptics optic;
		if( Class.CastTo(optic, action_data.m_MainItem) )
		{
			if (!optic.IsInOptics())
			{
				EnterOptics(optic, action_data.m_Player);
			}
			else
			{
				ExitOptics(optic, action_data.m_Player);
			}
		}
	}
	
	override void OnCancelClient( ActionData action_data )
	{
		ItemOptics optic;
		if( Class.CastTo(optic, action_data.m_MainItem) )
		{
			ExitOptics(optic, action_data.m_Player);
		}
	}
	
	override void OnCancelServer( ActionData action_data )
	{
		ItemOptics optic;
		if( Class.CastTo(optic, action_data.m_MainItem) )
		{
			ExitOptics(optic, action_data.m_Player);
		}
	}
	
	override void OnCompleteClient( ActionData action_data )
	{
		ItemOptics optic;
		if( Class.CastTo(optic, action_data.m_MainItem) )
		{
			ExitOptics(optic, action_data.m_Player);
		}
	}
	
	override void OnCompleteServer( ActionData action_data )
	{
		ItemOptics optic;
		if( Class.CastTo(optic, action_data.m_MainItem) )
		{
			ExitOptics(optic, action_data.m_Player);
		}
	}
	
	bool CanWork(ItemBase item)
	{
		//return true;
		// no energy needed, can work
		if ( !item.HasEnergyManager() )
		{
			return true;
		}
		else if ( item.GetCompEM() && item.GetCompEM().CanWork() )
		{
			return true;
		}
		
		return false;
	}
	
	void EnterOptics(ItemOptics optic, PlayerBase player)
	{
		optic.EnterOptics();
		optic.HideSelection("hide");
		if (optic.HasEnergyManager()) 	optic.GetCompEM().SwitchOn();
		player.GetAimingModel().SetAimNoiseAllowed(false);
	}
	
	void ExitOptics(ItemOptics optic, PlayerBase player)
	{
		optic.ShowSelection("hide");
		optic.ExitOptics(); 
		if (optic.HasEnergyManager()) 	optic.GetCompEM().SwitchOff();
		player.GetAimingModel().SetAimNoiseAllowed(true);
	}
}