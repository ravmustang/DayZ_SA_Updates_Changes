class ActionCoverHeadTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.COVER_HEAD);
	}
};

class ActionCoverHeadTarget: ActionContinuousBase
{	
	void ActionCoverHeadTarget()
	{
		m_CallbackClass = ActionCoverHeadTargetCB;
		m_MessageStartFail = "There's nothing left.";
		m_MessageStart = "Player started putting sack on your head.";
		m_MessageSuccess = "Player finished putting sack on your head.";
		m_MessageFail = "Player moved and putting sack on was canceled.";
		m_MessageCancel = "You stopped putting sack on.";
		//m_Animation = "INJECTEPIPENS";
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_LOW;
	}
	
	override void CreateConditionComponents()  
	{	
		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);		
	}

	override int GetType()
	{
		return AT_COVER_HEAD_T;
	}
		
	override string GetText()
	{
		return "Put on targets head";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		PlayerBase targetPlayer;
		Class.CastTo(targetPlayer, target.GetObject());
		if ( !IsWearingHeadgear(targetPlayer) ) 
		{
			return true;
		}
		
		return false;
	}

	override void OnCompleteServer( ActionData action_data )
	{	
		//setaperture will be called from here, or from statemachine
		PlayerBase ntarget;
		Class.CastTo(ntarget, action_data.m_Target.GetObject());
		ntarget.GetInventory().CreateInInventory("BurlapSackCover");
		//ntarget.CreateInInventory("BurlapSackCover","cargo_headgear");
		action_data.m_MainItem.TransferModifiers(ntarget);
		action_data.m_MainItem.Delete();

		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}

	bool IsWearingHeadgear( PlayerBase player)
	{
		if ( player.GetInventory().FindAttachment(InventorySlots.HEADGEAR) )
		{
			return true;
		}
		return false;		
	}
};