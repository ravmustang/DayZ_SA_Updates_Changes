class SewingKit: Inventory_Base
{
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionBandageSelf);
		AddAction(ActionBandageTarget);
	}
};
