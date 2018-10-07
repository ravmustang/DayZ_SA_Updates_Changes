class ActionSewTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.SEW_CUTS);
	}
};

class ActionSewTarget: ActionContinuousBase
{
	void ActionSewTarget()
	{
		m_CallbackClass = ActionSewTargetCB;
		m_MessageStartFail = "There's no sewing left.";
		m_MessageStart = "Player started sewing you.";
		m_MessageSuccess = "Player finished sewing you.";
		m_MessageFail = "Player moved and sewing was canceled.";
		m_MessageCancel = "You stopped sewing.";
		//m_Animation = "sew";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
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
		return AT_SEW_T;
	}
		
	override string GetText()
	{
		return "Sew target's cuts";
	}

	override void OnCompleteServer( ActionData action_data )
	{			
		const float ITEM_DAMAGE = 10;
		float delta = action_data.m_Player.GetSoftSkillManager().SubtractSpecialtyBonus( ITEM_DAMAGE, this.GetSpecialtyWeight() );
		PlayerBase ntarget = PlayerBase.Cast( action_data.m_Target.GetObject() );
		
		ntarget.m_ModifiersManager.DeactivateModifier(eModifiers.MDF_BLEEDING);
		action_data.m_MainItem.AddHealth("GlobalHealth","Health",-delta);

		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};