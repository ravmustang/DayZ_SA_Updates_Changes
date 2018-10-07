class ActionIgniteFireplaceCB : ActionContinuousBaseCB
{
	private const float TIME_TO_REPEAT_CHECK = 1;
	
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTimeIgnite( UATimeSpent.FIREPLACE_IGNITE, TIME_TO_REPEAT_CHECK );
	}
}

class ActionIgniteFireplace: ActionContinuousBase
{
	string 	m_ReasonToCancel;
	float 	m_HandDrillDamagePerUse = 20;
	float 	m_MatchSpentPerUse = 1;
	bool 	m_SkipKindlingCheck = false;
	
	protected ref Timer m_ClutterCutterDestroyTimer;
	protected Object clutter_cutter;
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
		m_ConditionItem = new CCINonRuined;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object target_object = target.GetObject();
		EntityAI target_entity = EntityAI.Cast( target_object ); 
		
		if ( target_object && target_object.IsFireplace() && !target_entity.GetHierarchyParent() )
		{	
			FireplaceBase fireplace_target = FireplaceBase.Cast( target_object );
			
			if ( !fireplace_target.IsBurning() ) 
			{
				if ( fireplace_target.IsBarrelWithHoles() ) 		//if barrel with holes
				{
					BarrelHoles_ColorBase barrel = BarrelHoles_ColorBase.Cast( fireplace_target );
					if ( !barrel.IsOpened() )
					{
						return false;
					}
				}
				
				return true;
			}
		}
		
		return false;
	}
	
	override void OnCancelServer( ActionData action_data  )
	{
		SendMessageToClient( action_data.m_Player, m_ReasonToCancel );
	}	
	
	void SetReasonToCancel( string reason )
	{
		m_ReasonToCancel = reason;
	}	

	void DestroyClutterCutterAfterTime()
	{
		m_ClutterCutterDestroyTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		m_ClutterCutterDestroyTimer.Run( 0.3, this, "DestroyClutterCutter", NULL, false );
		
	}
	
	void DestroyClutterCutter()
	{
		GetGame().ObjectDelete( clutter_cutter );
	}	
}