class EffBreathVapourLight : EffectParticle
{
	
	void EffBreathVapourLight()
 	{
		SetParticleID(ParticleList.BREATH_VAPOUR_LIGHT);
	}
	
	override void Event_OnStarted()
	{
		// Uncomment this to spawn fake decal of vomit on ground.
		/*
		vector pos = m_Character.GetPosition();
		vector ori = m_Character.GetOrientation();
		
		Particle p = Particle.Play(ParticleList.VOMIT_CHUNKS, pos);
		p.SetOrientation(ori);
		*/
	}
}