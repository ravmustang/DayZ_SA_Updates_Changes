class ShockDamage: ModifierBase
{
	override void Init()
	{
		m_TrackActivatedTime	= true;
		m_ID 					= eModifiers.MDF_SHOCK_DAMAGE;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= 0.5;
	}

	override void OnActivate(PlayerBase player)
	{
		
	}

	override void OnReconnect(PlayerBase player)
	{

	}

	override bool ActivateCondition(PlayerBase player)
	{
		if( !player.IsUnconscious() && player.GetHealth("","Blood") <= PlayerConstants.SHOCK_DAMAGE_BLOOD_THRESHOLD_HIGH )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	override bool DeactivateCondition(PlayerBase player)
	{
		return !ActivateCondition(player);
	}
	// ------------------------------------------------------------------------------

	override void OnTick(PlayerBase player, float deltaT)
	{
		float blood =  player.GetHealth("","Blood");
		float value = Math.InverseLerp( PlayerConstants.SHOCK_DAMAGE_BLOOD_THRESHOLD_HIGH, PlayerConstants.SHOCK_DAMAGE_BLOOD_THRESHOLD_LOW, blood );
		float dmg = Math.Lerp( PlayerConstants.SHOCK_DAMAGE_LOW, PlayerConstants.SHOCK_DAMAGE_HIGH, value);
		float damage =  -dmg * deltaT;
		player.AddHealth("","Shock", damage);
		//PrintString(damage.ToString());
	}
};