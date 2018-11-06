class ActionGiveBloodSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousQuantityRepeat(UAQuantityConsumed.BLOOD, UATimeSpent.DEFAULT);
	}
};

class ActionGiveBloodSelf: ActionContinuousBase
{
	void ActionGiveBloodSelf()
	{
		m_CallbackClass = ActionGiveBloodSelfCB;
		m_MessageStartFail = "There's no blood left.";
		m_MessageStart = "I have started giving blood myself";
		m_MessageSuccess = "I gave myself blood.";
		m_MessageFail = "I have moved and giving blood was canceled.";
		m_MessageCancel = "I stopped giving self a blood.";
		//m_Animation = "GIVEBLOODS";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_HIGH;
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_SALINEBLOODBAG;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINotRuinedAndEmpty;
		m_ConditionTarget = new CCTSelf;
	}

	override int GetType()
	{
		return AT_GIVE_BLOOD_S;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "#give_blood";
	}

	override void OnEndAnimationLoopServer( ActionData action_data )
	{
		OnFinishProgressServer(action_data);

		if ( action_data.m_MainItem.IsKindOf("BloodSyringe") )
		{
			SyringeLambda lambda = new SyringeLambda(action_data.m_MainItem, "Syringe", action_data.m_Player);
			lambda.SetTransferParams(true, true, true);
			MiscGameplayFunctions.TurnItemIntoItemEx(action_data.m_Player, lambda);
		}
		else
		{
			if ( action_data.m_MainItem.GetQuantity() <= 0 )
			{
				GetGame().ObjectDelete( action_data.m_MainItem );
			}	
		}
		
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
	
	override void OnEndServer(ActionData action_data)
	{
		Param1<float> nacdata = Param1<float>.Cast( action_data.m_ActionComponent.GetACData() );
		float delta = nacdata.param1;
		
		action_data.m_Player.AddHealth("","Blood",delta);
		//action_data.m_Player.SetHealth("GlobalHealth", "Blood", action_data.m_Player.GetHealth("GlobalHealth", "Blood") );
		
		int itembloodtype = action_data.m_MainItem.GetLiquidType();
		int bloodtypetarget = action_data.m_Player.GetStatBloodType().Get();
		bool bloodmatch = BloodTypes.MatchBloodCompatibility(itembloodtype, bloodtypetarget);

		if ( !bloodmatch )
		{
			action_data.m_Player.m_ModifiersManager.ActivateModifier(eModifiers.MDF_HEMOLYTIC_REACTION);
		}
		
		if ( action_data.m_MainItem && action_data.m_MainItem.GetQuantity() <= 0.01 )
		{
			action_data.m_MainItem.SetQuantity(0, false, false);
		}
	}
};

class SyringeLambda : TurnItemIntoItemLambda
{
	void SyringeLambda (EntityAI old_item, string new_item_type, PlayerBase player) { }
};