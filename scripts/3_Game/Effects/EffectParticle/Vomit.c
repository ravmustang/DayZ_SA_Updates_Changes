class EffVomit : EffectParticle
{
	void EffVomit()
	{
		SetParticleID(ParticleList.VOMIT);
//		SetSoundName("Action_Vomitting_0");
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