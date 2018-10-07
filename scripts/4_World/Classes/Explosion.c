// WIP

class Explosion
{
	void SpawnEffect( vector position, Effect eff, vector pos, vector ori)
	{
		SEffectManager.PlayInWorld(eff, pos, ori);
	}
}

class ExplosionTest : Building
{
	void ExplosionTest()
	{
		SetEventMask(EntityEvent.INIT); // Enable EOnInit event
	}
	
	override void EOnInit(IEntity other, int extra)
	{
		//SetPosition("0 101 0");
		Print(GetPosition());
		Explode("Explosion_NonLethal");
	}
}