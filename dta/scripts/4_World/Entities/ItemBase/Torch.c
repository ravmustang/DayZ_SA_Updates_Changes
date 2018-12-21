class Torch : ItemBase
{
	private SoundOnVehicle	m_LoopSoundEntity;
	Particle 				m_FireParticle;
	bool					m_CanReceiveUpgrade; // Synchronized variable
	
	static float 			m_BurnTimePerRag;
	static float 			m_BurnTimePerFullLard;
	static float 			m_MaxConsumableLardQuantity;
	
	void Torch()
	{
		if ( m_BurnTimePerRag == 0  ||  m_BurnTimePerFullLard == 0  ||  m_MaxConsumableLardQuantity == 0 )
		{
			string cfg_path = "CfgVehicles " + GetType();
			m_BurnTimePerRag = GetGame().ConfigGetFloat( cfg_path + " burnTimePerRag" );
			m_BurnTimePerFullLard = GetGame().ConfigGetFloat( cfg_path + " burnTimePerFullLardDose" );
			m_MaxConsumableLardQuantity = GetGame().ConfigGetFloat( cfg_path + " maxConsumableLardDose" );
		}
		
		RegisterNetSyncVariableBool("m_CanReceiveUpgrade");
	}
	
	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		
		if ( m_LoopSoundEntity != NULL && GetGame() != NULL )
		{
			GetGame().ObjectDelete( m_LoopSoundEntity );
		}
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		return CanBeTakenAsCargo();
	}
	
	override bool CanRemoveFromCargo(EntityAI parent)
	{
		return CanBeTakenAsCargo();
	}
	
	bool CanBeTakenAsCargo()
	{
		// Don't let players burn their pockets!
		return !GetCompEM().IsWorking();
	}
	
	override bool IsIgnited()
	{
		return GetCompEM().IsWorking();
	}
	
	override bool CanIgniteItem(EntityAI ignite_target = NULL)
	{
		return GetCompEM().IsWorking();
	}
	
	override bool HasFlammableMaterial()
	{
		return true;
	}
	
	// Checkes if Torch can be ignited
	override bool CanBeIgnitedBy(EntityAI igniter = NULL)
	{
		if ( !GetCompEM().CheckWetness() )
			return false;
		
		ItemBase rag = ItemBase.Cast( GetAttachmentByType(Rag) );
		
		if (rag)
		{
			float wetness = rag.GetWet();
			bool is_dry_enough = wetness <= 1-GetCompEM().GetWetnessExposure();
			return is_dry_enough;
		}
		
		if ( !GetCompEM().CanWork() )
			return false;
		
		if ( GetCompEM().GetEnergy() > 3 )
			return true;
		
		return false;
	}
	
	void UpdateCheckForReceivingUpgrade()
	{
		if ( GetGame().IsServer() )
		{
			float energy_limit = GetCompEM().GetEnergyMax() - GetCompEM().GetEnergy();
			
			if ( m_BurnTimePerRag < energy_limit )
			{
				m_CanReceiveUpgrade = true;
			}
			else
			{
				m_CanReceiveUpgrade = false;
			}
			
			SetSynchDirty();
		}
	}
	
	override void OnIgnitedThis(EntityAI fire_source)
	{
		if ( !GetCompEM().HasEnoughStoredEnergy() )
		{
			ConsumeRag();
		}
		
		GetCompEM().SwitchOn();
	}
	
	override void OnSwitchOn()
	{
		if ( !GetCompEM().HasEnoughStoredEnergy() )
			GetCompEM().SwitchOff();
		;
	}
	
	bool ConsumeRag()
	{
		ItemBase rag = ItemBase.Cast(GetAttachmentByType(Rag));
		
		if (rag)
		{
			rag.AddQuantity(-1);
			rag.SetHealth(0);
			GetCompEM().AddEnergy( m_BurnTimePerRag );
			return true;
		}
		
		return false;
	}
	
	bool ConsumeLard(Lard lard)
	{
		if (lard)
		{
			float lard_quant = lard.GetQuantity();
			
			float available_lard_quant = lard_quant;
			
			if ( available_lard_quant > m_MaxConsumableLardQuantity )
				available_lard_quant = m_MaxConsumableLardQuantity;
			
			float available_lard_coef = available_lard_quant / m_MaxConsumableLardQuantity;
			
			float add_energy = m_BurnTimePerFullLard * available_lard_coef;
			float add_energy_coef = 1;
			
			float energy_limit = GetCompEM().GetEnergyMax() - GetCompEM().GetEnergy();
			
			if (add_energy > energy_limit )
			{
				add_energy_coef = energy_limit / add_energy;
				add_energy = energy_limit;
				available_lard_quant = available_lard_quant * add_energy_coef;
			}
			
			GetCompEM().AddEnergy( add_energy );
			lard.AddQuantity(-available_lard_quant);
			
			CalculateQuantity();
			
			UpdateCheckForReceivingUpgrade();
			
			return true;
		}
		
		return false;
	}
	
	void RuinRags()
	{
		ItemBase rag = ItemBase.Cast(GetAttachmentByType(Rag));
		
		if (rag)
		{
			rag.SetHealth(1);
		}
	}
	
	// Inventory manipulation
	override void OnInventoryExit(Man player)
	{
		super.OnInventoryExit(player);
		
		StandUp();
	}
	
	// Stands up the torch, if possible. Returns true on success.
	bool StandUp()
	{
		string surface_type;
		vector position = GetPosition();
		GetGame().SurfaceGetType ( position[0], position[2], surface_type );
		bool is_surface_soft = GetGame().IsSurfaceSoftGround(surface_type);
		
		if ( is_surface_soft  &&  !IsRuined() )
		{
			vector ori_rotate = "0 0 0";
			ori_rotate[0] = Math.RandomFloat(0, 360);
			ori_rotate[1] = Math.RandomFloat(0, 20);
			SetOrientation(ori_rotate);
			
			return true; // I am standing up
		}
		
		return false; // I am NOT standing up
	}
	
	// Calculates torch's quantity, which represents the amount of fuel left
	void CalculateQuantity()
	{
		if (GetGame().IsServer())
		{
			float q_max = GetCompEM().GetEnergyMax() + m_BurnTimePerRag * 6; // TO DO: Replace 6 by max rag quantity
			float q_min = GetCompEM().GetEnergy();
			
			ItemBase rag = ItemBase.Cast( GetAttachmentByType(Rag) );
			
			if (rag)
			{
				q_min += m_BurnTimePerRag * rag.GetQuantity();
			}
			
			int quant = Math.Round(  (q_min / q_max) * 100  );
			
			float wetness = GetWet();
			float wetness_exposure = GetCompEM().GetWetnessExposure();
			
			if (wetness > 0  &&  wetness_exposure > 0)
			{
				quant = quant * ( 1- (wetness / wetness_exposure ) );
			}
			
			SetQuantity(quant);
		}
	}
	
	bool CanReceiveUpgrade()
	{
		if ( GetGame().IsServer() )
		{
			float energy_limit = GetCompEM().GetEnergyMax() - GetCompEM().GetEnergy();
			
			if ( Torch.m_BurnTimePerRag < energy_limit )
			{
				return m_CanReceiveUpgrade;
			}
		}
		else
		{
			return m_CanReceiveUpgrade;
		}
		
		return false;
	}
	
	override void EEItemAttached ( EntityAI item, string slot_name ) 
	{
		super.EEItemAttached( item, slot_name );
		CalculateQuantity();
		UpdateCheckForReceivingUpgrade();
	}
	
	override void EEItemDetached ( EntityAI item, string slot_name ) 
	{
		super.EEItemDetached( item, slot_name );
		CalculateQuantity();
		UpdateCheckForReceivingUpgrade();
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		// Uncomment this when its fixed
		/*
		if ( GetGame().IsServer()  &&  GetCompEM().GetEnergy0To100() < 1 )
		{
			//MiscGameplayFunctions.TurnItemIntoItemEx(player, new TurnItemIntoItemLambda(this, "WoodenStick", player));
		}
		*/
	}
	
	override void OnWorkStart()
	{
		SetPilotLight(true);
	}
	
	override void OnWork ( float consumed_energy )
	{
		if ( GetGame().IsServer() )
		{
			if ( GetCompEM().GetEnergy() <= GetCompEM().GetEnergyUsage() )
			{
				ConsumeRag();
			}
			
			RuinRags();
			
			CalculateQuantity();
			
			UpdateCheckForReceivingUpgrade();
			
			AddWet(-0.0005);
		}
		
		if ( !m_LoopSoundEntity && GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
		{
			m_LoopSoundEntity = PlaySoundLoop("torchLoop", 50);
		}
		
		// Particle scaling by fuel
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			if ( GetQuantity() < 40 )
			{
				if ( !m_FireParticle  ||  m_FireParticle.GetParticleID() != ParticleList.TORCH_T1 )
				{
					// Executes once when fire particle starts or changes
					
					if (m_FireParticle)
						m_FireParticle.Stop();
					
					m_FireParticle = Particle.Play(ParticleList.TORCH_T1, this, Vector(0, 0.65, 0));
				}
			}
			else
			{
				if ( !m_FireParticle  ||  m_FireParticle.GetParticleID() != ParticleList.TORCH_T2 )
				{
					// Executes once when fire particle starts or changes
					
					if (m_FireParticle)
						m_FireParticle.Stop();
					
					m_FireParticle = Particle.Play(ParticleList.TORCH_T2, this, Vector(0, 0.65, 0));
				}
			}
		}
	}
	
	override void OnWorkStop()
	{
		SetPilotLight(false);
		
		if ( m_LoopSoundEntity && GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
		{
			GetGame().ObjectDelete( m_LoopSoundEntity );
			m_LoopSoundEntity = NULL;
		}
		
		if ( m_FireParticle)
		{
			m_FireParticle.Stop();
			m_FireParticle = NULL;
		}
		
		CalculateQuantity();
		UpdateCheckForReceivingUpgrade();
	}
	
	// COMBAT
	// This needs refactor!
	override int GetMeleeMode()
	{
		if ( GetCompEM().IsWorking() )
			return 3; // ???
		else
			return 0; // ???
	}

	override int GetMeleeHeavyMode()
	{
		if ( GetCompEM().IsWorking() )
			return 4; // ???
		else
			return 1; // ???
	}
	
	override int GetMeleeSprintMode()
	{
		if ( GetCompEM().IsWorking() )
			return 5; // ???
		else
			return 2; // ???
	}
};