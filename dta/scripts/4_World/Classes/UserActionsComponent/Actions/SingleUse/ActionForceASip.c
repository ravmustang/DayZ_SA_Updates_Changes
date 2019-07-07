class ActionForceASip: ActionForceABite
{
	void ActionForceASip()
	{
		m_Sounds.Insert("DrinkBottle_0");
		m_Sounds.Insert("DrinkBottle_1");
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINotRuinedAndEmpty;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override string GetText()
	{
		return "#give_a_sip";
	}	
};