class ActionDigStashCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DIG_STASH);
	}
};

class ActionDigStash: ActionContinuousBase
{	
	void ActionDigStash()
	{
		m_CallbackClass = ActionDigStashCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DIGUPCACHE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		
		m_MessageStartFail = "I can't dig here.";
		m_MessageStart = "I've started digging.";
		m_MessageSuccess = "I have stashed the chest.";
		m_MessageFail = "I couldn't dig the hole.";
		m_MessageCancel = "I've stopped digging.";
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_LOW;
	}
	
	override void CreateConditionComponents()  
	{	
		
		m_ConditionTarget = new CCTObject(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_DIG_STASH;
	}

	override string GetText()
	{
		return "Dig stash";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ItemBase target_IB;
		
		// Check if player is standing on terrain
		vector plr_pos = player.GetPosition();
		float height = GetGame().SurfaceY(plr_pos[0], plr_pos[2]);
		height = plr_pos[1] - height;
		
		if ( height > 0.2 )
			return false; // Player is not standing on ground
		
		if ( Class.CastTo(target_IB, target.GetObject()) )
		{			
			// Check surface
			string surface_type;
			vector position = target_IB.GetPosition();
			GetGame().SurfaceGetType ( position[0], position[2], surface_type );
			
			if ( target_IB.ConfigGetBool("canBeDigged") )
			{
				if ( target_IB.IsInherited(UndergroundStash) )
				{
					return true;
				}
				
				if ( GetGame().IsSurfaceSoftGround(surface_type) )
				{
					// Check slope angle
					vector posA = position + "0.5 0 0.5";
					vector posB = position + "-0.5 0 0.5";
					vector posC = position + "0.5 0 -0.5";
					vector posD = position + "-0.5 0 -0.5";
					
					array<vector> positions = new array<vector>;
					positions.Insert( posA );
					positions.Insert( posB );
					positions.Insert( posC );
					positions.Insert( posD );
					
					float difference = GetGame().GetHighestSurfaceYDifference(positions);
					
					if ( difference < 0.6 )
					{
						return true;
					}
				}
			}
		}
		
		return false;
	}

	override void OnExecuteClient( ActionData action_data )
	{
		super.OnExecuteClient( action_data );
		
		SpawnParticleShovelRaise( action_data );
	}
	
	override void OnExecuteServer( ActionData action_data )
	{
		super.OnExecuteServer( action_data );
		
		if ( !GetGame().IsMultiplayer() ) // Only in singleplayer
		{
			SpawnParticleShovelRaise( action_data );
		}
	}
	
	void SpawnParticleShovelRaise( ActionData action_data )
	{
		PlayerBase player = action_data.m_Player;
		Particle.Play( ParticleList.DIGGING_STASH, player );
	}
	
	
	override void OnCompleteServer( ActionData action_data )
	{
		Object targetObject = action_data.m_Target.GetObject();
		UndergroundStash target_stash;
		
		if ( Class.CastTo(target_stash, targetObject) )
		{
			ItemBase chest = target_stash.GetStashedItem();
			
			if (chest)
			{
				DigOutStashLambda lambda(target_stash, "", action_data.m_Player);
				action_data.m_Player.ServerReplaceItemWithNew(lambda);
			}
			else
				g_Game.ObjectDelete( target_stash );
		}
		else
		{
			ItemBase stashed_item;
			UndergroundStash stash;
			vector pos = targetObject.GetPosition();
			
			Class.CastTo(stashed_item,  targetObject );
			Class.CastTo(stash,  GetGame().CreateObject("UndergroundStash", pos, false) );
			  
			if ( stash )
			{
				stash.PlaceOnGround();
				action_data.m_Player.ServerTakeEntityToTargetCargo(stash, stashed_item);
			}
			else
			{
				Print("ERROR! Stash not spawned!");
			}
		}

		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};

class DigOutStashLambda : DropEquipAndDestroyRootLambda
{
	void DigOutStashLambda (EntityAI old_item, string new_item_type, PlayerBase player)
	{ }

	override void CopyOldPropertiesToNew (notnull EntityAI old_item, EntityAI new_item)
	{
		super.CopyOldPropertiesToNew(old_item, new_item);
	}
};