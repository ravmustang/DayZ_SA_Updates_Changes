class HumanSteakMeat extends Edible_Base
{
	void HumanSteakMeat()
	{
		InsertAgent(eAgents.BRAIN, 1);
	}
	override bool IsMeat()
	{
		return true;
	}
}
