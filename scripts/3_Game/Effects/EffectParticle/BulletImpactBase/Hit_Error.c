class Hit_ErrorNoMaterial : EffBulletImpactBase
{
	void Hit_ErrorNoMaterial()
	{
		if ( GetGame().IsDebug() )
		{
			SetEnterParticle(ParticleList.IMPACT_TEST_NO_MATERIAL_ERROR);
		}
		else
		{
			SetEnterParticle(ParticleList.IMPACT_TEST);
		}
	}
}