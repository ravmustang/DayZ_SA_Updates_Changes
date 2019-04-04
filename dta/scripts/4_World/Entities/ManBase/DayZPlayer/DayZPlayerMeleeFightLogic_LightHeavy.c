//! Light / Heavy punches

enum EFightLogicCooldownCategory
{
	EVADE = 0
}

class DayZPlayerMeleeFightLogic_LightHeavy
{
	//! cooldown timers settings
	const float EVADE_COOLDOWN			= 0.5;
	
	//! dummy ammo types 
	const string DUMMY_LIGHT_AMMO		= "Dummy_Light";
	const string DUMMY_HEAVY_AMMO		= "Dummy_Heavy";


	protected DayZPlayerImplement					m_DZPlayer;
	protected ref DayZPlayerImplementMeleeCombat	m_MeleeCombat;
	protected EMeleeHitType							m_HitType;
	protected ref map <int, ref Timer>				m_CooldownTimers;

	protected bool									m_IsInBlock;
	protected bool									m_IsEvading;

	void DayZPlayerMeleeFightLogic_LightHeavy(DayZPlayerImplement player)
	{
		Init(player);
	}
	
	void Init(DayZPlayerImplement player)
	{
		m_DZPlayer 			= player;
		m_MeleeCombat		= m_DZPlayer.GetMeleeCombat();

		m_IsInBlock 		= false;
		m_IsEvading	 		= false;
		m_HitType			= EMeleeHitType.NONE;
		
		RegisterCooldowns();
	}

	void ~DayZPlayerMeleeFightLogic_LightHeavy() {}

	bool IsInBlock()
	{
		return m_IsInBlock;
	}
	
	bool IsEvading()
	{
		return m_IsEvading;
	}

	protected void RegisterCooldowns()
	{
		m_CooldownTimers	= new map<int, ref Timer>;
		m_CooldownTimers.Insert(EFightLogicCooldownCategory.EVADE, new Timer(CALL_CATEGORY_SYSTEM)); // evades
	}

	protected EMeleeHitType GetAttackTypeFromInputs(HumanInputController pInputs)
	{
		if (pInputs.IsMeleeFastAttackModifier() && m_DZPlayer.CanConsumeStamina(EStaminaConsumers.MELEE_HEAVY))
		{
			return EMeleeHitType.HEAVY;
		}
		else
		{
			return EMeleeHitType.LIGHT;
		}
	}
	
	protected EMeleeHitType GetAttackTypeByWeaponAttachments(EntityAI pEntity)
	{
		//! use stabbing if the bayonet/knife is attached to firearm
		if (pEntity.GetAttachmentByType(Mosin_Bayonet))
		{
			return EMeleeHitType.WPN_STAB;
		}
		else
		{
			return EMeleeHitType.WPN_HIT;
		}
	}

	bool HandleFightLogic(int pCurrentCommandID, HumanInputController pInputs, EntityAI pEntityInHands, HumanMovementState pMovementState)
	{
		InventoryItem itemInHands = InventoryItem.Cast(pEntityInHands);

		bool isFireWeapon = itemInHands && itemInHands.IsInherited(Weapon);
		bool isNotMeleeWeapon = itemInHands && !itemInHands.IsMeleeWeapon(); // TODO: allowed for everything that is not disabled in config (primarily for anim testing)

		if (pInputs.IsUseButtonDown() || (pInputs.IsImmediateAction() && isFireWeapon))
		{
			EntityAI target;
			
			//! do not perform attacks when blocking
			if (m_IsInBlock || m_IsEvading)
			{
				return true;
			}

			//! if the item in hands cannot be used as melee weapon
			if (isNotMeleeWeapon && !isFireWeapon)
			{
				return false;
			}

			if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE)
			{
				//! melee with firearm
				if (isFireWeapon)
				{
					if (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT)
					{
						m_HitType = GetAttackTypeByWeaponAttachments(pEntityInHands);
						m_MeleeCombat.Update(itemInHands, m_HitType);
						target = m_MeleeCombat.GetTargetObject();
						m_DZPlayer.StartCommand_Melee2(target, m_HitType == EMeleeHitType.WPN_HIT);
						if(m_HitType == EMeleeHitType.WPN_HIT)
							m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_HEAVY);
						m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_LIGHT);
						return true;
					}				
				}
				else
				{
					//! first attack in raised erc (light or heavy if modifier is used)
					if (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT)
					{
						m_HitType = GetAttackTypeFromInputs(pInputs);
						m_MeleeCombat.Update(itemInHands, m_HitType);
						target = m_MeleeCombat.GetTargetObject();
						m_DZPlayer.StartCommand_Melee2(target, m_HitType == EMeleeHitType.HEAVY);
						if(m_HitType == EMeleeHitType.HEAVY)
							m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_HEAVY);
						m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_LIGHT);
						return true;
					}
					//! kick from raised pne
					else if (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
					{
						m_HitType = EMeleeHitType.KICK;
						m_MeleeCombat.Update(itemInHands, m_HitType);
						target = m_MeleeCombat.GetTargetObject();
						m_DZPlayer.StartCommand_Melee2(target, false);
						m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_HEAVY);
						return true;
					}
					//! sprint attack in erc stance
					else if (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT && m_DZPlayer.IsSprintFull())
					{
						m_HitType = EMeleeHitType.SPRINT;
						m_MeleeCombat.Update(itemInHands, m_HitType);
						target = m_MeleeCombat.GetTargetObject();
						m_DZPlayer.StartCommand_Melee2(target, false);
						m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_HEAVY);
						return true;
					}
				}
			}
			//! combo hits - when we are already in Melee command and clicking UseButton
			else if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MELEE2)
			{
				HumanCommandMelee2 hmc2 = m_DZPlayer.GetCommand_Melee2();

				if (hmc2 && hmc2.IsInComboRange())
				{
					//! select if the next attack will light or heavy (based on key modifier)
					m_HitType = GetAttackTypeFromInputs(pInputs);					
					if (m_HitType == EMeleeHitType.HEAVY)
					{
						hmc2.ContinueCombo(true);
						m_MeleeCombat.Update(itemInHands, m_HitType);
    					m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_HEAVY);
					}
					else
					{
						hmc2.ContinueCombo(false);
						m_MeleeCombat.Update(itemInHands, m_HitType);
    					m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_LIGHT);
					}

					return true;
				}
				else
				{
					return true;
				}
			}
		}
		else if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MELEE2)
		{
			HumanCommandMelee2 hmc2a = m_DZPlayer.GetCommand_Melee2();
			if (hmc2a)
			{
				//! on anim event Hit received
				if (hmc2a.WasHit())
				{
					//! re-target (enemy can be already away)
					m_MeleeCombat.Update(itemInHands, m_HitType, true);
					//! evaluate hit - selection of cfg 'ammo' type
					EvaluateHit(itemInHands);

					//! reset - prepared for next hit
					m_MeleeCombat.SetTargetObject(null);
					m_MeleeCombat.SetHitZoneIdx(-1);
				}
			}
		}
		else if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE)
		{			
			HumanCommandMove cm = m_DZPlayer.GetCommand_Move();


			int 	roll = pInputs.IsMeleeLREvade();

			//! evades in raised erc stance while moving
			if (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT && roll != 0)
			{
				//! not enough stamina to do evades
				if (!m_DZPlayer.CanConsumeStamina(EStaminaConsumers.MELEE_EVADE)) return false;

				float angle;
				if (roll == 1)
				{
					angle = -90;	// left
				}
				else
				{
					angle = 90;		// right
				}

				// start melee evade
				m_IsEvading = true;
				SetCooldown(EVADE_COOLDOWN, EFightLogicCooldownCategory.EVADE);
				cm.StartMeleeEvadeA(angle);
				m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_EVADE);
			}

			//! stand up when crouching and raised pressed
			if (pInputs.IsWeaponRaised() && pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH)
			{
				cm.ForceStance(DayZPlayerConstants.STANCEIDX_RAISEDERECT);
			}

			//! blocks in raised erc/pro stance
			if (pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT || pMovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
			{
				float angle2;

				if (cm.GetCurrentInputAngle(angle2) && (angle2 < -130.0 || angle2 > 130))
				{
					cm.SetMeleeBlock(true);
					m_IsInBlock = true;
				}
				else
				{
					cm.SetMeleeBlock(false);
					m_IsInBlock = false;
				}
			}
			else
			{
				cm.SetMeleeBlock(false);
				m_IsInBlock = false;
			}
		}

		return false;
	}

	protected void EvaluateHit(InventoryItem weapon)
	{
		Object target = m_MeleeCombat.GetTargetObject();

		if( target )
		{			
			if( target.IsInherited(DayZPlayer) )
			{
				EvaluateHit_Player(weapon, target);
			}
			else if( target.IsInherited(DayZInfected) )
			{
				EvaluateHit_Infected(weapon, target);
			}
			else
			{
				EvaluateHit_Common(weapon, target);
			}
		}
	}

	protected void EvaluateHit_Player(InventoryItem weapon, Object target)
	{
		int hitZoneIdx = m_MeleeCombat.GetHitZoneIdx();
		int weaponMode = m_MeleeCombat.GetWeaponMode();
		vector hitPosWS;
		bool forcedDummy = false;

		PlayerBase targetPlayer = PlayerBase.Cast(target);

		//! Melee Hit/Impact modifiers
		if( targetPlayer )
		{
			//! if the oponnent is in Melee Block shift the damage down
			if( targetPlayer.GetMeleeFightLogic() && targetPlayer.GetMeleeFightLogic().IsInBlock())
			{
				if( weaponMode > 0 )
				{
					weaponMode--; // Heavy -> Light shift
				}
				else
				{
					forcedDummy = true; // dummy hits, cannot shift lower than 0
				}
			}
		}

		EvaluateHit_Common(weapon, target, forcedDummy);
	}
	
	protected void EvaluateHit_Infected(InventoryItem weapon, Object target)
	{
		int hitZoneIdx = m_MeleeCombat.GetHitZoneIdx();
		int weaponMode = m_MeleeCombat.GetWeaponMode();
		vector hitPosWS;

		DayZInfected targetInfected = DayZInfected.Cast(target);

		//! check and evaluate stealth kill
		if( targetInfected && weapon )
		{
			if(m_HitType == EMeleeHitType.HEAVY && weapon.IsMeleeFinisher() )
			{
				//! check if attacker is in right pos and angle against victim
				if( IsBehindEntity(45, m_DZPlayer, target) )
				{
					EntityAI targetEntity = EntityAI.Cast(target);

					//! translate idx to name
					string compName = targetEntity.GetDamageZoneNameByComponentIndex(hitZoneIdx);
	
					//check if name is in suitable components list
					if( targetEntity.GetSuitableFinisherHitComponents().Find(compName) > -1 )
					{
						// execute attack (dmg part)
						hitPosWS = targetEntity.ModelToWorld(targetEntity.GetDefaultHitPosition());
						DamageSystem.CloseCombatDamage(EntityAI.Cast(m_DZPlayer), target, hitZoneIdx, "FinisherHit", hitPosWS);
						return; //! early exit
					}
				}
			}
		}
		
		EvaluateHit_Common(weapon, target);
	}
	
	protected void EvaluateHit_Common(InventoryItem weapon, Object target, bool forcedDummy=false)
	{
		int hitZoneIdx = m_MeleeCombat.GetHitZoneIdx();
		int weaponMode = m_MeleeCombat.GetWeaponMode();
		vector hitPosWS;
		string ammo;

		EntityAI targetEntity = EntityAI.Cast(target);

		//! check if we need to use dummy hit
		if(!DummyHitSelector(m_HitType, ammo) && !forcedDummy)
		{
			//! normal hit with applied damage to targeted component
			if (hitZoneIdx >= 0)
			{
				hitPosWS = targetEntity.ModelToWorld(targetEntity.GetDefaultHitPosition());
				m_DZPlayer.ProcessMeleeHit(weapon, weaponMode, target, hitZoneIdx, hitPosWS);
			}
		}
		else
		{
			//! play hit animation for dummy hits
			if( GetGame().IsServer() && targetEntity )
			{
				hitPosWS = targetEntity.ModelToWorld(targetEntity.GetDefaultHitPosition()); //! override hit pos by pos defined in type
				targetEntity.EEHitBy(null, 0, EntityAI.Cast(m_DZPlayer), hitZoneIdx, "", ammo, hitPosWS);
			}
		}
	}

	//! evaluation of hit player vs. player
	protected bool DummyHitSelector(EMeleeHitType hitType, out string ammoType)
	{
		switch(hitType)
		{
		//! in case of kick (on back or push from erc) change the ammo type to dummy
		case EMeleeHitType.WPN_HIT:
		case EMeleeHitType.KICK:
			ammoType = DUMMY_HEAVY_AMMO;
			return true;
		break;
		}
		
		ammoType = DUMMY_LIGHT_AMMO;
		
		return false;
	}
	
	protected bool IsBehindEntity(int angle, DayZPlayer source, Object target)
	{
		//! behind entity check			
		vector targetDirection = target.GetDirection();
		vector toSourceDirection = (source.GetPosition() - target.GetPosition());

		targetDirection[1] = 0;
		toSourceDirection[1] = 0;

		targetDirection.Normalize();
		toSourceDirection.Normalize();

		float cosFi = vector.Dot(targetDirection, toSourceDirection);
		vector cross = targetDirection * toSourceDirection;

		int hitDir = Math.Acos(cosFi) * Math.RAD2DEG;
		
		if( cross[1] < 0 )
			hitDir = -hitDir;
		
		if( hitDir <= (-180 + angle) || hitDir >= (180 - angle) )
		{
			//Print("HitDir: " + hitDir + " angle passed");
			return true;
		}
		
		return false;
	}

	protected void SetCooldown(float time, EFightLogicCooldownCategory cooldownCategory)
	{
		//! stops currently running cooldown timer (for specific category)
		if( m_CooldownTimers.Get(cooldownCategory).IsRunning() )
		{
			m_CooldownTimers.Get(cooldownCategory).Stop();
		}
		
		//! param for ResetCooldown
		Param1<int> param = new Param1<int>(cooldownCategory);
		m_CooldownTimers.Get(cooldownCategory).Run(time, this, "ResetCooldown", param);
	}
	
	protected void ResetCooldown(EFightLogicCooldownCategory cooldownCategory)
	{
		switch(cooldownCategory)
		{
		case EFightLogicCooldownCategory.EVADE:
			m_IsEvading = false;
		break;
		}
	}
}