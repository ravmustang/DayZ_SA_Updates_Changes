class ActionMineTreeCB : ActionContinuousBaseCB
{
	private const float TIME_BETWEEN_MATERIAL_DROPS = 4;
	
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousMineWood(TIME_BETWEEN_MATERIAL_DROPS);
	} 
};

class ActionMineTree: ActionContinuousBase
{
	void ActionMineTree()
	{
		m_CallbackClass = ActionMineTreeCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_HACKTREE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageStartFail = "My tool is too damaged to cut.";
		m_MessageStart = "I started cutting down the tree.";
		m_MessageSuccess = "I have cut down the tree.";
		m_MessageFail = "My tool is ruined.";
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTTree(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_MINE_TREE;
	}
		
	override string GetText()
	{
		return "#cut_down_tree";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		
		if ( targetObject.IsTree() )
		{ 
			return true;
		}
		
		return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{			
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
		// TODO switch WoodenPlank item for logs, once those are ready
		//ItemBase wooden_logs = ItemBase.Cast(GetGame().CreateObject("WoodenPlank",action_data.m_Player.GetPosition(), false));
		//wooden_logs.SetQuantity(1);
		
		// "hiding" of map item
		//action_data.m_Target.GetObject().SetOrigin("0 0 0");
	}
	
	override void OnFinishProgressClient( ActionData action_data )
	{
		/*if (action_data && action_data.m_Target)
		{
		}*/
	}
};