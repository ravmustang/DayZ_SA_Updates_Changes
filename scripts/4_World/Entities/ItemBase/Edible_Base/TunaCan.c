class TunaCan : Edible_Base
{
	void TunaCan()
	{
		m_Opened = false;
	}
	
	override void Open()
	{
		super.Open();
		ReplaceEdibleWithNew("TunaCan_Opened");
	}
}
