class EffBulletImpactBase : Effect
{
	static const int	SURVIVOR_HEAD = 0; // Head component
	static const int	INFECTED_HEAD = 3; // Head component
	
	static const float	MIN_SCALING_PARAM = 0.5;
	
	Object 				m_directHit;
	float 				m_stopping_force;
	int 				m_impact_type;
	int 				m_componentIndex;
	vector 				m_pos;
	vector 				m_surfNormal;
	vector 				m_exitPos;
	vector 				m_inSpeed;
	vector 				m_outSpeed;
	string 				m_ammoType;
	
	static vector INVALID = "0 0 0";
	
	// Particle Effects
	int m_ParticleEnter = -1;
	int m_ParticleExit = -1;
	int m_ParticleRicochet = -1;
	
	// Calculations
	float m_EnterSplashCoef = 0.0015;
	float m_ExitSplashCoef = 0.0015;
	float m_RicochetSplashCoef = 0.0015;
	float m_EnterAngledSplashCoef = 0.0015;
	float m_AngledEnter = 0.40;
	
	void EffBulletImpactBase()
	{
		
	}
	
	void SetEnterParticle(int id)
	{
		m_ParticleEnter = id;
	}
	
	void SetExitParticle(int id)
	{
		m_ParticleExit = id;
	}
	
	void SetRicochetParticle(int id)
	{
		m_ParticleRicochet = id;
	}
	
	void SetAngledEnterValue(float f)
	{
		m_AngledEnter = f;
	}
	
	void EvaluateEffect(Object directHit, int componentIndex, vector pos, int impact_type, vector surfNormal, vector exitPos, vector inSpeed, vector outSpeed, string ammoType)
	{
		m_directHit 		= directHit;
		m_pos 				= pos;
		m_impact_type 		= impact_type;
		m_componentIndex 	= componentIndex;
		m_surfNormal 		= surfNormal;
		m_exitPos 			= exitPos;
		m_inSpeed 			= inSpeed;
		m_outSpeed 			= outSpeed;
		m_ammoType 			= ammoType;
		
		m_stopping_force 	= CalculateStoppingForce(m_inSpeed.Length(), m_outSpeed.Length(), ammoType);
	}
	
	float CalculateStoppingForce(float in_speedf, float out_speedf, string ammoType)
	{
		if ( m_impact_type == ImpactTypes.MELEE )
		{
			return 500;
		}
		
		float stopping_force = in_speedf - out_speedf;
		
		return stopping_force;
	}
	
	void OnEnterCalculations( Particle p )
	{
		// All values represent scale
		float velocity_min = MIN_SCALING_PARAM + (m_stopping_force * m_EnterSplashCoef);
		float velocity_max = MIN_SCALING_PARAM + (m_stopping_force * m_EnterSplashCoef);
		float size = MIN_SCALING_PARAM + ( m_stopping_force * m_EnterSplashCoef);
		float birth_rate = MIN_SCALING_PARAM + (m_stopping_force * m_EnterSplashCoef);
		
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
	
	void OnExitCalculations(Particle p, float outSpeedf)
	{
		float velocity_min = 1 + (outSpeedf * m_ExitSplashCoef);
		float velocity_max = 1 + (outSpeedf * m_ExitSplashCoef);
		float size = 1 + ( outSpeedf * m_ExitSplashCoef);
		float birth_rate = 1 + (outSpeedf * m_ExitSplashCoef);
		
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
	
	void OnRicochetCalculations(Particle p, float outspeedf)
	{
		float velocity_min = 1 + (m_stopping_force * m_RicochetSplashCoef);
		float velocity_max = 1 + (m_stopping_force * m_RicochetSplashCoef);
		float size = 1 + ( m_stopping_force * m_RicochetSplashCoef);
		float birth_rate = 1 + (m_stopping_force * m_RicochetSplashCoef);
		
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
	
	void OnEnterAngledCalculations(Particle p)
	{
		float velocity_min = 1 + (m_stopping_force * m_EnterAngledSplashCoef);
		float velocity_max = 1 + (m_stopping_force * m_EnterAngledSplashCoef);
		float size = 1 + (m_stopping_force * m_EnterAngledSplashCoef);
		float birth_rate = 1 + (m_stopping_force * m_EnterAngledSplashCoef);
		
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
	
	override void OnPlay()
	{
		Particle p;
		vector particle_orientation;
		float outSpeedf = m_outSpeed.Length();
		
		if ( m_impact_type == ImpactTypes.RICOCHET )
		{
			p = Particle.Play(m_ParticleRicochet, m_pos);
			
			if (p)
			{
				particle_orientation = m_outSpeed.VectorToAngles();
				particle_orientation = particle_orientation + "0 -90 0";
				p.SetOrientation(particle_orientation);
				
				OnRicochetCalculations(p, outSpeedf);
			}
		}
		else
		{
			p = Particle.Play(m_ParticleEnter, m_pos );
			
			if (p)
			{
				if (m_surfNormal != INVALID)
				{
					particle_orientation = m_surfNormal.VectorToAngles();
					particle_orientation = particle_orientation + "0 270 0";
				}
				else
				{
					particle_orientation = "0 0 0"; // This vector is in angles
				}
				
				p.SetOrientation(particle_orientation);
			
				OnEnterCalculations(p);
			}
			
			if (outSpeedf > 0  &&  m_surfNormal != INVALID)
			{
				p = Particle.Play(m_ParticleExit, m_exitPos);
				
				if (p)
				{
					particle_orientation = m_outSpeed.VectorToAngles();
					particle_orientation = particle_orientation + "0 -90 0";
					p.SetOrientation(particle_orientation);
					
					OnExitCalculations(p, outSpeedf);
				}
			}
			else
			{
				if (m_surfNormal != INVALID)
				{
					vector surfNormalN = m_surfNormal.Normalized();
					vector inSpeedN = m_inSpeed.Normalized();
					vector bounce_ori = surfNormalN + inSpeedN;
					
					float dot = vector.Dot(bounce_ori, surfNormalN);
					
					if ( dot > m_AngledEnter )
					{
						p = Particle.Play(m_ParticleRicochet, m_pos);
			
						if (p)
						{
							particle_orientation = bounce_ori.VectorToAngles();
							particle_orientation = particle_orientation + "0 -90 0";
							p.SetOrientation(particle_orientation);
							
							OnEnterAngledCalculations(p);
						}
					}
				}
			}
		}
		
		if (p)
		{
			p.SetEffectHolder(this);
		}
	}
}