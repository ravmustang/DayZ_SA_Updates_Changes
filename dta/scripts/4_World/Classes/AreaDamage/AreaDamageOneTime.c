class AreaDamageOneTime extends AreaDamageBase
{	
	void AreaDamageOneTime(EntityAI parent)
	{
		AreaDamageBase(parent);
	}
	
	void ~AreaDamageOneTime() {}

	override void SetHitZones( array<string> hitzones )
	{
		m_HitZones = hitzones;
	}
	
	override void OnEnterServer(Object object)
	{
		super.OnEnterServer(object);

		EvaluateDamage(object);
	}
}