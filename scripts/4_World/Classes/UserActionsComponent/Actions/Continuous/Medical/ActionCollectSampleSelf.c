class ActionCollectSampleSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.COLLECT_SAMPLE);
	}
};

class ActionCollectSampleSelf: ActionContinuousBase
{
	void ActionCollectSampleSelf()
	{
		m_CallbackClass = ActionCollectSampleSelfCB;
		m_MessageStartFail = "It's full";
		m_MessageStart = "I have started collecting sample";
		m_MessageSuccess = "I have collected sample.";
		m_MessageFail = "I have moved and fixing was canceled.";
		m_MessageCancel = "I stopped fixing.";
		//m_Animation = "bandage";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_COLLECTBLOODSELF;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_MEDIUM;
	}
	
	override void CreateConditionComponents()  
	{		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	override int GetType()
	{
		return AT_COLLECT_SAMPLE_S;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "Collect sample";
	}

	override void OnCompleteServer( ActionData action_data )
	{
		Param1<float> nacdata;
		Class.CastTo(nacdata,  action_data.m_ActionComponent.GetACData() );
		float delta = (nacdata.param1 / UATimeSpent.COLLECT_SAMPLE);
		
		ActionCollectBloodTargetLambda lambda = new ActionCollectBloodTargetLambda(action_data.m_MainItem, "BloodSyringe", action_data.m_Player, m_SpecialtyWeight, action_data.m_Player, delta);
		action_data.m_Player.ServerReplaceItemInHandsWithNew(lambda);
	}
	
	override void OnCancelServer( ActionData action_data )
	{
		OnCompleteServer(action_data);
	}
};

