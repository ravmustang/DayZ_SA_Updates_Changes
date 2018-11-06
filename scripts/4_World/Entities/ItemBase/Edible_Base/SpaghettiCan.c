class SpaghettiCan : Edible_Base
{
	void SpaghettiCan()
	{
		m_Opened = false;
	}
	
	override void Open()
	{
		super.Open();
		ReplaceEdibleWithNew("SpaghettiCan_Opened");
	}
}
