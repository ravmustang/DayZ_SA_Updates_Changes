class ActiondeployObjectCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_DEPLOY);
	}
	
	override void EndActionComponent()
	{
		super.EndActionComponent();
		
		if (m_ActionData.m_State == UA_CANCEL )
		{
			m_Canceled = true;
 			SetCommand(DayZPlayerConstants.CMD_ACTIONINT_END2);
			return;
		}
	}
};

class ActionDeployObject: ActionPlaceObject
{		
	void ActionDeployObject()
	{
		m_CallbackClass	= ActiondeployObjectCB;
		m_SpecialtyWeight 	= UASoftSkillsWeight.PRECISE_LOW;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}
	
	override int GetType()
	{
		return AT_DEPLOY_OBJECT;
	}

	override bool HasProgress()
	{
		return true;
	}
	
	override string GetText()
	{
		return "Deploy object";
	}
	
	override bool ActionConditionContinue( ActionData action_data )
	{
		return true;
	}
	
	override void OnStartClient( ActionData action_data )
	{		
		super.OnStartClient( action_data );
		
		EntityAI entity_for_placing = EntityAI.Cast( action_data.m_MainItem );
		action_data.m_Player.PredictiveDropEntity( entity_for_placing );
	}
	
	override void OnStartServer( ActionData action_data )
	{
		super.OnStartServer( action_data );
		
		EntityAI entity_for_placing = EntityAI.Cast( action_data.m_MainItem );
		
		if( GetGame().IsMultiplayer() )
		{
			GetGame().AddActionJuncture( action_data.m_Player, entity_for_placing, 10000 );
			action_data.m_MainItem.SetIsBeingPlaced( true );
		}
		else
		{
			//local singleplayer
			action_data.m_Player.LocalDropEntity( entity_for_placing );
		}
	}
		
	override void OnCancelClient( ActionData action_data  )
	{
		super.OnCancelClient( action_data );
		
		EntityAI entity_for_placing = EntityAI.Cast( action_data.m_MainItem );
		action_data.m_Player.PredictiveTakeEntityToHands( entity_for_placing );
	}
	
	override void OnCancelServer( ActionData action_data  )
	{
		super.OnCancelServer( action_data );
		
		EntityAI entity_for_placing = EntityAI.Cast( action_data.m_MainItem );
		action_data.m_MainItem.SetIsBeingPlaced( false );
		
		if( !GetGame().IsMultiplayer() )
		{	
			//local singleplayer
			action_data.m_Player.LocalTakeEntityToHands( entity_for_placing );
		}
	}
	
	override void OnCompleteServer( ActionData action_data )
	{	
		super.OnCompleteServer( action_data );
		
		EntityAI entity_for_placing = EntityAI.Cast( action_data.m_MainItem );
		GetGame().ClearJuncture( action_data.m_Player, entity_for_placing );
		action_data.m_MainItem.SetIsBeingPlaced( false );
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};
