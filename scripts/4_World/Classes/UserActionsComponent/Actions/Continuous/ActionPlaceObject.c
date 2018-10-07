class PlaceObjectActionData : ActionData
{
	vector m_Position;
	vector m_Orientation;
}

class ActionPlaceObjectCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_PLACE);
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

class ActionPlaceObject: ActionContinuousBase
{		
	void ActionPlaceObject()
	{
		m_CallbackClass		= ActionPlaceObjectCB;
		m_CommandUID		= 0;
		m_FullBody			= true;
		m_StanceMask		= DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
	}
		
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_PLACE_OBJECT;
	}

	override bool HasTarget()
	{
		return false;
	}

	override bool HasProgress()
	{
		return false;
	}
	
	override string GetText()
	{
		return "Place object";
	}

	override ActionData CreateActionData()
	{
		PlaceObjectActionData action_data = new PlaceObjectActionData;
		return action_data;
	}
	
	override bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extraData = NULL)
	{	
		SetupAnimation( item );
		
		if( super.SetupAction(player, target, item, action_data, extraData ))
		{
			if (!GetGame().IsMultiplayer() || GetGame().IsClient() )
			{
				PlaceObjectActionData poActionData;
				poActionData = Class.Cast(action_data);
			
				poActionData.m_Position = player.GetHologramLocal().GetProjectionPosition();
				poActionData.m_Orientation = player.GetHologramLocal().GetProjectionOrientation();
			
				poActionData.m_Player.SetLocalProjectionPosition( poActionData.m_Position );
				poActionData.m_Player.SetLocalProjectionOrientation( poActionData.m_Orientation );
			}
			
			return true;
		}
		return false;
	}
	
	
	override bool Can( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		//Client
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			if ( player.IsPlacingLocal() )
			{		
				if ( !player.GetHologramLocal().IsColliding() )
				{
					if ( item.CanBePlaced(player, player.GetHologramLocal().GetProjectionEntity().GetPosition()) )
					{
						return true;
					}
				}
			}
			return false;
		}
		//Server
		return true; 
	}
	
	override void OnStartClient( ActionData action_data )
	{		
		action_data.m_Player.PlacingCompleteLocal();
	}
	
	override void OnStartServer( ActionData action_data )
	{
		if( GetGame().IsMultiplayer() )
		{
			PlaceObjectActionData poActionData;
			poActionData = Class.Cast(action_data);
			
			poActionData.m_Player.SetLocalProjectionPosition( poActionData.m_Position );
			poActionData.m_Player.SetLocalProjectionOrientation( poActionData.m_Orientation );
			poActionData.m_Player.PlacingStartServer();
		}
		else
		{
			//local singleplayer
			action_data.m_Player.GetHologramLocal().SetUpdatePosition( false );
		}
	}
		
	override void OnCancelClient( ActionData action_data  )
	{
		//local singleplayer
		action_data.m_Player.PlacingCancelLocal();
	}
		
	override void OnCancelServer( ActionData action_data  )
	{
		if( GetGame().IsMultiplayer() )
		{	
			action_data.m_Player.PlacingCancelServer();
		}
		else
		{
			//local singleplayer
			action_data.m_Player.PlacingCancelLocal();
		}
	}
	
	override void OnInterruptClient( ActionData action_data  )
	{
		//local singleplayer
		action_data.m_Player.PlacingCancelLocal();
	}

	override void OnInterruptServer( ActionData action_data  )
	{		
		if( GetGame().IsMultiplayer() )
		{	
			action_data.m_Player.PlacingCancelServer();
		}
		else
		{
			//local singleplayer
			action_data.m_Player.PlacingCancelLocal();
		}
	}

	override void OnCompleteLoopClient( ActionData action_data )
	{	
		EntityAI entity_for_placing = EntityAI.Cast( action_data.m_MainItem );
		vector position = action_data.m_Player.GetLocalProjectionPosition();
		vector orientation = action_data.m_Player.GetLocalProjectionOrientation();
		vector rotation_matrix[3];
		float direction[4];
		InventoryLocation source = new InventoryLocation;
		InventoryLocation destination = new InventoryLocation;
		
		Math3D.YawPitchRollMatrix( orientation, rotation_matrix );
		Math3D.MatrixToQuat( rotation_matrix, direction );
		
		if (entity_for_placing.GetInventory().GetCurrentInventoryLocation( source ))
		{
			destination.SetGroundEx( entity_for_placing, position, direction );
			//entity_for_placing.PredictiveTakeToDst( source, destination );
			action_data.m_Player.GetInventory().TakeToDst(InventoryMode.PREDICTIVE, source, destination);
		}
	}

	override void OnCompleteLoopServer( ActionData action_data )
	{			
		EntityAI entity_for_placing = EntityAI.Cast( action_data.m_MainItem );
		
		if ( GetGame().IsMultiplayer() )
		{
			action_data.m_Player.GetHologramServer().PlaceEntity( entity_for_placing, action_data.m_Player.GetLocalProjectionPosition(), action_data.m_Player.GetLocalProjectionOrientation() );
			action_data.m_Player.GetHologramServer().CheckPowerSource();
			action_data.m_Player.PlacingCompleteServer();
		}			
		
		//local singleplayer
		if ( !GetGame().IsMultiplayer() )
		{			
			vector position = action_data.m_Player.GetHologramLocal().GetProjectionPosition();
			vector orientation = action_data.m_Player.GetHologramLocal().GetProjectionOrientation();
			vector rotation_matrix[3];
			float direction[4];
			InventoryLocation source = new InventoryLocation;
			InventoryLocation destination = new InventoryLocation;
			
			Math3D.YawPitchRollMatrix( orientation, rotation_matrix );
			Math3D.MatrixToQuat( rotation_matrix, direction );
		
			if ( entity_for_placing.GetInventory().GetCurrentInventoryLocation( source ) )
			{
				destination.SetGroundEx( entity_for_placing, position, direction );
				//entity_for_placing.LocalTakeToDst( source, destination );
				action_data.m_Player.GetInventory().TakeToDst(InventoryMode.LOCAL, source, destination);
			}
		
			action_data.m_Player.GetHologramLocal().PlaceEntity( entity_for_placing, action_data.m_Player.GetHologramLocal().GetProjectionPosition(), action_data.m_Player.GetHologramLocal().GetProjectionOrientation() );
			action_data.m_Player.PlacingCompleteLocal();
		}
	}

	override void OnCompleteServer( ActionData action_data )
	{	
		EntityAI entity_for_placing = EntityAI.Cast( action_data.m_MainItem );

		entity_for_placing.OnPlacementComplete( action_data.m_Player );
	}
	
	override void WriteToContext(ParamsWriteContext ctx, ActionData action_data)
	{
		PlaceObjectActionData poActionData;
		poActionData = Class.Cast(action_data);

		ctx.Write( poActionData.m_Position );
		ctx.Write( poActionData.m_Orientation );
	}
	override bool ReadFromContext(ParamsReadContext ctx, ActionData action_data )
	{
		PlaceObjectActionData poActionData;
		poActionData = Class.Cast(action_data);
		
		vector entity_position = "0 0 0";
		vector entity_orientation = "0 0 0";
		if (!ctx.Read(entity_position))
			return false;
		if (!ctx.Read(entity_orientation))
			return false;
		
		poActionData.m_Position = entity_position;
		poActionData.m_Orientation = entity_orientation;
		
		return true;
	}
	
	void SetupAnimation( ItemBase item )
	{
		if ( item.IsDeployable() )
		{
			if ( item.IsHeavyBehaviour() )
			{
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DEPLOY_HEAVY;
			}
			else if ( item.IsOneHandedBehaviour() )
			{
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DEPLOY_1HD; 
			}
			else if ( item.IsTwoHandedBehaviour() )
			{
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DEPLOY_2HD;
			}
			else
			{
				Print("check " + item + " behaviour");
			}
		}
		else
		{
			if ( item.IsHeavyBehaviour() )
			{
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_PLACING_HEAVY;
			}
			else if ( item.IsOneHandedBehaviour() )
			{
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_PLACING_1HD; 
			}
			else if ( item.IsTwoHandedBehaviour() )
			{
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_PLACING_2HD;
			}
			else
			{
				Print("check " + item + " behaviour");
			}
		}
	}
};
