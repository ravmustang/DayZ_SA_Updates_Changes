enum PumpShotgunAnimState
{
	DEFAULT 			= 0, 	///< default weapon state, closed and discharged
};

enum PumpShotgunStableStateID
{
	UNKNOWN				=  0,
	EmptyDischarged		=  1,
	LoadedFireout		=  2,
	LoadedCharged		=  3,
	LoadedJammed		=  4,
}

class PumpShotgunEmptyDischarged extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { EmptyDischarged E_D"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } EmptyDischarged E_D"); }
	override int GetCurrentStateID () { return RifleStableStateID.EmptyDischarged; }
	override bool HasBullet () { return false; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return false; }
};
class PumpShotgunLoadedFireout extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { LoadedFireout L_F"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } LoadedFireout L_F"); }
	override int GetCurrentStateID () { return RifleStableStateID.EmptyCharged; }
	override bool HasBullet () { return false; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return false; }
};
class PumpShotgunLoadedCharged extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { LoadedCharged L_C"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } LoadedCharged L_C"); }
	override int GetCurrentStateID () { return RifleStableStateID.LoadedCharged; }
	override bool HasBullet () { return true; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return false; }
};
class PumpShotgunJammed extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { LoadedJammed L_J"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } LoadedJammed L_J"); }
	override int GetCurrentStateID () { return RifleStableStateID.LoadedJammed; }
	override bool HasBullet () { return true; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return true; }
};

class LoopedChambering_Wait4ShowBullet extends WeaponStateBase { };

class ShotgunChambering extends WeaponStateBase
{
	WeaponActions m_action;
	int m_startActionType;
	int m_endActionType;
	Magazine m_srcMagazine; /// source of the cartridge

	ref WeaponStateBase m_start;
	ref WeaponChambering_Cartridge m_chamber;
	ref LoopedChambering_Wait4ShowBullet m_w4sb;
	ref LoopedChambering_EndLoop m_end;

	void ShotgunChambering (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int startActionType = -1, int endActionType = -1)
	{
		m_action = action;
		m_startActionType = startActionType;
		m_endActionType = endActionType;

		// setup nested state machine
		m_start = new WeaponChambering_Start(m_weapon, this, m_action, m_startActionType);
		m_chamber = new WeaponChambering_Cartridge(m_weapon, this);
		m_w4sb = LoopedChambering_Wait4ShowBullet(m_weapon, this);
		m_end = new LoopedChambering_EndLoop(m_weapon, this, m_action, m_endActionType);
		// events
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventContinuousLoadBulletStart __lS_ = new WeaponEventContinuousLoadBulletStart;
		WeaponEventContinuousLoadBulletEnd   __lE_ = new WeaponEventContinuousLoadBulletEnd;
		WeaponEventAnimBulletShow  __bs_ = new WeaponEventAnimBulletShow;
		WeaponEventAnimBulletInMagazine __bM_ = new WeaponEventAnimBulletInMagazine;
		WeaponEventAnimBulletHide  __bh_ = new WeaponEventAnimBulletHide;

		m_fsm = new WeaponFSM(this); // @NOTE: set owner of the submachine fsm
		m_fsm.AddTransition(new WeaponTransition(m_start  , __bs_, m_chamber));
		m_fsm.AddTransition(new WeaponTransition(m_chamber, __bM_, m_w4sb, NULL, new GuardAnd(new WeaponGuardHasAmmoInLoopedState(m_chamber), new WeaponGuardChamberHasRoomForMoreThanOne(m_weapon))));
		
		// @TODO: rychla sekvence start / stop zpusobi zaseknuti animace
		//m_fsm.AddTransition(new WeaponTransition(m_chamber, __bM_, m_end, NULL, new GuardOr(new GuardNot(new WeaponGuardHasAmmoInLoopedState(m_chamber)), new WeaponGuardChamberHasRoomForOne(m_weapon))));
		m_fsm.AddTransition(new WeaponTransition(m_chamber, __bh_, m_end, NULL, new GuardOr(new GuardNot(new WeaponGuardHasAmmoInLoopedState(m_chamber)), new WeaponGuardChamberHasRoomForOne(m_weapon))));
		m_fsm.AddTransition(new WeaponTransition(m_w4sb   , __bs_, m_chamber));
		m_fsm.AddTransition(new WeaponTransition(m_w4sb   , __lE_, m_end));
		m_fsm.AddTransition(new WeaponTransition(m_chamber, __lE_, m_end));
		m_fsm.AddTransition(new WeaponTransition(m_end    , _fin_, NULL));

		m_fsm.SetInitialState(m_start);
	}

	override void OnEntry (WeaponEventBase e)
	{
		if (e != NULL)
		{
			if (e.m_magazine != NULL)
			{
				wpnDebugPrint("[wpnfsm] LoopedChambering, taking mag from event e.mag=" + e.m_magazine.ToString());
				m_srcMagazine = e.m_magazine;
			}
		}
		else
		{
			wpnDebugPrint("[wpnfsm] LoopedChambering (e=NULL), m_srcMagazine=" + m_srcMagazine.ToString());
		}
		wpnDebugPrint("[wpnfsm] m_srcMagazine=" + m_srcMagazine.ToString());

		m_chamber.m_srcMagazine = m_srcMagazine;

		super.OnEntry(e); // @NOTE: super at the end (prevent override from submachine start)
	}
	override void OnExit (WeaponEventBase e)
	{
		super.OnExit(e);
	}
};



/**@class	Mp133Shotgun_Base
 * @brief	basic mp5 submachine gun
 **/
class Mp133Shotgun : Rifle_Base
{
	ref WeaponStateBase E;
ref WeaponStateBase F;
ref WeaponStateBase L;
ref WeaponStateBase J;
	
	override void GetContinuousActions( out TIntArray actions )
	{
		actions.Insert(AT_LOAD_MULTI_BULLET_TO_WEAPON);
	}
	
	override void InitStateMachine ()
	{
		// setup abilities
		m_abilities.Insert(new AbilityRecord(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START));
		m_abilities.Insert(new AbilityRecord(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_END));
	
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY));
		
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_SHOTGUN_COCKED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_SHOTGUN_UNCOCKED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE));
	
		// basic weapon states
		E = new PumpShotgunEmptyDischarged(this, NULL, RifleAnimState.DEFAULT);
		F = new PumpShotgunLoadedFireout(this, NULL, RifleAnimState.DEFAULT);
		L = new PumpShotgunLoadedCharged(this, NULL, RifleAnimState.DEFAULT);
		J = new PumpShotgunJammed(this, NULL, RifleAnimState.DEFAULT);
//--------------------------------------------------------------------------------------
		WeaponStateBase		Mech_E = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);
		WeaponStateBase		Mech_F = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);
		WeaponStateBase		Mech_L = new WeaponCharging(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);

		LoopedChambering 	Chamber_E = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_SHOTGUN_UNCOCKED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);
		LoopedChambering 	Chamber_F = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_SHOTGUN_UNCOCKED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);
		LoopedChambering 	Chamber_L = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_SHOTGUN_COCKED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);
		
		WeaponStateBase		Unjam_J = new WeaponUnjamming(this, NULL, WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START);
		
		WeaponStateBase		Trigger_E = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY);
		WeaponStateBase		Trigger_F = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY);
		WeaponStateBase		Trigger_L = new WeaponFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL);
		WeaponStateBase		Trigger_J = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY);
		// extend Rifle_Base fsm
		/*LoopedChambering lch = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_OPENED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);
		LoopedChambering psh = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);
		LoopedChambering lch2 = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_OPENED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE,true);
		LoopedChambering psh2 = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE,true);
		*/
		
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventBase __lS_ = new WeaponEventContinuousLoadBulletStart;
		WeaponEventBase __L__ = new WeaponEventLoad1Bullet;
		WeaponEventBase __T__ = new WeaponEventTrigger;
		WeaponEventBase __U__ = new WeaponEventUnjam;
		WeaponEventBase	__M__ = new WeaponEventMechanism;
		WeaponEventBase _abt_ = new WeaponEventHumanCommandActionAborted;
		WeaponEventBase _rto_ = new WeaponEventReloadTimeout;
		WeaponEventBase _dto_ = new WeaponEventDryFireTimeout;

		m_fsm = new WeaponFSM();
		
		// Mechanism
		/*m_fsm.AddTransition(new WeaponTransition( D1	, __M__,	Mech));
		m_fsm.AddTransition(new WeaponTransition( C1	, __M__,	Mech));
		m_fsm.AddTransition(new WeaponTransition( Mech  , _fin_,	C0, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech  , _fin_,	C1));
		m_fsm.AddTransition(new WeaponTransition( Mech	, _abt_,	C0, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech	, _abt_,	D1, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech	, _abt_,	C1));*/
//----------------------------------------	
			
		m_fsm.AddTransition(new WeaponTransition( L,		__M__,  Mech_L));
		m_fsm.AddTransition(new WeaponTransition( Mech_L,	_fin_,  E, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech_L,	_fin_,  L));
		m_fsm.AddTransition(new WeaponTransition( Mech_L,	_abt_,  E, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech_L,	_abt_,  L));
		
		m_fsm.AddTransition(new WeaponTransition( F,		__M__,  Mech_F));
		m_fsm.AddTransition(new WeaponTransition( Mech_F,	_fin_,  E, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech_F,	_fin_,  F, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech_F,	_fin_,  L));
		m_fsm.AddTransition(new WeaponTransition( Mech_F,	_abt_,  E, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech_F,	_abt_,  F, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech_F,	_abt_,  L));
		
				
//----------------------------------------		
		
		m_fsm.AddTransition(new WeaponTransition( E,			__L__,  Chamber_E));
		m_fsm.AddTransition(new WeaponTransition( Chamber_E,	_fin_,  L));
		m_fsm.AddTransition(new WeaponTransition( Chamber_E,	_abt_,  E, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( Chamber_E,	_abt_,	L));
		
		m_fsm.AddTransition(new WeaponTransition( L,			__L__,  Chamber_L, NULL, new GuardNot(new WeaponGuardChamberFull(this))));
		m_fsm.AddTransition(new WeaponTransition( Chamber_L,	_fin_,  L));
		m_fsm.AddTransition(new WeaponTransition( Chamber_L,	_abt_,  L));
		
		m_fsm.AddTransition(new WeaponTransition( F,			__L__,  Chamber_F));
		m_fsm.AddTransition(new WeaponTransition( Chamber_F,	_fin_,  L));
		m_fsm.AddTransition(new WeaponTransition( Chamber_F,	_abt_,  F, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition( Chamber_F,	_abt_,  L));
		
//----------------------------------------		
		
/*		m_fsm.AddTransition(new WeaponTransition( C0,			__lS_,  LChamber_C0));
		m_fsm.AddTransition(new WeaponTransition( LChamber_C0,	_fin_,  C1));
		m_fsm.AddTransition(new WeaponTransition( LChamber_C0,	_abt_,  C0, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( LChamber_C0,	_abt_,  C1));
		
		m_fsm.AddTransition(new WeaponTransition( C1,			__lS_,  LChamber_C1, NULL, new GuardNot(new WeaponGuardChamberFull(this))));
		m_fsm.AddTransition(new WeaponTransition( LChamber_C1,	_fin_,  C1));
		m_fsm.AddTransition(new WeaponTransition( LChamber_C1,	_abt_,  C1));
		
		m_fsm.AddTransition(new WeaponTransition( D1,			__lS_,  LChamber_D1));
		m_fsm.AddTransition(new WeaponTransition( LChamber_D1,	_fin_,  C1));
		m_fsm.AddTransition(new WeaponTransition( LChamber_D1,	_abt_,  D1, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition( LChamber_D1,	_abt_,  C1));*/
		
//------------------------------------------
		
			
		m_fsm.AddTransition(new WeaponTransition(J, 		__U__, Unjam_J));
		m_fsm.AddTransition(new WeaponTransition(Unjam_J,	_fin_, J, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_J,	_fin_, E, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_J,	_fin_, L));
		m_fsm.AddTransition(new WeaponTransition(Unjam_J,	_abt_, J, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_J,	_abt_, E, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_J,	_abt_, L));
		
		
//-----------------------------------------
		
		// fire
		m_fsm.AddTransition(new WeaponTransition(E,			__T__, Trigger_E)); // fire.cocked
		m_fsm.AddTransition(new WeaponTransition(Trigger_E,	_fin_, E));
		m_fsm.AddTransition(new WeaponTransition(Trigger_E,	_dto_, E));
		m_fsm.AddTransition(new WeaponTransition(Trigger_E,	_abt_, E));
		
		m_fsm.AddTransition(new WeaponTransition(L,			__T__, Trigger_L)); // fire.cocked
		m_fsm.AddTransition(new WeaponTransition(Trigger_L,	_fin_, F));
		m_fsm.AddTransition(new WeaponTransition(Trigger_L,	_rto_, J, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(Trigger_L,	_rto_, F, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition(Trigger_L,	_rto_, L));
		m_fsm.AddTransition(new WeaponTransition(Trigger_L,	_abt_, J, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(Trigger_L,	_abt_, F, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition(Trigger_L,	_abt_, L));
		
		m_fsm.AddTransition(new WeaponTransition(F,			__T__, Trigger_F)); // fire.cocked
		m_fsm.AddTransition(new WeaponTransition(Trigger_F,	_fin_, F));
		m_fsm.AddTransition(new WeaponTransition(Trigger_F,	_dto_, F));
		m_fsm.AddTransition(new WeaponTransition(Trigger_F,	_abt_, F));
		
		m_fsm.AddTransition(new WeaponTransition(J,			__T__, Trigger_J)); // fire.cocked
		m_fsm.AddTransition(new WeaponTransition(Trigger_J,	_fin_, J));
		m_fsm.AddTransition(new WeaponTransition(Trigger_J,	_dto_, J));
		m_fsm.AddTransition(new WeaponTransition(Trigger_J,	_abt_, J));
		
//-----------------------------------------	

		// load cartridge
/*		m_fsm.AddTransition(new WeaponTransition( E_C, __lS_,  lch, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( lch, _fin_,  L_C));
		
		m_fsm.AddTransition(new WeaponTransition( E_C, __L__,  lch2));
		m_fsm.AddTransition(new WeaponTransition( lch2, _fin_,  L_C));

		m_fsm.AddTransition(new WeaponTransition( E_D, __lS_,  lch, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( lch, _fin_,  L_C));
		
		m_fsm.AddTransition(new WeaponTransition( E_D, __L__,  lch2));
		m_fsm.AddTransition(new WeaponTransition( lch2, _fin_,  L_C));

		m_fsm.AddTransition(new WeaponTransition( L_C, __lS_,  psh, NULL, new GuardNot(new WeaponGuardChamberFull(this))));
		m_fsm.AddTransition(new WeaponTransition( psh, _fin_,  L_C));
		
	
		m_fsm.AddTransition(new WeaponTransition( L_C, __L__,  psh2, NULL, new GuardNot(new WeaponGuardChamberFull(this))));
		m_fsm.AddTransition(new WeaponTransition( psh2, _fin_,  L_C));

		// @TODO: chci load z loaded_fired_out - nefunguje mi nejak ten m_eject
		m_fsm.AddTransition(new WeaponTransition( E_D, __lS_, psh, NULL, new GuardNot(new WeaponGuardChamberFull(this))));
		m_fsm.AddTransition(new WeaponTransition(psh, _fin_,  L_C));
	*/	
		
		m_fsm.SetInitialState(E);

		SelectionBulletHide();
		SelectionMagazineHide();

		m_fsm.Start();
	
	
	
	
	
	
	
	
	
	
	

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
		
//---------------------------	
		// extend Rifle_Base fsm
		/*ShotgunChambering lch = new ShotgunChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_SHOTGUN_COCKED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);
		ShotgunChambering psh = new ShotgunChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_SHOTGUN_UNCOCKED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);
		ShotgunChambering psh2 = new ShotgunChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_SHOTGUN_UNCOCKED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);

		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventBase __lS_ = new WeaponEventContinuousLoadBulletStart;

		// load cartridge
		m_fsm.AddTransition(new WeaponTransition(E_C, __lS_, lch, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(lch, _fin_, L_C));

		m_fsm.AddTransition(new WeaponTransition(E_D, __lS_, lch, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(lch, _fin_, L_C));

		m_fsm.AddTransition(new WeaponTransition(L_C, __lS_, psh, NULL, new GuardNot(new WeaponGuardChamberFull(this))));
		m_fsm.AddTransition(new WeaponTransition(psh, _fin_, L_C));

		// @TODO: chci load z loaded_fired_out - nefunguje mi nejak ten m_eject
		m_fsm.AddTransition(new WeaponTransition(E_D, __lS_, psh2, NULL, new GuardNot(new WeaponGuardChamberFull(this))));
		m_fsm.AddTransition(new WeaponTransition(psh2, _fin_, L_C));*/
	}
};

