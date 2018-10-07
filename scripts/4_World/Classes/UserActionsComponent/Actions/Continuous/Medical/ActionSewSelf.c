class ActionSewSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.SEW_CUTS);
	}
};

class ActionSewSelf: ActionContinuousBase
{
	void ActionSewSelf()
	{
		m_CallbackClass = ActionSewSelfCB;
		m_MessageStartFail = "There's no sewing left.";
		m_MessageStart = "I have started sewing myself";
		m_MessageSuccess = "I have sewed myself.";
		m_MessageFail = "I have moved and sewing was canceled.";
		m_MessageCancel = "I stopped sewing.";
		//m_Animation = "bandage";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_STITCHUPSELF;
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
		return AT_SEW_S;
	}

	override bool HasTarget()
	{
		return false;
	}
			
	override string GetText()
	{
		return "Sew cuts";
	}

	override void OnCompleteServer( ActionData action_data )
	{	
		const float ITEM_DAMAGE = 10;
		float delta = action_data.m_Player.GetSoftSkillManager().SubtractSpecialtyBonus( ITEM_DAMAGE, this.GetSpecialtyWeight() );

		action_data.m_MainItem.AddHealth("GlobalHealth","Health",-delta);
		action_data.m_Player.m_ModifiersManager.DeactivateModifier(eModifiers.MDF_BLEEDING);
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};