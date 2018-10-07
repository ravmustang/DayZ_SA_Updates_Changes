class MainMenuStats extends ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	
	void MainMenuStats( Widget root )
	{
		m_Root = root;
	}
	
	void ShowStats()
	{
		m_Root.Show( true );
	}
	
	void HideStats()
	{
		m_Root.Show( false );
	}
}