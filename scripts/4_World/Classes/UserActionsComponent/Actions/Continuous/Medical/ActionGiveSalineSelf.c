class ActionGiveSalineSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.SALINE);
	}
};

class ActionGiveSalineSelf: ActionContinuousBase
{	
	void ActionGiveSalineSelf()
	{
		m_CallbackClass = ActionGiveSalineSelfCB;
		m_MessageStartFail = "There's nothing in it.";
		m_MessageStart = "I have started giving me saline";
		m_MessageSuccess = "I have salined myself.";
		m_MessageFail = "I have moved and giving saline was canceled.";
		m_MessageCancel = "I stopped giving saline myself.";
		//m_Animation = "givesaline";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_MEDIUM;
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_SALINEBLOODBAG;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	override int GetType()
	{
		return AT_GIVE_SALINE_S;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "Give saline";
	}

	override void OnCompleteServer( ActionData action_data )
	{		
		action_data.m_MainItem.TransferModifiers(action_data.m_Player);
		Param1<float> nacdata = Param1<float>.Cast( action_data.m_ActionComponent.GetACData() );		
		float delta = nacdata.param1;
		//action_data.m_Player.SetHealth("GlobalHealth", "Blood", action_data.m_Player.GetHealth("GlobalHealth", "Blood") + delta );
		//action_data.m_Player.AddHealth("", "Blood", delta);
		action_data.m_Player.GetModifiersManager().ActivateModifier(eModifiers.MDF_SALINE);
			action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
		action_data.m_MainItem.Delete();
	}
};