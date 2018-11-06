enum EActivationType {

	TRIGGER_EVENT_OFF,
	TRIGGER_EVENT_ON_ACTIVATION,
	TRIGGER_EVENT_ON_CONNECT
};

const int DEFAULT_TICK_TIME_ACTIVE = 3;
const int DEFAULT_TICK_TIME_INACTIVE = 3;

class ModifiersManager
{
	PlayerBase m_Player;
	ref map<int, ref ModifierBase> m_ModifierList;
	ref array<ref Param> m_ParamList;
	bool m_AllowModifierTick = false;
	
	void ModifiersManager(PlayerBase player)
	{
		m_ModifierList 	= new map<int, ref ModifierBase>;
		m_ParamList 	= new array<ref Param>;
		m_Player 		= player;
		
		Init();
	}

	void Init()
	{
		#ifdef DEVELOPER
		AddModifier(new TestDiseaseMdfr);
		#endif
		AddModifier(new BloodRegenMdfr);
		//AddModifier(new BoneRegen);
		//AddModifier(new Health);
		AddModifier(new SalineMdfr);
		AddModifier(new HealthRegenMdfr);
		AddModifier(new HungerMdfr);
		//AddModifier(new Shock);
		AddModifier(new ImmuneSystemMdfr);
		AddModifier(new StomachMdfr);
		AddModifier(new HeatComfortMdfr);
		AddModifier(new ThirstMdfr);
		AddModifier(new BleedingCheckMdfr);
		//AddModifier(new Blinded);
		//AddModifier(new BrokenArms);
		//AddModifier(new BrokenLegs);
		AddModifier(new VomitStuffedMdfr);
		AddModifier(new BurningMdfr);
		AddModifier(new FeverMdfr);
		AddModifier(new HeartAttackMdfr);
		AddModifier(new HemolyticReactionMdfr);
		AddModifier(new PoisoningMdfr);
		AddModifier(new StuffedStomachMdfr);
		//AddModifier(new Tremor);
		AddModifier(new UnconsciousnessMdfr);
		AddModifier(new ShockDamageMdfr);
		AddModifier(new CommonColdMdfr);
		AddModifier(new CholeraMdfr);
		AddModifier(new InfluenzaMdfr);
		AddModifier(new SalmonellaMdfr);
		AddModifier(new BrainDiseaseMdfr);
		AddModifier(new WetMdfr);
	}

	void SetModifiers(bool enable)
	{
		
		m_AllowModifierTick = enable;
		if( enable == false )
		{
			for(int i = 0; i < m_ModifierList.Count(); i++)
			{
				m_ModifierList.GetElement(i).ResetLastTickTime();
			}
		}
	}

	bool IsModifiersEnable()
	{
		return m_AllowModifierTick;
	}
	
	void AddModifier(ModifierBase modifier)
	{
		modifier.InitBase(m_Player,this);
		int id = modifier.GetModifierID();
		
		if(id < 1)
		{
			Error("modifiers ID must be 1 or higher(for debugging reasons)");			
		}
		
		//TODO: add a check for duplicity
		m_ModifierList.Insert(id, modifier);
	}
	
	bool IsModifierActive(eModifiers modifier_id)
	{
		return m_ModifierList.Get(modifier_id).IsActive();
	}

	void OnScheduledTick(float delta_time)
	{
		if(!m_AllowModifierTick) return;

		for(int i = 0; i < m_ModifierList.Count(); i++)
		{
			m_ModifierList.GetElement(i).Tick(delta_time);
		}

	}
	
	void ActivateModifier(int modifier_id, bool triggerEvent = EActivationType.TRIGGER_EVENT_ON_ACTIVATION)
	{
		m_ModifierList.Get(modifier_id).ActivateRequest(triggerEvent);
	}

	void DeactivateModifier(int modifier_id, bool triggerEvent = true)
	{
		m_ModifierList.Get(modifier_id).Deactivate(triggerEvent);
	}
	
	void OnStoreSave( ParamsWriteContext ctx )
	{
		CachedObjectsArrays.ARRAY_INT.Clear();
		
		int modifier_count;
		for(int x = 0; x < m_ModifierList.Count(); x++)
		{
			ModifierBase mdfr = m_ModifierList.GetElement(x);
			if( mdfr.IsActive() && mdfr.IsPersistent() ) 
			{
				modifier_count++;
				//save the modifier id
				CachedObjectsArrays.ARRAY_INT.Insert( mdfr.GetModifierID() );
				if( mdfr.IsTrackAttachedTime() )
				{
					//save the overall attached time
					CachedObjectsArrays.ARRAY_INT.Insert( mdfr.GetAttachedTime() );
				}
			}
		}

		//write the count
		//CachedObjectsParams.PARAM1_INT.param1 = modifier_count;
		//PrintString("Saving modifiers count: "+ modifier_count);
		ctx.Write(modifier_count);

		//write the individual modifiers and respective attached times
		for(int i = 0; i < CachedObjectsArrays.ARRAY_INT.Count(); i++)
		{
			int item = CachedObjectsArrays.ARRAY_INT.Get(i);
			//PrintString( "saving item: "+item );
			ctx.Write(item);
		}

		for (int z = 0; z < m_ParamList.Count(); z++)
		{
			m_ParamList.Get(z).Serialize(ctx);
		}

	}
	
	void OnStoreLoad( ParamsReadContext ctx )
	{
		int modifier_count;
		ctx.Read(modifier_count);
		//PrintString("Loading modifiers count: "+ modifier_count);
		for(int i = 0; i < modifier_count; i++)
		{
			int modifier_id;
			ctx.Read(modifier_id);
			//PrintString( "loading item: "+modifier_id );
			//int modifier_id = CachedObjectsParams.PARAM1_INT.param1;
			ModifierBase modifier = GetModifier(modifier_id);
			if( modifier )
			{
				if( modifier.IsTrackAttachedTime() )
				{
					int time;
					ctx.Read(time);//get the attached time
					modifier.SetAttachedTime( time );
				}
				
				ActivateModifier(modifier_id, EActivationType.TRIGGER_EVENT_ON_CONNECT);
			}
			else
			{
				Debug.LogError("DB loading: non-existent modifier with id:"+modifier_id.ToString());
			}
		}
		
		for (int x = 0; x < m_ParamList.Count(); x++)
		{
			m_ParamList.Get(x).Deserializer(ctx);
		}
		
	}

	ModifierBase GetModifier(int modifier_id)
	{
		return m_ModifierList.Get(modifier_id);
	}
		
	PlayerBase GetPlayer()
	{
		return m_Player;
	}

	void SetModifierLock(int modifier_id, bool state)
	{
		m_ModifierList.Get(modifier_id).SetLock(state);
	}


	bool GetModifierLock(int modifier_id)
	{
		return m_ModifierList.Get(modifier_id).IsLocked();
	}

	void DbgGetModifiers(map<int, string> modifiers_map)
	{
		for(int i = 0;i < m_ModifierList.Count(); i++)
		{
			ModifierBase modifier = m_ModifierList.GetElement(i);
			int modifier_id = modifier.GetModifierID();
			string modifier_name = modifier.GetName();
			
			if( !modifier.IsActive() ) 
			{
				modifier_id = -modifier_id;
			}
			
			modifiers_map.Insert( modifier_id, modifier_name );
		}
	}
}
