class Epinephrine: Inventory_Base
{
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionInjectEpinephrineTarget);
		AddAction(ActionInjectEpinephrineSelf);
	}
	
	override void OnApply(PlayerBase player)
	{
		player.GiveShock(100);
		player.GetStaminaHandler().SetStamina(100);
		
		if( player.GetModifiersManager().IsModifierActive(eModifiers.MDF_EPINEPHRINE ) )//effectively resets the timer
		{
			player.GetModifiersManager().DeactivateModifier( eModifiers.MDF_EPINEPHRINE );
		}
		player.GetModifiersManager().ActivateModifier( eModifiers.MDF_EPINEPHRINE );
	}
};
