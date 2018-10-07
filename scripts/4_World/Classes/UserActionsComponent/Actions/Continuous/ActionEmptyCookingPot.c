class ActionEmptyCookingPot: ActionEmptyBottleBase
{
	void ActionEmptyCookingPot()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EMPTYPOT;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		
		m_CallbackClass = ActionEmptyBottleBaseCB;
		m_FullBody = false;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
	}
	
	override int GetType()
	{
		return AT_EMPTY_COOKING_POT;
	}
};