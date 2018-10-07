class ActionFillGeneratorTankCB : ActionContinuousBaseCB
{
	private const float QUANTITY_FILLED_PER_SEC = 50;
	
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousFill(QUANTITY_FILLED_PER_SEC, LIQUID_GASOLINE);
	}
};

class ActionFillGeneratorTank: ActionContinuousBase
{	
	void ActionFillGeneratorTank()
	{
		m_CallbackClass = ActionFillGeneratorTankCB;
		m_MessageStartFail = "There's nothing left.";
		m_MessageStart = "I am filling the tank.";
		m_MessageSuccess = "I've poured fuel into generator.";
		m_MessageFail = "I've stopped filling the tank.";
		m_MessageCancel = "I've stopped filling the tank.";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_FILL_GENERATOR_TANK;
	}
		
	override string GetText()
	{
		return "Fill the tank";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		PowerGenerator pg;
		if ( Class.CastTo(pg,target.GetObject()) )
		{
			string selection = pg.GetActionComponentName(target.GetComponentIndex());
			
			if ( pg.CanAddFuel( item )  &&  selection == "fuel_tank" )
			{
				return true;
			}
		}
		
		return false;
	}
	
	override void OnCompleteServer( ActionData action_data )
	{
		Object targetObject = action_data.m_Target.GetObject();
		PowerGenerator generator = PowerGenerator.Cast(targetObject);
		Param1<float> nacdata = Param1<float>.Cast( action_data.m_ActionComponent.GetACData() );
		int consumed_fuel = generator.AddFuel( nacdata.param1 );
		action_data.m_MainItem.AddQuantity( -consumed_fuel );

		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};