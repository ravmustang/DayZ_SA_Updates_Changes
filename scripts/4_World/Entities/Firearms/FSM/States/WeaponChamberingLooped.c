// load x bullets
class LoopedChambering_EndLoop extends WeaponStartAction
{
	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);

		//Print("[wpnfsm] LoopedChambering --------- ENDDDDDD ");

	}
	//override bool IsWaitingForActionFinish () { return true; }
};

class LoopedChambering_Wait4ShowBullet2 extends WeaponStateBase
{ 

};




/*class WeaponChambering_Start2 extends WeaponChambering_Start
{
	override void OnExit (WeaponEventBase e)
	{
		super.OnExit(e);
		
		if (e.m_player)
		{
			HumanCommandWeapons hcw = e.m_player.GetCommandModifier_Weapons();
			if (hcw)
			{
				hcw.StartAction(2, 11);
			}
		}
	}
 };*/




/*
class WeaponChambering_CartridgeEnd extends WeaponChambering_Cartridge
{
	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);
		
		if (e.m_player)
		{
			HumanCommandWeapons hcw = e.m_player.GetCommandModifier_Weapons();
			if (hcw)
			{
				hcw.StartAction(2, 11);
			}
		}
	}
	
}

*/


class LoopedChambering extends WeaponStateBase
{
	WeaponActions m_action;
	int m_startActionType;
	int m_endActionType;
	Magazine m_srcMagazine; /// source of the cartridge

	ref WeaponStateBase m_start;
	ref WeaponEjectCasing m_eject;
	ref WeaponChambering_Cartridge m_chamber;
	ref LoopedChambering_Wait4ShowBullet2 m_w4sb2;
	ref WeaponStartAction m_endLoop;
	ref BulletHide_W4T m_hideB;

	void LoopedChambering (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int startActionType = -1, int endActionType = -1)
	{
		m_action = action;
		m_startActionType = startActionType;
		m_endActionType = endActionType;

		// setup nested state machine
		m_start = new WeaponChambering_Start(m_weapon, this, m_action, m_startActionType);
		m_eject = new WeaponEjectCasing(m_weapon, this);
		m_chamber = new WeaponChambering_Cartridge(m_weapon, this);
		m_w4sb2 = LoopedChambering_Wait4ShowBullet2(m_weapon, this);
		m_hideB = new BulletHide_W4T(m_weapon, this);
		m_endLoop = new WeaponStartAction(m_weapon, this, m_action, m_endActionType); // @NOTE: termination playing action - dummy?
		// events
		WeaponEventBase							_fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventContinuousLoadBulletStart	__lS_ = new WeaponEventContinuousLoadBulletStart;
		WeaponEventContinuousLoadBulletEnd		__lE_ = new WeaponEventContinuousLoadBulletEnd;
		WeaponEventAnimBulletShow				__bs_ = new WeaponEventAnimBulletShow;
		WeaponEventAnimBulletHide				__bh_ = new WeaponEventAnimBulletHide;
		WeaponEventAnimBulletEject				__be_ = new WeaponEventAnimBulletEject;
		WeaponEventAnimBulletInMagazine			__bM_ = new WeaponEventAnimBulletInMagazine;
		WeaponEventAnimBulletShow2				_bs2_ = new WeaponEventAnimBulletShow2;
		WeaponEventAnimReloadingLoopStart		_rls_ = new WeaponEventAnimReloadingLoopStart;

		m_fsm = new WeaponFSM(this); // @NOTE: set owner of the submachine fsm
		m_fsm.AddTransition(new WeaponTransition(m_start  , __be_, m_eject, NULL, new WeaponGuardChamberFiredOut(m_weapon)));
		m_fsm.AddTransition(new WeaponTransition(m_start  , __bs_, m_chamber));
		m_fsm.AddTransition(new WeaponTransition(m_eject  , __bs_, m_chamber));
		
		/*if ( !loop )
		{
			// Automaticaly end after first bullet
			m_fsm.AddTransition(new WeaponTransition(m_chamber, _rls_, m_endLoop));
			//m_fsm.AddTransition(new WeaponTransition(m_hideB, _rls_, m_endLoop));
		}
		else
		{*/
			m_fsm.AddTransition(new WeaponTransition(m_chamber, __bM_, m_w4sb2, NULL, new GuardAnd(new GuardAnd(new WeaponGuardHasAmmoInLoopedState(m_chamber), new WeaponGuardChamberHasRoomForMoreThanOne(m_weapon)),new WeaponGuardWeaponManagerWantContinue())));
			m_fsm.AddTransition(new WeaponTransition(m_chamber, __bM_, m_endLoop));
			m_fsm.AddTransition(new WeaponTransition(m_w4sb2, __bh_, m_hideB));
			m_fsm.AddTransition(new WeaponTransition(m_hideB, __bs_, m_chamber));
			
			
		
		
		//}
		
		
		

		
		m_fsm.AddTransition(new WeaponTransition(m_endLoop, _fin_, NULL));
		

		m_fsm.SetInitialState(m_start);
	}

	override void OnEntry (WeaponEventBase e)
	{
		if (e != NULL)
		{
			if (e.m_magazine != NULL)
			{
				Print("[wpnfsm] LoopedChambering, taking mag from event e.mag=" + e.m_magazine.ToString());
				m_srcMagazine = e.m_magazine;
			}
		}
		else
		{
			Print("[wpnfsm] LoopedChambering (e=NULL), m_srcMagazine=" + m_srcMagazine.ToString());
		}
		Print("[wpnfsm] m_srcMagazine=" + m_srcMagazine.ToString());

		m_chamber.m_srcMagazine = m_srcMagazine;
		


		super.OnEntry(e); // @NOTE: super at the end (prevent override from submachine start)
	}
	override void OnExit (WeaponEventBase e)
	{
		super.OnExit(e);
	}
	override void OnAbort (WeaponEventBase e)
	{
		super.OnAbort(e);
	}
};
/*
class LoopedChamberingLoadSingleBullet extends WeaponStateBase
{
	WeaponActions m_action;
	int m_startActionType;
	int m_endActionType;
	Magazine m_srcMagazine; /// source of the cartridge

	ref WeaponChambering_Start m_start;
	ref WeaponEjectCasing m_eject;
	ref WeaponChambering_CartridgeEnd m_chamber;
	ref LoopedChambering_Wait4ShowBullet2 m_w4sb2;
	ref LoopedChambering_EndLoop m_endLoop;
	ref BulletHide_W4T m_hideB;

	void LoopedChamberingLoadSingleBullet (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int startActionType = -1, int endActionType = -1)
	{
		m_action = action;
		m_startActionType = startActionType;
		m_endActionType = endActionType;

		// setup nested state machine
		m_start = new WeaponChambering_Start(m_weapon, this, m_action, m_startActionType);
		m_eject = new WeaponEjectCasing(m_weapon, this);
		m_chamber = new WeaponChambering_CartridgeEnd(m_weapon, this);
		m_w4sb2 = LoopedChambering_Wait4ShowBullet2(m_weapon, this);
		m_hideB = new BulletHide_W4T(m_weapon, this);
		m_endLoop = new LoopedChambering_EndLoop(m_weapon, this, m_action, m_endActionType); // @NOTE: termination playing action - dummy?
		// events
		WeaponEventBase							_fin_ = new WeaponEventHumanCommandActionFinished;
		WeaponEventContinuousLoadBulletStart	__lS_ = new WeaponEventContinuousLoadBulletStart;
		WeaponEventContinuousLoadBulletEnd		__lE_ = new WeaponEventContinuousLoadBulletEnd;
		WeaponEventAnimBulletShow				__bs_ = new WeaponEventAnimBulletShow;
		WeaponEventAnimBulletHide				__bh_ = new WeaponEventAnimBulletHide;
		WeaponEventAnimBulletEject				__be_ = new WeaponEventAnimBulletEject;
		WeaponEventAnimBulletInMagazine			__bM_ = new WeaponEventAnimBulletInMagazine;
		WeaponEventAnimBulletShow2				_bs2_ = new WeaponEventAnimBulletShow2;
		WeaponEventAnimReloadingLoopStart		_rls_ = new WeaponEventAnimReloadingLoopStart;

		m_fsm = new WeaponFSM(this); // @NOTE: set owner of the submachine fsm
		m_fsm.AddTransition(new WeaponTransition(m_start  , __be_, m_eject, NULL, new WeaponGuardChamberFiredOut(m_weapon)));
		m_fsm.AddTransition(new WeaponTransition(m_start  , __bs_, m_chamber));
		m_fsm.AddTransition(new WeaponTransition(m_eject  , __bs_, m_chamber));
		
		//if ( !loop )
		//{
			// Automaticaly end after first bullet
			//m_fsm.AddTransition(new WeaponTransition(m_chamber, _rls_, m_endLoop));
			//m_fsm.AddTransition(new WeaponTransition(m_hideB, _rls_, m_endLoop));
		//}
		//else
		/*{
			m_fsm.AddTransition(new WeaponTransition(m_chamber, __bM_, m_w4sb2, NULL, new GuardAnd(new WeaponGuardHasAmmoInLoopedState(m_chamber), new WeaponGuardChamberHasRoomForMoreThanOne(m_weapon))));
			m_fsm.AddTransition(new WeaponTransition(m_chamber, __bM_, m_endLoop));
			m_fsm.AddTransition(new WeaponTransition(m_w4sb2, __bh_, m_hideB));
			m_fsm.AddTransition(new WeaponTransition(m_hideB, __bs_, m_chamber));
			
			
		
		
		}*//*
		m_fsm.AddTransition(new WeaponTransition(m_chamber, __bM_, m_w4sb2));
		m_fsm.AddTransition(new WeaponTransition(m_w4sb2, __bh_, m_hideB));
		
		m_fsm.AddTransition(new WeaponTransition(m_hideB  , _fin_, NULL));
		

		
		//m_fsm.AddTransition(new WeaponTransition(m_endLoop, _fin_, NULL));
		

		m_fsm.SetInitialState(m_start);
	}

	override void OnEntry (WeaponEventBase e)
	{
		if (e != NULL)
		{
			if (e.m_magazine != NULL)
			{
				Print("[wpnfsm] LoopedChambering, taking mag from event e.mag=" + e.m_magazine.ToString());
				m_srcMagazine = e.m_magazine;
			}
		}
		else
		{
			Print("[wpnfsm] LoopedChambering (e=NULL), m_srcMagazine=" + m_srcMagazine.ToString());
		}
		Print("[wpnfsm] m_srcMagazine=" + m_srcMagazine.ToString());

		m_chamber.m_srcMagazine = m_srcMagazine;
		


		super.OnEntry(e); // @NOTE: super at the end (prevent override from submachine start)
	}
	override void OnExit (WeaponEventBase e)
	{
		super.OnExit(e);
	}
	override void OnAbort (WeaponEventBase e)
	{
		super.OnAbort(e);
	}
};
*/

