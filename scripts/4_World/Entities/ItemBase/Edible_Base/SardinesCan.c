class SardinesCan : Edible_Base
{
	void SardinesCan()
	{
		m_Opened = false;
	}
	
	override void Open()
	{
		super.Open();
		ReplaceEdibleWithNew("SardinesCan_Opened");
	}
}
