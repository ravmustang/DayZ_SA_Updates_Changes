class ContainerWithElectricManager: ContainerWithCargoAndAttachments
{
	override void UpdateInterval()
	{
		super.UpdateInterval();
		if( m_Entity )
		{
			RefreshSocketIcons();
			ElectricityIcon();
		}
	}
	
	void ElectricityIcon()
	{
		if( GetRootWidget().FindAnyWidget( "electricity" ) )
		{
			bool show_electricity_icon = m_Entity.GetCompEM().HasElectricityIcon();
			GetRootWidget().FindAnyWidget( "electricity" ).Show( show_electricity_icon );
		}
		
		if( GetRootWidget().FindAnyWidget( "electricity_can_work" ) )
		{
			bool show_electricity_can_work_icon = m_Entity.GetCompEM().CanWork() && !m_Entity.IsRuined();
			GetRootWidget().FindAnyWidget( "electricity_can_work" ).Show( show_electricity_can_work_icon );
		}
	}
	
	void RefreshSocketIcons()
	{
		int count = m_Entity.GetCompEM().GetSocketsCount();

		Widget ghost_widget;
		ImageWidget ghost_icon;
		for ( int i = 0; i < count; i++ )
		{
			int row = ( i / ITEMS_IN_ROW ) + 1;
			int column = i % ITEMS_IN_ROW;
			
			Widget icon = m_Body.Get( row ).GetMainWidget().FindAnyWidget( "Icon" + column );
			
			ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( icon.FindAnyWidget( "Render" + column ) );
			ImageWidget image_widget = ImageWidget.Cast( icon.FindAnyWidget( "GhostSlot" + column ) );
			
			if( !m_Entity.GetCompEM().GetDeviceBySocketID( i ) )
			{
				image_widget.Show( true );
				item_preview.Show( false );
			}
			else
			{
				item_preview.Show( true );
				image_widget.Show( false );
				item_preview.SetItem( m_Entity.GetCompEM().GetDeviceBySocketID( i ) );
				item_preview.SetModelOrientation( Vector( 0, 0, 0 ) );
				item_preview.SetView( m_Entity.GetViewIndex() );
			}
		}
	}

	override void SetEntity( EntityAI entity )
	{
		super.SetEntity( entity );
		RefreshSocketIcons();
		ElectricityIcon();
	}
}
