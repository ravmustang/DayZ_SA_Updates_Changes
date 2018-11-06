class BakedBeansCan : Edible_Base
{
	void BakedBeansCan()
	{
		m_Opened = false;
	}
	
	override void Open()
	{	
		super.Open();
		ReplaceEdibleWithNew("BakedBeansCan_Opened");
	}
}
