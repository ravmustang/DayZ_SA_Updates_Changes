class TacticalBaconCan : Edible_Base
{
	void TacticalBaconCan()
	{
		m_Opened = false;
	}
	
	override void Open()
	{
		super.Open();
		ReplaceEdibleWithNew("TacticalBaconCan_Opened");
	}
}
