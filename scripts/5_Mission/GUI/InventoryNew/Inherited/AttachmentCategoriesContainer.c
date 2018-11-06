class AttachmentCategoriesContainer: CollapsibleContainer
{
	protected ref map<int, ref Widget> m_inventory_slots;
	protected EntityAI m_Entity;

	void AttachmentCategoriesContainer( LayoutHolder parent )
	{
		m_inventory_slots = new ref map<int, ref Widget>;
	}

	void SetEntity( EntityAI entity )
	{
		m_Entity = entity;
		InitIconsContainers( entity );
		InitGhostSlots( entity );
		
		( Container.Cast( m_Parent ) ).m_Body.Insert( this );
		m_Parent.Refresh();
		SetHeaderName( entity );
	}

	void SetHeaderName( EntityAI entity )
	{
		Header h = Header.Cast( m_Body.Get(0) );
		h.SetName( entity.GetDisplayName() );
	}

	override void UpdateInterval()
	{
		if( m_Entity )
		{
			if( m_Entity.GetInventory().IsInventoryLockedForLockType( HIDE_INV_FROM_SCRIPT ) || m_Hidden )
			{
				if( m_Body.Count() > 0 && !m_Hidden )
					m_Body.Get( 0 ).OnHide();
				HideContent();
				GetMainWidget().Show( false );
			}
			else
			{
				if( m_Body.Count() > 0 && !m_Hidden )
					m_Body.Get( 0 ).OnShow();
				ShowContent();
				GetMainWidget().Show( true );
			}
	
			InitGhostSlots( m_Entity );
		}
	}
	
	override void SelectItem()
	{
		GetFocusedContainer().SelectItem();	
	}
	
	/*void SetLastActive()
	{
		Container cont = Container.Cast( m_Body[m_Body.Count() - 1] );
		cont.SetActive( true );
	}*/
	
	void LoadAttachmentCategoriesIcon( SlotsContainer items_cont, string icon_name, int slot_number )
	{
		ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( items_cont.GetMainWidget().FindAnyWidget( "Icon"+ slot_number ) );
		ImageWidget image_widget = ImageWidget.Cast( item_preview.FindAnyWidget( "GhostSlot" + slot_number ) );
		image_widget.Show( true );
		image_widget.LoadImageFile( 0, "set:dayz_inventory image:" + icon_name );
		if( m_Body.Count() > ( slot_number + 2 ) )
		{
			ClosableContainer c = ClosableContainer.Cast( m_Body.Get( slot_number + 2 ) );
			if( c.IsOpened() )
				item_preview.FindAnyWidget( "RadialIcon" + slot_number ).Show( true );
			else
				item_preview.FindAnyWidget( "RadialIcon" + slot_number ).Show( false );
		}
	}

	int GetAttachmentCategoriesCount( string config_path )
	{
		return GetGame().ConfigGetChildrenCount( config_path );
	}

	private SlotsContainer GetSlotsContainer( int icons_row )
	{
		AttachmentCategoriesSlotsContainer items_cont;
		items_cont = AttachmentCategoriesSlotsContainer.Cast( m_Body.Get( icons_row + 1 ) );

		return items_cont.GetSlotsContainer();
	}

	string GetAttachmentCategory( string config_path_attachment_categories, int i )
	{
		string attachment_category;
		GetGame().ConfigGetChildName(config_path_attachment_categories, i, attachment_category);
		return attachment_category;
	}

	string GetIconName( string config_path_attachment_categories, string attachment_category )
	{
		string icon_path = config_path_attachment_categories+ " " + attachment_category + " icon";
		string icon_name;
		GetGame().ConfigGetText(icon_path, icon_name);
		return icon_name;
	}
	
	int GetViewIndex( string config_path_attachment_categories, string attachment_category )
	{
		string preview_path = config_path_attachment_categories+ " " + attachment_category + " view_index";
		return GetGame().ConfigGetInt( preview_path );;
	}

	void MouseClick( Widget w )
	{
		if( m_Body.Count() > ( w.GetUserID() + 2 ) )
		{
			ClosableContainer c = ClosableContainer.Cast( m_Body.Get( w.GetUserID() + 2 ) );
			if( c )
			{
				if( c.IsOpened() )
				{
					c.Close();
					w.GetParent().FindAnyWidget( "RadialIcon" + w.GetUserID()%7 ).Show( true );
				}
				else
				{
					c.Open();
					w.GetParent().FindAnyWidget( "RadialIcon" + w.GetUserID()%7 ).Show( false );
				}
			}
		}
	}
	
	void InitIconsContainers( EntityAI entity )
	{
		string type = entity.GetType();
		string config_path_attachment_categories = "CfgVehicles " + type + " GUIInventoryAttachmentsProps";

		int attachments_categories_count = GetAttachmentCategoriesCount( config_path_attachment_categories );
		AttachmentCategoriesSlotsContainer items_cont;
		for ( int i = 0; i < (attachments_categories_count/7) + 1; i++ )
		{
			items_cont = new AttachmentCategoriesSlotsContainer(this);
			m_Body.Insert(items_cont);
		}
	}
	
	void InitGhostSlots( EntityAI entity )
	{
		string type = entity.GetType();
		string config_path_attachment_categories = "CfgVehicles " + type + " GUIInventoryAttachmentsProps";

		int attachments_categories_count = GetAttachmentCategoriesCount( config_path_attachment_categories );

		for (int i = 0; i < attachments_categories_count; i++)
		{
			ref SlotsContainer items_cont = GetSlotsContainer(i/7);
			string attachment_category = GetAttachmentCategory( config_path_attachment_categories, i );
			string icon_name = GetIconName( config_path_attachment_categories, attachment_category );

			if(items_cont)
			{
				int slot_number = i%7;
				
				ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( items_cont.GetMainWidget().FindAnyWidget( "Icon"+ slot_number ) );
				item_preview.Show( true );
				item_preview.SetItem( entity );
				item_preview.SetView( GetViewIndex( config_path_attachment_categories, attachment_category ) );
				
				//WidgetEventHandler.GetInstance().RegisterOnDrag( item_preview.GetParent(),  this, "OnIconDrag" );
				ImageWidget image_widget = ImageWidget.Cast( item_preview.FindAnyWidget( "GhostSlot" + slot_number%7 ) );
				image_widget.Show( false );
				//image_widget.LoadImageFile( 0, "set:dayz_inventory image:" + icon_name );
				
				int num = i + 2 + attachments_categories_count/7;
				if( m_Body.Count() > num )
				{
					ClosableContainer c = ClosableContainer.Cast( m_Body.Get( num ) );
					if( c.IsOpened() )
						item_preview.FindAnyWidget( "RadialIcon" + slot_number ).Show( true );
					else
						item_preview.FindAnyWidget( "RadialIcon" + slot_number ).Show( false );
				}
				
				WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( items_cont.GetMainWidget().FindAnyWidget( "PanelWidget"+slot_number ),  this, "MouseClick" );
				WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( items_cont.GetMainWidget().FindAnyWidget( "Icon"+slot_number ),  this, "MouseClick" );
				items_cont.GetMainWidget().FindAnyWidget( "PanelWidget"+slot_number ).SetUserID( i );
				items_cont.GetMainWidget().FindAnyWidget( "Icon"+slot_number ).SetUserID( i );
				AttachmentCategoriesRow ar;
				if( m_Body.Count() < attachments_categories_count + 2 + attachments_categories_count/7 )
				{
					ar = new AttachmentCategoriesRow(this);
				}
				else
				{
					ar = AttachmentCategoriesRow.Cast( m_Body.Get(num) );
				}
				ar.Init(attachments_categories_count, i, attachment_category, config_path_attachment_categories, entity,m_Body.Count() );
				if( m_Body.Count() < attachments_categories_count + 2 + attachments_categories_count/7 )
				{
					this.Insert(ar);
				}
			}
		}
		
		if( m_Body.Count() < attachments_categories_count + 3 + attachments_categories_count/7 )
		{
			if( entity.GetInventory().GetCargo() )
			{
				ContainerWithCargo iwc = new ContainerWithCargo( this );
				iwc.SetEntity( entity );
				
				ref SlotsContainer items_cont2 = GetSlotsContainer(attachments_categories_count/7);
				if(items_cont2)
				{
					int slot_number2 = i%7;
					ItemPreviewWidget item_preview2 = ItemPreviewWidget.Cast( items_cont2.GetMainWidget().FindAnyWidget( "Icon"+ slot_number2 ) );
					item_preview2.Show( true );
					item_preview2.SetItem( entity );					
					item_preview2.SetView( GetViewIndex( config_path_attachment_categories, attachment_category ) );
					
					ImageWidget image_widget2 = ImageWidget.Cast( item_preview2.FindAnyWidget( "GhostSlot" + slot_number2 ) );
					WidgetEventHandler.GetInstance().RegisterOnDrag( item_preview2.FindAnyWidget( "PanelWidget"+ slot_number2 ),  this, "OnIconDrag" );
					image_widget2.Show( false );
					//image_widget2.LoadImageFile( 0, "set:dayz_inventory image:" + icon_name );
					
					if( m_Body.Count() > ( slot_number2 + 2 ) )
					{
						ClosableContainer c2 = ClosableContainer.Cast( m_Body.Get( slot_number2 + 2 ) );
						if( c2.IsOpened() )
							item_preview2.FindAnyWidget( "RadialIcon" + slot_number2 ).Show( true );
						else
							item_preview2.FindAnyWidget( "RadialIcon" + slot_number2 ).Show( false );
					}
					
					WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( items_cont2.GetMainWidget().FindAnyWidget( "PanelWidget"+slot_number2 ),  this, "MouseClick" );
					WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( items_cont2.GetMainWidget().FindAnyWidget( "Icon"+slot_number2 ),  this, "MouseClick" );
					items_cont2.GetMainWidget().FindAnyWidget( "PanelWidget"+slot_number2 ).SetUserID( i );
					items_cont2.GetMainWidget().FindAnyWidget( "Icon"+slot_number2 ).SetUserID( i );
				}
			}
		}
		else
		{
			ContainerWithCargo iwc2 = ContainerWithCargo.Cast( m_Body.Get(attachments_categories_count + 2 + attachments_categories_count/7 ) );
			iwc2.UpdateInterval();
		}
	}
	
	void OnIconDrag( Widget w )
	{
		string name = w.GetName();
		name.Replace( "PanelWidget", "RadialIcon" );
		ClosableContainer c = ClosableContainer.Cast( m_Body.Get( w.GetUserID() + 2 ) );
		w.GetParent().FindAnyWidget( name ).Show( false );
	}
	
	void OnIconDrop( Widget w )
	{
		ItemManager.GetInstance().HideDropzones();
		
		string name = w.GetName();
		name.Replace( "PanelWidget", "RadialIcon" );
		ClosableContainer c = ClosableContainer.Cast( m_Body.Get( w.GetUserID() + 2 ) );
		w.GetParent().FindAnyWidget( name ).Show( c.IsOpened() );
	}

	override void OnDropReceivedFromHeader( Widget w, int x, int y, Widget receiver )
	{
		ItemPreviewWidget iw = ItemPreviewWidget.Cast( w.FindAnyWidget("Render") );
		if(!iw)
		{
			string name = w.GetName();
			name.Replace("PanelWidget", "Render");
			iw = ItemPreviewWidget.Cast( w.FindAnyWidget(name) );
		}
		if(!iw)
		{
			iw = ItemPreviewWidget.Cast( w );
		}
		if( !iw.GetItem() )
		return;
		if( m_Entity.GetInventory().CanAddAttachment( iw.GetItem() ) )
		{
			GetGame().GetPlayer().PredictiveTakeEntityToTargetAttachment( m_Entity, iw.GetItem() );
		}
		else if ( m_Entity.GetInventory().CanAddEntityToInventory( iw.GetItem() ))
		{
			GetGame().GetPlayer().PredictiveTakeEntityToTargetInventory( m_Entity, FindInventoryLocationType.ANY, iw.GetItem() );
		}
	}
	
	override void DraggingOverHeader( Widget w, int x, int y, Widget receiver )
	{
		if( w == NULL )
		{
			return;
		}
		ItemPreviewWidget iw = ItemPreviewWidget.Cast( w.FindAnyWidget("Render") );
		if(!iw)
		{
			string name = w.GetName();
			name.Replace("PanelWidget", "Render");
			iw = ItemPreviewWidget.Cast( w.FindAnyWidget(name) );
		}
		if(!iw)
		{
			iw = ItemPreviewWidget.Cast( w );
		}
		if( !iw || !iw.GetItem() )
		{
			return;
		}
		if( m_Entity.GetInventory().CanAddAttachment( iw.GetItem() ) || m_Entity.GetInventory().CanAddEntityToInventory( iw.GetItem() ) )
		{
			ItemManager.GetInstance().HideDropzones();
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
			ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
		}
		else
		{
			ItemManager.GetInstance().ShowSourceDropzone( iw.GetItem() );
			ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
		}
	}
	
	override void CollapseButtonOnMouseButtonDown(Widget w)
	{
		super.CollapseButtonOnMouseButtonDown(w);
	}
}
