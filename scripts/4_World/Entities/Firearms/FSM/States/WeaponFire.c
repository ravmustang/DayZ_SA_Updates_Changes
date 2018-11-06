// fire
class WeaponDryFire extends WeaponStartAction
{
	float m_dtAccumulator;

	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);
		m_dtAccumulator = 0;
	}

	override bool IsWaitingForActionFinish () { return true; }

	override void OnUpdate (float dt)
	{
		m_dtAccumulator += dt;
		DayZPlayer p;
		Class.CastTo(p, m_weapon.GetHierarchyParent());

		HumanInputController hic = p.GetInputController();

		int muzzleIndex = m_weapon.GetCurrentMuzzle();
		float reloadTime = m_weapon.GetReloadTime(muzzleIndex);
		if ( hic.IsUseButton() && m_dtAccumulator >= reloadTime)
			if (m_weapon.CanProcessWeaponEvents())
				m_weapon.ProcessWeaponEvent(new WeaponEventDryFireTimeout(p));
	}

	override void OnExit (WeaponEventBase e)
	{
		m_dtAccumulator = 0;
		super.OnExit(e);
	}
	
};

// fire
class WeaponFire extends WeaponStartAction
{
	float m_dtAccumulator;
	int m_normalFireType;
	int m_jamFireType;

	void WeaponFire (Weapon_Base w = NULL, WeaponStateBase parent = NULL, WeaponActions action = WeaponActions.NONE, int actionType = -1, int jamFireType = -1)
	{
		m_action = action;
		m_actionType = actionType;
		m_normalFireType = actionType;
		m_jamFireType = jamFireType;
	}

	override bool IsWaitingForActionFinish () { return true; }

	override void OnEntry (WeaponEventBase e)
	{
		m_dtAccumulator = 0;

		wpnPrint("[wpnfsm] WeaponFire bang!");
		//m_weapon.Fire();
		int mi = m_weapon.GetCurrentMuzzle();
		if (TryFireWeapon(m_weapon, mi))
		{
			if (m_weapon.IsJammed())
			{
				wpnPrint("[wpnfsm] WeaponFire --jammed--");
				m_actionType = m_jamFireType;
			}
			else
			{
				wpnPrint("[wpnfsm] WeaponFire --ok--");
				m_actionType = m_normalFireType;
			}
			DayZPlayerImplement pl;
			if (Class.CastTo(pl, e.m_player))
				pl.GetAimingModel().SetRecoil(m_weapon);
		}
		super.OnEntry(e);
	}

	override void OnUpdate (float dt)
	{
		m_dtAccumulator += dt;

		DayZPlayer p;
		Class.CastTo(p, m_weapon.GetHierarchyParent());

		int muzzleIndex = m_weapon.GetCurrentMuzzle();
		float reloadTime = m_weapon.GetReloadTime(muzzleIndex);
		if (m_dtAccumulator >= reloadTime)
			if (m_weapon.CanProcessWeaponEvents())
				m_weapon.ProcessWeaponEvent(new WeaponEventReloadTimeout(p));
	}

	override void OnExit (WeaponEventBase e)
	{
		m_dtAccumulator = 0;
		super.OnExit(e);
	}
};


class WeaponFireAndChamber extends WeaponFire
{
	override void OnEntry (WeaponEventBase e)
	{
		super.OnEntry(e);
		
		if (!m_weapon.IsJammed())
		{
			wpnDebugPrint("[wpnfsm] ejected fired out casing");
			int mi = m_weapon.GetCurrentMuzzle();
			m_weapon.EjectCasing(mi);
			m_weapon.SelectionBulletHide();

			chamberFromAttachedMagazine(m_weapon, mi);
		}
	}
};

