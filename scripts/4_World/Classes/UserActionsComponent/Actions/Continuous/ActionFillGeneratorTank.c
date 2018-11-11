class ActionFillGeneratorTankCB : ActionContinuousBaseCB
{
	private const float QUANTITY_FILLED_PER_SEC = 400;
	
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousFillPowerGenerator(QUANTITY_FILLED_PER_SEC, LIQUID_GASOLINE);
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
		return "#fill_the_tank";
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
};