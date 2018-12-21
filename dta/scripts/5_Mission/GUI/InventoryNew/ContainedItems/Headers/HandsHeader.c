class HandsHeader: Header
{
	void HandsHeader( LayoutHolder parent, string function_name ) { }

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.HandsHeader;
	}
	
	override void UpdateInterval()
	{
		PlayerBase p = PlayerBase.Cast( GetGame().GetPlayer() );
		if(!p)
			return;
		EntityAI item = p.GetHumanInventory().GetEntityInHands();
		if( item )
		{
			m_MainWidget.FindAnyWidget( "collapse_button" ).Show( true );
		}
		else
		{
			m_MainWidget.FindAnyWidget( "collapse_button" ).Show( false );
		}
	}
}
