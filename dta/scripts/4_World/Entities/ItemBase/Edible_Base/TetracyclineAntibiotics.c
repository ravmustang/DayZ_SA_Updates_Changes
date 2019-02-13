class TetracyclineAntibiotics : Edible_Base
{
	override void OnConsume(float amount, PlayerBase consumer)
	{
		if( consumer.GetModifiersManager().IsModifierActive(eModifiers.MDF_ANTIBIOTICS))//effectively resets the timer
		{
			consumer.GetModifiersManager().DeactivateModifier( eModifiers.MDF_ANTIBIOTICS );
		}
		consumer.GetModifiersManager().ActivateModifier( eModifiers.MDF_ANTIBIOTICS );
	}
}