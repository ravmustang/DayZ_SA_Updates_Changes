class TLRLight extends Switchable_Base
{
	override bool CanPutAsAttachment( EntityAI parent )
	{
		if(!super.CanPutAsAttachment(parent)) {return false;}
		if ( !parent.IsKindOf("PlateCarrierHolster") && !parent.IsKindOf("PlateCarrierComplete") && !parent.IsKindOf("CarrierHolsterSolo") && !parent.IsKindOf("ChestHolster") )
		{
			return true;
		}
		return false;
	}
	
	//copied from Flashlight
	override void OnWorkStart()
	{
		SetPilotLight(true);
	}

	override void OnWorkStop()
	{
		SetPilotLight(false);
	}
}