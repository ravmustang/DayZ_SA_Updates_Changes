class Bottle_Base extends Edible_Base
{
	//Particles
	protected Particle 	m_ParticleCooking;
	protected int 		m_ParticlePlaying 	= ParticleList.INVALID;
	//Boiling
	//waiting for proper particle effects
	/*
	protected int PARTICLE_BOILING_EMPTY 	= ParticleList.COOKING_BOILING_EMPTY;
	protected int PARTICLE_BOILING_START 	= ParticleList.COOKING_BOILING_START;
	protected int PARTICLE_BOILING_DONE 	= ParticleList.COOKING_BOILING_DONE;
	//Baking
	protected int PARTICLE_BAKING_START 	= ParticleList.COOKING_BAKING_START;
	protected int PARTICLE_BAKING_DONE 		= ParticleList.COOKING_BAKING_DONE;
	//Drying
	protected int PARTICLE_DRYING_START 	= ParticleList.COOKING_DRYING_START;
	protected int PARTICLE_DRYING_DONE 		= ParticleList.COOKING_DRYING_DONE;
	//Burning
	protected int PARTICLE_BURNING_DONE		= ParticleList.COOKING_BURNING_DONE;
	*/
	protected int PARTICLE_BOILING_EMPTY 	= ParticleList.INVALID;
	protected int PARTICLE_BOILING_START 	= ParticleList.INVALID;
	protected int PARTICLE_BOILING_DONE 	= ParticleList.INVALID;
	//Baking
	protected int PARTICLE_BAKING_START 	= ParticleList.INVALID;
	protected int PARTICLE_BAKING_DONE 		= ParticleList.INVALID;
	//Drying
	protected int PARTICLE_DRYING_START 	= ParticleList.INVALID;
	protected int PARTICLE_DRYING_DONE 		= ParticleList.INVALID;
	//Burning
	protected int PARTICLE_BURNING_DONE		= ParticleList.INVALID;
	
	//Sounds
	protected SoundOnVehicle	m_SoundCooking;
	protected string			m_SoundPlaying = "";
	//Boiling
	const string SOUND_BOILING_EMPTY 		= "boilingWater";
	const string SOUND_BOILING_START 		= "boilingWater";
	const string SOUND_BOILING_DONE 		= "boilingWaterDone";
	//Baking
	const string SOUND_BAKING_START 		= "bake";
	const string SOUND_BAKING_DONE 			= "bakeDone";
	//Drying
	const string SOUND_DRYING_START 		= "dry";
	const string SOUND_DRYING_DONE 			= "dryDone";	
	//Burning
	const string SOUND_BURNING_DONE 		= "bakeDone";

	override void EEDelete( EntityAI parent )
	{
		super.EEDelete( parent );
		
		//remove audio visuals
		RemoveAudioVisuals();
	}	
		
	//================================================================
	// PARTICLES & SOUNDS
	//================================================================
	//Refreshes the audio and partcile effects on cooking pot
	//is_done - is the food baked, boiled, dried?
	//is_empty - is cooking quipment (cargo) empty?
	//is_burned - is any of the food items in the cargo in burned food stage?
	void RefreshAudioVisuals( CookingMethodType cooking_method, bool is_done, bool is_empty, bool is_burned )
	{
		string sound_name;
		int particle_id;
		
		//if at least one of the food items is burned
		if ( is_burned )		
		{
			sound_name 		= SOUND_BURNING_DONE;
			particle_id 	= PARTICLE_BURNING_DONE;
		}
		//proper cooking methods
		else
		{
			if ( cooking_method == CookingMethodType.BOILING )
			{
				if ( is_empty )											//EMPTY
				{
					sound_name 		= SOUND_BOILING_EMPTY;
					particle_id 	= PARTICLE_BOILING_EMPTY;
				}
				else
				{
					if ( is_done )										//DONE
					{
						sound_name 		= SOUND_BOILING_DONE;
						particle_id 	= PARTICLE_BOILING_DONE;
					}
					else												//START
					{
						sound_name 		= SOUND_BOILING_START;
						particle_id 	= PARTICLE_BOILING_START;
					}
				}
			}
			else if ( cooking_method == CookingMethodType.BAKING )
			{
				if ( is_done )											//DONE
				{
					sound_name 		= SOUND_BAKING_DONE;
					particle_id 	= PARTICLE_BAKING_DONE;
				}
				else													//START
				{
					sound_name 		= SOUND_BAKING_START;
					particle_id 	= PARTICLE_BAKING_START;
				}
			}
			else if ( cooking_method == CookingMethodType.DRYING )
			{
				if ( is_done )											//DONE
				{
					sound_name 		= SOUND_DRYING_DONE;
					particle_id 	= PARTICLE_DRYING_DONE;
				}
				else													//START
				{
					sound_name 		= SOUND_DRYING_START;
					particle_id 	= PARTICLE_DRYING_START;
				}
			}			
		}
		
		//play effects
		ParticleCookingStart( particle_id );
		SoundCookingStart( sound_name );
	}
	
	void RemoveAudioVisuals()
	{
		ParticleCookingStop();
		SoundCookingStop();
	}
	
	//particles
	void ParticleCookingStart( int particle_id )
	{
		if ( m_ParticlePlaying != particle_id )
		{
			//stop previous particles
			ParticleCookingStop();
			
			//create new
			if ( GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
			{
				vector local_pos = GetSteamPosition();
				//TODO set steam position to pot (proxy) memory point (new hierarchy needed)
				//m_ParticleCooking = Particle.Create( particle_id, this, local_pos );
				m_ParticleCooking = Particle.Create( particle_id, local_pos );
				m_ParticleCooking.Play();
				m_ParticlePlaying = particle_id;
			}
		}
	}
	
	void ParticleCookingStop()
	{
		if ( m_ParticleCooking && GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
		{
			m_ParticleCooking.Stop();
			m_ParticleCooking = NULL;
			m_ParticlePlaying = ParticleList.INVALID;
		}
	}
	
	//get position for steam particle
	protected vector GetSteamPosition()
	{
		//TODO set steam position to pot (proxy) memory point (new hierarchy needed)
		/*
		vector steam_point_local_pos = GetSelectionPosition( "steam" );
		vector steam_point_world_pos = ModelToWorld( steam_point_local_pos );
		
		return WorldToModel( steam_point_world_pos );
		*/
		
		EntityAI parent = GetHierarchyParent();
		vector fireplace_pos = parent.GetPosition();
		
		FireplaceBase fireplace = FireplaceBase.Cast( parent );
		float steam_offset = 0;
		if ( fireplace.IsBaseFireplace() )
		{
			steam_offset = 0.8;
		}
		else if ( fireplace.IsBarrelWithHoles() )
		{
			steam_offset = 1.1;
		}
		else if ( fireplace.IsFireplaceIndoor() )
		{
			steam_offset = 0.45;
		}		
		
		fireplace_pos[1] = fireplace_pos[1] + steam_offset;
		
		return fireplace_pos;
	}

	//sounds
	protected void SoundCookingStart( string sound_name )
	{
		if ( GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
		{	
			if ( m_SoundPlaying != sound_name )
			{
				//stop previous sound
				SoundCookingStop();
				
				//create new
				m_SoundCooking = PlaySoundLoop( sound_name, 50 );
				m_SoundPlaying = sound_name;
			}
		}
	}
	
	protected void SoundCookingStop()
	{
		if ( m_SoundCooking )
		{
			GetGame().ObjectDelete( m_SoundCooking );
			m_SoundCooking = NULL;
			m_SoundPlaying = "";
		}
	}	
}
