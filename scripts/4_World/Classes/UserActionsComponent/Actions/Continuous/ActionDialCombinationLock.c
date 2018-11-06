class ActionDialCombinationLockCB : ActionContinuousBaseCB
{
	private const float REPEAT_AFTER_SEC = 0.5;
	
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousRepeat(REPEAT_AFTER_SEC);
	}
}

class ActionDialCombinationLock: ActionContinuousBase
{
	ActionNextCombinationLockDial m_NextDialAction;
	
	void ActionDialCombinationLock()
	{
		m_CallbackClass = ActionDialCombinationLockCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_TUNE;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_ITEM_TUNE;		
		m_MessageStartFail = "I have failed the tunning.";
		m_MessageStart = "I have started the tunning.";
		m_MessageFail = "I have failed the tunning.";
		m_MessageCancel = "I have stopped the tunning.";
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_LOW;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTNone;
		m_ConditionItem = new CCINonRuined;
	}
	
	override bool HasProneException()
	{
		return true;
	}
	
	override int GetType()
	{
		return AT_DIAL_COMBINATION_LOCK;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "#dial_combination_lock" + " " + GetDialNumberText();
	}

	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		m_NextDialAction = ActionNextCombinationLockDial.Cast( player.GetActionManager().GetAction( AT_NEXT_COMBINATION_LOCK_DIAL ) );
		
		return true;
	}

	override void OnFinishProgressServer( ActionData action_data )
	{	
		CombinationLock combination_lock =  CombinationLock.Cast( action_data.m_MainItem );
		combination_lock.DialNextNumber( m_NextDialAction.GetDialIndex() );
	}
	
	protected string GetDialNumberText()
	{
		string dial_text;
		string combination_text = m_NextDialAction.GetCombinationLock().m_Combination.ToString();
		
		//insert zeros to dials with 0 value
		int length_diff = m_NextDialAction.GetCombinationLock().COMBINATION_LENGTH - combination_text.Length();
		for ( int i = 0; i < length_diff; ++i )
		{
			combination_text = "0" + combination_text;
		}
		
		//assemble the whole combination with selected part
		for ( int j = 0; j < m_NextDialAction.GetCombinationLock().COMBINATION_LENGTH; ++j )
		{
			if ( j == m_NextDialAction.GetDialIndex() )
			{
				dial_text += string.Format( "[%1]", combination_text.Get( j ) );
			}
			else
			{
				dial_text += string.Format( " %1 ", combination_text.Get( j ) );
			}
		}
		
		return dial_text;
	}	
}