class Torch : ItemBase
{
	private SoundOnVehicle	m_LoopSoundEntity;
	Particle 				m_FireParticle;
	
	void ~Torch()
	{
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
	
	bool CanIgnite()
	{
		if ( !GetCompEM().CheckWetness() )
			return false;
		
		ItemBase rag = Class.Cast( GetAttachmentByType(Rag) );
		
		if (rag)
		{
			float wetness = rag.GetWet();
			bool is_dry_enough = wetness <= 1-GetCompEM().GetWetnessExposure();
			return is_dry_enough;
		}
		
		if ( GetCompEM().CanWork() )
			return true;
		
		return false;
	}
	
	void Ignite()
	{
		GetCompEM().SwitchOn();
	}
	
	override void OnSwitchOn()
	{
		if (!GetCompEM().IsWorking())
		{
			ConsumeRag();
		}
	}
	
	bool ConsumeRag()
	{
		ItemBase rag = Class.Cast(GetAttachmentByType(Rag));
		
		if (rag)
		{
			rag.AddQuantity(-1);
			rag.SetHealth(0);
			GetCompEM().SetEnergy( GetCompEM().GetEnergyMax() );
			return true;
		}
		
		return false;
	}
	
	void RuinRags()
	{
		ItemBase rag = Class.Cast(GetAttachmentByType(Rag));
		
		if (rag)
		{
			rag.SetHealth(0);
		}
	}
	
	// Inventory manipulation
	override void OnInventoryExit(Man player)
	{
		super.OnInventoryExit(player);
		
		StandUp();
	}
	
	bool IsSurfaceSoftEnough(string surface)
	{
		bool is_soft_enough = GetGame().IsSurfaceSoftGround(surface);
		
		if (!is_soft_enough)
		{
			if ( surface == "cp_gravel" )
			{
				is_soft_enough = true;
			}
		}
		
		return is_soft_enough;
	}
	
	// Stands up the torch, if possible. Returns true on success.
	bool StandUp()
	{
		string surface_type;
		vector position = GetPosition();
		GetGame().SurfaceGetType ( position[0], position[2], surface_type );
		bool is_surface_soft = IsSurfaceSoftEnough(surface_type);
		
		if ( is_surface_soft  &&  GetCompEM().IsWorking() )
		{
			vector ori_rotate = "0 0 0";
			ori_rotate[0] = Math.RandomFloat(0, 360);
			ori_rotate[1] = Math.RandomFloat(10, 20);
			SetOrientation(ori_rotate);
			
			return true; // I am standing up
		}
		
		return false; // I am NOT standing up
	}
	
	override void OnWorkStart()
	{
		SetPilotLight(true);
		
		if ( !m_LoopSoundEntity && GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
		{
			m_LoopSoundEntity = PlaySoundLoop("torchLoop", 50);
		}
		
		if ( !m_FireParticle  &&  ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
		{
			m_FireParticle = Particle.Play(ParticleList.TORCH, this, Vector(0, 0.5, 0));
			Print(m_FireParticle);
		}		
	}
	
	override void OnWork ( float consumed_energy )
	{
		if (GetGame().IsServer())
		{
			if ( GetCompEM().GetEnergy() <= GetCompEM().GetEnergyUsage() )
			{
				ConsumeRag();
			}
			
			RuinRags();
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
	}
};