enum BARIMAnimState
{
	DEFAULT 			= 0, 	///< default weapon state, closed and discharged
	OPENED 				= 1,
	JAMMED 				= 2,
};

enum BARIMStableStateID
{
	UNKNOWN				=  0,
	EmptyDischarged		=  1,
	LoadedCharged		=  2,
	LoadedDischarged	=  3,
	LoadedJammed		=  4,
}


class BARIMEmptyDischarged extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { EmptyDischarged C0"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } EmptyDischarged C0"); }
	override int GetCurrentStateID () { return BARIMStableStateID.EmptyDischarged; }
	override bool HasBullet () { return false; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return false; }
};
class BARIMLoadedCharged extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { LoadedCharged C1"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } LoadedCharged C1"); }
	override int GetCurrentStateID () { return BARIMStableStateID.LoadedCharged; }
	override bool HasBullet () { return false; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return false; }
};
class BARIMLoadedDischarged extends WeaponStableState
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { LoadedDischarged D1"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } LoadedDischarged D1"); }
	override int GetCurrentStateID () { return BARIMStableStateID.LoadedDischarged; }
	override bool HasBullet () { return true; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return false; }
};
class BARIMLoadedJammed extends WeaponStateJammed
{
	override void OnEntry (WeaponEventBase e) { wpnPrint("[wpnfsm] { LoadedJammed J1"); super.OnEntry(e); }
	override void OnExit (WeaponEventBase e) { super.OnExit(e); wpnPrint("[wpnfsm] } LoadedJammed J1"); }
	override int GetCurrentStateID () { return BARIMStableStateID.LoadedJammed; }
	override bool HasBullet () { return true; }
	override bool HasMagazine () { return false; }
	override bool IsJammed () { return true; }
};


/**@class		BoltRifle_Base
 * @brief		base for bolt action rifles
 * @NOTE		name copies config base class
 **/
class BoltActionRifle_InnerMagazine_Base extends BoltActionRifle_Base
{
	ref WeaponStateBase C0;
	ref	WeaponStateBase C1;
	ref	WeaponStateBase D1;
	ref	WeaponStateBase J1;
	
	
	override void InitStateMachine ()
	{
		m_abilities.Insert(new AbilityRecord(WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED_KEEP));
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE));
		m_abilities.Insert(new AbilityRecord(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START));
		m_abilities.Insert(new AbilityRecord(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_END));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_COCKED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_UNCOCKED));
		m_abilities.Insert(new AbilityRecord(WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY));
		
		
		C0 = new BARIMEmptyDischarged(this, NULL, BARIMAnimState.DEFAULT);
		C1 = new BARIMLoadedCharged(this, NULL, BARIMAnimState.DEFAULT);
		J1 = new BARIMLoadedJammed(this, NULL, BARIMAnimState.JAMMED);
		D1 = new BARIMLoadedDischarged(this, NULL, BARIMAnimState.DEFAULT);
		
		WeaponStateBase		Mech_C1 = new WeaponChargingInnerMag(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);
		WeaponStateBase		Mech_D1 = new WeaponChargingInnerMag(this, NULL, WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED);

		
		LoopedChambering 	Chamber_C0 = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);
		LoopedChambering 	Chamber_C1 = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED_KEEP, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);
		LoopedChambering 	Chamber_D1 = new LoopedChambering(this, NULL, WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE);
		
		WeaponStateBase		Unjam_J1 = new WeaponUnjamming(this, NULL, WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START);
		
		WeaponStateBase		Trigger_C0 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY);
		WeaponStateBase		Trigger_C1 = new WeaponFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL);
		WeaponStateBase		Trigger_D1 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY);
		WeaponStateBase		Trigger_J1 = new WeaponDryFire(this, NULL, WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY);

		
		WeaponEventBase _fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventBase __L__ = new WeaponEventLoad1Bullet;
		WeaponEventBase __T__ = new WeaponEventTrigger;
		WeaponEventBase __U__ = new WeaponEventUnjam;
		WeaponEventBase	__M__ = new WeaponEventMechanism;
		WeaponEventBase _abt_ = new WeaponEventHumanCommandActionAborted;
		WeaponEventBase _rto_ = new WeaponEventReloadTimeout;
		WeaponEventBase _dto_ = new WeaponEventDryFireTimeout;

		m_fsm = new WeaponFSM();
		
		// Mechanism
//----------------------------------------	
		m_fsm.AddTransition(new WeaponTransition( C0,		__M__,  Mech_C1));
				
		m_fsm.AddTransition(new WeaponTransition( C1,		__M__,  Mech_C1));
		m_fsm.AddTransition(new WeaponTransition( Mech_C1,	_fin_,  C0, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech_C1,	_fin_,  C1));
		m_fsm.AddTransition(new WeaponTransition( Mech_C1,	_abt_,  C0, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech_C1,	_abt_,  C1));
		
		m_fsm.AddTransition(new WeaponTransition( D1,		__M__,  Mech_D1));
		m_fsm.AddTransition(new WeaponTransition( Mech_D1,	_fin_,  C0, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech_D1,	_fin_,  D1, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech_D1,	_fin_,  C1));
		m_fsm.AddTransition(new WeaponTransition( Mech_D1,	_abt_,  C0, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech_D1,	_abt_,  D1, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition( Mech_D1,	_abt_,  C1));
		
		
//----------------------------------------		
		
		m_fsm.AddTransition(new WeaponTransition( C0,			__L__,  Chamber_C0));
		m_fsm.AddTransition(new WeaponTransition( Chamber_C0,	_fin_,  C1));
		m_fsm.AddTransition(new WeaponTransition( Chamber_C0,	_abt_,  C0, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition( Chamber_C0,	_abt_,  C1));
		
		m_fsm.AddTransition(new WeaponTransition( C1,			__L__,  Chamber_C1, NULL, new GuardNot(new WeaponGuardInnerMagazineFullShareChamber(this))));
		m_fsm.AddTransition(new WeaponTransition( Chamber_C1,	_fin_,  C1));
		m_fsm.AddTransition(new WeaponTransition( Chamber_C1,	_abt_,  C1));
		
		m_fsm.AddTransition(new WeaponTransition( D1,			__L__,  Chamber_D1));
		m_fsm.AddTransition(new WeaponTransition( Chamber_D1,	_fin_,  C1));
		m_fsm.AddTransition(new WeaponTransition( Chamber_D1,	_abt_,  D1, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition( Chamber_D1,	_abt_,  C1));
		
//------------------------------------------
		
			
		m_fsm.AddTransition(new WeaponTransition(J1, 		__U__, Unjam_J1));
		m_fsm.AddTransition(new WeaponTransition(Unjam_J1,	_fin_, J1, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_J1,	_fin_, C0, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_J1,	_fin_, C1));
		m_fsm.AddTransition(new WeaponTransition(Unjam_J1,	_abt_, J1, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_J1,	_abt_, C0, NULL, new WeaponGuardChamberEmpty(this)));
		m_fsm.AddTransition(new WeaponTransition(Unjam_J1,	_abt_, C1));
		
		
//-----------------------------------------
		
		// fire
		m_fsm.AddTransition(new WeaponTransition(C0,			__T__, Trigger_C0)); // fire.cocked
		m_fsm.AddTransition(new WeaponTransition(Trigger_C0,	_fin_, C0));
		m_fsm.AddTransition(new WeaponTransition(Trigger_C0,	_dto_, C0));
		m_fsm.AddTransition(new WeaponTransition(Trigger_C0,	_abt_, C0));
		
		m_fsm.AddTransition(new WeaponTransition(C1,			__T__, Trigger_C1)); // fire.cocked
		m_fsm.AddTransition(new WeaponTransition(Trigger_C1,	_fin_, J1, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(Trigger_C1,	_fin_, D1));
		m_fsm.AddTransition(new WeaponTransition(Trigger_C1,	_rto_, J1, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(Trigger_C1,	_rto_, D1, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition(Trigger_C1,	_rto_, C1));
		m_fsm.AddTransition(new WeaponTransition(Trigger_C1,	_abt_, J1, NULL, new WeaponGuardJammed(this)));
		m_fsm.AddTransition(new WeaponTransition(Trigger_C1,	_abt_, D1, NULL, new WeaponGuardChamberFiredOut(this)));
		m_fsm.AddTransition(new WeaponTransition(Trigger_C1,	_abt_, C1));
		
		m_fsm.AddTransition(new WeaponTransition(D1,			__T__, Trigger_D1)); // fire.cocked
		m_fsm.AddTransition(new WeaponTransition(Trigger_D1,	_fin_, D1));
		m_fsm.AddTransition(new WeaponTransition(Trigger_D1,	_dto_, D1));
		m_fsm.AddTransition(new WeaponTransition(Trigger_D1,	_abt_, D1));
		
		m_fsm.AddTransition(new WeaponTransition(J1,			__T__, Trigger_J1)); // fire.cocked
		m_fsm.AddTransition(new WeaponTransition(Trigger_J1,	_fin_, J1));
		m_fsm.AddTransition(new WeaponTransition(Trigger_J1,	_dto_, J1));
		m_fsm.AddTransition(new WeaponTransition(Trigger_J1,	_abt_, J1));
		
//---------------------------------------------	
		
		m_fsm.SetInitialState(C0);

		SelectionBulletHide();
		SelectionMagazineHide();

		m_fsm.Start();
	}	
};


