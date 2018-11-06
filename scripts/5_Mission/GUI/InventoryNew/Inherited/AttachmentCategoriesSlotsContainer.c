class AttachmentCategoriesSlotsContainer: Container
{
	ref SlotsContainer m_ItemsCont;
	void AttachmentCategoriesSlotsContainer( LayoutHolder parent )
	{
		m_ItemsCont = new SlotsContainer(this);
		m_Body.Insert(m_ItemsCont);
		for( int k = 0; k < 7; k++ )
		{
			WidgetEventHandler.GetInstance().RegisterOnDropReceived( m_ItemsCont.GetMainWidget().FindAnyWidget( "Icon" + k ),  m_Parent, "OnDropReceivedFromHeader" );
			WidgetEventHandler.GetInstance().RegisterOnDropReceived( m_ItemsCont.GetMainWidget().FindAnyWidget( "GhostSlot" + k ),  m_Parent, "OnDropReceivedFromHeader" );
			WidgetEventHandler.GetInstance().RegisterOnDropReceived( m_ItemsCont.GetMainWidget().FindAnyWidget( "PanelWidget" + k ),  m_Parent, "OnDropReceivedFromHeader" );
			      
			WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_ItemsCont.GetMainWidget().FindAnyWidget( "Icon" + k ),  m_Parent, "DraggingOverHeader" );
			WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_ItemsCont.GetMainWidget().FindAnyWidget( "GhostSlot" + k ),  m_Parent, "DraggingOverHeader" );
			WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_ItemsCont.GetMainWidget().FindAnyWidget( "PanelWidget" + k ),  m_Parent, "DraggingOverHeader" );
		}
	}
  
	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.ClosableContainer;
	}
  
	SlotsContainer GetSlotsContainer()
	{
		return m_ItemsCont;
	}
}
