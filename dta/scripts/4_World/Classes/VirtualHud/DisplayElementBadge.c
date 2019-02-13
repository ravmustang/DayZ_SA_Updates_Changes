class DisplayElementBadge extends DisplayElementBase
{
	void SetStatus(int level)
	{
		SetValue(level);
	}
	
	override void UpdateHUD()
	{
		if(m_Value == 1)
		{
			m_ModulePlayerStatus.SetBadge(m_Key, true);
		}
		else
		{
			m_ModulePlayerStatus.SetBadge(m_Key, false);
		}
	}
}