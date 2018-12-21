class SlotsContainer: LayoutHolder
{
	int m_ColumnCount;
	void SlotsContainer( LayoutHolder parent ) 
	{
		//TODO: create layout for SlotsContainer and set ARGB there
		//m_ParentWidget.SetColor( ARGB( 175, 155, 155, 155 ) );
	}

	void SetColumnCount( int column_count )
	{
		m_ColumnCount = column_count;
	}

	int GetColumnCount()
	{
		return m_ColumnCount;
	}

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.InventorySlotsContainer;
	}
}
