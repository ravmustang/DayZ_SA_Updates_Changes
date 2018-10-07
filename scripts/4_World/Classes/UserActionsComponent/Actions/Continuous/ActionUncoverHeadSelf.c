class ActionUncoverHeadSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.COVER_HEAD);
	}
};

class ActionUncoverHeadSelf: ActionContinuousBase
{
	void ActionUncoverHeadSelf()
	{
		m_CallbackClass = ActionUncoverHeadSelfCB;
		m_MessageStartFail = "";
		m_MessageStart = "I have started removing the sack";
		m_MessageSuccess = "I have removed the sack.";
		m_MessageFail = "I have moved and removing sack was canceled.";
		m_MessageCancel = "I stopped removing the sack.";
		//m_Animation = "INJECTEPIPENS";
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_UNCOVER_HEAD_S;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "Uncover head";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if ( IsWearingBurlap(player) ) 
			return true;

		return false;
	}

	override void OnCompleteServer( ActionData action_data )
	{	
		action_data.m_Player.GetInventory().CreateInInventory("BurlapSack");
		
		EntityAI attachment;
		Class.CastTo(attachment, action_data.m_Player.GetInventory().FindAttachment(InventorySlots.HEADGEAR));
		if ( attachment && attachment.GetType() == "BurlapSackCover" )
		{
			attachment.Delete();
		}
		
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}


	bool IsWearingBurlap( PlayerBase player )
	{
		EntityAI attachment;
		Class.CastTo(attachment, player.GetInventory().FindAttachment(InventorySlots.HEADGEAR));
		if ( attachment && attachment.GetType() == "BurlapSackCover" )
		{
			return true;
		}
		return false;		
	}
};