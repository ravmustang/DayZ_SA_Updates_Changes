class ActionMeasureTemperatureSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.MEASURE_TEMP);
	}
};

class ActionMeasureTemperatureSelf: ActionContinuousBase
{
	void ActionMeasureTemperatureSelf()
	{
		m_CallbackClass = ActionMeasureTemperatureSelfCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_TAKETEMPSELF;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_TAKETEMPSELF;
		//m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;

		m_MessageStartFail = "There's nothing to measure.";
		m_MessageStart = "I have started measuring myself";
		m_MessageSuccess = "I have measured myself.";
		m_MessageFail = "I have moved and measuring was canceled.";
		m_MessageCancel = "I stopped measuring.";
		//m_Animation = "measure";
		
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}
	
	override bool HasProneException()
	{
		return true;
	}
	
	override int GetType()
	{
		return AT_MEASURE_TEMPERATURE_S;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "Measure temperature";
	}

	override void OnCompleteServer( ActionData action_data )
	{	
		if (action_data.m_Player.GetStatTemperature())
		{
			float temperature = Math.Floor(action_data.m_Player.GetStatTemperature().Get()*10)/10;
			string message = "Thermometer displays "+temperature.ToString()+" ?C";
			SendMessageToClient(action_data.m_Player, message);
		}
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};