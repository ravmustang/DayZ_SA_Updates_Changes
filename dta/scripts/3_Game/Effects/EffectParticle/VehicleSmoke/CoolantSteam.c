class EffCoolantSteam : EffVehicleSmoke
{
	override void SetParticleStateLight()
	{
		SetParticleState( ParticleList.HATCHBACK_COOLANT_OVERHEATING );
	}
	
	override void SetParticleStateHeavy()
	{
		SetParticleState( ParticleList.HATCHBACK_COOLANT_OVERHEATED );
	}
}