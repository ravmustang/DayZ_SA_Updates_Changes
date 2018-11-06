class ActionGiveBloodTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousQuantityRepeat(UAQuantityConsumed.BLOOD, UATimeSpent.DEFAULT);
	}
};

class ActionGiveBloodTarget: ActionContinuousBase
{
	void ActionGiveBloodTarget()
	{
		m_CallbackClass = ActionGiveBloodTargetCB;
		m_MessageStartFail = "It is empty.";
		m_MessageStart = "Player started giving you blood.";
		m_MessageSuccess = "Player finished giving you blood.";
		m_MessageFail = "Player moved and giving you blood was canceled.";
		m_MessageCancel = "You stopped giving blood.";
		//m_Animation = "giveblood";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_HIGH;
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_SALINEBLOODBAGTARGET;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_GIVE_BLOOD_T;
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
			MiscGameplayFunctions.TurnItemIntoItemEx(action_data.m_Player, new SyringeLambda(action_data.m_MainItem, "Syringe", action_data.m_Player));
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
	
	override void OnFinishProgressServer(ActionData action_data)
	{
		PlayerBase ntarget = PlayerBase.Cast( action_data.m_Target.GetObject() );
		action_data.m_MainItem.TransferModifiers(ntarget);
		Param1<float> nacdata = Param1<float>.Cast( action_data.m_ActionComponent.GetACData() );
		float delta = nacdata.param1;
		
		ntarget.AddHealth("","Blood",delta);
		
		int itembloodtype = action_data.m_MainItem.GetLiquidType();
		int bloodtypetarget = ntarget.GetStatBloodType().Get();
		bool bloodmatch = BloodTypes.MatchBloodCompatibility(itembloodtype, bloodtypetarget);

		if ( !bloodmatch )
		{
			ntarget.m_ModifiersManager.ActivateModifier(eModifiers.MDF_HEMOLYTIC_REACTION);
		}
		
		if ( action_data.m_MainItem && action_data.m_MainItem.GetQuantity() <= 0.01 )
		{
			action_data.m_MainItem.SetQuantity(0, false, false);
		}
	}
	
	override void OnEndServer(ActionData action_data)
	{
		OnFinishProgressServer(action_data);
	}
};