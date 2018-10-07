class EmoteCB : HumanCommandActionCallback
{
	bool 			m_IsFullbody;
	PlayerBase 		m_player;
	
	bool CancelCondition()
	{
		return false;
	}
	
	bool IsEmoteCallback()
	{
		return true;
	}
	
	override void OnAnimationEvent(int pEventID)
	{
		if (pEventID == 1)
		{
			m_player.GetEmoteManager().KillPlayer();
		}
	}
};

/*
class EmoteSittingAndDoingSmthingCB : HumanCommandActionCallback //jtomasik - sem se daj asi dopsat nejaky dalsi callback extra burty pro sezeni se zbrani atp
{	
	bool IsEmoteCallback()
	{
		return true;
	}
};
*/

class EmoteFromMenu
{
	int m_ID;
	
	void EmoteFromMenu(int emoteID)
	{
		m_ID = emoteID;
	}
}

class EmoteManager
{		
	PlayerBase 				m_Player;
	ItemBase				m_item;
	EmoteCB					m_Callback;
	HumanInputController 	m_HIC;
	ref EmoteFromMenu 		m_MenuEmote;
	bool					m_bEmoteIsPlaying;
	protected bool			m_BelayedEmote;
	protected bool			m_ItemToHands;
	protected bool			m_ItemIsOn;
	protected bool			m_MouseButtonPressed;
	protected bool 			m_PlayerDies;
	protected bool 			m_controllsLocked;
	protected int  			m_GestureID;
	protected int			m_BelayedEmoteID;
	protected int			m_PreviousGestureID;
	protected int			m_CurrentGestureID;
	protected int 			m_LastMask;
	protected int 			m_RPSOutcome;
	
	
	void EmoteManager( PlayerBase player ) 
	{
		m_Player = player;
		m_HIC = m_Player.GetInputController();
		m_ItemIsOn = false;
		m_RPSOutcome = -1;
	}
	
	void SetGesture(int id)
	{
		m_GestureID = id;
	}
	
	int GetGesture()
	{
		return m_GestureID;
	}
	
	//Called from players commandhandler each frame, checks input
	void Update( float deltaT )
	{
		if ( m_Callback ) 
		{
			bool uiGesture = false;
			if( !GetGame().IsMultiplayer() || GetGame().IsClient() )
			{
				uiGesture = GetGame().GetUIManager().IsMenuOpen(MENU_GESTURES);
			}
			
			//Movement lock in fullbody anims
			if (m_Callback.m_IsFullbody && !m_controllsLocked)
			{
				m_controllsLocked = true;
				//m_Player.GetInputController().OverrideAimChangeX(true,0);
				//m_Player.GetInputController().OverrideMovementSpeed(true,0);
			}
			
			//jtomasik - asi bych nemel checkovat jestli hrac klika v menu nebo ve scene tady, ale mel by to vedet input manager?
			//HACK - handle differently with new input controller
			if( (m_HIC.IsGestureSlot() > 0 && m_HIC.IsGestureSlot() != 12 ) /*|| uiGesture*/|| (m_HIC.IsSingleUse() && !uiGesture) || (m_HIC.IsContinuousUseStart() && !uiGesture) || (m_Callback.m_IsFullbody && !uiGesture && m_HIC.IsWeaponRaised()) ) 
			{
				//Print("gestureslot pressed: " + m_HIC.IsGestureSlot());
				if (m_CurrentGestureID == ID_EMOTE_SUICIDE  && m_HIC.IsSingleUse() /*m_MouseButtonPressed*/)
				{
					if (m_Callback.GetState() == m_Callback.STATE_LOOP_LOOP)
					{
						CommitSuicide();
					}
					else
					{
						return;
					}
				}
				else if ( (m_CurrentGestureID == ID_EMOTE_THUMB || m_CurrentGestureID == ID_EMOTE_THUMBDOWN) && m_HIC.IsSingleUse() )
				{
					m_Callback.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_ACTION);
				}
				else if ( m_CurrentGestureID == ID_EMOTE_RPS || m_CurrentGestureID == ID_EMOTE_RPS_R || m_CurrentGestureID == ID_EMOTE_RPS_P || m_CurrentGestureID == ID_EMOTE_RPS_S )
				{
					if ( m_RPSOutcome != -1 )
					{
						if (m_RPSOutcome == 0) 			m_Callback.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_RPS_ROCK);
						else if (m_RPSOutcome == 1) 	m_Callback.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_RPS_PAPER);
						else if (m_RPSOutcome == 2) 	m_Callback.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_RPS_SCISSORS);
						m_RPSOutcome = -1;
					}
					else
					{
						m_Callback.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_END);
					}
				}
				//TODO rework with new input controller; registered KeyCode.KC_PERIOD as m_HIC.IsSingleUse() for some reason
				else
				{
					EndCallback();
				}

				if ( m_HIC.IsGestureSlot() && m_PreviousGestureID != m_HIC.IsGestureSlot() )
				{
					m_BelayedEmote = true;
					m_BelayedEmoteID = m_HIC.IsGestureSlot();
				}
			}
			
			/*if ( m_MenuEmote && m_MenuEmote.m_ID != m_PreviousGestureID )
			{
				m_BelayedEmote = true;
				m_BelayedEmoteID = m_MenuEmote.m_ID;
			}*/
			
			//HOTFIX for stance changing in additive emotes
			if ( m_LastMask != -1 && m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE) )
			{
				m_Callback.Cancel();
			}
			
			if ( m_MenuEmote && m_bEmoteIsPlaying )
			{
				//PlayEmoteFromMenu();
				EndCallback();
			}
		}
		
		//no m_Callback exists
		else
		{
			if (m_bEmoteIsPlaying)
			{
				m_bEmoteIsPlaying = false;
				//m_LaunchedFromRadial = false;
				OnEmoteEnd();
			}
			else if (!m_bEmoteIsPlaying && m_MenuEmote)
			{
				PlayEmoteFromMenu();
			}
			else
			{
				PickEmote(m_HIC.IsGestureSlot());
			}
		}
	}
	
	void OnEmoteEnd()
	{
		if ( m_ItemToHands )
		{
			ShowItemInHands();
		}
		
		//if suicide has been completed, kill player
		if ( m_PlayerDies )
		{
			m_Player.SetHealth(0.0);
			return;
		}
		
		if ( m_BelayedEmote )
		{
			if ( m_PreviousGestureID != m_BelayedEmoteID )
			{
				PickEmote(m_BelayedEmoteID);
				//PlayEmote(m_BelayedEmoteID);
			}
			m_BelayedEmoteID = -1;
			m_BelayedEmote = false;
		}
		else
		{
			if ( m_Player.GetActionManager() ) m_Player.GetActionManager().EnableActions();
			m_Player.m_InventorySoftLocked = false;
			
			if ( m_controllsLocked )
			{
				m_controllsLocked = false;
				m_Player.GetInputController().OverrideAimChangeX(false,0);
				m_Player.GetInputController().OverrideMovementSpeed(false,0);
			}
		}

		//! back to the default - shoot from camera - if not set already
		if (!m_Player.IsShootingFromCamera()) m_Player.OverrideShootFromCamera(true);
	}
	
	bool OnInputUserDataProcess(int userDataType, ParamsReadContext ctx)
	{
		if ( userDataType == INPUT_UDT_GESTURE)
		{
			int gestureID = -1;
			int random = -1;

			if (ctx.Read(gestureID))
			{
				if (ctx.Read(random))
				{
					m_RPSOutcome = random;
				}
				
				if ( gestureID >= 0 )
				{
					PlayEmote(gestureID);
				}
				else
				{
					EndCallback();
				}
			}
			return true;
		}
		return false;
	}
	
	//Configure Emote parameters for callback here
	bool PlayEmote( int id )
	{
		if (!CanPlayEmote())
		{
			return false;
		}
		
		m_CurrentGestureID = id;
		if( id > 0)
		{
			switch ( id )
			{
				case ID_EMOTE_GREETING :
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_GREETING,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_GREETING,DayZPlayerConstants.STANCEMASK_PRONE,true);
					}
				break;
			
				case ID_EMOTE_SOS :
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE)) 	CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_SOS,DayZPlayerConstants.STANCEMASK_ERECT,true);
					HideItemInHands();
				break;
			
				case ID_EMOTE_HEART :
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_HEART,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_HEART,DayZPlayerConstants.STANCEMASK_PRONE,true);
					}
					HideItemInHands();
				break;
			
				case ID_EMOTE_TAUNT :
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_TAUNT,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_TAUNT,DayZPlayerConstants.STANCEMASK_PRONE,true);
					}
				break;
			
				case ID_EMOTE_LYINGDOWN :
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_LYINGDOWN,DayZPlayerConstants.STANCEMASK_CROUCH,true);
					HideItemInHands();
				break;
			
				case ID_EMOTE_TAUNTKISS :
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_TAUNTKISS,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_TAUNTKISS,DayZPlayerConstants.STANCEMASK_PRONE,true);
					}
				break;
			
				case ID_EMOTE_POINT :
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_POINT,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_POINT,DayZPlayerConstants.STANCEMASK_PRONE,true);
					}
					//m_bEmoteIsPlaying = false;
				break;
				
				case ID_EMOTE_TAUNTELBOW :
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_TAUNTELBOW,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
				break;
				
				case ID_EMOTE_THUMB :
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_THUMB,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_THUMB,DayZPlayerConstants.STANCEMASK_PRONE,true);
					}
				break;
				
				case ID_EMOTE_THUMBDOWN :
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_THUMBDOWN,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_THUMBDOWN,DayZPlayerConstants.STANCEMASK_PRONE,true);
					}
				break;
				
				case ID_EMOTE_THROAT :
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_THROAT,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_THROAT,DayZPlayerConstants.STANCEMASK_PRONE,true);
					}
				break;
			
				case ID_EMOTE_SUICIDE :
					int suicideID = DayZPlayerConstants.CMD_SUICIDEFB_UNARMED;
					string suicideStr;
					ItemBase weapon;
					weapon = m_Player.GetItemInHands();
					//suicideID = DayZPlayerConstants.CMD_SUICIDEFB_UNARMED; //unarmed suicide...optional?
					if (weapon)
					{
						if (weapon.ConfigIsExisting("suicideAnim"))
						{
							suicideStr = weapon.ConfigGetString("suicideAnim");
						}
						
						if (weapon.IsKindOf("Pistol_Base"))
						{
							suicideID = DayZPlayerConstants.CMD_SUICIDEFB_PISTOL;
							//! switch to shoot from weapons instead of camera
							m_Player.OverrideShootFromCamera(false);
						}

						else if (weapon.IsKindOf("Rifle_Base"))
						{
							suicideID = DayZPlayerConstants.CMD_SUICIDEFB_RIFLE;
							//! switch to shoot from weapons instead of camera
							m_Player.OverrideShootFromCamera(false);
						}
						
						else if (suicideStr == "onehanded") 	suicideID = DayZPlayerConstants.CMD_SUICIDEFB_1HD;
						
						else if (suicideStr == "fireaxe") 		suicideID = DayZPlayerConstants.CMD_SUICIDEFB_FIREAXE;
						
						else if (suicideStr == "pitchfork") 	suicideID = DayZPlayerConstants.CMD_SUICIDEFB_PITCHFORK;
						
						else if (suicideStr == "sword") 		suicideID = DayZPlayerConstants.CMD_SUICIDEFB_SWORD;
						
						else if (suicideStr == "spear") 		suicideID = DayZPlayerConstants.CMD_SUICIDEFB_SPEAR;
						
						else if (suicideStr == "woodaxe") 		suicideID = DayZPlayerConstants.CMD_SUICIDEFB_WOODAXE;
					
						else //item in hands is not eligible for suicide
							suicideID = -1;
					}
			
					if (suicideID > -1)
					{
						CreateEmoteCallback(EmoteCB,suicideID,DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH,true);
						//m_Callback.RegisterAnimationEvent("Death",1);
					}
				break;
			
				case ID_EMOTE_DANCE:
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE)) 	CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_DANCE,DayZPlayerConstants.STANCEMASK_ERECT,true);
					HideItemInHands();
				break;
				
				case ID_EMOTE_SALUTE:
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE)) 	CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_SALUTE,DayZPlayerConstants.STANCEMASK_ERECT,true);
					//HideItemInHands();
				break;
				
				case ID_EMOTE_TIMEOUT:
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_TIMEOUT,DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH,false);
						HideItemInHands();
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_TIMEOUT,DayZPlayerConstants.STANCEMASK_PRONE,true);
						HideItemInHands();
					}
					
				break;
				
				case ID_EMOTE_DABBING:
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_DABBING,DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH,false);
					HideItemInHands();
				break;
				
				case ID_EMOTE_FACEPALM:
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_FACEPALM,DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH,false);
					//HideItemInHands();
				break;
				
				case ID_EMOTE_CLAP:
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_CLAP,DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH,false);
					HideItemInHands();
				break;
				
				case ID_EMOTE_SILENT:
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_SILENCE,DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH,false);
						//HideItemInHands();
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_SILENCE,DayZPlayerConstants.STANCEMASK_PRONE,true);
						//HideItemInHands();
					}
				break;
				
				case ID_EMOTE_WATCHING:
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_WATCHING,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_WATCHING,DayZPlayerConstants.STANCEMASK_PRONE,true);
					}
				break;
				
				case ID_EMOTE_HOLD:
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_HOLD,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_HOLD,DayZPlayerConstants.STANCEMASK_PRONE,true);
					}
				break;
				
				case ID_EMOTE_LISTENING:
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_LISTENING,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_LISTENING,DayZPlayerConstants.STANCEMASK_PRONE,true);
					}
				break;
				
				case ID_EMOTE_POINTSELF:
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_POINTSELF,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_POINTSELF,DayZPlayerConstants.STANCEMASK_PRONE,true);
					}
				break;
				
				case ID_EMOTE_LOOKATME:
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_LOOKATME,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_LOOKATME,DayZPlayerConstants.STANCEMASK_PRONE,true);
					}
				break;
				
				case ID_EMOTE_TAUNTTHINK :
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_TAUNTTHINK,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_TAUNTTHINK,DayZPlayerConstants.STANCEMASK_PRONE,true);
					}
				break;
				
				case ID_EMOTE_MOVE :
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_MOVE,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_MOVE,DayZPlayerConstants.STANCEMASK_PRONE,true);
					}
				break;
				
				case ID_EMOTE_DOWN :
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_DOWN,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_DOWN,DayZPlayerConstants.STANCEMASK_PRONE,true);
					}
				break;
				
				case ID_EMOTE_COME :
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_COME,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_COME,DayZPlayerConstants.STANCEMASK_PRONE,true);
					}
				break;
				
				case ID_EMOTE_CAMPFIRE :
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_CAMPFIRE,DayZPlayerConstants.STANCEMASK_CROUCH,true);
				break;
				
				case ID_EMOTE_SITA :
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_SITA,DayZPlayerConstants.STANCEMASK_CROUCH,true);
				break;
				
				case ID_EMOTE_SITB :
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_SITB,DayZPlayerConstants.STANCEMASK_CROUCH,true);
				break;
				
				case ID_EMOTE_RPS :
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_RPS,DayZPlayerConstants.STANCEMASK_ERECT,false);
				break;
				
				case ID_EMOTE_RPS_R :
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_RPS,DayZPlayerConstants.STANCEMASK_ERECT,false);
				break;
				
				case ID_EMOTE_RPS_P :
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_RPS,DayZPlayerConstants.STANCEMASK_ERECT,false);
				break;
				
				case ID_EMOTE_RPS_S :
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_RPS,DayZPlayerConstants.STANCEMASK_ERECT,false);
				break;
				
				//new stuff
				case ID_EMOTE_NOD :
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_NODHEAD,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_NODHEAD,DayZPlayerConstants.STANCEMASK_PRONE,true);
					}
				break;
				
				case ID_EMOTE_SHAKE :
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_SHAKEHEAD,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_SHAKEHEAD,DayZPlayerConstants.STANCEMASK_PRONE,true);
					}
				break;
				
				case ID_EMOTE_SHRUG :
					if (!m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_SHRUG,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					}
					else
					{
						CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_SHRUG,DayZPlayerConstants.STANCEMASK_PRONE,true);
					}
				break;
				
				default :
					Print("EmoteManager.c | PlayEmote | WRONG ID");
					m_bEmoteIsPlaying = false;
					return false;
				break;
			}
		}
		if ( m_bEmoteIsPlaying )
		{
			if ( m_Player.GetActionManager() )	 m_Player.GetActionManager().DisableActions();
			//m_Player.GetInventory().LockInventory(LOCK_FROM_SCRIPT);
			m_Player.m_InventorySoftLocked = true;
		}
		
		return true;
	}
	
	protected void PickEmote(int gestureslot)
	{
		if ( m_Player )
		{
			m_PreviousGestureID = gestureslot;
			//HACK - to be removed with new input controller
			if ( m_PreviousGestureID == 12)
			{
				return;
			}
			
			switch ( gestureslot )
			{
				case 1 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot1() );
				break;
				
				case 2 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot2() );
				break;
				
				case 3 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot3() );
				break;
				
				case 4 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot4() );
				break;
				
				case 5 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot5() );
				break;
				
				case 6 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot6() );
				break;
				
				case 7 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot7() );
				break;
				
				case 8 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot8() );
				break;
				
				case 9 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot9() );
				break;
				
				case 10 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot10() );
				break;
				
				case 11 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot11() );
				break;
				
				/*case 12 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot12() );
				break;*/
			}
		}
	}
	
	//creates Emote callback
	protected void CreateEmoteCallback(typename callbacktype, int id, int mask, bool fullbody )
	{
		if ( m_Player )
		{
			m_LastMask = -1;
			if ( fullbody )
			{
				Class.CastTo(m_Callback, m_Player.StartCommand_Action(id,callbacktype,mask));
				m_Callback.m_IsFullbody = true;
				m_Callback.EnableCancelCondition(true);
			}
			else if (m_Player.IsPlayerInStance(mask))
			{
				m_LastMask = mask;  //character is probably not prone now
				Class.CastTo(m_Callback, m_Player.AddCommandModifier_Action(id,callbacktype));
			}
			
			if (m_Callback)
			{
				m_bEmoteIsPlaying = true;
				m_Callback.m_player = m_Player;
			}
		}
	}
	
	protected void HideItemInHands()
	{
		m_item = m_Player.GetItemInHands();
		if(m_Callback && m_item)
		{
			m_ItemIsOn = m_item.IsPilotLight();
			if (m_ItemIsOn)
				m_item.SetPilotLight(false);
			m_item.SetInvisible(true);
			m_ItemToHands = true;
		}
	}
	
	protected void ShowItemInHands()
	{
		if (m_item)
		{
			if (m_ItemIsOn)
			{
				m_item.SetPilotLight(m_ItemIsOn);
				m_ItemIsOn = false;
			}
			m_item.SetInvisible(false);
			m_Player.GetItemAccessor().OnItemInHandsChanged();
		}
		m_ItemToHands = false;
	}
	
	protected void CommitSuicide()
	{
		Weapon_Base weapon;
		WeaponEventBase weapon_event = new WeaponEventTrigger;
		
		//firearm suicide
		if ( Weapon_Base.CastTo(weapon,m_Player.GetItemInHands()) )
		{
			//TODO : check multiple muzzles for shotguns, eventually
			if (weapon.CanFire())
			{
				m_Player.SetSuicide(true);
				weapon.ProcessWeaponEvent(weapon_event);
				m_Callback.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_END2);
				if (m_Player.IsAlive()) 	(GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Call(this.KillPlayer);
			}
			else
			{
				weapon.ProcessWeaponEvent(weapon_event);
				m_Callback.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_END);
			}
		}
		//melee weapon suicide
		else if ( m_Player.GetItemInHands() && m_Player.GetItemInHands().ConfigIsExisting("suicideAnim") )
		{
			m_Callback.RegisterAnimationEvent("Death",1);
			m_Player.SetSuicide(true);
			m_Callback.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_END2);
			//GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.KillPlayer, 4000, false);
		}
		//unarmed "suicide" :)
		else
		{
			m_Callback.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_END2);
		}
	}

	void KillPlayer()
	{
		if (GetGame().IsServer())
		{
			m_Player.SetHealth(0);
		}
	}
	
	void CreateEmoteCBFromMenu(int id)
	{
		m_MenuEmote = new EmoteFromMenu(id);
	}
	
	void EndCallback()
	{
		if (!m_Callback)
		{
			//Print("no callback!");
			return;
		}
		
		ScriptInputUserData ctx = new ScriptInputUserData;
		if (GetGame().IsMultiplayer() && GetGame().IsClient() && !ctx.CanStoreInputUserData())
		{
			//Print("ctx unavailable");
			return;
		}
		else if (GetGame().IsMultiplayer() && GetGame().IsClient() && ctx.CanStoreInputUserData())
		{
			//Print("sending ECB cancel request");
			ctx.Write(INPUT_UDT_GESTURE);
			ctx.Write(-1);
			ctx.Send();
		}
		
		//Print("ending ECB - proper");
		if (m_CurrentGestureID == ID_EMOTE_DANCE)
		{
			m_Callback.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_INTERRUPT);
		}
		else
		{
			m_Callback.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_END);
		}
	}
	
	void PlayEmoteFromMenu()
	{
		ScriptInputUserData ctx = new ScriptInputUserData;
		if (GetGame().IsMultiplayer() && GetGame().IsClient() && ctx.CanStoreInputUserData())
		{
			ctx.Write(INPUT_UDT_GESTURE);
			ctx.Write(m_MenuEmote.m_ID);
			m_RPSOutcome = -1;
			switch (m_MenuEmote.m_ID)
			{
				case ID_EMOTE_RPS :
					m_RPSOutcome = Math.RandomInt(0,3);
				break;
				
				case ID_EMOTE_RPS_R :
					m_RPSOutcome = 0;
				break;
				
				case ID_EMOTE_RPS_P :
					m_RPSOutcome = 1;
				break;
				
				case ID_EMOTE_RPS_S :
					m_RPSOutcome = 2;
				break;
			}
			if (m_RPSOutcome != -1) 	ctx.Write(m_RPSOutcome);
			ctx.Send();
			PlayEmote(m_MenuEmote.m_ID);
			m_MenuEmote = NULL;
		}
		else if (!GetGame().IsMultiplayer())
		{
			PlayEmote(m_MenuEmote.m_ID);
			m_MenuEmote = NULL;
		}
	}
	
	bool IsControllsLocked()
	{
		return m_controllsLocked;
	}
	
	bool CanPlayEmote()
	{
		if ( !m_Player || m_Player.GetCommand_Action() || m_Player.GetCommandModifier_Action() )
		{	
			return false;
		}
		if ( m_Player.GetWeaponManager().IsRunning() || m_Player.GetActionManager().GetRunningAction() )
		{
			return false;
		}
		
		if ( m_HIC.IsWeaponRaised() || m_Player.IsFighting() || m_Player.IsSwimming() || m_Player.IsClimbingLadder() || m_Player.IsFalling() || m_Player.IsUnconscious() ) 	//TODO rework conditions into something better?
		{
			return false;
		}
		if (m_Player.GetCommand_Vehicle())
		{
			if (m_Player.GetCommand_Vehicle().GetTransport())
			{
				return false;
			}
		}
		return true;
	}
};
