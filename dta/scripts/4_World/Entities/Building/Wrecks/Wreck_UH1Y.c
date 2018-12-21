class Wreck_UH1Y extends House
{
	Particle m_ParticleEfx;
	void Wreck_UH1Y()
	{
		m_ParticleEfx = Particle.Play(ParticleList.SMOKING_HELI_WRECK, this, Vector(-0.5, 0, -1.0));
	}
	
	void ~Wreck_UH1Y()
	{
		if ( m_ParticleEfx )
			m_ParticleEfx.Stop();
	}
}