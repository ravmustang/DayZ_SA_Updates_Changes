class ActionDigGardenPlotCB : ActionPlaceObjectCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DIG_GARDEN);
	}
};

class ActionDigGardenPlot: ActionPlaceObject
{	
	void ActionDigGardenPlot()
	{
		m_CallbackClass	= ActionDigGardenPlotCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DIGSHOVEL;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_LOW;
	}

	override int GetType()
	{
		return AT_DIG_GARDEN_PLOT;
	}
		
	override string GetText()
	{
		return "Make garden plot";
	}
	
	override bool Can ( PlayerBase player, ActionTarget target, ItemBase item )
	{
		//Client
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			if ( player.IsPlacingLocal() )
			{
				Hologram hologram = player.GetHologramLocal();
				GardenPlot_TO_BE_RELEASED item_GP;
				Class.CastTo(item_GP,  hologram.GetProjectionEntity() );	
				CheckSurfaceBelowGardenPlot(player, item_GP, hologram);
	
				if ( !hologram.IsColliding() )
				{
					if ( item.IsInherited(Shovel) )
					{
						m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DIGSHOVEL;
					}	
					else
					{
						m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DIGHOE;
					}
					return true;
				}
			}
			return false;
		}
		//Server
		return true;
	}

	void CheckSurfaceBelowGardenPlot(PlayerBase player, GardenPlot_TO_BE_RELEASED item_GP, Hologram hologram)
	{
		if (item_GP) // TO DO: When GardenPlot_TO_BE_RELEASED is renamed back to GardenPlot then remove this check.
		{
			if ( item_GP.CanBePlaced(player, item_GP.GetPosition() )  )
			{
				if( item_GP.CanBePlaced(NULL, item_GP.CoordToParent(hologram.GetLeftCloseProjectionVector())) )
				{
					if( item_GP.CanBePlaced(NULL, item_GP.CoordToParent(hologram.GetRightCloseProjectionVector())) )
					{
						if( item_GP.CanBePlaced(NULL, item_GP.CoordToParent(hologram.GetLeftFarProjectionVector())) )
						{
							if( item_GP.CanBePlaced(NULL, item_GP.CoordToParent(hologram.GetRightFarProjectionVector())) )
							{
								hologram.SetIsCollidingGPlot( false );
	
								return;
							}
						}
					}
				}
			}
			
			hologram.SetIsCollidingGPlot( true );
		}
	}
};
