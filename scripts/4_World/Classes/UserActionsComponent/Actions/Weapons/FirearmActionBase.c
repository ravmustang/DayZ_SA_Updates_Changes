/*class ActionData
{
	ActionBase							m_Action;
	ItemBase							m_MainItem;
	ActionBaseCB 						m_Callback;
	ref CABase							m_ActionComponent;
	int									m_State;
	ref ActionTarget					m_Target;
	PlayerBase							m_Player;
	int 								m_PossibleStanceMask;
	ref array<ref InventoryLocation>	m_ReservedInventoryLocations;
}*/

class FirearmActionBase : ActionBase
{	
	//-----------------------------------------------------
	// 	Action events and methods
	//-----------------------------------------------------
	void FirearmActionBase() 
	{
	}
	
	/*override void OnUpdate(ActionData action_data)
	{
		if(!ActionConditionContinue(action_data))
		{
			action_data.m_Player.GetActionManager().OnActionEnd();
		}
	}*/
	
	/*bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extraData = NULL)
	{
		action_data = CreateActionData();
		action_data.m_Action = this;
		action_data.m_Player = player;
		action_data.m_Target = target;
		action_data.m_MainItem = item;
		action_data.m_PossibleStanceMask = GetStanceMask(player);
		action_data.m_ReservedInventoryLocations = new array<ref InventoryLocation>;
		
		if (!GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			if (!InventoryReservation(action_data))
				return false;
		}
		
		return true;
	}*/
	
	/*ActionData CreateActionData()
	{
		ActionData action_data = new ActionData;
		return action_data;
	}*/
			
	/*void CreateConditionComponents() //Instantiates components, called once from ActionManager on actions construction
	{
		m_ConditionItem = new CCIDummy;
		m_ConditionTarget = new CCTDummy;
	}*/
	
	override int GetStanceMask(PlayerBase player)
	{
		return DayZPlayerConstants.STANCEMASK_ALL;
	}
	
	void OnWeaponFsmEnd(ActionData action_data)
	{
		action_data.m_Player.GetActionManager().OnActionEnd();
	}
	
	
	override int GetActionCategory()
	{
		return AC_SINGLE_USE;
	}
	
	/*override int GetType()  //returns action uid
	{
		return AT_LOAD_MAGAZINE_TO_WEAPON;
	}*/

	/*override string GetText() //text game displays in HUD hint 
	{
		return "Attachchchchchch magazine";
	}*/
	
	/*string GetTargetDescription()
	{
		return "default target description";
	}*/
	
	override bool ActionConditionContinue( ActionData action_data ) //condition for action
	{
		Weapon_Base wpn = Weapon_Base.Cast(action_data.m_MainItem);
		return !wpn.IsIdle();
	}
	
	override void Start( ActionData action_data )
	{
		action_data.m_State = UA_PROCESSING;
		bool in_iron_sights = action_data.m_Player.IsInIronsights();
		if(in_iron_sights)
		{
			action_data.m_Player.ExitSights();
		}
	}
	
	override bool CanBePerformedFromInventory()
	{
		return true;
	}
	

	/*void WriteToContext(ParamsWriteContext ctx, ActionData action_data)
	{
		if( HasTarget() )
		{
			// callback data
			Object targetObject = action_data.m_Target.GetObject();
			ctx.Write(targetObject);
			Object targetParent = action_data.m_Target.GetParent();
			ctx.Write(targetParent);
			int componentIndex = action_data.m_Target.GetComponentIndex();
			ctx.Write(componentIndex);
		}
	}*/
	
	/*bool ReadFromContext(ParamsReadContext ctx, ActionData action_data )
	{				
		if( HasTarget() )
		{
			Object actionTargetObject = null;
			Object actionTargetParent = null;
			int componentIndex = -1;
			
			if ( !ctx.Read(actionTargetObject) )
				return false;
							
			if ( !ctx.Read(actionTargetParent))
				return false;

			if ( !ctx.Read(componentIndex) )
				return false;
			
			ref ActionTarget target;
			target = new ActionTarget(actionTargetObject, actionTargetParent, componentIndex, vector.Zero, 0);
						
			action_data.m_Target = target;
		}


		return true;
	}*/
	
	//	ACTION LOGIC -------------------------------------------------------------------
	// called from actionmanager.c
/*	void Start( ActionData action_data ) //Setup on start of action
	{
		if( GetGame().IsServer() )
		{
			OnStartServer(action_data);
		}
		else
		{
			OnStartClient(action_data);
		}	
		
		InformPlayers(action_data.m_Player,action_data.m_Target,UA_START);	
	}*/	
	
	/*void OnContinuousCancel(ActionData action_data)
	{}*/
	
	
	// call only on client side for lock inventory before action
	// return if has successfuly reserved inventory
	/*bool InventoryReservation(ActionData action_data)
	{
		if( (IsLocal() || !UseAcknowledgment()) && IsInstant() )
			return true;
		
		//action_data.m_ReservedInventoryLocations = new array<ref InventoryLocation>;
		bool success = true;
		InventoryLocation targetInventoryLocation = NULL;
		InventoryLocation handInventoryLocation = NULL;
		
		// lock target if it has target
		if( HasTarget() )
		{
			ItemBase targetItem;
			if ( ItemBase.CastTo(targetItem,action_data.m_Target.GetObject()) )
			{
				targetInventoryLocation = new InventoryLocation;
				targetItem.GetInventory().GetCurrentInventoryLocation(targetInventoryLocation);
				if ( !action_data.m_Player.GetInventory().AddInventoryReservation( targetItem, targetInventoryLocation, 10000) )
				{
					success = false;
				}				
			}
		}	
		
		handInventoryLocation = new InventoryLocation;
		handInventoryLocation.SetHands(action_data.m_Player,action_data.m_Player.GetItemInHands());

		if ( !action_data.m_Player.GetInventory().AddInventoryReservation( action_data.m_Player.GetItemInHands(), handInventoryLocation, 10000) )
		{
			success = false;
		}
		
		
		if ( success )
		{
			if( targetInventoryLocation )
				action_data.m_ReservedInventoryLocations.Insert(targetInventoryLocation);
			
			if( handInventoryLocation )
				action_data.m_ReservedInventoryLocations.Insert(handInventoryLocation);
		}
		// lock Hands
		// On Fail unlock targetEntity
		//if ( targetEntity ) player.GetInventory().ClearReservation(player,targetEntity);
		// find way how to lock only hand slot
		
		return success;
	}*/
	
	// action need first have permission from server before can start
	/*bool UseAcknowledgment()
	{
		return true;
	}*/
	
	override float GetProgress( ActionData action_data )
	{
		return -1;
	}
};