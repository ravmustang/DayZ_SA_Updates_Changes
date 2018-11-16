class Wreck_Mi8 extends House
{
	Particle m_ParticleEfx;
	void Wreck_Mi8()
	{
		m_ParticleEfx = Particle.Play(ParticleList.SMOKING_HELI_WRECK, this, Vector(0, 0.5, -3.5));
	}
}

class Wreck_UH1Y extends Wreck_Mi8
{
	void Wreck_UH1Y()
	{
		m_ParticleEfx = Particle.Play(ParticleList.SMOKING_HELI_WRECK, this, Vector(0, 0, -1.0));
	}
}