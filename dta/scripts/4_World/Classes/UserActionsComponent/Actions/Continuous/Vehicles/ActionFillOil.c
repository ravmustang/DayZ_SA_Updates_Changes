class ActionFillOilCB : ActionContinuousBaseCB
{
	private const float TIME_TO_REPEAT = 0.05;

	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousFillOil( UAQuantityConsumed.POUR_LIQUID, TIME_TO_REPEAT );
	}
};

class ActionFillOil: ActionContinuousBase
{
	void ActionFillOil()
	{
		m_CallbackClass = ActionFillOilCB;
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
		return AT_FILL_OIL;
	}

	override string GetText()
	{
		return "#refill_car";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		//ItemBase tgt_item = ItemBase.Cast( item.GetObject() );
		//EntityAI tgt_parent = EntityAI.Cast( target.GetParent() );
		EntityAI tgt_entity = EntityAI.Cast( target.GetObject() );

		if( item.GetQuantity() <= 0 )
			return false;

		if( !IsInReach(player, target, UAMaxDistances.DEFAULT) )
			return false;

		Car car = Car.Cast(tgt_entity);
		if( car )
		{
			if( car.GetFluidFraction( CarFluid.OIL ) >= 0.988 )
				return false;

			if( car.IsActionComponentPartOfSelection(target.GetComponentIndex(), "engine") )
				return true;		
		}

		return false;
	}

	override void OnEndAnimationLoopServer( ActionData action_data )
	{	
		if ( action_data.m_MainItem && action_data.m_MainItem.GetQuantity() <= 0.01 )
		{
			action_data.m_MainItem.SetQuantity(0);
		}
	}
};