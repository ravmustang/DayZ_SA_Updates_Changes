class PeachesCan : Edible_Base
{
	void PeachesCan()
	{
		m_Opened = false;
	}
	
	override void Open()
	{
		super.Open();
		ReplaceEdibleWithNew("PeachesCan_Opened");
	}
}
