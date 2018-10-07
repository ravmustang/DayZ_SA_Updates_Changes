/*
DayZPlayerImplement

this file is implemenation of dayzPlayer in script
- logic of movement
- camera switching logic

*/

class DayZPlayerImplement extends DayZPlayer
{
	// Timer 	m_UpdateTick;

	protected ref HumanMovementState 					m_MovementState = new HumanMovementState();	//!< cached ptr for movement state
	protected ref DayZPlayerImplementAiming				m_AimingModel;
	protected ref DayZPlayerImplementMeleeCombat		m_MeleeCombat;
	protected ref DayZPlayerMeleeFightLogic_LightHeavy	m_MeleeFightLogic;
	protected ref DayZPlayerImplementSwimming			m_Swimming;
	protected ref DayZPlayerImplementFallDamage			m_FallDamage;
	protected float 									m_FallYDiff;
	protected float										m_SprintedTime;
	protected bool										m_SprintFull;
	protected bool										m_IsFireWeaponRaised;
	protected bool										m_Camera3rdPerson;
	protected bool										m_CameraEyeZoom;
	protected bool										m_CameraIronsighs;
	protected bool										m_CameraOptics;
	protected float 									m_CameraIronsighsNotRaisedTime;
	protected bool										m_IsTryingHoldBreath;
	protected bool										m_IsShootingFromCamera;
	protected bool										m_PlayerSelected;
	protected bool 										m_Suicide;
	protected bool										m_IsUnconscious;
	protected bool										m_ShouldBeUnconscious;
	protected int			 							m_LastCommandBeforeUnconscious;
	ref WeaponDebug										m_WeaponDebug;
	protected bool 										m_ShouldReturnToOptics;
	protected bool 										m_ForceHandleOptics;
	

	//! constructor 
	void 	DayZPlayerImplement()
	{
		//Print("DayZPlayerImplement: Init");
		m_IsFireWeaponRaised = false;
		m_SprintFull = false;
		m_SprintedTime = 0;
		m_AimingModel = new DayZPlayerImplementAiming(this);
		m_MeleeCombat = new DayZPlayerImplementMeleeCombat(this);
		m_MeleeFightLogic = new DayZPlayerMeleeFightLogic_LightHeavy(this);
		m_Swimming = new DayZPlayerImplementSwimming(this);
		m_FallDamage = new DayZPlayerImplementFallDamage(this);
		m_CameraEyeZoom = false;
		m_CameraOptics = false;
		m_IsShootingFromCamera = true;
		#ifdef PLATFORM_CONSOLE
		m_Camera3rdPerson = true;
		#endif
	}

	DayZPlayerImplementAiming GetAimingModel()
	{
		return m_AimingModel;
	}

	DayZPlayerImplementMeleeCombat GetMeleeCombat()
	{
		return m_MeleeCombat;
	}
	
	DayZPlayerMeleeFightLogic_LightHeavy GetMeleeFightLogic()
	{
		return m_MeleeFightLogic;
	}

	//! destructor 
	void 	~DayZPlayerImplement()
	{
	}

	override bool IsEyeZoom()
	{
		return m_CameraEyeZoom;
	}
	
	override bool IsShootingFromCamera()
	{
		return m_IsShootingFromCamera;
	}
	
	void OverrideShootFromCamera(bool pState)
	{
		m_IsShootingFromCamera = pState;
	}

	bool IsInIronsights()
	{
		return m_CameraIronsighs;
	}
	
	bool IsInOptics()
	{
		return m_CameraOptics;
	}

	bool IsFireWeaponRaised()
	{
		return m_IsFireWeaponRaised;
	}

	bool IsTryingHoldBreath()
	{
		return m_IsTryingHoldBreath;
	}

	bool IsSprintFull()
	{
		return m_SprintFull;
	}

	void ShowWeaponDebug(bool show)
	{
		if (show)
		{
			m_WeaponDebug = new WeaponDebug;
		}
		else
		{
			m_WeaponDebug = null;
		}
	}
	
	//TODO: Swap from optics
	void SetIronsights(bool value)
	{
		m_CameraIronsighs = value;
	}

	//! Implementations only! - used on PlayerBase
	bool CanConsumeStamina(EStaminaConsumers consumer);
	void DepleteStamina(EStaminaModifiers modifier) {}


	//-------------------------------------------------------------
	//!
	//! HandleDeath
	//! 

	bool	HandleDeath(int pCurrentCommandID)
	{
		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_DEATH)
		{
			return true;
		}
		
		if ( ((GetGame().IsMultiplayer() && !GetGame().IsServer()) || (!GetGame().IsMultiplayer())) && GetGame().GetUIManager().ScreenFadeVisible())
		{
			return true;
		}

		if (!IsAlive() && g_Game.GetMissionState() == g_Game.MISSION_STATE_GAME)
		{
			//GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(ShowDeadScreen, DEAD_SCREEN_DELAY, false, true);
			//GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(ShowDeadScreen, true);
			//if (IsPlayerSelected()) 	GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(SimulateDeath, true);
			
			if (!m_Suicide) 	StartCommand_Death();
			//StartCommand_Death();
			
			// disable voice communication
			GetGame().GetWorld().SetVoiceOn(false);
			
			if( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT && !GetGame().GetUIManager().IsMenuOpen(MENU_INGAME))
			{
				/*AbstractSoundScene asc = GetGame().GetSoundScene();
				asc.SetSoundVolume(0,5);*/
				
				// hide exit dialog if displayed
				GetGame().GetUIManager().CloseDialog();
			}
			return true;
		}

		return false;
	}
	
	static const int DEAD_SCREEN_DELAY = 1000;
	
	void ShowDeadScreen(bool show)
	{
	#ifndef NO_GUI
		if (show && IsPlayerSelected())
		{
		#ifdef PLATFORM_CONSOLE
			GetGame().GetUIManager().ScreenFadeIn(0, "You are dead", FadeColors.DARK_RED, FadeColors.WHITE);
		#else
			GetGame().GetUIManager().ScreenFadeIn(0, "You are dead", FadeColors.BLACK, FadeColors.WHITE);
		#endif
		}
		else
		{
			GetGame().GetUIManager().ScreenFadeOut(0);
		}
	#endif
	}
	
	void SimulateDeath(bool state)
	{
		if (g_Game.GetMissionState() != DayZGame.MISSION_STATE_GAME)	 return;
		//Print("Calling simulate death in state: " + state)
		//controlls
		LockControls(state);
		
		//video
		ShowDeadScreen(state);
		
		//audio
		if (state == true)
		{
			GetGame().GetSoundScene().SetSoundVolume(0,0);
			GetGame().GetSoundScene().SetSpeechExVolume(0,0);
			GetGame().GetSoundScene().SetMusicVolume(0,0);
			GetGame().GetSoundScene().SetVOIPVolume(0,0);
			GetGame().GetSoundScene().SetRadioVolume(0,0);
		}
		else
		{
			GetGame().GetSoundScene().SetSoundVolume(g_Game.m_volume_sound,1);
			GetGame().GetSoundScene().SetSpeechExVolume(g_Game.m_volume_speechEX,1);
			GetGame().GetSoundScene().SetMusicVolume(g_Game.m_volume_music,1);
			GetGame().GetSoundScene().SetVOIPVolume(g_Game.m_volume_VOIP,1);
			GetGame().GetSoundScene().SetRadioVolume(g_Game.m_volume_radio,1);
		}
	}
	
	void LockControls(bool state)
	{
		if (state == true)
		{
			GetGame().GetInput().ChangeGameFocus(1, INPUT_DEVICE_MOUSE);
			GetGame().GetInput().ChangeGameFocus(1, INPUT_DEVICE_KEYBOARD);
			GetGame().GetInput().ChangeGameFocus(1, INPUT_DEVICE_GAMEPAD);
			
			if (GetGame().GetUIManager()) 	GetGame().GetUIManager().ShowUICursor(true);
		}
		else
		{
			GetGame().GetInput().ChangeGameFocus(-1, INPUT_DEVICE_MOUSE);
			GetGame().GetInput().ChangeGameFocus(-1, INPUT_DEVICE_KEYBOARD);
			GetGame().GetInput().ChangeGameFocus(-1, INPUT_DEVICE_GAMEPAD);
			
			if (GetGame().GetUIManager())
			{
				if (GetGame().GetUIManager().GetMenu())
				{
					GetGame().GetUIManager().ShowUICursor(true);
				}
				else
				{
					GetGame().GetUIManager().ShowUICursor(false);
				}
			}
		}
	}
	
	DayZPlayerInventory GetDayZPlayerInventory ()
	{
		DayZPlayerInventory inv = DayZPlayerInventory.Cast(GetInventory());
		return inv;
	}

	override void OnInputForRemote (ParamsReadContext ctx)
	{
		int userDataType = 0;
		if (!ctx.Read(userDataType))
		{
			Error("DayZPlayer: OnInputForRemote - cannot read input type");
			return;
		}

		//Print("[inv] Input For Remote! type=" + userDataType);
		switch (userDataType)
		{
			case INPUT_UDT_WEAPON_REMOTE_SYNC:
				GetDayZPlayerInventory().OnSyncFromRemoteWeapon(ctx);
				break;
			case INPUT_UDT_WEAPON_REMOTE_EVENT:
				GetDayZPlayerInventory().OnEventFromRemoteWeapon(ctx);
				break;
			case INPUT_UDT_HAND_REMOTE_EVENT:
				GetDayZPlayerInventory().OnHandEventFromRemote(ctx);
				break;
			case INPUT_UDT_INVENTORY:
				GetDayZPlayerInventory().OnInputUserDataFromRemote(ctx);
				break;
			default:
				Error("OnInputForRemote - unknown userDataType=" + userDataType);
				break;
		}
	}

	override void OnInputFromServer (ParamsReadContext ctx)
	{
		int userDataType = 0;
		if (!ctx.Read(userDataType))
		{
			Error("DayZPlayer: OnInputFromServer - cannot read input type");
			return;
		}

		switch (userDataType)
		{
			case INPUT_UDT_INVENTORY:
				GetDayZPlayerInventory().OnServerInventoryCommand(ctx);
				break;
			default:
				Error("OnInputFromServer - unknown userDataType=" + userDataType);
				break;
		}
	}

	void SendSoundEvent(EPlayerSoundEventID id);
	
	override void OnItemInHandsChanged () { GetItemAccessor().OnItemInHandsChanged(); }

	WeaponManager GetWeaponManager () { return null; }

	//-------------------------------------------------------------
	//!
	//! HandleWeapons
	//!
	void HandleWeapons(float pDt, Entity pInHands, HumanInputController pInputs, out bool pExitIronSights)
	{
		HumanCommandWeapons		hcw = GetCommandModifier_Weapons();
    	GetDayZPlayerInventory().HandleWeaponEvents(pDt, pExitIronSights);

		Weapon_Base weapon;
		Class.CastTo(weapon, pInHands);

		// @TODO: refactor to action (rm from input, mk action)
		if (pInputs.IsReloadOrMechanismSingleUse() && GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT )
		{
			if (weapon && weapon.CanProcessWeaponEvents())
			{
				if (GetWeaponManager().CanEjectBullet(weapon))
				{
					GetWeaponManager().EjectBullet();
					pExitIronSights = true;
				}
			}
		}

		GetMovementState(m_MovementState);

		// hold breath
		if (pInputs.IsHoldBreath() && m_MovementState.IsRaised() && (IsInIronsights() || IsInOptics()))
		{
			m_IsTryingHoldBreath = true;
		}
		else
		{
			m_IsTryingHoldBreath = false;
		}

		if (pInputs.IsFireModeChange())
		{
			weapon.SetNextMuzzleMode(weapon.GetCurrentMuzzle());
		}
		if (pInputs.IsZeroingUp())
		{
			weapon.StepZeroingUp();
		}
		if (pInputs.IsZeroingDown())
		{
			weapon.StepZeroingDown();
		}
		
		if (m_CameraIronsighs || !weapon.CanEnterIronsights() || m_ForceHandleOptics) 	// HACK straight to optics, if ironsights not allowed
		{
			ItemOptics optic = weapon.GetAttachedOptics();
			if (optic)
				HandleOptic(optic, false, pInputs, pExitIronSights);
		}

		if (weapon && weapon.CanProcessWeaponEvents())
		{
			if (pInputs.IsReloadOrMechanismContinuousUseStart())
			{
				weapon.ProcessWeaponEvent(new WeaponEventUnjam(this));
			}
		}

		if (!m_MovementState.IsRaised())
		{
			m_IsFireWeaponRaised = false;
			if (weapon && weapon.IsInOptics())
				weapon.ExitOptics();

			return; // if not raised => return
		}
		else
			m_IsFireWeaponRaised = true;
		
		//! fire
		if (weapon && weapon.CanProcessWeaponEvents() && !GetGame().GetInput().GetActionDown(UAHeavyMeleeAttack, false))
		{
			bool autofire = weapon.GetCurrentModeAutoFire(weapon.GetCurrentMuzzle());
			int burst = weapon.GetCurrentModeBurstSize(weapon.GetCurrentMuzzle());
			if (!autofire)
			{
				if (pInputs.IsUseButtonDown())
				{
					weapon.ProcessWeaponEvent(new WeaponEventTrigger(this));
				}
			}
			else
			{
				if (pInputs.IsUseButton())
				{
					weapon.ProcessWeaponEvent(new WeaponEventTrigger(this));
				}
			}
		}
	}
	
	void HandleOptic (notnull ItemOptics optic, bool inHands, HumanInputController pInputs, out bool pExitOptics)
	{
		Weapon_Base weapon = Weapon_Base.Cast(optic.GetHierarchyParent());
		if (inHands)
		{
			if (m_CameraOptics)
			{
				if (!optic.IsInOptics())
					m_CameraOptics = false;
			}
			else
			{
				if (optic.IsInOptics())
					m_CameraOptics = true;
			}
		}
		else if (weapon)
		{
			/*if (pInputs.IsWeaponRaised() && m_ShouldReturnToOptics && !m_CameraOptics && optic)
			{
				SwitchOptics(optic,true);
			}*/
			
			if (pInputs.IsZoomIn())
			{
				if (!m_CameraOptics && (weapon.CanEnterIronsights() && m_CameraIronsighs))
				{
					SwitchOptics(optic,true);
				}
				else
				{
					optic.StepFOVUp();
				}
			}
	
			if (pInputs.IsZoomOut())
			{
				if (m_CameraOptics)
				{
					if (optic.GetStepFOVIndex() > 0)
					{
						optic.StepFOVDown();
					}
					else if (weapon.CanEnterIronsights())
					{
						//SwitchOptics(optic,false);
						ExitSights();
						m_CameraIronsighs = true;
						m_ShouldReturnToOptics = false;
						m_ForceHandleOptics = false;
					}
				}
			}
		}
	}

	//-------------------------------------------------------------
	//!
	//! HandleDamageHit
	//! 

	bool m_DamageHitFullbody = false;
	int m_DamageHitAnimType = -1;
	float m_DamageHitDir = 0;

	bool HandleDamageHit(int pCurrentCommandID)
	{
		if (m_DamageHitAnimType != -1)
		{
			if (m_DamageHitFullbody)
			{
				StartCommand_Damage(m_DamageHitAnimType, m_DamageHitDir);
				m_DamageHitAnimType = -1;
				return true;
			}
			else
			{
				AddCommandModifier_Damage(m_DamageHitAnimType, m_DamageHitDir);
				m_DamageHitAnimType = -1;
				return false;
			}
		}

		return pCurrentCommandID == DayZPlayerConstants.COMMANDID_DAMAGE;
	}

	//! selects animation type and direction based on damage system data
	bool EvaluateDamageHitAnimation(TotalDamageResult pDamageResult, int pDamageType, EntityAI pSource, string pComponent, string pAmmoType, vector pModelPos, out int pAnimType, out float pAnimHitDir, out bool pAnimHitFullbody)
	{
		pAnimType = 0;
		pAnimHitFullbody = false; // additive anm
		
		switch(pDamageType)
		{
			case 0: // DT_CLOSE_COMBAT
				//! ignore hit impacts in prone (for now)
				GetMovementState(m_MovementState);
				if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE || m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
					return false;

				//! impact from infected attack will be light only
			 	if (pSource.IsInherited(DayZInfected))
					break;

				pAnimType = GetGame().ConfigGetInt("cfgAmmo " + pAmmoType + " hitAnimation");			
				if (pAnimType == 1 && !m_MeleeFightLogic.IsInBlock())
					pAnimHitFullbody = true;
			break;
			case 1: // DT_FIREARM
				return false; // skip evaluation of dmg hit anim (tmp)
				/*
				float fireDamage = pDamageResult.GetHighestDamage("Health");
				if (fireDamage > 80.0)
					pAnimHitFullbody = true;
				*/
			break;
			case 2: // DT_EXPLOSION
			break;
			case 3: // DT_CUSTOM (used by Fall Damage)
				return false; //! skip evaluation of dmg hit animation
			break;
		}

		//! direction
		vector targetDirection = GetDirection();
		vector toSourceDirection = (pSource.GetPosition() - GetPosition());

		targetDirection[1] = 0;
		toSourceDirection[1] = 0;

		targetDirection.Normalize();
		toSourceDirection.Normalize();

		float cosFi = vector.Dot(targetDirection, toSourceDirection);
		vector cross = targetDirection * toSourceDirection;

		pAnimHitDir = Math.Acos(cosFi) * Math.RAD2DEG;
		if (cross[1] < 0)
			pAnimHitDir = -pAnimHitDir;

		//Print("hitdir: " + pAnimHitDir.ToString());

		return true;
	}

	//! event from damage system
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, string component, string ammo, vector modelPos)
	{
		super.EEHitBy(damageResult, damageType, source, component, ammo, modelPos);

		int animType;
		float animHitDir;
		bool animHitFullbody;
		if (EvaluateDamageHitAnimation(damageResult, damageType, source, component, ammo, modelPos, animType, animHitDir, animHitFullbody))
		{
			DayZPlayerSyncJunctures.SendDamageHit(this, animType, animHitDir, animHitFullbody);
		}
		else
		{
			SendSoundEvent(EPlayerSoundEventID.TAKING_DMG_LIGHT);
			//add code here
		}

		// interupt melee for non-blocked hit or heavy hit
		if( !m_MeleeFightLogic.IsInBlock() || animHitFullbody )
		{
			HumanCommandMelee hcm = GetCommand_Melee();
			if(hcm) hcm.Cancel();
		}
	}

	override void EEHitByRemote(int damageType, EntityAI source, string component, string ammo, vector modelPos)
	{
		super.EEHitByRemote(damageType, source, component, ammo, modelPos);
		
		Print("DayZPlayerImplement : EEHitByRemote");
	}

	//-------------------------------------------------------------
	//!
	//! This HeadingModel
	//! 

	float 	m_fLastHeadingDiff = 0;

	//!
	override bool	HeadingModel(float pDt, SDayZPlayerHeadingModel pModel)
	{
		//! on ladder - do nothing 
		if (GetCommand_Ladder())
		{
			m_fLastHeadingDiff = 0;
			return false;
		}
		if (GetCommand_Vehicle())
		{
			m_fLastHeadingDiff = 0;
			return false;
		}
		if( GetCommand_Unconscious() )
		{
			m_fLastHeadingDiff = 0;
			//pModel.m_iCamMode = DayZPlayerConstants.CAMERAMODE_HEAD;
			return false;
		}


#ifdef DEVELOPER
		int	actMenuValue = DiagMenu.GetValue(DayZPlayerConstants.DEBUG_TURNSLIDE);
		if (actMenuValue != 0)
		{
			//! this clamps heading 
			return DayZPlayerImplementHeading.ClampHeading(pDt, pModel, m_fLastHeadingDiff);
		}
#endif

		GetMovementState(m_MovementState);
		if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
		{
			m_fLastHeadingDiff = 0;
			return false;
		}

		return DayZPlayerImplementHeading.RotateOrient(pDt, pModel, m_fLastHeadingDiff);	
	}


	//-------------------------------------------------------------
	//!
	//! This Aiming Model
	//! 

	override bool	AimingModel(float pDt, SDayZPlayerAimingModel pModel)
	{
		GetMovementState(m_MovementState);
		
		bool isInIronsights = IsInIronsights();
		bool isFireWeaponRaised = IsFireWeaponRaised();
		
		if (isInIronsights)
		{
			pModel.m_fAimSensitivity = 0.3;
		}
		else if (isFireWeaponRaised)
		{
			pModel.m_fAimSensitivity = 0.5;
		}
		else
		{
			pModel.m_fAimSensitivity = 1.0;
		}
		
		return m_AimingModel.ProcessAimFilters(pDt, pModel, m_MovementState.m_iStanceIdx);
	}

	//-------------------------------------------------------------
	//!
	//! This is main command logic
	//! 

	//! 
	override void  CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		HumanInputController hic = GetInputController();
		GetMovementState(m_MovementState);
		/*
		// Unconscious test 
		if (hic.IsUseButtonDown())
		{
			if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE)
			{
				StartCommand_Unconscious();
			}
			else if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_UNCONSCIOUS)
			{
				HumanCommandUnconscious	hcu = GetCommand_Unconscious();
				hcu.WakeUp();
			}
		}
		*/

		//! 3rd person camera
		if (hic.CameraViewChanged())
		{
			if (!GetGame().GetWorld().Is3rdPersonDisabled())
			{
				m_Camera3rdPerson = !m_Camera3rdPerson;
			}
			else
			{
				m_Camera3rdPerson = false;
			}
		}

		//! reset ironsights && optics
		bool	prevIronSights 	= m_CameraIronsighs;
		bool	prevOptics		= m_CameraOptics;
		m_CameraIronsighs		= false;
		m_CameraOptics			= false;

		//! top priority - handle death first
		if (HandleDeath(pCurrentCommandID))
		{
			return;
		}
		
		////////////////////////////////////////////////
		// ironsights logic

		// now 
		m_CameraIronsighs = prevIronSights;
		m_CameraOptics = prevOptics;

		//! check for raise double click
		/*
		bool	sightChange = false;
		
		if (hic.IsWeaponRaised())
		{	
			if (m_CameraIronsighsNotRaisedTime > 0 && m_CameraIronsighsNotRaisedTime < 0.3)
			{
				sightChange 	= true;
			}
			m_CameraIronsighsNotRaisedTime = 0.0;
		}
		else 
		{
			m_CameraIronsighsNotRaisedTime += pDt;
		}
		*/
	
		if (hic.IsSightChange()) // || sightChange)
		{
			HumanItemAccessor 	hia = GetItemAccessor();
			PlayerBase playerPB = PlayerBase.Cast(this);
			if (hia.IsItemInHandsWeapon() && playerPB.GetWeaponManager() && !playerPB.GetWeaponManager().IsRunning() )
			{
				Weapon_Base weapon = Weapon_Base.Cast(GetHumanInventory().GetEntityInHands());
				ItemOptics optic = weapon.GetAttachedOptics();
				if (weapon.CanEnterIronsights() && !m_ShouldReturnToOptics)
				{
					m_CameraIronsighs = !m_CameraIronsighs;
					Print("To ironsights " +  m_CameraIronsighs.ToString());
	
					if (m_CameraIronsighs)
					{
						// go to ironsights - disable ironsights when
						//! if !raised
						//! if sprinting
						if ( !GetWeaponManager().IsRunning() )
						{
							GetMovementState(m_MovementState);
							if (!hic.IsWeaponRaised() || m_MovementState.m_iMovement == 3)
							{
								Print("To ironsights or optics: failed -> sprint or no aim");
								ExitSights();
							}
						}
						else
						{
							m_CameraIronsighs = !m_CameraIronsighs;
						}
					}
					else
					{
						ExitSights();
					}
				}
				else if (optic)
				{
					if (!m_CameraOptics)
					{
						SwitchOptics(optic, true);
					}
					else
					{
						//m_CameraOptics = false;
						ExitSights();
					}
				}
			}
		}
		else if ( m_CameraIronsighs || m_CameraOptics )
		{
			hia = GetItemAccessor();
			if (hia.IsItemInHandsWeapon())
			{
				// clear ironsights when lowered
				GetMovementState(m_MovementState);
				if (!m_MovementState.IsRaised())
				{
					Print("From ironsights and optics");
					ExitSights();
				}
			}
		}

		//! handle finished commands
		if (pCurrentCommandFinished)
		{
			if( pCurrentCommandID == DayZPlayerConstants.COMMANDID_UNCONSCIOUS && m_LastCommandBeforeUnconscious == DayZPlayerConstants.COMMANDID_SWIM)
			{
				StartCommand_Swim();
				return;
			}
			// if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_ACTION || pCurrentCommandID == DayZPlayerConstants.COMMANDID_MELEE)
			// start moving

			//! default behaviou after finish is to start move
			StartCommand_Move();

			return;
		};

		//! Sprint attack limiting - player has to be in full sprint for at least 0.5s
		//--------------------------------------------
		
		HumanCommandMove hcm = GetCommand_Move();
		if (hcm && hcm.GetCurrentMovementSpeed() > 2.99 && m_MovementState.m_iMovement == 3)
		{
			m_SprintedTime += pDt;
			if( m_SprintedTime > 0.5)
				m_SprintFull = true;
			else
				m_SprintFull = false;

			//Print(m_SprintedTime);
		}
		else
		{
			m_SprintedTime = 0.0;
			m_SprintFull = false;
		}

		//--------------------------------------------
		// swimming handling
		if (m_Swimming.HandleSwimming(pCurrentCommandID, hcm, m_MovementState))
		{
			return;			
		}
		
		//--------------------------------------------
		// ladder handling
		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_LADDER)
		{
			return;
		}

		//--------------------------------------------
		// vehicle handling
		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_VEHICLE)
		{
			return;
		}


		//--------------------------------------------
		// fall handling

		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_FALL)
		{
			// vector	vel;
			// PhysicsGetVelocity(vel);
			// Print(vel);

			HumanCommandFall fall = GetCommand_Fall();

			if (fall.PhysicsLanded())
			{
				// land
				m_FallYDiff = m_FallYDiff - GetPosition()[1];
				Print(m_FallYDiff);
				if (m_FallYDiff < 0.5)
				{
					fall.Land(HumanCommandFall.LANDTYPE_NONE);
				}
				else if (m_FallYDiff < 1.0)
				{
					fall.Land(HumanCommandFall.LANDTYPE_LIGHT);
				}
				else if (m_FallYDiff < 2.0)
				{
					fall.Land(HumanCommandFall.LANDTYPE_MEDIUM);
				}
				else
				{
					fall.Land(HumanCommandFall.LANDTYPE_HEAVY);
				}

				m_FallDamage.HandleFallDamage(m_FallYDiff);
			}

			return;
		}

		// start falling ? 
		if (PhysicsIsFalling())
		{
			StartCommand_Fall(0);
			m_FallYDiff = GetPosition()[1];
			return;
		}

		//--------------------------------------------
		// damage hits
		if (HandleDamageHit(pCurrentCommandID))
		{
			return;
		}

		//--------------------------------------------
		// handle jumping



#ifndef NO_GUI
#ifdef DEVELOPER
		//! enable this later for everything

		if (DiagMenu.GetValue(DayZPlayerConstants.DEBUG_ENABLEJUMP))
		{
			SHumanCommandClimbResult 	ret;

			//! debug draw climb heuristics
			HumanCommandClimb.DoClimbTest(this, ret, 0x3);

		}

#endif
#endif 

		// start jump 
		if(hic.IsJumpClimb())
		{
			if( !IsRestrained() && !IsUnconscious() && CanConsumeStamina(EStaminaConsumers.JUMP) )
			{
				m_FallYDiff = GetPosition()[1];
				StartCommand_Fall(3.5);
				DepleteStamina(EStaminaModifiers.JUMP);
				return;
			}
		}

		////////////////////////////////////////////////
		// Eye Zoom logic
		
		if (hic.IsZoom() && !m_CameraEyeZoom && !hic.IsMeleeFastAttackModifier()) // tmp uses check for shift modifier
		{
			m_CameraEyeZoom = true;
			//Print("To EyeZoom " +  m_CameraEyeZoom.ToString());
		}
		else if (!hic.IsZoom() && m_CameraEyeZoom )
		{
			//Print("From EyeZoom " +  m_CameraEyeZoom.ToString());
			m_CameraEyeZoom = false;
		}

		//--------------------------------------------
		// anything whats handled by InputController

		//! 

		if (hic)
		{
			//! get weapon 
			EntityAI entityInHands = GetHumanInventory().GetEntityInHands();
			bool isWeapon		= entityInHands	&& entityInHands.IsInherited(Weapon);
			bool isOptics		= entityInHands	&& entityInHands.IsInherited(ItemOptics);
			if (isWeapon)
			{
				bool exitIronSights = false;
				HandleWeapons(pDt, entityInHands, hic, exitIronSights);
				if (exitIronSights)
					return;
			}
			else if (isOptics)
			{
				ItemOptics opticInHands = ItemOptics.Cast(entityInHands);
				bool exitOptic = false;
				HandleOptic(opticInHands, true, hic, exitOptic);
				if (exitOptic)
					return;				
			}
			else
			{
				GetMovementState(m_MovementState);
				if(m_MeleeFightLogic.Process(pCurrentCommandID, hic, entityInHands, m_MovementState))
				{
					return;
				}
			}
		}
	}

	void ExitSights ()
	{
		if (m_CameraOptics)
			m_ShouldReturnToOptics = true;
		else
			m_ShouldReturnToOptics = false;
		
		m_CameraIronsighs = false;
		m_CameraOptics = false;

		ItemOptics optics = null;
		EntityAI entityInHands = GetHumanInventory().GetEntityInHands();
		Weapon weapon = Weapon.Cast(entityInHands);
		if (weapon)
		{
			optics = weapon.GetAttachedOptics();
		}
		else
		{
			optics = ItemOptics.Cast(entityInHands);
		}

		if (optics)
		{
			SwitchOptics(optics,false);
		}
	}
	
	void SwitchOptics(ItemOptics optic, bool state)
	{
		// ready for backpack hiding, if needed...
		Clothing backpack;
		
		if (state)
		{
			m_CameraOptics = true;
			if (optic.HasEnergyManager())
				optic.GetCompEM().SwitchOn();
			optic.HideSelection("hide");
			
			if (Clothing.CastTo(backpack,FindAttachmentBySlotName("Back")))
			{
				backpack.HideSelection("hide_OpticsView");
			}
			optic.EnterOptics();
			m_ForceHandleOptics = true;
		}
		else
		{
			optic.ShowSelection("hide");
			
			if (Clothing.CastTo(backpack,FindAttachmentBySlotName("Back")))
			{
				backpack.ShowSelection("hide_OpticsView");
			}
			optic.ExitOptics();
			if (optic.HasEnergyManager())
				optic.GetCompEM().SwitchOff();
			m_CameraOptics = false;
		}
	}

	//-------------------------------------------------------------
	//!
	//! This is Debug functionality 
	//! 

	float 	m_TestDamageCounter 	= -1;	//! VKOSTIK - tady dat 0 misto -1
	float 	m_DebugDirectionVal 	= -180;
	bool	m_DebugTypeVal 			= false;

	int 		m_DebugWeaponChangeStage = 0;
	string	m_DebugWeaponChangeItem;
	int			m_DebugWeaponChangeShowSlot;

	//! 
	void  CommandHandlerDebug(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{

		//! 
		/*
		// debug - print ironsights pos / dir

		HumanItemAccessor hia = GetItemAccessor();

		vector pos, dir;

		if (hia.WeaponGetCameraPoint(pos, dir))
		{
			string 	a = "ITACC: ";
			a += pos.ToString();
			a += " ";
			a += dir.ToString();

		//Print("DayZPlayerImplement: Init");

			Print(a);
		}
		*/

		/* {
			HumanCommandMove mc = GetCommand_Move();
			if (mc)
			{

				float gcms = mc.GetCurrentMovementSpeed();
				Print(gcms);
			}
		} */

		//! 
		if (GetPluginManager())
		{
			PluginDayzPlayerDebug playerDebug = PluginDayzPlayerDebug.Cast(GetPluginManager().GetPluginByType(PluginDayzPlayerDebug));
			if (playerDebug)
			{
				playerDebug.CommandHandler();

				//! debug handling of adding commands 
				if (m_DebugWeaponChangeStage == 0)
				{

					// debug weapon change
					int 		hideSlot;

					if (playerDebug.IsWeaponChange(m_DebugWeaponChangeItem, hideSlot, m_DebugWeaponChangeShowSlot))
					{
						Print("Change Weapon started: " + m_DebugWeaponChangeItem + "," + hideSlot.ToString() + "," + m_DebugWeaponChangeShowSlot.ToString());

						HumanCommandWeapons	w = GetCommandModifier_Weapons();
						w.StartAction(WeaponActions.HIDE, hideSlot);

						m_DebugWeaponChangeStage = 1;
					}
				}
				else if (m_DebugWeaponChangeStage == 1)
				{
					// wait for weapon hide to meet event 
					HumanCommandWeapons	w2 = GetCommandModifier_Weapons();
					if (w2 && w2.GetRunningAction() == WeaponActions.HIDE)
					{
						if (w2.IsEvent() == WeaponEvents.CHANGE_HIDE)
						{
							//! set item in hands
							Print("Change Weapon - hidden: ");

							//! fuj
							PlayerBase player = PlayerBase.Cast(this);
							EntityAI item_in_hands = player.GetHumanInventory().GetEntityInHands();
							if (item_in_hands != NULL && player.CanDropEntity(item_in_hands) && GetGame().GetPlayer().GetHumanInventory().CanRemoveEntityInHands())
							{
								player.PredictiveDropEntity(item_in_hands);
							}

							if (m_DebugWeaponChangeItem != "")
							{
								InventoryLocation dst = new InventoryLocation;
								dst.SetHands(this, NULL);
								EntityAI item = GetGame().SpawnEntity(m_DebugWeaponChangeItem, dst);
							}

							// LocalTakeEntityToHands(item, dst);

							//! start show
							w2.StartAction(WeaponActions.SHOW, 3);

							m_DebugWeaponChangeStage = 2;
						}
					}
				}
				else if (m_DebugWeaponChangeStage == 2)
				{
					HumanCommandWeapons	w3 = GetCommandModifier_Weapons();
					if (w3 && w3.IsActionFinished())
					{
						m_DebugWeaponChangeStage = 0;
					}
				}
			}
		}
	

		//! damage test
		if (m_TestDamageCounter >= 0)
		{
			// HumanCommandDamage damage = GetCommandModifier_Damage();
			if (m_TestDamageCounter > 3)
			{
				m_DebugDirectionVal = m_DebugDirectionVal + 90;
				m_TestDamageCounter = 0;

				if (m_DebugDirectionVal > 90)
				{
					m_DebugDirectionVal = -180;
					m_DebugTypeVal		= !m_DebugTypeVal;
				}

				if (m_DebugTypeVal)
				{
					AddCommandModifier_Damage(0, m_DebugDirectionVal); // starts light
				}
				else
				{
					StartCommand_Damage(0, m_DebugDirectionVal); // starts heavy (full body)
				}
				// 0.. types
				// from: -180 back, -90 left, 0 front, 90 right, 180 back 
				//AddCommandModifier_Damage(0, directionVal); // starts light
				//StartCommand_Damage(0, 0); // starts heavy (full body)
			}

			m_TestDamageCounter += pDt;
		}


#ifndef NO_GUI
#ifdef DEVELOPER

		// injury / exhaustion
		{
			HumanCommandAdditives ad = GetCommandModifier_Additives();

			//! diag menu - injured
			float v = DiagMenu.GetValue(DayZPlayerConstants.DEBUG_SHOWINJURY);
			if (v > 0)
			{
				v = (v - 1.0) * 0.1;
				ad.SetInjured(v, true);
			}

			//! diag menu - injured
			v = DiagMenu.GetValue(DayZPlayerConstants.DEBUG_SHOWEXHAUSTION);
			if (v > 0)
			{
				v = (v - 1) * 0.1;
				ad.SetExhaustion(v, true);
			}
		}
#endif
#endif 

	}



	//-------------------------------------------------------------
	//!
	//! This is main camera selection logic
	//! 
	override int 	CameraHandler(int pCameraMode)
	{
		// Print("DayZPlayerCfgBase: Camera Handler called");

		/* {
			int perItemCamUD = GetCurrentPerItemCameraUD();
			string 		a	= "Per Item Camera User Data: " + ToString(perItemCamUD);
			Print(a);
		} */

		//! ironsights
		if (m_CameraIronsighs)
		{
			if (m_CameraOptics)
				return DayZPlayerCameras.DAYZCAMERA_OPTICS;
			else
				return DayZPlayerCameras.DAYZCAMERA_IRONSIGHTS;
		}
		else if (m_CameraOptics)
			return DayZPlayerCameras.DAYZCAMERA_OPTICS;
		
		//ResetOpticsPP();
		
		if (!m_Camera3rdPerson)
		{
			if( m_IsUnconscious )
			{
				return DayZPlayerCameras.DAYZCAMERA_1ST_UNCONSCIOUS;
			}
			return DayZPlayerCameras.DAYZCAMERA_1ST;
		}
		else // if (pCameraMode == DayZPlayerConstants.CAMERAMODE_EXTERNAL)
		{
			HumanCommandVehicle vehicleCommand = GetCommand_Vehicle();
			if( vehicleCommand )
			{
				Transport transport = vehicleCommand.GetTransport();
				return transport.Get3rdPersonCameraType();
			}
			
			//! get movement state 
			GetMovementState(m_MovementState);
			
			//! VKOSTIK: v ostatnich commandech mimo COMMANDID_MOVE je 
			//! m_MovementState.m_iStanceIdx - DayZPlayerConstants.STANCEIDX_ERECT
			//! m_MovementState.m_iMovement = 0 (idle)

			//! COMMANDID_ACTION returns stance right - ERECT/CROUCH

			//! melee camera - not melee - stays in stance camera

			/* if (m_MovementState.m_CommandTypeId == DayZPlayerConstants.COMMANDID_MELEE)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_ERC_RAISED;
			}*/

			//		
			// normal movement cameras			
			if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_CRO;
			}

			else if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDCROUCH)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_CRO_RAISED;
			}

			if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE)
			{
				if( m_IsUnconscious )
				{
					return DayZPlayerCameras.DAYZCAMERA_3RD_ERC;
				}
				return DayZPlayerCameras.DAYZCAMERA_3RD_PRO;
			}
			else if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_PRO_RAISED;
			}

			if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT)
			{
				EntityAI entityInHands = GetHumanInventory().GetEntityInHands();
				bool isWeapon		= entityInHands	&& entityInHands.IsInherited(Weapon);

				if (isWeapon) 
				{
					return DayZPlayerCameras.DAYZCAMERA_3RD_ERC_RAISED;
				}
				else
				{
					return DayZPlayerCameras.DAYZCAMERA_3RD_ERC_RAISED_MELEE;
				}
			}

			else if (m_MovementState.m_iMovement == 3)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_ERC_SPR;
			}

			// DayZPlayerConstants.STANCEIDX_ERECT
			return DayZPlayerCameras.DAYZCAMERA_3RD_ERC;

			//
		}

		return DayZPlayerCameras.DAYZCAMERA_1ST;
	}


	//-------------------------------------------------------------
	//!
	//! Input userdata
	//! 

	void OnInputUserDataReceived(ParamsReadContext ctx)
	{
		int userDataTypeParam = 0;
		if (!ctx.Read(userDataTypeParam))
		{
			Error("DayZPlayerImplement: cannot read input type");
			return;
		}

		OnInputUserDataProcess(userDataTypeParam, ctx);
	}

	bool OnInputUserDataProcess(int userDataType, ParamsReadContext ctx)
	{
		if (userDataType == INPUT_UDT_MELEE_TARGET)
		{
			EntityAI target = null;
			if (ctx.Read(target))
			{
				m_MeleeCombat.SetTargetObject(target);
			}

			int hitZoneIdx = -1;
			if (ctx.Read(hitZoneIdx))
			{
				m_MeleeCombat.SetHitZoneIdx(hitZoneIdx);
			}
			return true;
		}

		return false;
	}

	//-------------------------------------------------------------
	//!
	//! SyncJunctures
	//! 

	void OnSyncJuncture(int pJunctureID, ParamsReadContext pCtx)
	{
		switch (pJunctureID)
		{
		case DayZPlayerSyncJunctures.SJ_DAMAGE_HIT:
			DayZPlayerSyncJunctures.ReadDamageHitParams(pCtx, m_DamageHitAnimType, m_DamageHitDir, m_DamageHitFullbody);
			break;
		}
	}

	AnimBootsType GetBootsType()
	{
		Entity boots = GetInventory().FindAttachment(InventorySlots.FEET);
		if (boots == NULL)
			return AnimBootsType.None;

		string bootsName = boots.GetType();

		if (bootsName.Contains("Sneakers") || bootsName.Contains("AthleticShoes") || bootsName.Contains("JoggingShoes"))
		{
			return AnimBootsType.Sneakers;
		}
		else if (bootsName.Contains("Boots") || bootsName.Contains("Wellies") || bootsName.Contains("Shoes"))
		{
			return AnimBootsType.Boots;
		}
		else
		{
			Print("Unrecognized boots type: \"" + bootsName + "\"");
			return AnimBootsType.None;
		}
	}
	
	//Get top body clothes
	AnimUpperBodyType GetBodyAttachmentType()
	{
		Entity attachment = GetInventory().FindAttachment(InventorySlots.BODY);
		if(attachment == NULL)
			return AnimUpperBodyType.None;
		
		string name = attachment.GetType();
		
		if (name.Contains("BomberJacket") || name.Contains("HikingJacket") || name.Contains("HuntingJacket") || name.Contains("QuiltedJacket") || name.Contains("Raincoat") || name.Contains("TacticalShirt") || name.Contains("TrackSuitJacket"))
		{
			return AnimUpperBodyType.NylonJacket;
		}
		else if (name.Contains("TShirt") || name.Contains("MedicalScrubsShirt") || name.Contains("NurseDress"))
		{
			return AnimUpperBodyType.TShirt;
		}
		else if (name.Contains("Shirt") || name.Contains("WoolDress") || name.Contains("Sweater"))
		{
			return AnimUpperBodyType.WoolShirt;
		}
		else if (name.Contains("ParamedicJacket") || name.Contains("FirefighterJacket") || name.Contains("GorkaEJacket") || name.Contains("TTSKOJacket") || name.Contains("M65Jacket"))
		{
			return AnimUpperBodyType.HeavyJacket;
		}
		else if (name.Contains("LeatherJacket") || name.Contains("RidersJacket"))
		{
			return AnimUpperBodyType.LeatherJacket;
		}
		else if (name.Contains("WoolCoat"))
		{
			return AnimUpperBodyType.Coat;
		}
		else if (name.Contains("PrisonersJacket") || name.Contains("PoliceJacket") || name.Contains("LabCoat") || name.Contains("USMCJacket") || name.Contains("PoliceJacketOrel"))
		{
			return AnimUpperBodyType.ChemlonDress;
		}
		else if (name.Contains("Ghillie"))
		{
			return AnimUpperBodyType.Ghillie;
		}
		else
		{
			Print("Unrecognized upper body type: \"" + name + "\"");
			return AnimUpperBodyType.None;
		}
	}
	
	//Get weapon on back
	AnimRangedWeaponType GetShoulderAttachmentType()
	{
		Entity shoulderAttachment = GetInventory().FindAttachment(InventorySlots.SHOULDER);
		if(shoulderAttachment == NULL)
			return AnimRangedWeaponType.None;
		
		string shoulderAttachmentName = shoulderAttachment.GetType();
		
		if (shoulderAttachmentName.Contains("Shotgun"))
		{
			return AnimRangedWeaponType.Shotgun;
		}
		else
		{
			return AnimRangedWeaponType.Rifle;
		}
	}

	//-------------------------------------------------------------
	//!
	//! Sounds
	//! 

	AbstractWave PlaySound(SoundObject so, SoundObjectBuilder sob)
	{
		if (so == NULL)
		{
			//Print("PlaySound: NULL argument");
			return NULL;
		}

		so.SetPosition(GetPosition());
		AbstractWave wave = GetGame().GetSoundScene().Play3D(so, sob);
		return wave;
	}

	void AddNoise(NoiseParams noisePar, float noiseMultiplier = 1.0)
	{
		if(noisePar != NULL)
			GetGame().GetNoiseSystem().AddNoise(this, noisePar, noiseMultiplier);
	}

	//-------------------------------------------------------------
	//!
	//! Event Processing
	//! 

	void OnStepEvent(string pEventType, string pUserString, int pUserInt)
	{
		HumanCommandFall fall = GetCommand_Fall();
		if(fall != NULL)
			return;

		DayZPlayerType type = GetDayZPlayerType();
		ref HumanMovementState	state = new HumanMovementState();
		GetMovementState(state);

		if(pUserString == "walk")
			state.m_iMovement = DayZPlayerConstants.MOVEMENTIDX_WALK;
		else if(pUserString == "run")
			state.m_iMovement = DayZPlayerConstants.MOVEMENTIDX_RUN;
		else if(pUserString == "sprint")
			state.m_iMovement = DayZPlayerConstants.MOVEMENTIDX_SPRINT;
		
		if (state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDCROUCH || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
			state.m_iMovement--;
		
		if(GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			DayZPlayerTypeStepSoundLookupTable table = type.GetStepSoundLookupTable();
			SoundObjectBuilder sound_builder = table.GetSoundBuilder(pUserInt, state.m_iMovement, GetSurfaceType().Hash(), GetBootsType());

			if(sound_builder != NULL && GetGame().GetPlayer())
			{
				SoundObject sound_object = sound_builder.BuildSoundObject();
				if (sound_object != NULL)
				{
					WaveKind kind = WaveKind.WAVEENVIRONMENTEX;
					if (GetGame().GetPlayer() != NULL && IsSoundInsideBuilding() != GetGame().GetPlayer().IsSoundInsideBuilding())
					{
						kind = WaveKind.WAVEATTALWAYS;
					}
					sound_object.SetKind(kind);
					
					PlaySound(sound_object, sound_builder);
				}
			}
		}
		
		if(GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			float noiseMultiplier = 0;
			float speedNoiseMultiplier = 0;
			float bootsNoiseMultiplier = 0;
			float surfaceNoiseMultiplier = GetSurfaceNoise(); //! gets noise multiplayer base on surface player walks on
			
			ref HumanMovementState	hms = new HumanMovementState();

			//! noise multiplier based on player speed
			GetMovementState(hms);
			switch(hms.m_iMovement)
			{
				case DayZPlayerConstants.MOVEMENTIDX_SPRINT:
					speedNoiseMultiplier = 1.0;
					break;
				case DayZPlayerConstants.MOVEMENTIDX_RUN:
					speedNoiseMultiplier = 0.66;
					break;
				case DayZPlayerConstants.MOVEMENTIDX_WALK:
					speedNoiseMultiplier = 0.33;
					break;
			}

			//! noise multiplier based on type of boots
			switch(GetBootsType())
			{
				case AnimBootsType.Boots:
					bootsNoiseMultiplier = 1.0;
					break;
				case AnimBootsType.Sneakers:
					bootsNoiseMultiplier = 0.66;
					break;
				case AnimBootsType.None:
					bootsNoiseMultiplier = 0.33;
					break;
			}

			NoiseParams noiseParams;
			if (state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT)
				noiseParams = type.GetNoiseParamsStand();
			else if (state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDCROUCH)
				noiseParams = type.GetNoiseParamsCrouch();
			else if (state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
				noiseParams = type.GetNoiseParamsProne();
			else
			{
				Print("OnStepEvent: wrong stance, id: " + state.m_iStanceIdx + "using backup with stand stance (id: 0)");
				noiseParams = type.GetNoiseParamsStand();
			}
			
			//! calcs noise multiplier from players speed, stance and surface under the player when not in idle
			if (hms.m_iMovement > DayZPlayerConstants.MOVEMENTIDX_IDLE)
			{
				noiseMultiplier = (speedNoiseMultiplier + bootsNoiseMultiplier + surfaceNoiseMultiplier) / 3;
			}

			AddNoise(noiseParams, noiseMultiplier);
		}
	}

	void OnSoundEvent(string pEventType, string pUserString, int pUserInt)
	{
		if (pEventType == "Sound")
		{
			ProcessSoundEvent(pEventType, pUserString, pUserInt);
		}
		else if (pEventType == "SoundWeapon")
		{
			ProcessWeaponEvent(pEventType, pUserString, pUserInt);
		}
		else if (pEventType == "SoundAttachment")
		{
			ProcessAttachmentEvent(pEventType, pUserString, pUserInt);
		}
		else if (pEventType == "SoundVoice")
		{
			ProcessVoiceEvent(pEventType, pUserString, pUserInt);
		}
		else
		{
			Print("OnSoundEvent: Unknown sound event \"" + pEventType + "\"");
		}
	}

	void OnParticleEvent(string pEventType, string pUserString, int pUserInt)
	{
		//if(pEventType != "Sound" && pEventType != "RFootDown")
		Print("OnParticleEvent: Not implemented yet");
	}

	
	void ProcessWeaponEvent(string pEventType, string pUserString, int pUserInt)
	{
		DayZPlayerType type = GetDayZPlayerType();
		AnimSoundEvent sound_event = NULL;

		float quantity = 0;
		
		EntityAI entityInHands = GetHumanInventory().GetEntityInHands();
		if (entityInHands != NULL && entityInHands.IsInherited(ItemBase))
		{
			ItemBase item;
			Class.CastTo(item, entityInHands);
			if(item.HasQuantity())
				quantity = (float)item.GetQuantity() / (item.GetQuantityMax() - item.GetQuantityMin());
			InventoryItemType invItemType = item.GetInventoryItemType();
			sound_event = invItemType.GetSoundEvent(pUserInt);
		}

		if (sound_event == NULL)
		{
			sound_event = type.GetSoundWeaponEvent(pUserInt);
		}

		if (sound_event != NULL)
		{
			if(GetGame().IsClient() || !GetGame().IsMultiplayer())
			{
				SoundObjectBuilder builder = sound_event.GetSoundBuilder();
				builder.SetVariable("quantity", quantity);
				builder.SetVariable("interior", IsSoundInsideBuilding());
				
				SoundObject sound_object = builder.BuildSoundObject();
				if (sound_object != NULL)
				{
					WaveKind kind = WaveKind.WAVEWEAPONSEX;
					if (GetGame().GetPlayer() != NULL && IsSoundInsideBuilding() != GetGame().GetPlayer().IsSoundInsideBuilding())
					{
						kind = WaveKind.WAVEATTALWAYS;
					}
					sound_object.SetKind(kind);
					
					PlaySound(sound_object, builder);
				}
			}
			
			if(GetGame().IsServer() || !GetGame().IsMultiplayer())
			{
				AddNoise(sound_event.m_NoiseParams);
			}
		}
		else
		{
			//commented out, some events in animation should be processed only when some items are in hand
			//string className;
			//if (entityInHands != NULL)
			//	className = entityInHands.GetType();
			//else
			//	className = "SurvivorBase";
			//Print("OnSoundEvent: eventType: \"SoundWeapon\", Not defined event id: " + pUserInt + ", in class \"" + className + "\"");
		}
	}

	void ProcessAttachmentEvent(string pEventType, string pUserString, int pUserInt)
	{
		DayZPlayerType type = GetDayZPlayerType();
		DayZPlayerTypeAttachmentSoundLookupTable table = type.GetAttachmentSoundLookupTable();
			
		array<string> attachments = new array<string>();
		pUserString.Split(",", attachments);
		for(int i = 0; i < attachments.Count(); i++)
		{
			int attachmentHash = -1;
			if(attachments[i] == "shoulder")
				attachmentHash = GetShoulderAttachmentType();
			else if(attachments[i] == "body")
				attachmentHash = GetBodyAttachmentType();
			SoundObjectBuilder sound_builder = table.GetSoundBuilder(pUserInt, attachments[i], attachmentHash);
			
			if (sound_builder != NULL)
			{
				SoundObject sound_object = sound_builder.BuildSoundObject();
				if (sound_object != NULL)
				{
					WaveKind kind = WaveKind.WAVEENVIRONMENTEX;
					if (GetGame().GetPlayer() != NULL && IsSoundInsideBuilding() != GetGame().GetPlayer().IsSoundInsideBuilding())
					{
						kind = WaveKind.WAVEATTALWAYS;
					}
					sound_object.SetKind(kind);
					
					PlaySound(sound_object, sound_builder);
				}
			}
		}
	}
	
	void ProcessSoundEvent(string pEventType, string pUserString, int pUserInt)
	{
		DayZPlayerType type = GetDayZPlayerType();
		DayZPlayerTypeAnimTable table = type.GetSoundTable();
		AnimSoundEvent sound_event;
		if( table )
		{
			sound_event = table.GetSoundEvent(pUserInt);
		}
		
		if(sound_event != NULL)
		{
			if(GetGame().IsClient() || !GetGame().IsMultiplayer())
			{
				SoundObjectBuilder objectBuilder = sound_event.GetSoundBuilder();
				objectBuilder.UpdateEnvSoundControllers(GetPosition());
				SoundObject soundObject = objectBuilder.BuildSoundObject();
				PlaySound(soundObject, objectBuilder);
			}
			
			if(GetGame().IsServer() || !GetGame().IsMultiplayer())
			{
				if(sound_event.m_NoiseParams != NULL)
					GetGame().GetNoiseSystem().AddNoise(this, sound_event.m_NoiseParams);
			}
		}
	}
	
	void ProcessVoiceEvent(string pEventType, string pUserString, int pUserInt)
	{
		DayZPlayerType type = GetDayZPlayerType();
		DayZPlayerTypeAnimTable table = type.GetSoundVoiceTable();
		AnimSoundEvent sound_event;
		if( table )
		{
			sound_event = table.GetSoundEvent(pUserInt);
		}
		
		if(sound_event != NULL)
		{
			if(GetGame().IsClient() || !GetGame().IsMultiplayer())
			{
				SoundObjectBuilder objectBuilder = sound_event.GetSoundBuilder();
				
				PlayerBase player = PlayerBase.Cast(this);
				int isMale = 0;
				int isFemale = 0;
				if(player.IsMale())
					isMale = 1;
				else
					isFemale = 1;
				
				objectBuilder.SetVariable("male", isMale);
				objectBuilder.SetVariable("female", isFemale);
				
				SoundObject soundObject = objectBuilder.BuildSoundObject();
				PlaySound(soundObject, objectBuilder);
			}
			
			if(GetGame().IsServer() || !GetGame().IsMultiplayer())
			{
				if(sound_event.m_NoiseParams != NULL)
					GetGame().GetNoiseSystem().AddNoise(this, sound_event.m_NoiseParams);
			}
		}
	}
	
	//-------------------------------------------------------------
	//!
	//! Voice over network conditions
	//! 
	bool CheckForVoNUse(EntityAI item)
	{
		PlayerBase player = PlayerBase.Cast(this);
		if (!player)
			return false;
		
		if (!IsAlive() || player.IsUnconscious())
			return false;
		return true;
	}
	
	//-------------------------------------------------------------
	//!
	//! anti-cheat condition
	//! 

	bool CheckForTakeItem(EntityAI item)
	{
		bool restrained = IsRestrained(); // IsRestrained()
		bool unconscious = IsUnconscious(); // IsRestrained()
		if (!IsAlive() || restrained || unconscious)
			return true;
		return false;
	}
	
	bool CheckForDropItem(EntityAI item)
	{
		bool restrained = IsRestrained(); // IsRestrained()
		bool unconscious = IsUnconscious(); // IsRestrained()
		if (!IsAlive() || restrained || unconscious)
			return true;
		return false;
	}
	
	//-------------------------------------------------------------
	//!
	//! Player respawn
	//! 
	bool CheckForRespawn(EntityAI item)
	{
		PlayerBase player = PlayerBase.Cast(this);
		if (!player)
			return true;
		
		if (!IsAlive() || player.IsUnconscious() /*|| IsRestrained()*/)
			return true;
		return false;
	}

#ifdef DEVELOPER

	// -------------------------------------------------------------------------
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		if (rpc_type == ERPCs.RPC_DAYZPLAYER_DEBUGSERVERWALK)
		{
			Print("ERPCs.RPC_DAYZPLAYER_DEBUGSERVERWALK");

			Param1<bool> rp = new Param1<bool>(false);

			ctx.Read(rp);

			GetInputController().OverrideMovementSpeed(rp.param1, 1);
			GetInputController().OverrideAimChangeX(rp.param1, 0.01);

		}
	}

#endif
	
	bool IsPlayerSelected()
	{
		return m_PlayerSelected;
	}
	
	bool IsRestrained();
	
	void SetSuicide(bool state)
	{
		m_Suicide = state;
	}
	
	void SetReturnToOptics(bool state)
	{
		m_ShouldReturnToOptics = state;
	}
	
/*	void ResetOpticsPP()
	{
		PPEffects.ResetPPMask();
		PPEffects.SetLensEffect(0,0,0,0);
		PPEffects.OverrideDOF(false,0,0,0,0,0);
		PPEffects.SetBlurOptics(0);
	}*/
}
