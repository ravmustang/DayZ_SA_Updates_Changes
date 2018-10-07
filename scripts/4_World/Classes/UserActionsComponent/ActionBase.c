class ActionData
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
	bool								m_WasExecuted;
}

class ActionBase
{	
	//STATIC DATA
	// Configurable action parameters
	protected string				m_MessageStartFail; //called from ontick method of quantity and time baseclasses, usually informing player that item is empty
	protected string				m_MessageStart; //called from onstart method, usually informing player that he started the action
	protected string				m_MessageSuccess; //called from ontick method, ussualy informing player that he succesfully finished the action
	protected string 				m_MessageFail; //called from cancel or ontick methods,  ussualy informing player that he or target moved and thus was action canceled
	protected string				m_MessageCancel; //called from cancel method, ussualy informing player that he stoped holding RMB and canceled action
	protected string				m_MessageSuccessTarget; //called from ontick method, ussualy informing other player that he succesfully finished the action
	protected string 				m_MessageStartTarget; //called from cancel or ontick methods,  ussualy informing other player that he or target moved and thus was action canceled
	protected string				m_MessageCancelTarget; //called from cancel method, ussualy informing other player that he stoped holding RMB and canceled action
	protected string				m_Sound; //sound played at the beggining of action

	protected bool					m_LockTargetOnUse;	//this parameter sets wheter player can perform actions on target while other player is already performing action on it. defaulted as true
	protected bool 					m_FullBody; //tells whether action is full body or additive
	protected int					m_StanceMask;
	protected ref TStringArray		m_Sounds;			//User action sound is picked from this array randomly
	ref CCIBase 					m_ConditionItem;	//Condition Component
	ref CCTBase						m_ConditionTarget; 	//Condition Component

	//RUNTIME DATA
	protected ref Param1<string> 	m_MessageParam; //used for passing messages from server to client
	protected ref Param2<int,int>	m_MessagesParam; 	
	
	//SOFT SKILLS
	protected float					m_SpecialtyWeight;
	
	//-----------------------------------------------------
	// 	Action events and methods
	//-----------------------------------------------------
	void ActionBase() 
	{
		// definable
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_PRONE;
		m_FullBody = false;
		m_Sound = "";
		m_LockTargetOnUse = HasTarget();
		m_MessageStartFail = "The action failed";
		m_MessageStart = "I have started an action.";
		m_MessageSuccess = "The action succesfully finished.";
		m_MessageFail = "The action failed.";
		m_MessageCancel = "I have canceled the action.";
		m_MessageStartTarget = "Other player started performing action on you.";
		m_MessageSuccessTarget = "Other player succesfuly performed action on you.";
		m_MessageCancelTarget = "Other player canceled the action.";
		
		// dont override
		m_MessageParam = new Param1<string>("");
		m_MessagesParam = new Param2<int,int>(0,0);
		m_Sounds = new TStringArray;
	}
	
	bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extraData = NULL)
	{
		action_data = CreateActionData();
		action_data.m_Action = this;
		action_data.m_Player = player;
		action_data.m_Target = target;
		action_data.m_MainItem = item;
		action_data.m_PossibleStanceMask = GetStanceMask(player);
		action_data.m_ReservedInventoryLocations = new array<ref InventoryLocation>;
		action_data.m_WasExecuted = false;
		
		if ( (!GetGame().IsMultiplayer() || GetGame().IsClient()) && !IsInstant() )
		{
			if (!InventoryReservation(action_data))
				return false;
		}
		
		return true;
	}
	
	ActionData CreateActionData()
	{
		ActionData action_data = new ActionData;
		return action_data;
	}
			
	void CreateConditionComponents() //Instantiates components, called once from ActionManager on actions construction
	{
		m_ConditionItem = new CCIDummy;
		m_ConditionTarget = new CCTDummy;
	}

	//! For UI: true - widget will be on item/traced pos; false - will be in under Item in hands widget
	bool HasTarget()
	{
		return true;
	}
	
	//! For UI: hiding of progress bar
	bool HasProgress()
	{
		return true;
	}
	
	//Action isn't synchronize to server 
	bool IsLocal()
	{
		return false;
	}
	
	//Action not using animation/callback action are executed instantly (OnStart)
	bool IsInstant()
	{
		return false;
	}
	
	bool RemoveForceTargetAfterUse()
	{
		return true;
	}

	bool Consumable()
	{
		return false;
	}
	
	int GetActionCategory()
	{
		return AC_UNCATEGORIZED;
	}
	
	int GetType()  //returns action uid
	{
		return 0;
	}

	string GetText() //text game displays in HUD hint 
	{
		return "default action text";
	}
	
	string GetTargetDescription()
	{
		return "default target description";
	}
	
	bool CanBePerformedFromQuickbar()
	{
		return false;
	}
	
	bool CanBePerformedFromInventory()
	{
		return false;
	}
	
	bool CanBeSetFromInventory()
	{
		return !CanBePerformedFromInventory();
	}
	 
	
	protected bool ActionConditionContinue( ActionData action_data ) //condition for action
	{
		return ActionCondition(action_data.m_Player,action_data.m_Target,action_data.m_MainItem);
	}
	
	protected bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ) //condition for action
	{
		return true;
	}
	
	void ApplyModifiers( ActionData action_data ) // method that is planned to be called after every succesful completion of action to transfer diseases and other modifiers, now is called before oncompletes
	{
	}

	void WriteToContext(ParamsWriteContext ctx, ActionData action_data)
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
	}
	
	bool ReadFromContext(ParamsReadContext ctx, ActionData action_data )
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
	}
	
	//----------------------------------------------------------------------------------------------
	// Core methods don't override unless you know what you are doing
	//----------------------------------------------------------------------------------------------		
	// COMMANDS -----------------------------------------------------------------------		
	protected int GetStanceMask(PlayerBase player)
	{
		if ( HasProneException() )
		{
			if ( player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT))
				return DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
			else
				return DayZPlayerConstants.STANCEMASK_PRONE;
		}
		return m_StanceMask;
	}
	
	protected bool IsFullBody(PlayerBase player)
	{
		if ( HasProneException() )
		{
			if ( player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT))
				return false;
			else
				return true;
		}
		return m_FullBody;
	}
	
	// if it is set to true if action have special fullbody animation for prone and additive for crouch and erect
	protected bool HasProneException()
	{
		return false;
	}
	
	//	ACTION LOGIC -------------------------------------------------------------------
	// called from actionmanager.c
	void Start( ActionData action_data ) //Setup on start of action
	{
/*		if( GetGame().IsServer() )
		{
			OnStartServer(action_data);
		}
		else
		{
			OnStartClient(action_data);
		}	
		*/	
		InformPlayers(action_data.m_Player,action_data.m_Target,UA_START);	
	}
	
	void OnContinuousCancel(ActionData action_data)
	{}

	bool Can( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !m_FullBody && (!player || !player.IsPlayerInStance(GetStanceMask(player))) )
		{
			return false;
		}
		if ( m_ConditionItem && m_ConditionItem.Can(player, item) && m_ConditionTarget && m_ConditionTarget.Can(player, target) && ActionCondition(player, target, item) ) 
		{	
			return true;
		}
		return false;
	}
	
	protected bool CanContinue( ActionData action_data )
	{
		if ( action_data.m_Player.IsPlayerInStance(action_data.m_PossibleStanceMask) && m_ConditionItem && m_ConditionItem.CanContinue(action_data.m_Player,action_data.m_MainItem) && m_ConditionTarget && m_ConditionTarget.CanContinue(action_data.m_Player,action_data.m_Target) && ActionConditionContinue(action_data) )
		{
			return true;
		}
		return false;
	}
	
	// call only on client side for lock inventory before action
	// return if has successfuly reserved inventory
	bool InventoryReservation(ActionData action_data)
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
	}

	void ClearInventoryReservation(ActionData action_data)
	{
		if(action_data.m_ReservedInventoryLocations)
		{
			InventoryLocation il;
			for ( int i = 0; i < action_data.m_ReservedInventoryLocations.Count(); i++)
			{
				il = action_data.m_ReservedInventoryLocations.Get(i);
				EntityAI entity = il.GetItem();
				action_data.m_Player.GetInventory().ClearInventoryReservation( il.GetItem() , il );
			}
			action_data.m_ReservedInventoryLocations.Clear();
		}
	}
		
	// MESSAGES --------------------------------------------------------------------
	string GetMessageText( int state ) //returns text of action based on given id
	{
		string message = "";
		switch ( state )
		{
			case UA_ERROR:
				message = GetMessageStartFail();
				break;
				
			case UA_FAILED:
				message = GetMessageFail();
				break;
			
			case UA_INITIALIZE:
				message = "Initialize";
				break;
				
			case UA_PROCESSING:
				message = "Processing";
				break;
				
			case UA_REPEAT:
				message = "loop end";
				break;
				
			case UA_START:
				message = GetMessageStart();
				break;
				
			case UA_FINISHED:
				message = GetMessageSuccess();
				break;
				
			case UA_CANCEL:
				message = GetMessageCancel();
				break;
			
			case UA_INTERRUPT:
				message = GetMessageInterrupt();
				break;
				
			case UA_STARTT:
				message = GetMessageStartTarget();
				break;
				
			case UA_FINISHEDT:
				message = GetMessageSuccessTarget();
				break;
				
			case UA_CANCELT:
				message = GetMessageCancelTarget();
				break;
				
			default:
				Print("ActionBase.c | Informplayers | CALLED WITH WRONG STATE PARAMETER");
				break;
		}

		return message;
	}
	
	// action need first have permission from server before can start
	bool UseAcknowledgment()
	{
		return true;
	}
	
	protected void InformPlayers( PlayerBase player, ActionTarget target, int state ) //delivers message ids to clients based on given context
	{	
		/*player.MessageAction(GetMessageText(state)); //jtomasik - tohle se smaze az to pujde ven, ted je to jako debug pro lokalni testovani
		
		if( GetGame().IsServer() && player && m_MessagesParam && state > 0 && state < 12 )
		{
			int message_target = 0;
			switch ( state )
			{
				case UA_START:
					message_target = UA_STARTT;
					break;
					
				case UA_FINISHED:
					message_target = UA_FINISHEDT;
					break;
					
				case UA_CANCEL:
					message_target = UA_CANCELT;
					break;
			}
			if ( player.IsDamageDestroyed() )
			{
				m_MessagesParam.param1 = GetType();
				m_MessagesParam.param2 = state;
				GetGame().RPCSingleParam(player, RPC_USER_ACTION_MESSAGES, m_MessagesParam, player.GetIdentity());
			}
			if ( message_target > 0 && m_ConditionTarget && m_ConditionTarget.IsActionOnTarget() && target && target.IsMan() && target.IsAlive() )
			{
				m_MessagesParam.param1 = GetType();
				m_MessagesParam.param2 = message_target;
				GetGame().RPCSingleParam(target, RPC_USER_ACTION_MESSAGES, m_MessagesParam, target.GetIdentity());
			}
		}*/
	}
		
	void SendMessageToClient( Object reciever, string message ) //sends given string to client, don't use if not nescessary
	{
		PlayerBase man;
		if( GetGame().IsServer() && Class.CastTo(man, reciever) && m_MessageParam && reciever.IsAlive() && message != "" )
		{
			m_MessageParam.param1 = message;
			GetGame().RPCSingleParam(man, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, man.GetIdentity());
		}
	}
	
	protected string GetMessageStartFail()
	{
		return m_MessageStartFail;
	}
	
	protected string GetMessageStart()
	{
		return m_MessageStart;
	}
	
	protected string GetMessageSuccess() 
	{
		return m_MessageSuccess;
	}
	
	protected string GetMessageFail()
	{
		return m_MessageFail;
	}
	
	protected string GetMessageCancel() 
	{
		return m_MessageCancel;
	}
	
	protected string GetMessageInterrupt() 
	{
		return "";
		//return m_MessageInterrupt;
	}
	
	protected string GetMessageSuccessTarget() 
	{
		return m_MessageSuccessTarget;
	}
	
	protected string GetMessageStartTarget()
	{
		return m_MessageStartTarget;
	}
	
	protected string GetMessageCancelTarget()
	{
		return m_MessageCancelTarget;
	}

	// ActionCondition Rules
	// ------------------------------------------------------
	protected bool IsDamageDestroyed(ActionTarget target)
	{
		Object obj;
		if(Class.CastTo(obj, target.GetObject()) )
		{
			if( obj.IsDamageDestroyed() )
				return true;
		}

		return false;
	}

	protected bool IsBuilding(ActionTarget target)
	{
		Object obj;
		
		if( Class.CastTo(obj, target.GetObject()) )
			return obj.IsBuilding();

		return false;
	}
	
	protected bool IsTransport(ActionTarget target)
	{
		Object obj;
		if( Class.CastTo(obj, target.GetObject()) )
			return obj.IsTransport();

		return false;		
	}

	protected bool IsInReach(PlayerBase player, ActionTarget target, float maxDistance = 1.0 )
	{
		Object obj;
		if( Class.CastTo(obj, target.GetObject()) )
		{
			string compName;
			float distanceRoot, distanceHead;
			vector modelPos, worldPos, playerRootPos, playerHeadPos;
			
			// we're using sq distance in comparison
			maxDistance = maxDistance * maxDistance;
			playerRootPos = player.GetPosition();
			
			// get position of Head bone
			MiscGameplayFunctions.GetHeadBonePos(player, playerHeadPos);

			compName = target.GetObject().GetActionComponentName(target.GetComponentIndex());
			modelPos = target.GetObject().GetSelectionPosition(compName);
			worldPos = target.GetObject().ModelToWorld(modelPos);

			distanceRoot = Math.AbsFloat(vector.DistanceSq(worldPos, playerRootPos));
			distanceHead = Math.AbsFloat(vector.DistanceSq(worldPos, playerHeadPos));
			if ( distanceRoot <= maxDistance || distanceHead <= maxDistance )
			{	
				return true;			
			}
		}

		return false;
	}	
	// ------------------------------------------------------
	
	
	
	// SOUNDS ------------------------------------------------------
	SoundOnVehicle PlayActionSound( PlayerBase player )
	{
		if( GetGame().IsServer() )
		{
			if ( player ) 
			{
				if ( m_Sound != "" )
				{
					return GetGame().CreateSoundOnObject(player, m_Sound, 6, false);
				}
				if ( m_Sounds && m_Sounds.Count() > 0 )
				{
					int rand_num =  Math.RandomInt(0, m_Sounds.Count());
					return GetGame().CreateSoundOnObject(player, m_Sounds.Get(rand_num), 6, false);
				}
			}
		}

		return NULL;
	}
	
	void OnUpdate(ActionData action_data)
	{
	}

	// SOFT SKILLS ------------------------------------------------
	float GetSpecialtyWeight()
	{
		if(m_SpecialtyWeight == 0)
		{
#ifdef DEVELOPER
			//Print("UserAction does not use SoftSkills");
#endif
		}	

		return m_SpecialtyWeight;	
	}
	
	int GetState( ActionData action_data )
	{
		return action_data.m_State;
	}
	
	float GetProgress( ActionData action_data )
	{
		return -1;
	}
};