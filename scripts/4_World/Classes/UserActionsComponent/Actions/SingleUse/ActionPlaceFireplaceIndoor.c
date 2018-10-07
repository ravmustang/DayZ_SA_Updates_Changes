class ActionPlaceFireplaceIndoor: ActionSingleUseBase
{
	private ref FireplacePoint 	m_ActualFireplacePoint;
	private const float 		FIRE_POINT_MAX_DISTANCE = 2;		//maximum distance from the fireplace interact point

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
		return "#place_fireplace";
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
		string action_selection = target_object.GetActionComponentName(target.GetComponentIndex());
		
		if ( target_object && target_object.IsInherited( BuildingWithFireplace ) )
		{
			BuildingWithFireplace building = BuildingWithFireplace.Cast( target_object );
			
			if ( item && building.IsFireplaceActionSelection( action_selection )  )
			{
				FireplacePoint fireplace_point = building.GetFirePointByActionSelection( action_selection );
				string fire_point = fireplace_point.GetFirePoint();
				vector fire_point_pos = building.GetSelectionPosition( fire_point );
				vector fire_point_pos_world = building.ModelToWorld( fire_point_pos );				
				float fire_point_dist = vector.Distance( fire_point_pos_world, player.GetPosition() );
				
				//check point availability and distance from action_data.m_Player
				if ( building.IsPointAvailable( action_selection ) && fire_point_dist <= FIRE_POINT_MAX_DISTANCE )
				{
					return true;
				}
			}			
		}
		
		return false;
	}
	
	override void OnStartServer( ActionData action_data )
	{
		//get actual action_selection
		BuildingWithFireplace building = BuildingWithFireplace.Cast( action_data.m_Target.GetObject() );
		string action_selection = building.GetActionComponentName(action_data.m_Target.GetComponentIndex());
		
		//get and set FireplacePoint reference
		m_ActualFireplacePoint = building.GetFirePointByActionSelection( action_selection );
	}
	
	override void OnCompleteServer( ActionData action_data )
	{	
		FireplaceBase fireplace_in_hands = FireplaceBase.Cast( action_data.m_MainItem );
		
		//get 'fire point pos' from 'fire point' memory point
		string fire_point = m_ActualFireplacePoint.GetFirePoint();
		BuildingWithFireplace building = BuildingWithFireplace.Cast( action_data.m_Target.GetObject() );
		//
		//set position and direction to fireplace_pointX
		vector fire_point_pos = building.GetSelectionPosition( fire_point );
		vector fire_point_pos_world = building.ModelToWorld( fire_point_pos );

		FireplaceToIndoorsLambda lambda = new FireplaceToIndoorsLambda(fireplace_in_hands, "FireplaceIndoor", action_data.m_Player, fire_point_pos_world);
		lambda.SetTransferParams(true, true, true);
		action_data.m_Player.ServerReplaceItemInHandsWithNewElsewhere(lambda);

		//add to soft skills
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
}


class FireplaceToIndoorsLambda : TurnItemIntoItemLambda
{
	vector m_Pos;

	void FireplaceToIndoorsLambda (EntityAI old_item, string new_item_type, PlayerBase player, vector pos)
	{
		m_Pos = pos;

		InventoryLocation gnd = new InventoryLocation;
		vector mtx[4];
		Math3D.MatrixIdentity4(mtx);
		mtx[3] = pos;
		gnd.SetGround(NULL, mtx);
		OverrideNewLocation(gnd);
	}

	override void CopyOldPropertiesToNew (notnull EntityAI old_item, EntityAI new_item)
	{
		super.CopyOldPropertiesToNew(old_item, new_item);
	}
};
