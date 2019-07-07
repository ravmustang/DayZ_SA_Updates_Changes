class Land_FuelStation_Feed extends FuelStation
{
	override void EEKilled (Object killer)
	{
		super.EEKilled(killer);

	 	Explode(DT_EXPLOSION, "Explosion_NonLethal");
	}
	
	override void OnExplosionEffects (Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal, float energyFactor, float explosionFactor, bool isWater, string ammoType)
	{
		if ( !GetGame().IsServer() || !GetGame().IsMultiplayer() )
		{	
			vector n = surfNormal.VectorToAngles();
			Particle p1 = Particle.Play(ParticleList.SMOKE_GENERIC_WRECK, pos);
			p1.SetOrientation(n);
		
			Particle p2 = Particle.Play(ParticleList.EXPLOSION_LANDMINE, pos);
			p2.SetOrientation(n);

			Particle p3 = Particle.Play(ParticleList.IMPACT_METAL_RICOCHET, pos);
			p3.SetOrientation(n);
		
			Particle p4 = Particle.Play(ParticleList.IMPACT_GRAVEL_RICOCHET, pos);
			p4.SetOrientation(n);
		}
	}
}
