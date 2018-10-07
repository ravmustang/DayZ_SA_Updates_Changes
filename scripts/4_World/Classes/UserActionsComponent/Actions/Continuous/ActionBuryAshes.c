class ActionBuryAshesCB : ActionContinuousBaseCB
{
	private const float TIME_TO_REPEAT_CHECK = 1;
	
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTimeBuryAshes( UATimeSpent.BURY_ASHES, TIME_TO_REPEAT_CHECK );
	}
}

class ActionBuryAshes: ActionContinuousBase
{
	string 	m_ReasonToCancel;
	
	void ActionBuryAshes()
	{
		m_CallbackClass = ActionBuryAshesCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DIGSHOVEL;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageStartFail = "I can't bury it here.";
		m_MessageStart = "I've started burying the ashes.";
		m_MessageSuccess = "I have buried the ashes.";
		m_MessageFail = "I couldn't bury the ashes.";
		m_MessageCancel = "I've stopped burying the ashes.";
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_LOW;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_BURY_ASHES;
	}

	override string GetText()
	{
		return "Bury ashes";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		FireplaceBase fireplace_target = FireplaceBase.Cast( target.GetObject() );
		
		if ( fireplace_target && fireplace_target.HasAshes() )
		{
			return true;
		}
		
		return false;
	}

	override void OnCompleteServer( ActionData action_data )
	{
		//destroy fireplace with ashes
		GetGame().ObjectDelete( action_data.m_Target.GetObject() );

		//add soft skill specialty
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( UASoftSkillsWeight.ROUGH_LOW );	
	}
	
	override void OnCancelServer( ActionData action_data  )
	{
		SendMessageToClient( action_data.m_Player, m_ReasonToCancel );
	}
	
	void SetReasonToCancel( string reason )
	{
		m_ReasonToCancel = reason;
	}	
}