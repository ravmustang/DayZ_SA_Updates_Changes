class ActionGiveSalineTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.SALINE);
	}
};

class ActionGiveSalineTarget: ActionContinuousBase
{
	void ActionGiveSalineTarget()
	{
		m_CallbackClass = ActionGiveSalineTargetCB;
		m_MessageStartFail = "Bag is empty.";
		m_MessageStart = "Player started giving you saline.";
		m_MessageSuccess = "Player finished giving you saline.";
		m_MessageFail = "Player moved and giving you saline was canceled.";
		m_MessageCancel = "You stopped giving saline.";
		//m_Animation = "givesaline";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_MEDIUM;
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_SALINEBLOODBAGTARGET;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	override void CreateConditionComponents()  
	{	
		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);	
	}

	override int GetType()
	{
		return AT_GIVE_SALINE_T;
	}
		
	override string GetText()
	{
		return "Give saline";
	}

	override void OnCompleteServer( ActionData action_data )
	{
		//TODO Daniel: integrate ss 2.0
		/*float efficiency = action_data.m_Player.GetQuantityEfficiency( GetType() );
		if ( efficiency == -1 )
		{
			efficiency = 1;
		}*/
		PlayerBase ntarget = PlayerBase.Cast( action_data.m_Target.GetObject() );
		Param1<float> nacdata = Param1<float>.Cast( action_data.m_ActionComponent.GetACData() );
		float delta = nacdata.param1;
		//ntarget.AddHealth("", "Blood", delta);
		//ntarget.m_PlayerStats.m_Blood.Add(nacdata.param1 * efficiency);//BLOOD_REPLACE
		//float delta = nacdata.param1 * efficiency;
		//action_data.m_Player.SetHealth("GlobalHealth", "Blood", action_data.m_Player.GetHealth("GlobalHealth", "Blood") + delta );
		
		ntarget.GetModifiersManager().ActivateModifier(eModifiers.MDF_SALINE);
		
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
		
		action_data.m_MainItem.Delete();
	}
};