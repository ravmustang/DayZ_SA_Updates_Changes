class ActionDialCombinationLockOnTargetCB : ActionContinuousBaseCB
{
	private const float REPEAT_AFTER_SEC = 0.5;
	
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousRepeat(REPEAT_AFTER_SEC);
	}
}

class ActionDialCombinationLockOnTarget: ActionContinuousBase
{
	ActionNextCombinationLockDialOnTarget m_NextDialAction;
	
	void ActionDialCombinationLockOnTarget()
	{
		m_CallbackClass = ActionDialCombinationLockOnTargetCB;
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
		m_ConditionItem = new CCINotPresent;
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
	}
	
	override bool HasProneException()
	{
		return true;
	}
	
	override int GetType()
	{
		return AT_DIAL_COMBINATION_LOCK_ON_TARGET;
	}

	override string GetText()
	{
		return "#dial_combination_lock_on_target" + " " + GetDialNumberText();
	}

	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		if ( targetObject && targetObject.CanUseConstruction() )
		{
			Fence fence = Fence.Cast( targetObject );
			
			if ( fence && fence.IsLocked() )
			{
				string selection = fence.GetActionComponentName( target.GetComponentIndex() );
				
				if ( selection == "wall_interact" )
				{
					m_NextDialAction = ActionNextCombinationLockDialOnTarget.Cast( player.GetActionManager().GetAction( AT_NEXT_COMBINATION_LOCK_DIAL_ON_TARGET ) );
					
					return true;
				}
			}
		}
		
		return false;
	}

	override void OnFinishProgressServer( ActionData action_data )
	{	
		Fence fence = Fence.Cast( action_data.m_Target.GetObject() );
		CombinationLock combination_lock =  fence.GetCombinationLock();
		
		if ( combination_lock )
		{
			combination_lock.DialNextNumber( m_NextDialAction.GetDialIndex() );
			
			//if not locked, unlock the combination lock
			if ( !combination_lock.IsLockedOnGate() )
			{
				EntityAI target_entity = EntityAI.Cast( action_data.m_Target.GetObject() );
				combination_lock.Unlock( target_entity );
			}
		}
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