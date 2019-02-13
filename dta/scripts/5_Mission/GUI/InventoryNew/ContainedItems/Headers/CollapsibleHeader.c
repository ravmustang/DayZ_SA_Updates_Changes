class CollapsibleHeader: Header
{
	void CollapsibleHeader( LayoutHolder parent, string function_name )
	{
		m_DefaultFontSize = 24;
	}

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.CollapsibleHeader;
	}
}
