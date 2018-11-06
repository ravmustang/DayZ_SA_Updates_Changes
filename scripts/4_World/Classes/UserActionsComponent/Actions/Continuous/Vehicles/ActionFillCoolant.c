class ActionFillCoolantCB : ActionContinuousBaseCB
{
	private const float TIME_TO_REPEAT = 0.075;

	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousFillCoolant( UAQuantityConsumed.FUEL, TIME_TO_REPEAT );
	}
};

class ActionFillCoolant: ActionContinuousBase
{
	void ActionFillCoolant()
	{
		m_CallbackClass = ActionFillCoolantCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_EMPTY_VESSEL;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_FullBody = true;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		m_LockTargetOnUse = false;

		m_MessageStartFail = "";
		m_MessageStart = "";
		m_MessageSuccess = "";
		m_MessageFail = "";
		m_MessageCancel = "";

	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_FILL_COOLANT;
	}

	override string GetText()
	{
		return "#refill_car";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		ItemBase tgt_item = ItemBase.Cast( target.GetObject() );
		EntityAI tgt_parent = EntityAI.Cast( target.GetParent() );
		EntityAI tgt_entity = EntityAI.Cast( target.GetObject() );
		
		if( !tgt_item )
			return false;

		if( tgt_item.GetType() != "CarRadiator" )
			return false;
		
		//if( !IsTransport(tgt_parent) )
			//return false;

		if( item.GetQuantity() <= 0 )
			return false;
		
		if( item.GetLiquidType() != LIQUID_WATER && item.GetLiquidType() != LIQUID_RIVERWATER )
			return false;
		
		if( !IsInReach(player, target, UAMaxDistances.DEFAULT) )
			return false;

		Car car = Car.Cast(target.GetObject());
		if( car && car.GetFluidFraction( CarFluid.COOLANT ) >= 0.98 )
			return false;

		/*
		if( car.IsActionComponentPartOfSelection(target.GetComponentIndex(), "radiator") )
		{
			// not full tank con &&
			return true;
		}
		*/
		
		return true;
	}
/*
	override void OnCompleteServer( ActionData action_data )
	{
		Car car = Car.Cast(action_data.m_Target.GetObject());
		Param1<float> nacdata;
		Class.CastTo(nacdata,  action_data.m_ActionComponent.GetACData() );
		float amount = nacdata.param1;
		if ( car && action_data.m_MainItem && action_data.m_MainItem.GetQuantity() <= UAQuantityConsumed.FUEL )
		{
			action_data.m_MainItem.AddQuantity( -amount );
			car.Fill( CarFluid.FUEL, amount );

			action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
		}
	}
*/
};