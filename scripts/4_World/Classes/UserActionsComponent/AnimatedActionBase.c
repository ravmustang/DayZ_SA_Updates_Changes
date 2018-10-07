class ActionBaseCB : HumanCommandActionCallback
{
	protected ActionData				m_ActionData;
	protected SoundOnVehicle 			m_SoundObject;//object of sound playing on entity
	protected bool 						m_Canceled;//helps prevent doubled calling of actionbase End method
	protected bool						m_Interrupted;//force callback to wait till action syncs its interruption
		
		
	void ActionBaseCB()
	{
	}
	
	int GetPossileStanceMask()
	{
		return m_ActionData.m_PossibleStanceMask;
	}
	
	//Command events
	override void OnFinish(bool pCanceled)	
	{
		PrintString("ActionBase.c | ActionBaseCB | OnFinish called : " + pCanceled.ToString() );	
		if ( m_SoundObject ) 
		{
			GetGame().ObjectDelete(m_SoundObject);
		}
		if ( m_ActionData && m_ActionData.m_Player )
		{
			if ( pCanceled ) 
			{
				m_ActionData.m_State = m_ActionData.m_ActionComponent.Interrupt(m_ActionData);
			}
			
			AnimatedActionBase action = AnimatedActionBase.Cast(m_ActionData.m_Action);
			
			if(action)
				action.End(m_ActionData, m_ActionData.m_State);
			
			m_ActionData.m_Player.GetActionManager().OnActionEnd();
		}

		
	}
	
	void SetCommand( int command_uid )
	{		
		InternalCommand(command_uid);
	}

	//---------------------------------------------------------------
	//Action Component handlers
	void CreateActionComponent() 
	{
		m_ActionData.m_ActionComponent = new CADummy;
	}
	
	void InitActionComponent()
	{
	}
	
	void ProgressActionComponent()
	{
		if ( m_ActionData.m_ActionComponent )
		{ 
			m_ActionData.m_State = m_ActionData.m_ActionComponent.Execute(m_ActionData);
		}
	}	
	
	void EndActionComponent()
	{
	}	
	
	//data
	void Interrupt()
	{
		if ( GetGame().IsServer() )
		{
			PrintString("Interupt in actionbase called - server");
			if( GetGame().IsMultiplayer() )
			{
				DayZPlayerSyncJunctures.SendActionInterrupt(m_ActionData.m_Player);
			}
		}
		m_Interrupted = true;
	}
	
	void SetActionData(ActionData action_data )
	{
		m_ActionData = action_data;
	}
	
	override bool IsUserActionCallback()
	{
		return true;
	}
	
	float GetActionComponentProgress()
	{
		if ( m_ActionData.m_ActionComponent )
		{
			return m_ActionData.m_ActionComponent.GetProgress();
		}
		return 0;
	}
	
	int GetActionState()
	{
		return m_ActionData.m_State;
	}
	
	ActionTarget GetTarget()
	{
		return m_ActionData.m_Target;
	}
};



class AnimatedActionBase : ActionBase
{	
	protected int 					m_CommandUID;	//tells which command should be used for callback
	protected int					m_CommandUIDProne;
	protected typename 				m_CallbackClass; //tells which callback should be instantiated

	
	//-----------------------------------------------------
	// 	Action events and methods
	//-----------------------------------------------------
	void AnimatedActionBase() 
	{
		// definable
		m_CallbackClass = ActionBaseCB;
	}
	

	
	// Server version call on single too
	protected void OnStartServer( ActionData action_data ) //setup of action parameters, called before performing action
	{
	}
	protected void OnStartClient( ActionData action_data ) //setup of action parameters, called before performing action
	{
	}

	protected void OnCancelServer( ActionData action_data  ) //method called when player stops inputing
	{
	}
	protected void OnCancelClient( ActionData action_data  ) //method called when player stops inputing
	{
	}
	
	protected void OnInterruptServer( ActionData action_data  ) //method called when player can no longer continue action
	{
		OnCancelServer( action_data );
	}
	protected void OnInterruptClient( ActionData action_data  ) //method called when player can no longer continue action
	{
		OnCancelClient( action_data );
	}
	
	protected void OnAlternativeEndServer( PlayerBase player ) //method called when action has not met conditions in action component
	{
	}
	protected void OnAlternativeEndClient( PlayerBase player ) //method called when action has not met conditions in action component
	{
	}

	protected void OnCompleteServer( ActionData action_data ) //method called on succesful finishing of action (after out animation)
	{
	}
	protected void OnCompleteClient( ActionData action_data ) //method called on succesful finishing of action (after out animation)
	{
	}
	
	protected void OnCompleteLoopServer( ActionData action_data ) //method called on succesful finishing of action (before out animation - continuous actions only )
	{
	}
	protected void OnCompleteLoopClient( ActionData action_data ) //method called on succesful finishing of action (before out animation - continuous actions only )
	{
	}	
	
	protected void OnRepeatServer( ActionData action_data ) //method called on succesful finishing of action
	{
	}
	protected void OnRepeatClient( ActionData action_data ) //method called on succesful finishing of action
	{
	}
	
	protected void OnExecuteServer( ActionData action_data )
	{
	}
	
	protected void OnExecuteClient( ActionData action_data )
	{
	}
	
	void OnAnimationEvent( ActionData action_data )
	{
		if(!action_data.m_WasExecuted)
		{
			if(GetGame().IsServer())
			{
				OnExecuteServer(action_data);
			}
			else
			{
				OnExecuteClient(action_data);
			}
			action_data.m_WasExecuted = true;
		}
	}
	
	override bool ActionConditionContinue( ActionData action_data ) //condition for action
	{
		return ActionCondition(action_data.m_Player,action_data.m_Target,action_data.m_MainItem);
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ) //condition for action
	{
		return true;
	}
	
	void Interrupt( ActionData action_data )
	{
		
	}
	
	protected int GetActionCommand(PlayerBase player)
	{
		if ( HasProneException() )
		{
			if ( player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT))
				return m_CommandUID;
			else
				return m_CommandUIDProne;
		}
		return m_CommandUID;
	}
	
	protected typename GetCallbackClassTypename()
	{
		return m_CallbackClass; 
	}
	
	// if it is set to true if action have special fullbody animation for prone and additive for crouch and erect
	override bool HasProneException()
	{
		return false;
	}
	
	protected void CreateAndSetupActionCallback( ActionData action_data )
	{
		//Print("ActionBase.c | CreateAndSetupActionCallback | DBG ACTION CALLBACK CREATION CALLED");
		ActionBaseCB callback;
		if (  IsFullBody(action_data.m_Player) )
		{
			Class.CastTo(callback, action_data.m_Player.StartCommand_Action(GetActionCommand(action_data.m_Player),GetCallbackClassTypename(),GetStanceMask(action_data.m_Player)));	
			//Print("ActionBase.c | CreateAndSetupActionCallback |  DBG command starter");		
		}
		else
		{
			Class.CastTo(callback, action_data.m_Player.AddCommandModifier_Action(GetActionCommand(action_data.m_Player),GetCallbackClassTypename()));
			//Print("ActionBase.c | CreateAndSetupActionCallback |  DBG command modif starter: "+callback.ToString()+"   id:"+GetActionCommand().ToString());
			
		}
		callback.SetActionData(action_data); 
		callback.InitActionComponent(); //jtomasik - tohle mozna patri do constructoru callbacku?
		action_data.m_Callback = callback;
	}
		
		
		
	//	ACTION LOGIC -------------------------------------------------------------------
	// called from actionmanager.c
	override void Start( ActionData action_data ) //Setup on start of action
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
		if( !IsInstant() )
		{
		//player.GetActionManager().DisableActions();
			CreateAndSetupActionCallback(action_data);
		}
		/*Debug
		SendMessageToClient( player, "ActionBase.c : Start");
		Print("ActionBase.c : Start");
		*/
	}

	/*override bool Can( PlayerBase player, ActionTarget target, ItemBase item )
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
	
	protected override bool CanContinue( ActionData action_data )
	{
		if ( action_data.m_Player.IsPlayerInStance(action_data.m_PossibleStanceMask) && m_ConditionItem && m_ConditionItem.CanContinue(action_data.m_Player,action_data.m_MainItem) && m_ConditionTarget && m_ConditionTarget.CanContinue(action_data.m_Player,action_data.m_Target) && ActionConditionContinue(action_data) )
		{
			return true;
		}
		return false;
	}*/
		
	// called from ActionBaseCB.c
	void Do( ActionData action_data, int state )
	{
		if ( state == UA_ERROR || !action_data.m_Callback || !action_data.m_Player || !action_data.m_ActionComponent ) //jtomasik - tohle mozna muze byt v CancelCondtion metodu callbacku?
		{
#ifdef DEVELOPER
			Print("ActionBase.c | Do | ABRUPT CANCEL, CONDITIONS NOT MET");
#endif
			if ( action_data.m_Callback && action_data.m_Player )
			{			
				action_data.m_Callback.Interrupt();
			}
			else
			{
				Print("ActionBase.c | Do | PLAYER LOST");
			}	
		}
		else
		{
			switch ( state )
			{				
				//case UA_SETEND_2:
					//OnAlternativeEnd();
					//InformPlayers(player,target,UA_ERROR);
					//callback.Interrupt();
					//callback.SetCommand(DayZPlayerConstants.CMD_ACTIONINT_END2);//jtomasik - tohle pak zapoj do toho endovani az ti vse ostatni pobezi,  ted prehraje END2 (a tim to i ukonci), pozdeji by mozna piti prazdne lahve atp. mela byt samostatna akce
					//break;
				
				case UA_PROCESSING:	
					if ( CanContinue(action_data) )
					{	
						action_data.m_Callback.ProgressActionComponent();
						InformPlayers(action_data.m_Player, action_data.m_Target, UA_PROCESSING);
					}
					else
					{
						action_data.m_Callback.Interrupt();
						InformPlayers(action_data.m_Player, action_data.m_Target, UA_CANCEL);
						Do(action_data, UA_CANCEL);
					}
					break;
					
				case UA_REPEAT:
					if ( CanContinue(action_data) )
					{
						if ( GetGame().IsServer() )
						{
							OnRepeatServer(action_data);
							ApplyModifiers(action_data);
						}
						else
						{
							OnRepeatClient(action_data);
						}
						InformPlayers(action_data.m_Player,action_data.m_Target,UA_REPEAT);
						action_data.m_WasExecuted = false;

						Do(action_data, UA_PROCESSING);
					}
					else
					{
						action_data.m_Callback.Interrupt();
						InformPlayers(action_data.m_Player,action_data.m_Target,UA_FINISHED);
						Do(action_data, UA_FINISHED);
					}				
					break;
				
				/*case UA_ANIM_EVENT:
					if( CanContinue(action_data))
					{
						if ( GetGame().IsServer() )
						{
							OnExecuteServer(action_data);
							ApplyModifiers(action_data);
						}
						else
						{
							OnExecuteClient(action_data);
						}
						InformPlayers(action_data.m_Player,action_data.m_Target,UA_REPEAT);
						Do(action_data, UA_PROCESSING);
					}
					else
					{
						action_data.m_Callback.Interrupt();
						InformPlayers(action_data.m_Player, action_data.m_Target, UA_CANCEL);
						Do(action_data, UA_CANCEL);
					}
					break;*/
				case UA_FINISHED:
					if ( GetGame().IsServer() )
					{
						OnCompleteLoopServer(action_data);
						ApplyModifiers(action_data);
					}
					else
					{
						OnCompleteLoopClient(action_data);
					}
					InformPlayers(action_data.m_Player,action_data.m_Target,UA_FINISHED);
					action_data.m_Callback.EndActionComponent();
					break;

				case UA_CANCEL:
					InformPlayers(action_data.m_Player,action_data.m_Target,UA_CANCEL);
					action_data.m_Callback.EndActionComponent();
					break;	
				
				case UA_INITIALIZE:
					if ( !CanContinue(action_data) )
					{						
						action_data.m_Callback.Interrupt();
						InformPlayers(action_data.m_Player, action_data.m_Target, UA_CANCEL);
						Do(action_data, UA_CANCEL);
					}
				default:
					PrintString("ActionBase.c | Do | ACTION COMPONENT RETURNED WRONG VALUE: "+state.ToString());
					action_data.m_Callback.Interrupt();
					break;
			}
		}
	}
	
	// called from ActionBaseCB.c 
	void End( ActionData action_data, int state )
	{
		if ( action_data.m_Player && action_data.m_ActionComponent ) 
		{
			switch ( state )
			{
				case UA_FINISHED:		
					if ( GetGame().IsServer() )
					{
						OnCompleteServer(action_data);
						ApplyModifiers(action_data);
					}
					else
					{
						OnCompleteClient(action_data);					
					}
					InformPlayers(action_data.m_Player,action_data.m_Target,UA_FINISHED);
					break;
					
				case UA_CANCEL:
					if ( GetGame().IsServer() )
					{
						OnCancelServer(action_data);
					}
					else
					{
						OnCancelClient(action_data);					
					}
					InformPlayers(action_data.m_Player,action_data.m_Target,UA_CANCEL);
					break;
				case UA_INTERRUPT:
					if ( GetGame().IsServer() )
					{
						OnInterruptServer(action_data);
					}
					else
					{
						OnInterruptClient(action_data);					
					}
					InformPlayers(action_data.m_Player,action_data.m_Target,UA_INTERRUPT);
					break;
				case UA_PROCESSING:
				case UA_REPEAT:
				case UA_START:
				case UA_STARTT:
					if ( GetGame().IsServer() )
					{
						OnCompleteServer(action_data);
						ApplyModifiers(action_data);
					}
					else
					{
						OnCompleteClient(action_data);					
					}
					InformPlayers(action_data.m_Player,action_data.m_Target,UA_FINISHED);
					break;
				
					break;
				default:
					Print("ActionBase.c | End | ACTION COMPONENT RETURNED WRONG VALUE");
					action_data.m_Callback.Interrupt();
				break;
			}
		}
		else
		{
			Print("ActionBase.c | End | ACTION COULD NOT BE FINISHED RIGHT AT THE END");
			action_data.m_Callback.Interrupt();
		}
		//player.GetActionManager().EnableActions();
	}
	
	override float GetProgress( ActionData action_data )
	{
		if ( action_data.m_ActionComponent )
		{	 
			return action_data.m_ActionComponent.GetProgress();
		}
		return 0;
	}
};