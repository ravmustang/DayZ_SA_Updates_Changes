class ActionCollectSampleTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.COLLECT_SAMPLE);
	}
};

class ActionCollectSampleTarget : ActionContinuousBase
{
	void ActionCollectSampleTarget()
	{
		m_CallbackClass = ActionCollectSampleTargetCB;
		m_MessageStartFail = "iT'S full.";
		m_MessageStart = "Player started collectin sample of your blood.";
		m_MessageSuccess = "Player finished collecting sample of your blood.";
		m_MessageFail = "Player moved and collecting of sample was canceled.";
		m_MessageCancel = "You stopped collecting.";
		//m_Animation = "splint";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_SALINEBLOODBAGTARGET;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_MEDIUM;
	}
	
	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_COLLECT_SAMPLE_T;
	}

	override string GetText()
	{
		return "Collect sample";
	}

	override void OnCompleteServer( ActionData action_data )
	{
		PlayerBase ntarget = PlayerBase.Cast( action_data.m_Target.GetObject() );
		Param1<float> nacdata;
		Class.CastTo(nacdata,  action_data.m_ActionComponent.GetACData() );
		float delta = (nacdata.param1 / UATimeSpent.COLLECT_SAMPLE);

		ActionCollectBloodTargetLambda lambda = new ActionCollectBloodTargetLambda(action_data.m_MainItem, "BloodSyringe", action_data.m_Player, m_SpecialtyWeight, ntarget, delta);
		action_data.m_Player.ServerReplaceItemInHandsWithNew(lambda);
	}
	
	override void OnCancelServer( ActionData action_data )
	{
		OnCompleteServer(action_data);
	}
};

