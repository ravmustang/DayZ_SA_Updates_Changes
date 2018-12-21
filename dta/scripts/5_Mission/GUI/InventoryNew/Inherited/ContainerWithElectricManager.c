class ContainerWithElectricManager: ClosableContainer
{
	
	ref Attachments m_Atts;
	protected ref map<EntityAI, ref IconsContainer>		m_AttachmentCargoGrids;
	protected ref map<EntityAI, ref UICargoGrid>		m_AttachmentCargos;
	
	void ContainerWithElectricManager( Container parent, int sort = -1 )
	{
		m_Parent = parent;
		m_AttachmentCargoGrids	= new map<EntityAI, ref IconsContainer>;
		m_AttachmentCargos		= new map<EntityAI, ref UICargoGrid>;
	}

	override void UpdateInterval()
	{
		if( m_Entity )
		{
			int i;
			/*
			array<EntityAI> cargo_attachments = m_Entity.GetAttachmentsWithCargo();
			if( cargo_attachments && m_AttachmentCargoGrids )
			{
				for( i = 0; i < m_AttachmentCargoGrids.Count(); i++ )
				{
					EntityAI e			= m_AttachmentCargoGrids.GetKey( i );
					
					if( e )
					{
						if( cargo_attachments.Find( e ) == -1 )
						{
							IconsContainer old_cont = m_AttachmentCargoGrids.GetElement( i );
							m_Body.RemoveItem( old_cont );
							m_AttachmentCargoGrids.Remove( e );
							m_AttachmentCargos.Remove( e );
						}
						else
						{
							m_AttachmentCargos.Get( e ).UpdateInterval();
						}
					}
				}
				
				for( i = 0; i < cargo_attachments.Count(); i++ )
				{
					if( !m_AttachmentCargoGrids.Contains( cargo_attachments.Get( i ) ) )
					{
						ref IconsContainer cont = new IconsContainer( this );
						cont.GetRootWidget().SetSort( i + 3 );
						this.Insert( cont );
						
						ref UICargoGrid att_grid = new UICargoGrid( cargo_attachments.Get( i ), cont );
						m_AttachmentCargos.Insert( cargo_attachments.Get( i ), att_grid );
						att_grid.SetParent( this );
						
						m_AttachmentCargoGrids.Insert( cargo_attachments.Get( i ), cont );
					}
				}
			}
			*/
			m_Atts.RefreshAtt();
			RefreshSocketIcons();
			ElectricityIcon();
		}
	}
	
	override void MoveGridCursor( int direction )
	{
		m_Atts.MoveGridCursor( direction );
	}
	
	void ElectricityIcon()
	{
		if( GetMainWidget().FindAnyWidget( "electricity" ) )
		{
			bool show_electricity_icon = m_Entity.GetCompEM().HasElectricityIcon();
			GetMainWidget().FindAnyWidget( "electricity" ).Show( show_electricity_icon );
		}
		
		if( GetMainWidget().FindAnyWidget( "electricity_can_work" ) )
		{
			bool show_electricity_can_work_icon = m_Entity.GetCompEM().CanWork() && !m_Entity.IsRuined();
			GetMainWidget().FindAnyWidget( "electricity_can_work" ).Show( show_electricity_can_work_icon );
		}
	}
	
	void RefreshSocketIcons()
	{
		int count = m_Entity.GetCompEM().GetSocketsCount();

		Widget ghost_widget;
		ImageWidget ghost_icon;
		for ( int i = 0; i < count; i++ )
		{
			string icon_name = "missing";

			if ( ghost_icon )
			{
				ghost_icon.LoadImageFile( 0, "set:dayz_inventory image:" + icon_name );
			}
			
			int row = ( i / 7 ) + 2;
			int column = i % 7;
			ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( m_Body.Get( row ).GetMainWidget().FindAnyWidget( "Icon" + column ).FindAnyWidget( "Render" + column ) );
			Widget icon = m_Body.Get( row ).GetMainWidget().FindAnyWidget( "Icon" + column );
			
			ImageWidget image_widget = ImageWidget.Cast( icon.FindAnyWidget( "GhostSlot" + column ) );

			bool show_ghost_image = m_Entity.GetCompEM().GetDeviceBySocketID( i ) == NULL;
			image_widget.Show( show_ghost_image );

			image_widget.LoadImageFile( 0, "set:dayz_inventory image:" + icon_name );
			item_preview.SetItem( m_Entity.GetCompEM().GetDeviceBySocketID( i ) );
			item_preview.SetModelOrientation( Vector( 0, 0, 0 ) );
			item_preview.SetView( m_Entity.GetViewIndex() );
		}
	}
	
	void OnIconDrag( Widget w )
	{
		ItemManager.GetInstance().HideDropzones();
		if( m_Entity.GetHierarchyParent() == GetGame().GetPlayer() )
		{
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
		}
		else
		{
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
		}
		ItemManager.GetInstance().SetIsDragging( true );
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget(name) );
		float icon_x, icon_y, x_content, y_content;
		int m_sizeX, m_sizeY;

		InventoryItem i_item = InventoryItem.Cast( ipw.GetItem() );
		GetGame().GetInventoryItemSize( i_item, m_sizeX, m_sizeY );

		m_Parent.m_Parent.GetMainWidget().FindAnyWidget( "body" ).GetScreenSize( x_content, y_content );	
		icon_x = x_content / 10;
		icon_y = x_content / 10;
		w.SetFlags( WidgetFlags.EXACTSIZE );
		w.SetSize( icon_x, icon_y );
		w.SetSize( icon_x * m_sizeX - 1 , icon_y * m_sizeY + m_sizeY - 1 );
		
		if( !ipw.GetItem() )
		{
			return;
		}
		name.Replace( "Render", "Col" );
		w.FindAnyWidget( name ).Show( true );
		name.Replace( "Col", "RadialIcon" );
		w.FindAnyWidget( name ).Show( false );
	}
	
	void OnIconDrop( Widget w )
	{
		ItemManager.GetInstance().HideDropzones();
		
		ItemManager.GetInstance().SetIsDragging( false );
		w.ClearFlags( WidgetFlags.EXACTSIZE );
		w.SetSize( 1, 1 );
		string name = w.GetName();
		name.Replace( "PanelWidget", "Col" );
		w.FindAnyWidget( name ).Show( false );
		name.Replace( "Col", "GhostSlot" );
		w.GetParent().FindAnyWidget( name ).Show( true );
	}
	
	override void DraggingOver( Widget w, int x, int y, Widget receiver )
	{
		DraggingOverHeader( w, x, y, receiver );
	}
	
	override void DraggingOverHeader( Widget w, int x, int y, Widget receiver )
	{
		if( w == NULL )
		{
			return;
		}
		EntityAI item = GetItemPreviewItem( w );		
		if( !item )
		{
			return;
		}
		
		EntityAI receiver_item;
		string name = receiver.GetName();
		name.Replace("PanelWidget", "Render");
		ItemPreviewWidget receiver_iw = ItemPreviewWidget.Cast( receiver.FindAnyWidget(name) );
		if(receiver_iw)
		receiver_item = receiver_iw.GetItem();

		Weapon_Base wpn;
		Magazine mag;
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( Class.CastTo(wpn,  m_Entity ) && Class.CastTo(mag,  item ) )
		{
			if( player.GetWeaponManager().CanAttachMagazine( wpn, mag ) )
			{
				ItemManager.GetInstance().HideDropzones();
				if( m_Entity.GetHierarchyParent() == GetGame().GetPlayer() )
				{
					ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
				}
				else
				{
					ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
				}
				ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
			}
		}
		else if( receiver_item )
		{
			if ( ( ItemBase.Cast( receiver_item ) ).CanBeCombined( ItemBase.Cast( item ) ) )
			{
				ItemManager.GetInstance().HideDropzones();
				if( m_Entity.GetHierarchyParent() == GetGame().GetPlayer() )
				{
					ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
				}
				else
				{
					ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
				}
				ColorManager.GetInstance().SetColor( w, ColorManager.COMBINE_COLOR );
			}
			else if( GameInventory.CanSwapEntities( receiver_item, item ) )
			{
				ItemManager.GetInstance().HideDropzones();
				if( m_Entity.GetHierarchyParent() == GetGame().GetPlayer() )
				{
					ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
				}
				else
				{
					ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
				}
				ColorManager.GetInstance().SetColor( w, ColorManager.SWAP_COLOR );
			}
		}
		else
		if( m_Entity.GetInventory().CanAddAttachment( item ) )
		{
			ItemManager.GetInstance().HideDropzones();
			if( m_Entity.GetHierarchyParent() == GetGame().GetPlayer() )
			{
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
			}
			else
			{
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
			}
			ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
		}
		else if( ( m_Entity.GetInventory().CanAddEntityInCargo( item ) && (!player.GetInventory().HasEntityInInventory( item ) || !m_Entity.GetInventory().HasEntityInCargo( item )) ) || player.GetHumanInventory().HasEntityInHands( item ) )
		{
			ItemManager.GetInstance().HideDropzones();
			if( m_Entity.GetHierarchyParent() == GetGame().GetPlayer() )
			{
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "RightPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
			}
			else
			{
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
			}
			ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
		}
		else
		{
			ItemManager.GetInstance().ShowSourceDropzone( item );
			ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
		}
	}
	
	void DraggingOverHeader2(Widget w, int x, int y, Widget receiver )
	{
		DraggingOverHeader(w, x, y, receiver );
	}

	EntityAI GetItemPreviewItem( Widget w )
	{
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		if( !ipw )
		{
		  string name = w.GetName();
		  name.Replace( "PanelWidget", "Render" );
		  ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		}
		if( !ipw )
		{
		  ipw = ItemPreviewWidget.Cast( w );
		}
		if( !ipw.IsInherited( ItemPreviewWidget ) )
		{
			return NULL;	
		}
		return ipw.GetItem();
	}

	void MouseClick2( Widget w, int x, int y, int button )
	{
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );
		ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		
		if( !item_preview )
		{
			return;
		}
		
		EntityAI item = item_preview.GetItem();
		InventoryItem itemAtPos = InventoryItem.Cast( item );
		
		if( item )
		{
			if ( button == MouseState.MIDDLE )
			{
				InspectItem( itemAtPos );
			}
		}
	}
	
	void OnDropReceivedFromHeader2( Widget w, int x, int y, Widget receiver )
	{
		TakeAsAttachment( w, receiver );
	}

	void TakeAsAttachment( Widget w, Widget receiver )
	{
		EntityAI receiver_item;
		string name = receiver.GetName();
		name.Replace("PanelWidget", "Render");
		ItemPreviewWidget receiver_iw = ItemPreviewWidget.Cast( receiver.FindAnyWidget(name) );
		if(receiver_iw)
		receiver_item = receiver_iw.GetItem();
		
		EntityAI item = GetItemPreviewItem( w );		
		if( !item )
		{
			return;
		}
		
		Weapon_Base wpn;
		Magazine mag;
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		if( !item.GetInventory().CanRemoveEntity() )
			return;
		
		if ( Class.CastTo(wpn,  m_Entity ) && Class.CastTo(mag,  item ) )
		{
			if( player.GetWeaponManager().CanAttachMagazine( wpn, mag ) )
			{
				player.GetWeaponManager().AttachMagazine( mag );	
			}
		}
		else if (receiver_item)
		{
			if ( ( ItemBase.Cast( receiver_item ) ).CanBeCombined( ItemBase.Cast( item ) ) )
			{
				( ItemBase.Cast( receiver_item ) ).CombineItemsClient( ItemBase.Cast( item ) );
			}
			else if( GameInventory.CanSwapEntities( receiver_item, item ) )
			{
				if( !receiver_item.GetInventory().CanRemoveEntity() )
					return;
				GetGame().GetPlayer().PredictiveSwapEntities( item, receiver_item );
			}
		}
		else if( m_Entity.GetInventory().CanAddAttachment( item ) )
		{
			GetGame().GetPlayer().PredictiveTakeEntityToTargetAttachment( m_Entity, item );
		}
	}
	

	void SetEntity( EntityAI entity )
	{
		m_Entity = entity;

		Header h = Header.Cast( m_Body.Get( 0 ) );
		h.SetName( entity.GetDisplayName() );
		h.SetItemPreview( m_Entity );
		m_Atts = new Attachments( this, entity );
		m_Atts.InitAttachmentGrid( 1 );
		//START - InitSocketGrid();
			int count = m_Entity.GetCompEM().GetSocketsCount();

			if( count > 0 )
			{
				for ( int i = 0; i < ( count / 7 ) + 1; i++ )
				{
					SlotsContainer ic = new SlotsContainer( this );
					this.Insert( ic );
				}
			}
			
			RefreshSocketIcons();
		//END - InitSocketGrid();
		
		ElectricityIcon();

		( Container.Cast( m_Parent ) ).Insert( this );

		m_Parent.m_Parent.Refresh();
	}
	
		int GetCargoHeight()
	{
		int count;
		foreach( UICargoGrid grid : m_AttachmentCargos )
		{
			count += grid.GetCargoHeight();
		}
		return count;
	}
	
	Widget GetLastRowWidget()
	{
		if( m_AttachmentCargos.Count() > 0 )
			return m_AttachmentCargos.GetElement( m_AttachmentCargos.Count() - 1 ).GetLastRowWidget();	
		else
			return null;
	}
	
	bool DraggingOverGrid( Widget w,  int x, int y, Widget reciever, IconsContainer cargo )
	{
		if( w == NULL )
		{
			return false;
		}
		
		EntityAI item = GetItemPreviewItem( w );
		
		if( !item )
		{
			return false;
		}

		int color;
		int idx = 0;
		int c_x, c_y;
		
		EntityAI target_entity;
		CargoBase target_cargo;
		
		target_entity	= m_AttachmentCargoGrids.GetKeyByValue( cargo );
		if( target_entity )
		{
			target_cargo 	= target_entity.GetInventory().GetCargo();
		}
		else
			return false;
		
		if( target_cargo && target_entity )
		{
			c_x = target_cargo.GetHeight();
			c_y = target_cargo.GetWidth();
		}
		else
			return false;
		
		if( c_x > x && c_y > y && target_entity.GetInventory().CanAddEntityInCargoEx( item, idx, x, y ) )
		{
			color = ColorManager.GREEN_COLOR;
		}
		else
		{
			color = ColorManager.RED_COLOR;
		}

		if( w.FindAnyWidget("Color") )
		{
			w.FindAnyWidget("Color").SetColor( color );
		}
		else
		{
			string name = w.GetName();
			name.Replace( "PanelWidget", "Col" );
			if( w.FindAnyWidget( name ) )
			{
				w.FindAnyWidget( name ).SetColor( color );
			}
		}

		return true;
	}

	void DropReceived( Widget w, int x, int y, IconsContainer cargo )
	{
		EntityAI item = GetItemPreviewItem( w );
		if( !item )
		{
			return;
		}

		int idx = 0;
		int c_x, c_y;
		
		EntityAI target_entity;
		CargoBase target_cargo;
		
		target_entity	= m_AttachmentCargoGrids.GetKeyByValue( cargo );
		if( target_entity )
		{
			target_cargo 	= target_entity.GetInventory().GetCargo();
		}
		else
			return;
		
		if( target_cargo && target_entity )
		{
			c_x = target_cargo.GetHeight();
			c_y = target_cargo.GetWidth();
		}
		else
			return;
		
		if( c_x > x && c_y > y && target_entity.GetInventory().CanAddEntityInCargoEx( item, idx, x, y ) )
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );

			TakeIntoCargo( player, target_entity, item, idx, x, y );

			Icon icon = cargo.GetIcon( item.GetID() );
			
			if( icon )
			{
				//w.FindAnyWidget("Color").SetColor( ColorManager.BASE_COLOR );
				icon.RefreshPos( x, y );
				icon.Refresh();
				Refresh();
			}
		}
	}
	
	void TakeIntoCargo( notnull PlayerBase player, notnull EntityAI entity, notnull EntityAI item, int idx = -1, int row = 0, int col = 0 )
	{
		ItemBase item_base = ItemBase.Cast( item );
		float stackable = item_base.ConfigGetFloat("varStackMax");
		
		if( !item.GetInventory().CanRemoveEntity() )
			return;
		
		if( stackable == 0 || stackable >= item_base.GetQuantity() )
		{
			if( idx != -1 )
				player.PredictiveTakeEntityToTargetCargoEx( entity, item, idx, row, col );
			else
				player.PredictiveTakeEntityToTargetAttachment(entity, item);
		}
		else if( stackable != 0 && stackable < item_base.GetQuantity() )
		{
			item_base.SplitIntoStackMaxCargoClient( entity, idx, row, col );
		}
	}

	void ~ContainerWithElectricManager()
	{
		if( this && m_Parent )
		{
			delete this.GetMainWidget();
		}
	}

	EntityAI GetEntity()
	{
		return m_Entity;
	}
}
