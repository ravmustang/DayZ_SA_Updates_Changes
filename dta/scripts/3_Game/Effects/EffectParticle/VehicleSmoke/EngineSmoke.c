class EffEngineSmoke : EffVehicleSmoke
{
	override void SetParticleStateLight()
	{
		SetParticleState( ParticleList.HATCHBACK_ENGINE_OVERHEATING );
	}
	
	override void SetParticleStateHeavy()
	{
		SetParticleState( ParticleList.HATCHBACK_ENGINE_OVERHEATED );
	}
}