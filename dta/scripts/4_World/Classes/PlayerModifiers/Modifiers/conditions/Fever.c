class FeverMdfr: ModifierBase
{
	override void Init()
	{
		m_TrackActivatedTime = false;
		m_ID 					= eModifiers.MDF_FEVER;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE_LONG;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	override bool ActivateCondition(PlayerBase player)
	{
		return false;
		return player.GetModifiersManager().IsModifierActive(eModifiers.MDF_INFLUENZA) || player.GetModifiersManager().IsModifierActive(eModifiers.MDF_WOUND_INFECTION);
	}

	override void OnActivate(PlayerBase player)
	{
		if( player.m_NotifiersManager ) 
			player.m_NotifiersManager.ActivateByType(eNotifiers.NTF_FEVERISH);
		
		player.GetSymptomManager().QueueUpSecondarySymptom(SymptomIDs.SYMPTOM_FEVERBLUR);
	}

	override void OnReconnect(PlayerBase player)
	{
		this.OnActivate(player);
	}


	override void OnDeactivate(PlayerBase player)
	{
		if( player.m_NotifiersManager ) 
			player.m_NotifiersManager.DeactivateByType(eNotifiers.NTF_FEVERISH);
		player.GetSymptomManager().RemoveSecondarySymptom(SymptomIDs.SYMPTOM_FEVERBLUR);
	}


	override bool DeactivateCondition(PlayerBase player)
	{
		return !ActivateCondition(player);
	}

	override void OnTick(PlayerBase player, float deltaT)
	{
		float water_loss = deltaT * PlayerConstants.WATER_LOSS_FEVER;
		player.GetStatWater().Add(-water_loss);
	}
};