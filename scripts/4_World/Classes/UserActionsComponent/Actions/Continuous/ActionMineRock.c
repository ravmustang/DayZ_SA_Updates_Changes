class ActionMineRockCB : ActionContinuousBaseCB
{
	private const float TIME_BETWEEN_MATERIAL_DROPS = 8;
	
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousMineRock(TIME_BETWEEN_MATERIAL_DROPS);
	}
};

class ActionMineRock: ActionContinuousBase
{
	void ActionMineRock()
	{
		m_CallbackClass = ActionMineRockCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_MINEROCK;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageStartFail = "My tool is too damaged to mine.";
		m_MessageStart = "I started mining the rock.";
		m_MessageSuccess = "I have mined the rock.";
		m_MessageFail = "My tool is ruined.";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_HIGH;
	}
	
	override void CreateConditionComponents()  
	{		
		m_ConditionTarget = new CCTTree(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		if ( targetObject.IsRock() )
		{
			return true;
		}
		return false;
	}

	override int GetType()
	{
		return AT_MINE_ROCK;
	}
		
	override string GetText()
	{
		return "Mine";
	}


	override void OnCompleteServer( ActionData action_data )
	{	
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};