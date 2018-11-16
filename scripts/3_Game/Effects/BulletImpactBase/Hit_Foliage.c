class Hit_Foliage : EffBulletImpactBase
{
	void Hit_Foliage()
	{
		SetEnterParticle(ParticleList.IMPACT_FOLIAGE_ENTER);
		//SetExitParticle(ParticleList.IMPACT_FOLIAGE_EXIT);
		//SetRicochetParticle(ParticleList.IMPACT_FOLIAGE_RICOCHET);
	}
	
	override float CalculateStoppingForce(float in_speedf, float out_speedf, string ammoType)
	{
		if ( m_impact_type == ImpactTypes.MELEE )
		{
			return 500;
		}
		
		float stopping_force = in_speedf * 0.5;
				
		return stopping_force;
	}
}