class EffExhaustSmoke : EffVehicleSmoke
{
	override void SetParticleStateLight()
	{
		SetParticleState( ParticleList.HATCHBACK_EXHAUST_SMOKE );
	}
}