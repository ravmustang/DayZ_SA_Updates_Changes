class ActionMeasureTemperatureTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.MEASURE_TEMP);	
	}
};

class ActionMeasureTemperatureTarget : ActionContinuousBase
{
	void ActionMeasureTemperatureTarget()
	{
		m_CallbackClass = ActionMeasureTemperatureTargetCB;
		m_MessageStartFail = "There's nothing to measure.";
		m_MessageStart = "Player started messuring you.";
		m_MessageSuccess = "Player finished measuring you.";
		m_MessageFail = "Player moved and measuring was canceled.";
		m_MessageCancel = "You stopped measuring.";
		//m_Animation = "measure";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_SALINEBLOODBAGTARGET;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_MEASURE_TEMPERATURE_T;
	}
		
	override string GetText()
	{
		return "#measure_temperature";
	}

	override void OnFinishProgressServer( ActionData action_data )
	{	
		PlayerBase ntarget = PlayerBase.Cast( action_data.m_Target.GetObject() );
		Thermometer thermometer = Thermometer.Cast(action_data.m_MainItem);
		
		if(thermometer)
		{
			SendMessageToClient(action_data.m_Player, thermometer.GetTemperatureMessage(ntarget));
		}
		
	}
};