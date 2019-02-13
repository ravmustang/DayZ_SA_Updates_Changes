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
		#ifdef PLATFORM_CONSOLE
			m_LayoutName = WidgetLayoutName.InventorySlotsContainerXbox;
		#else
			switch( InventoryMenu.GetWidthType() )
			{
				case ScreenWidthType.NARROW:
				{
					m_LayoutName = WidgetLayoutName.InventorySlotsContainerNarrow;
					break;
				}
				case ScreenWidthType.MEDIUM:
				{
					m_LayoutName = WidgetLayoutName.InventorySlotsContainerMedium;
					break;
				}
				case ScreenWidthType.WIDE:
				{
					m_LayoutName = WidgetLayoutName.InventorySlotsContainerWide;
					break;
				}
			}
		#endif
			
	}
}
