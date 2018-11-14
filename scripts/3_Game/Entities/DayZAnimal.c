class DayZCreature extends EntityAI 
{
	#ifdef _DAYZ_CREATURE_DEBUG_SHADOW
	proto native void DebugSetShadow(DayZCreature creature);
	#endif
	
	proto native bool RegisterAnimationEvent(string event_name, string function_name);
	proto native void UpdateSimulationPrecision(int simLOD);
	
	override bool IsDayZCreature()
	{
		return true;
	}
	
	override bool CanBeSkinned()
	{
		return true;
	}
}

class DayZCreatureAI extends DayZCreature 
{
	proto native AIAgent GetAIAgent();

	proto native bool IsSoundInsideBuilding();

	proto native void DebugDisableAIControl();
	proto native void DebugRestoreAIControl();
	
	proto native void AddDamageSphere(string bone_name, string ammo_name, float radius, float duration);
	
	proto native DayZCreatureAIType GetCreatureAIType();
	
	void DayZCreatureAI()
	{
		RegisterAnimEvents();
	}
	
	void AddDamageSphere(AnimDamageParams damage_params)
	{ 
		AddDamageSphere(damage_params.m_sBoneName, damage_params.m_sAmmoName, damage_params.m_fRadius, damage_params.m_fDuration); 
	}
	
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		CreateComponent(COMP_TYPE_BODY_STAGING); // TO DO: This is never called on clients in multiplayer! That's why skinning doesn't work properly in MP. DAYZ-28269
	}
	
	AnimBootsType GetBootsType()
	{
		return AnimBootsType.None;
	}
	
	AbstractWave PlaySound(SoundObject so, SoundObjectBuilder sob)
	{
		if(so == NULL)
		{
			return NULL;
		}
		
		so.SetPosition(GetPosition());
		AbstractWave wave = GetGame().GetSoundScene().Play3D(so, sob);
		return wave;
	}
	
	void OnSoundEvent(int event_id, string event_user_string) 
	{
		AnimSoundEvent sound_event = GetCreatureAIType().GetSoundEvent(event_id);
		if(sound_event != NULL)
		{
			ProcessSoundEvent(sound_event);
		}
	}

	void OnSoundVoiceEvent(int event_id, string event_user_string)
	{
		AnimSoundVoiceEvent voice_event = GetCreatureAIType().GetSoundVoiceEvent(event_id);
		if(voice_event != NULL)
		{
			ProcessSoundVoiceEvent(voice_event);
		}
	}
	
	void OnStepEvent(int event_id, string event_user_string) 
	{
		AnimStepEvent step_event = GetCreatureAIType().GetStepEvent(event_id);
		if(step_event != NULL)
		{
			ProcessStepEvent(step_event);
		}
	}
	
	void OnDamageEvent(int event_id, string event_user_string)
	{
		AnimDamageEvent damage_event = GetCreatureAIType().GetDamageEvent(event_id);
		if(damage_event != NULL)
		{
			ProcessDamageEvent(damage_event);
		}
	}
	
	protected void RegisterAnimEvents()
	{
		if(!RegisterAnimationEvent("Sound", "OnSoundEvent"))
		{
			Print("Error registering anim. event (Sound)");
		}

		if(!RegisterAnimationEvent("SoundVoice", "OnSoundVoiceEvent"))
		{
			Print("Error registering anim. event (SoundVoice)");
		}
		
		if(!RegisterAnimationEvent("Step", "OnStepEvent"))
		{
			Print("Error registering anim. event (Step)");
		}
		
		if(!RegisterAnimationEvent("Damage", "OnDamageEvent"))
		{
			Print("Error registering anim. event (Damage)");
		}
	}
	
	private void ProcessSoundEvent(AnimSoundEvent sound_event)
	{
		if(GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			SoundObjectBuilder objectBuilder = sound_event.GetSoundBuilder();
			if(NULL != objectBuilder)
			{
				objectBuilder.UpdateEnvSoundControllers(GetPosition());
				SoundObject soundObject = objectBuilder.BuildSoundObject();
				PlaySound(soundObject, objectBuilder);
			}
		}
		
		if(GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			if(sound_event.m_NoiseParams != NULL)
				GetGame().GetNoiseSystem().AddNoise(this, sound_event.m_NoiseParams);
		}
	}

	private void ProcessSoundVoiceEvent(AnimSoundVoiceEvent sound_event)
	{
		if(GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			SoundObjectBuilder objectBuilder = sound_event.GetSoundBuilder();
			if(NULL != objectBuilder)
			{
				objectBuilder.UpdateEnvSoundControllers(GetPosition());
				SoundObject soundObject = objectBuilder.BuildSoundObject();
				AttenuateSoundIfNecessary(soundObject);
				PlaySound(soundObject, objectBuilder);
			}
		}
		
		if(GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			if(sound_event.m_NoiseParams != NULL)
				GetGame().GetNoiseSystem().AddNoise(this, sound_event.m_NoiseParams);
		}
	}

	private void ProcessStepEvent(AnimStepEvent step_event)
	{
		if(GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			SoundObjectBuilder soundBuilder = step_event.GetSoundBuilder(GetSurfaceType().Hash());
			if(soundBuilder == NULL)
				return;
			
			soundBuilder.UpdateEnvSoundControllers(GetPosition());
			SoundObject soundObject = soundBuilder.BuildSoundObject();
			AttenuateSoundIfNecessary(soundObject);
			PlaySound(soundObject, soundBuilder);
		}
		
		if(GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			if(step_event.m_NoiseParams != NULL)
			{
				float noiseMultiplier = GetSurfaceNoise();
				GetGame().GetNoiseSystem().AddNoise(this, step_event.m_NoiseParams, noiseMultiplier);
			}
		}
		//TODO effects
	}
	
	private void ProcessDamageEvent(AnimDamageEvent damage_event)
	{
		AddDamageSphere(damage_event.m_DamageParams);
	}
	
	protected void AttenuateSoundIfNecessary(SoundObject soundObject)
	{
		if (GetGame().GetPlayer() != NULL && (IsSoundInsideBuilding() != GetGame().GetPlayer().IsSoundInsideBuilding() || GetGame().GetPlayer().IsCameraInsideVehicle()))
		{
			soundObject.SetKind(WaveKind.WAVEATTALWAYS);
		}
		else
		{
			soundObject.SetKind(WaveKind.WAVEEFFECTEX);
		}
	}
}

enum DayZAnimalConstants
{
	COMMANDID_MOVE,
	COMMANDID_JUMP,
	COMMANDID_DEATH,
	COMMANDID_HIT,
	COMMANDID_ATTACK,
};

class DayZAnimalInputController
{
	proto native bool IsDead();
	proto native bool IsJump();
	proto native bool IsAttack();
	
	//--------------------------------------------------------------

	//! never created by script
	private void DayZAnimalInputController()
	{
	}

	//! never destroyed by script
	private void ~DayZAnimalInputController()
	{
	}	
};

//in configs all the animals inherits from AnimalBase class
typedef DayZAnimal AnimalBase;

class DayZAnimal extends DayZCreatureAI
{	
	proto native DayZAnimalInputController GetInputController();

	proto native void StartCommand_Death(int pType, int pDirection);
	proto native void StartCommand_Move();
	proto native void StartCommand_Jump();
	proto native void StartCommand_Attack();
	proto native void StartCommand_Hit(int pType, int pDirection);
	
	proto native void SignalAIAttackStarted();
	proto native void SignalAIAttackEnded();
	
	void DayZAnimal()
	{
		// testing: animals have no inventory by default
		//GetInventory().LockInventory(LOCK_FROM_SCRIPT); // Hides animals from vicinity in inventory. Remove this if wanted otherwise.
	}
	
	override bool IsHealthVisible()
	{
		return false;
	}	
	
	void CommandHandler(float dt, int currentCommandID, bool currentCommandFinished)
	{
		DayZAnimalInputController inputController = GetInputController();
		
		if (HandleDeath(currentCommandID, inputController))
		{
			return;
		}
		
		if (currentCommandFinished)
		{
			if (currentCommandID == DayZAnimalConstants.COMMANDID_ATTACK)
			{
				SignalAIAttackEnded();
			}
	
			StartCommand_Move();
	
			return;
		}
	
		if (HandleDamageHit(currentCommandID))
		{
			return;
		}
	
		if (currentCommandID == DayZAnimalConstants.COMMANDID_MOVE)
		{
			if (inputController.IsJump())
			{
				StartCommand_Jump();
				return;
			}
	
			if (inputController.IsAttack())
			{
				StartCommand_Attack();
				SignalAIAttackStarted();
				return;
			}
		}
	}
	
	bool m_DamageHitToProcess = false;
	int m_DamageHitType = 0;
	int m_DamageHitDirection = 0;
	
	bool HandleDeath(int currentCommandID, DayZAnimalInputController inputController)
	{
		if (inputController.IsDead())
		{
			if (currentCommandID == DayZAnimalConstants.COMMANDID_DEATH)
			{
				return true;
			}
	
			if (m_DamageHitToProcess)
			{
				m_DamageHitToProcess = false;
				
				StartCommand_Death(m_DamageHitType, m_DamageHitDirection);
			}
			else
			{
				StartCommand_Death(0, 0);
			}
	
			return true;
		}
		
		return false;
	}
	
	bool HandleDamageHit(int currentCommandID)
	{
		if (m_DamageHitToProcess)
		{
			m_DamageHitToProcess = false;
			
			if (currentCommandID != DayZAnimalConstants.COMMANDID_HIT)
			{
				StartCommand_Hit(m_DamageHitType, m_DamageHitDirection);
			}
			return true;
		}
		return false;
	}
	
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos);
		
		ComponentAnimalBleeding animal_bleeding = ComponentAnimalBleeding.Cast( GetComponent( COMP_TYPE_ANIMAL_BLEEDING ) );
		animal_bleeding.CreateWound( dmgZone, ammo );
		
		int type = 0;
		int direction = 0;
		if (ComputeDamageHitParams(source, dmgZone, type, direction) == true)
		{
			QueueDamageHit(type, direction);
		}
		
	}
	
	void QueueDamageHit(int type, int direction)
	{
		m_DamageHitToProcess = true;
		m_DamageHitType = type;
		m_DamageHitDirection = direction;
	}
	
	bool ComputeDamageHitParams(EntityAI source, string dmgZone, out int type, out int direction)
	{
		type = 0; // not used right now
		
		float angleDeg = ComputeHitDirectionAngleDeg(source);
		direction = TranslateHitAngleDegToDirectionIndex(angleDeg);
		
		direction += FindComponentDirectionOffset(dmgZone);
		
		return true;	
	}
	
	float ComputeHitDirectionAngleDeg(EntityAI source)
	{
		vector targetDirection = GetDirection();
		vector toSourceDirection = (source.GetPosition() - GetPosition());

		targetDirection[1] = 0;
		toSourceDirection[1] = 0;

		targetDirection.Normalize();
		toSourceDirection.Normalize();

		float cosFi = vector.Dot(targetDirection, toSourceDirection);
		vector cross = targetDirection * toSourceDirection;

		float dirAngleDeg = Math.Acos(cosFi) * Math.RAD2DEG;
		if( cross[1] < 0 )
			dirAngleDeg = -dirAngleDeg;
		
		return dirAngleDeg;
	}
	
	int TranslateHitAngleDegToDirectionIndex(float angleDeg)
	{
		if (angleDeg >= -20 && angleDeg <= 20) // front
		{
			return 1; 
		}
		else if (angleDeg < 0) // left
		{
			return 2;
		}
		
		return 3; // right
	}
	
	int FindComponentDirectionOffset(string component)
	{
		const int directionCount = 4;
		
		int offset = 0;
		if (component.Length() == 0)
		{
			offset = 0;
		}
		else if (component == "Zone_Head")
		{
			offset = directionCount;
		}
		else if (component == "Zone_Chest" || component == "Zone_Legs_Front" || component == "Zone_Spine_Front" || component == "Zone_Neck")
		{
			offset = 2 * directionCount;
		}
		else
		{
			offset = 3 * directionCount;
		}
		
		return offset;
	}
}

	

class Animal_CervusElaphus_2 extends DayZAnimal
{
}