class ContainerWithCargoAndAttachments extends ClosableContainer
{
	protected ref Attachments							m_Atts;
	protected ref IconsContainer						m_IconsContainer;
	protected ref UICargoGrid							m_CargoGrid;
	
	protected ref map<EntityAI, ref IconsContainer>			m_AttachmentCargoContainers;
	protected ref map<EntityAI, ref UICargoGrid>			m_AttachmentCargos;
	protected ref map<EntityAI, ref AttachmentsWrapper>		m_AttachmentAttachmentsContainers;
	protected ref map<EntityAI, ref Attachments>			m_AttachmentAttachments;
	
	override void UpdateInterval()
	{
		int i;
		if( m_Entity )
		{
			EntityAI e;/*
			array<EntityAI> cargo_attachments = m_Entity.GetAttachmentsWithCargo();
			if( cargo_attachments && m_AttachmentCargoContainers )
			{
				for( i = 0; i < m_AttachmentCargoContainers.Count(); i++ )
				{
					e			= m_AttachmentCargoContainers.GetKey( i );
					if( e )
					{
						if( cargo_attachments.Find( e ) == -1 )
						{
							IconsContainer old_cont = m_AttachmentCargoContainers.GetElement( i );
							m_Body.RemoveItem( old_cont );
							m_AttachmentCargoContainers.Remove( e );
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
					if( !m_AttachmentCargoContainers.Contains( cargo_attachments.Get( i ) ) )
					{
						ref IconsContainer cont = new IconsContainer( this );
						cont.GetRootWidget().SetSort( i + 50 );
						this.Insert( cont );
						
						ref UICargoGrid att_grid = new UICargoGrid( cargo_attachments.Get( i ), cont );
						att_grid.SetParent( this );
						
						m_AttachmentCargos.Insert( cargo_attachments.Get( i ), att_grid );
						m_AttachmentCargoContainers.Insert( cargo_attachments.Get( i ), cont );
					}
				}
			}
			
			array<EntityAI> attachment_attachments = m_Entity.GetAttachmentsWithAttachments();
			if( attachment_attachments && m_AttachmentAttachments )
			{
				for( i = 0; i < m_AttachmentAttachmentsContainers.Count(); i++ )
				{
					e			= m_AttachmentAttachmentsContainers.GetKey( i );
					if( e )
					{
						if( attachment_attachments.Find( e ) == -1 )
						{
							AttachmentsWrapper old_att_cont = m_AttachmentAttachmentsContainers.GetElement( i );
							m_Body.RemoveItem( old_att_cont );
							m_AttachmentAttachmentsContainers.Remove( e );
							m_AttachmentAttachments.Remove( e );
							
							m_Body.RemoveItem( old_att_cont );
							m_AttachmentAttachments.Remove( e );
						}
						else
						{
							m_AttachmentAttachments.Get( e ).RefreshAtt();
						}
					}
				}
				
				for( i = 0; i < attachment_attachments.Count(); i++ )
				{
					if( !m_AttachmentAttachments.Contains( attachment_attachments.Get( i ) ) )
					{
						ref Attachments att_cont = new Attachments( this, attachment_attachments.Get( i ) );
						att_cont.InitAttachmentGrid( i + 2 );
						
						
						m_AttachmentAttachments.Insert( attachment_attachments.Get( i ), att_cont );
						m_AttachmentAttachmentsContainers.Insert( attachment_attachments.Get( i ), att_cont.m_AttachmentsContainer );
						
						m_Atts.RefreshAtt();
					}
				}
			}
			*/
			if( m_Entity.GetInventory().IsInventoryLockedForLockType( HIDE_INV_FROM_SCRIPT ) || m_Closed )
			{
				HideContent();
			}
			else
			{
				ShowContent();
			}
			
			if( m_Atts )
			{
				m_Atts.RefreshAtt();
			}
			
			if( m_CargoGrid )
			{
				m_CargoGrid.UpdateInterval();
			}
		}
	}
	
	bool CanEquip()
	{
		return m_Atts.CanEquip();
	}
	
	bool CanCombine()
	{
		return m_Atts.CanCombine();
	}
	
	void TransferItemToVicinity()
	{
		m_Atts.TransferItemToVicinity();
	}
	
	override void SelectItem()
	{
		m_Atts.SelectItem();
	}
	
	EntityAI GetFocusedItem()
	{
		return EntityAI.Cast( m_Atts.GetFocusedEntity() );
	}
	
	bool IsEmpty()
	{
		return m_Atts.IsEmpty();
	}
	
	bool IsItemActive()
	{
		return m_Atts.IsItemActive();
	}

	bool IsItemWithQuantityActive()
	{
		return m_Atts.IsItemWithQuantityActive();
	}
	
	/*void SetLastActive()
	{
		
	}*/

	override void Select()
	{
		m_Atts.Select();
	}

	void SetDefaultFocus()
	{
		//m_CargoGrid.SetDefaultFocus();
		m_Atts.SetDefaultFocus();
	}

	override void UnfocusAll()
	{
		//m_CargoGrid.Unfocus();
		m_Atts.UnfocusAll();
	}

	override void MoveGridCursor( int direction )
	{
		//m_CargoGrid.MoveGridCursor( direction );
		m_Atts.MoveGridCursor( direction );
	}

	void SetEntity( EntityAI entity )
	{
		m_Entity = entity;

		Header h = Header.Cast( m_Body.Get(0) );
		h.SetName( entity.GetDisplayName() );
		h.SetItemPreview( m_Entity );
		
		m_Atts = new Attachments( this, entity );
		m_Atts.InitAttachmentGrid( 1 );

		m_IconsContainer = new IconsContainer( this );
		this.Insert( m_IconsContainer );
		m_IconsContainer.GetRootWidget().SetSort( 2 );
		if( entity.GetInventory().GetCargo() != NULL)
		{
			m_CargoGrid = new UICargoGrid( entity, m_IconsContainer );
			m_CargoGrid.SetParent(this);
		}

		array<EntityAI> cargo_attachments	= entity.GetAttachmentsWithCargo();
		m_AttachmentCargoContainers			= new map<EntityAI, ref IconsContainer>;
		m_AttachmentCargos					= new map<EntityAI, ref UICargoGrid>;
		m_AttachmentAttachmentsContainers	= new map<EntityAI, ref AttachmentsWrapper>;
		m_AttachmentAttachments				= new map<EntityAI, ref Attachments>;
		
		( Container.Cast( m_Parent ) ).Insert( this );
		
		if( m_Entity.GetInventory().IsInventoryLockedForLockType( HIDE_INV_FROM_SCRIPT ) || m_Closed )
		{
			HideContent();
		}
		else
		{
			ShowContent();
		}
		SetOpenState( ItemManager.GetInstance().GetDefaultOpenState( m_Entity.GetType() ) );
		m_Parent.m_Parent.Refresh();
	}

	void ~ContainerWithCargoAndAttachments()
	{
		if( this )
		{
			if( this.GetMainWidget() )
			{
				delete this.GetMainWidget();
			}
		}
	}

	EntityAI GetEntity()
	{
		return m_Entity;
	}

	ItemPreviewWidget GetItemPreviewWidgetDragOrDrop( Widget w )
	{
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );
		return ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
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
		if( !ipw || !ipw.IsInherited( ItemPreviewWidget ) )
		{
			return NULL;
		}
		return ipw.GetItem();
	}

	Widget GetItemPreviewWidget( Widget w )
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
		return ipw;
	}
	
	int GetCargoHeight()
	{
		int count;
		if( m_CargoGrid )
			count = m_CargoGrid.GetCargoHeight();
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
		else if( m_CargoGrid )
			return m_CargoGrid.GetLastRowWidget();
		else
			return null;
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
		
		bool inventory_locked = GetGame().GetPlayer().GetInventory().IsInventoryLocked();
		ItemManager.GetInstance().SetWidgetDraggable( w, !inventory_locked );
		EntityAI item = item_preview.GetItem();
		InventoryItem itemAtPos = InventoryItem.Cast( item );
		
		if( item )
		{
			if( button == MouseState.MIDDLE )
			{
				InspectItem( itemAtPos );
			}
		}
	}
	
	void DraggingOverHeader2(Widget w, int x, int y, Widget receiver )
	{
		DraggingOverHeader(w, x, y, receiver );
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
		name.Replace( "RadialIcon", "Temperature" );
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
		name.Replace( "Col", "Temperature" );
		w.FindAnyWidget( name ).Show( false );
		name.Replace( "Temperature", "GhostSlot" );
		w.GetParent().FindAnyWidget( name ).Show( true );
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
		
		if( cargo == m_IconsContainer )
		{
			target_entity	= m_Entity;
			target_cargo 	= m_Entity.GetInventory().GetCargo();
		}
		else
		{
			target_entity	= m_AttachmentCargoContainers.GetKeyByValue( cargo );
			if( target_entity )
			{
				target_cargo 	= target_entity.GetInventory().GetCargo();
			}
			else
				return;
		}
		
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

	void TakeAsAttachment( Widget w, Widget receiver )
	{
		EntityAI receiver_item;
		string name = receiver.GetName();
		name.Replace("PanelWidget", "Render");
		
		ItemPreviewWidget receiver_iw = ItemPreviewWidget.Cast( receiver.FindAnyWidget(name) );
		
		if( receiver_iw )
		{
			receiver_item = receiver_iw.GetItem();
		}
		
		EntityAI item = GetItemPreviewItem( w );
		if( !item )
		{
			return;
		}
		
		if( !item.GetInventory().CanRemoveEntity() )
			return;

		Weapon_Base wpn;
		Magazine mag;
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if( Class.CastTo(wpn,  m_Entity ) && Class.CastTo(mag,  item ) )
		{
			if( player.GetWeaponManager().CanAttachMagazine( wpn, mag ) )
			{
				player.GetWeaponManager().AttachMagazine( mag );
			}
		}
		else if(receiver_item)
		{
			if( ( ItemBase.Cast( receiver_item ) ).CanBeCombined( ItemBase.Cast( item ) ) )
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
			TakeIntoCargo( PlayerBase.Cast( GetGame().GetPlayer() ), m_Entity, item );
		}
		else if( ( m_Entity.GetInventory().CanAddEntityInCargo( item ) && (!player.GetInventory().HasEntityInInventory( item ) || !m_Entity.GetInventory().HasEntityInCargo( item )) ) || player.GetHumanInventory().HasEntityInHands( item ) )
		{
			TakeIntoCargo( PlayerBase.Cast( GetGame().GetPlayer() ), m_Entity, item );
		}
	}

	override void OnDropReceivedFromHeader( Widget w, int x, int y, Widget receiver )
	{
		TakeAsAttachment( w, receiver );
	}

	void OnDropReceivedFromHeader2( Widget w, int x, int y, Widget receiver )
	{
		TakeAsAttachment( w, receiver );
	}

	void DoubleClick(Widget w, int x, int y, int button)
	{
		if( button == MouseState.LEFT )
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			if( w == NULL || player.GetInventory().IsInventoryLocked() )
			{
				return;
			}
			ItemPreviewWidget iw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
			if( !iw )
			{
			  string name = w.GetName();
			  name.Replace( "PanelWidget", "Render" );
			  iw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
			}
			if( !iw )
			{
			  iw = ItemPreviewWidget.Cast( w );
			}
			if( !iw.GetItem() )
			{
				return;
			}
	
			EntityAI item = iw.GetItem();
			
			if( !item.GetInventory().CanRemoveEntity() )
				return;
			
			if( player.GetInventory().HasEntityInInventory( item ) && player.GetHumanInventory().CanAddEntityInHands( item ) )
			{
				player.PredictiveTakeEntityToHands( item );
			}
			else
			{
				if(player.GetInventory().CanAddEntityToInventory( item ) && item.GetInventory().CanRemoveEntity())
				{
					if( item.ConfigGetFloat("varStackMax") )
						player.PredictiveTakeEntityToInventory( FindInventoryLocationType.ANY, InventoryItem.Cast( item ) );
					else
						player.PredictiveTakeEntityToInventory( FindInventoryLocationType.ANY, InventoryItem.Cast( item ) );
				}
				else
				{
					if( player.GetHumanInventory().CanAddEntityInHands( item ) )
					{
						player.PredictiveTakeEntityToHands( item );
					}
				}
			}
	
			ItemManager.GetInstance().HideTooltip();
	
			name = w.GetName();
			name.Replace( "PanelWidget", "Temperature" );
			w.FindAnyWidget( name ).Show( false );
		}
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
		
		if( cargo == m_IconsContainer )
		{
			target_entity	= m_Entity;
			target_cargo 	= m_Entity.GetInventory().GetCargo();
		}
		else
		{
			target_entity	= m_AttachmentCargoContainers.GetKeyByValue( cargo );
			if( target_entity )
			{
				target_cargo 	= target_entity.GetInventory().GetCargo();
			}
			else
				return false;
		}
		
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
		if( receiver_iw )
			receiver_item = receiver_iw.GetItem();

		Weapon_Base wpn;
		Magazine mag;
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if( m_Entity )
		{
			if( Class.CastTo(wpn,  m_Entity ) && Class.CastTo(mag,  item ) )
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
				if( ( ItemBase.Cast( receiver_item ) ).CanBeCombined( ItemBase.Cast( item )) )
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
	}
}
