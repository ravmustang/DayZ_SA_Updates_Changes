class Hit_MeatBones : EffBulletImpactBase
{
	float m_ScalingByDistance;
	
	void Hit_MeatBones()
	{
		SetEnterParticle(ParticleList.IMPACT_MEATBONES_ENTER);
		SetExitParticle(ParticleList.IMPACT_MEATBONES_EXIT);
		//SetRicochetParticle(ParticleList.IMPACT_MEATBONES_RICOCHET);
		
		m_AngledEnter = 10;
		m_EnterSplashCoef = 0.001;
		m_ExitSplashCoef = 0.002;
		m_ScalingByDistance = 0.02;
	}
	
	override float CalculateStoppingForce(float in_speedf, float out_speedf, string ammoType)
	{
		if ( m_impact_type == ImpactTypes.MELEE )
		{
			return 500;
		}
		
		float stopping_force = in_speedf;
		
		if (m_directHit)
		{
			/*
			// TO DO: Doesn't work because IsAlive() is false, even when it shouldn't be.
			if ( m_directHit.IsMan()  &&  m_componentIndex == SURVIVOR_HEAD  &&  m_directHit.IsAlive() ) // Is the victim a survivor?
			{
				stopping_force = stopping_force * 2;
			}
			*/
			
			/*
			// TO DO: Doesn't work. Need to recognize a zombie somehow
			else if ( m_directHit.IsInherited(DayZCreatureAIType)  &&  m_componentIndex == INFECTED_HEAD ) // Is the victim a survivor that's hit in the head?
			{
				stopping_force = stopping_force * 2;
			}*/
		}
		
		return stopping_force;
	}

	override void OnPlay()
	{
		super.OnPlay();
		
		if (m_impact_type != ImpactTypes.MELEE)
		{
			vector in_speed = m_inSpeed*(-1); // Compiler demands to have this variable
			
			BloodSplatGround( GetPosition(), in_speed , 0.5 );
			
			if (m_outSpeed.Length() > 0)
			{
				BloodSplatGround( m_exitPos, m_outSpeed, 0.8  );
			}
		}
	}
	
	void BloodSplatGround( vector start_pos, vector speed_vector, float decay_coef )
	{
		vector pos = start_pos;
		float power = m_stopping_force;
		float upscale = 1;
		float rnd_offset = 0.5;
		
		while (power > 250)
		{
			pos = pos + ( speed_vector * 0.001 );
			pos = pos + Vector( rnd_offset/2 - Math.RandomFloat( 0,rnd_offset ), 0, rnd_offset/2 - Math.RandomFloat( 0,rnd_offset ) );
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]);
			Particle blood = Particle.Play(ParticleList.BLOOD_SURFACE_DROPS, pos); // BLOOD_SURFACE_DROPS IMPACT_TEST_ENTER_DEBUG

			// TO DO: Align to surface
			vector ori = GetGame().GetSurfaceOrientation(pos[0], pos[2]);
			//vector ori = blood.GetOrientation();
			//ori[0] = Math.RandomFloat(0, 360);
			
			blood.SetOrientation(ori);
			blood.ScaleParticleParam(EmitorParam.SIZE, upscale);
			
			Particle blood_chunks = Particle.Play(ParticleList.BLOOD_SURFACE_CHUNKS, pos);
			blood_chunks.SetOrientation(ori);
			
			power = power * decay_coef;
			upscale = upscale + Math.RandomFloat(0.1, 1);
		}
	}
	
	override void OnEnterCalculations( Particle p )
	{
		// Calculate particle's size based on bullet's speed
		float velocity_min = MIN_SCALING_PARAM + (m_stopping_force * m_EnterSplashCoef);
		float velocity_max = MIN_SCALING_PARAM + (m_stopping_force * m_EnterSplashCoef);
		float size = MIN_SCALING_PARAM + ( m_stopping_force * m_EnterSplashCoef);
		float birth_rate = MIN_SCALING_PARAM + (m_stopping_force * m_EnterSplashCoef);
		
		// Additional size increase by distance from camera
		vector camera_pos = GetGame().GetCurrentCameraPosition();
		float distance = vector.Distance(camera_pos, m_pos);
		float scaling_by_distance = distance * m_ScalingByDistance;
		
		// Now scale down the above size increase by player's zoom-in value
		float current_FOV = Camera.GetCurrentFOV();
		float config_FOV = GetDayZGame().GetUserFOVFromConfig();
		float FOV_scale = current_FOV / config_FOV;
		scaling_by_distance = scaling_by_distance * FOV_scale;

		if (scaling_by_distance > 3)
			scaling_by_distance = 3;
		
		size = size + scaling_by_distance;
		
		if (velocity_min < MIN_SCALING_PARAM)
			velocity_min = MIN_SCALING_PARAM;
		
		if (size < MIN_SCALING_PARAM)
			size = MIN_SCALING_PARAM;
		
		if (birth_rate < MIN_SCALING_PARAM)
			birth_rate = MIN_SCALING_PARAM;
		
		p.ScaleParticleParam(EmitorParam.VELOCITY, velocity_min);
		p.ScaleParticleParam(EmitorParam.VELOCITY_RND, velocity_max);
		p.ScaleParticleParam(EmitorParam.SIZE, size);
		p.ScaleParticleParam(EmitorParam.BIRTH_RATE, birth_rate);
	}
	
	override void OnExitCalculations(Particle p, float outSpeedf)
	{
		
		float velocity_rnd = outSpeedf * m_ExitSplashCoef;
		float birth_rate_rnd_def = outSpeedf * 0.005;
		float size = outSpeedf * 0.003;
		
		p.ScaleParticleParam(EmitorParam.VELOCITY_RND, velocity_rnd);
		p.ScaleParticleParam(EmitorParam.BIRTH_RATE, birth_rate_rnd_def);
		p.ScaleParticleParam(EmitorParam.SIZE, size);
	}
	
	/*override void OnRicochetCalculations(Particle p, float outSpeedf)
	{
		
		float velocity_rnd_def = m_stopping_force * m_RicochetSplashCoef;
		float birth_rate_rnd_def = m_stopping_force;
		
		p.SetParticleParam(EmitorParam.VELOCITY_RND, velocity_rnd_def);
		p.SetParticleParam(EmitorParam.BIRTH_RATE_RND, birth_rate_rnd_def);
		p.IncrementParticleParam(EmitorParam.BIRTH_RATE, birth_rate_rnd_def);
		
	}*/
	
	/*override void OnEnterAngledCalculations(Particle p)
	{
		
		float velocity_rnd_def2 = m_stopping_force * m_EnterAngledSplashCoef;
		float birth_rate_rnd_def2 = m_stopping_force;
		
		p.SetParticleParam(EmitorParam.VELOCITY_RND, velocity_rnd_def2);
		p.SetParticleParam(EmitorParam.BIRTH_RATE_RND, birth_rate_rnd_def2);
		p.IncrementParticleParam(EmitorParam.BIRTH_RATE, birth_rate_rnd_def2);
		
	}*/
}