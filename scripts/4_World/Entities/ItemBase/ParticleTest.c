// Particle test for Maxman

//TO DO: FIX! Particles are now defined by integer, not string!

class ParticleTest extends ItemBase
{
	protected const int	PARTICLE_PATH = ParticleList.SMOKING_CAR_ENGINE;
	protected Particle 	m_Particle;
	
	// Constructor
	void ParticleTest()
	{
		m_Particle = Particle.Play( PARTICLE_PATH, this, GetPosition());
	}

	// Destructor
	void ~ParticleTest()
	{
		if (m_Particle  &&  GetGame()) // GetGame() is null when the game is being shut down
		{
			m_Particle.Stop();
			GetGame().ObjectDelete(m_Particle);
		}
	}
}