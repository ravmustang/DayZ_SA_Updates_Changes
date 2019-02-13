class ActionPlaceFireplaceIndoor: ActionSingleUseBase
{
	void ActionPlaceFireplaceIndoor()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageStartFail = "";
		m_MessageStart = "";
		m_MessageSuccess = "I placed the fireplace inside.";
		m_MessageFail = "I was unable to place the fireplace inside.";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}
	
	override int GetType()
	{
		return AT_PLACE_FIREPLACE_INDOOR;
	}

	override string GetText()
	{
		return "#place_object";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !target ) return false;
		if ( !IsInReach(player, target, UAMaxDistances.DEFAULT) ) return false;

		Object target_object = target.GetObject();
		string action_selection = target_object.GetActionComponentName( target.GetComponentIndex() );
		BuildingWithFireplace building = BuildingWithFireplace.Cast( target_object );
		
		if ( target_object && building && action_selection.Contains( FireplaceIndoor.FIREPOINT_ACTION_SELECTION ) )
		{
			vector fire_point_pos_world;
			int fire_point_index = FireplaceIndoor.GetFirePointIndex( action_selection );
			if ( FireplaceIndoor.CanPlaceFireplaceInSelectedSpot( building, fire_point_index, fire_point_pos_world ) )
			{
				float fire_point_dist = vector.Distance( fire_point_pos_world, player.GetPosition() );
				if ( fire_point_dist <= 2 )
				{
					player.SetLastFirePoint( fire_point_pos_world );
					player.SetLastFirePointIndex( fire_point_index );
					
					return true;
				}
			}
		}
		
		return false;
	}
		
	override void OnExecuteServer( ActionData action_data )
	{	
		FireplaceBase fireplace_in_hands = FireplaceBase.Cast( action_data.m_MainItem );
		
		//replace fireplace with lambda
		FireplaceToIndoorsLambda lambda = new FireplaceToIndoorsLambda( fireplace_in_hands, "FireplaceIndoor", action_data.m_Player, action_data.m_Player.GetLastFirePoint(), action_data.m_Target.GetObject() );
		lambda.SetTransferParams( true, true, true );
		action_data.m_Player.ServerReplaceItemInHandsWithNewElsewhere( lambda );

		//add to soft skills
		action_data.m_Player.GetSoftSkillsManager().AddSpecialty( m_SpecialtyWeight );
	}
}


class FireplaceToIndoorsLambda : TurnItemIntoItemLambda
{
	int 		m_FirePointIndex;
	vector 		m_SmokePosition;
	
	void FireplaceToIndoorsLambda( EntityAI old_item, string new_item_type, PlayerBase player, vector pos, Object target )
	{
		InventoryLocation gnd = new InventoryLocation;
		vector mtx[4];
		Math3D.MatrixIdentity4( mtx );
		mtx[3] = pos;
		gnd.SetGround( NULL, mtx );
		OverrideNewLocation( gnd );
		
		//set fire point index and smoke point position in world
		m_FirePointIndex = player.GetLastFirePointIndex();
		
		vector smoke_point_pos = target.GetSelectionPositionMS( FireplaceIndoor.FIREPOINT_SMOKE_POSITION + m_FirePointIndex.ToString() );
		vector smoke_point_pos_world = target.ModelToWorld( smoke_point_pos );		
		m_SmokePosition = smoke_point_pos_world;
	}

	override void CopyOldPropertiesToNew (notnull EntityAI old_item, EntityAI new_item)
	{
		super.CopyOldPropertiesToNew( old_item, new_item );
		
		FireplaceIndoor fireplace_indoor = FireplaceIndoor.Cast( new_item );
		if ( fireplace_indoor )
		{
			//set fire point index
			fireplace_indoor.SetFirePointIndex( m_FirePointIndex );
			
			//get fire and smoke position
			fireplace_indoor.SetSmokePointPosition( m_SmokePosition );
			
			//synchronize
			fireplace_indoor.Synchronize();
		}
	}
};
