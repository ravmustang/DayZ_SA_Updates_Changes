class AreaDamageOneTimeDeferred extends AreaDamageBase
{	
	void AreaDamageOneTimeDeferred(EntityAI parent)
	{
		AreaDamageBase(parent);
	}

	void ~AreaDamageOneTimeDeferred() {}
	
	override void SetDeferDuration(float time)
	{
		m_DeferDuration = time;
	}

	override void SetHitZones( array<string> hitzones )
	{
		m_HitZones = hitzones;
	}

	override void OnEnterServer(Object object)
	{
		super.OnEnterServer(object);;

		EvaluateDamage_Defer(object);
	}
}