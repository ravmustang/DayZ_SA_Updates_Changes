class VicinityContainer: CollapsibleContainer
{
	ref VicinitySlotsContainer			m_VicinityIconsContainer;
	ref map<EntityAI, ref Container>	m_ShowedItems				= new ref map<EntityAI, ref Container>;
	ref map<int, ref Container>			m_ShowedItemsIDs			= new ref map<int, ref Container>;
	ref array<Object>					m_ShowedItemIcons			= new array<Object>;

	void VicinityContainer( LayoutHolder parent )
	{
		m_VicinityIconsContainer = new VicinitySlotsContainer( this );
		m_Body.Insert( m_VicinityIconsContainer );
		RecomputeOpenedContainers();
		Header h = Header.Cast( m_Body.Get(0) );
		h.SetName("VICINITY");
		LoadDefaultState();
	}
	
	bool IsVicinityContainerIconsActive()
	{
		return m_VicinityIconsContainer.IsActive();
	}
	
	VicinitySlotsContainer GetVicinityIconsContainer()
	{
		return m_VicinityIconsContainer;
	}
	
	override void RefreshItemPosition( EntityAI item_to_refresh )
	{
		for ( int i = 0; i < m_ShowedItems.Count(); i++ )
		{
			Container container = m_ShowedItems.GetElement( i );
			container.RefreshItemPosition( item_to_refresh );
		}
	}
	
	override void RefreshQuantity( EntityAI item_to_refresh )
	{
		for ( int i = 0; i < m_ShowedItems.Count(); i++)
		{
			m_ShowedItems.GetElement( i ).RefreshQuantity( item_to_refresh );
		}
	}
	
	bool IsContainerWithCargoActive()
	{
		return GetFocusedContainer().IsInherited( ContainerWithCargo );
	}
	
	bool IsItemWithAttachmentsActive()
	{
		return GetFocusedContainer().IsInherited( ContainerWithCargoAndAttachments );
	}
	
	override void SelectItem()
	{
		if( GetFocusedContainer().IsInherited( ContainerWithCargo ) || GetFocusedContainer().IsInherited( ContainerWithCargoAndAttachments ) )
		{
			ContainerWithCargo iwc = ContainerWithCargo.Cast( GetFocusedContainer() );
			ContainerWithCargoAndAttachments iwca = ContainerWithCargoAndAttachments.Cast( GetFocusedContainer() );
			if( iwc )
			{
				iwc.SelectItem();
			}
			else if ( iwca )
			{
				iwca.SelectItem();
			}
		}
		else
		{
			m_VicinityIconsContainer.SelectItem();
		}
	}

	override void Select()
	{
		if( GetFocusedContainer().IsInherited( ContainerWithCargo ) || GetFocusedContainer().IsInherited( ContainerWithCargoAndAttachments ) )
		{
			ContainerWithCargo iwc = ContainerWithCargo.Cast( GetFocusedContainer() );
			ContainerWithCargoAndAttachments iwca = ContainerWithCargoAndAttachments.Cast( GetFocusedContainer() );
			if( iwc )
			{
				iwc.Select();
			}
			else if ( iwca )
			{
				iwca.Select();
			}
		}
		else
		{
			m_VicinityIconsContainer.Select( );
		}
	}
	
	override void EquipItem( )
	{
		if( GetFocusedContainer().IsInherited( ContainerWithCargo ) )
		{
			ContainerWithCargo iwc = ContainerWithCargo.Cast( GetFocusedContainer() );
			if( iwc )
			{
				iwc.EquipItem();
			}
		}
		else
		{
			m_VicinityIconsContainer.EquipItem( );
		}
	}
	
	override void TransferItem( )
	{
		if( GetFocusedContainer().IsInherited( ContainerWithCargo ) )
		{
			ContainerWithCargo iwc = ContainerWithCargo.Cast( GetFocusedContainer() );
			if( iwc )
			{
				iwc.TransferItem();
			}
		}
		else
		{
			m_VicinityIconsContainer.TransferItem( );
		}
	}
	
	override void Combine( )
	{
		if( GetFocusedContainer().IsInherited( ContainerWithCargo ) )
		{
			ContainerWithCargo iwc = ContainerWithCargo.Cast( GetFocusedContainer() );
			if( iwc )
			{
				iwc.Combine();
			}
		}
		else
		{
			m_VicinityIconsContainer.Combine();
		}
	}

	override void MoveGridCursor( int direction )
	{
		if( !GetFocusedContainer() || !GetFocusedContainer().IsActive() )
		{
			return;
		}
		if( GetFocusedContainer() && (GetFocusedContainer().IsInherited( ContainerWithCargo ) || GetFocusedContainer().IsInherited( ContainerWithCargoAndAttachments )) )
		{
			ContainerWithCargo iwc = ContainerWithCargo.Cast( GetFocusedContainer() );
			ContainerWithCargoAndAttachments iwca = ContainerWithCargoAndAttachments.Cast( GetFocusedContainer() );
			if( iwc )
			{
				iwc.MoveGridCursor(direction);
			}
			else if ( iwca )
			{
				iwca.MoveGridCursor( direction );
			}
		}
		else
		{
			m_VicinityIconsContainer.MoveGridCursor( direction );
		}
	}


	/*bool IsActive()
	{
		for ( int i = 1; i < m_Body.Count(); i++ )
		{
			Container cont = m_Body[i];
			if( cont.IsActive() )
			{
				return true;
			}
		}
		return false;
	}

	void SetActive( bool active )
	{
		if( active )
		{
			m_VicinityIconsContainer.SetActive( true );
			m_ActiveIndex = 1;
		}
		else
		{
			for ( int i = 1; i < m_Body.Count(); i++ )
			{
				Container cont = m_Body[i];
				if( cont.IsActive() )
				{
					cont.SetActive( false );
					m_ActiveIndex = 1;
				}
			}
		}
	}*/

	/*int m_ActiveIndex = 1;
	void SetNextActive()
	{
		Container active = m_Body[m_ActiveIndex];
		if( !active.IsActive() )
		{
			return;
		}
		active.SetActive( false );
		Container next;
		if( ++m_ActiveIndex < m_Body.Count() )
		{
			next = m_Body[m_ActiveIndex];
		}
		while( next && !next.GetMainWidget().IsVisible() )
		{
			if( ++m_ActiveIndex < m_Body.Count() )
			{
				next = m_Body[m_ActiveIndex];
			}
		}
		if( next )
		{
			next.SetActive( true );
		}
		else
		{
			m_ActiveIndex = 1;
			Container first = m_Body[m_ActiveIndex];
			first.SetActive( true );
		}
	}

	void SetPreviousActive()
	{
		Container active = m_Body[m_ActiveIndex];
		active.SetActive( false );
		Container prev;
		if( --m_ActiveIndex > 0 )
		{
			prev = m_Body[m_ActiveIndex];
		}
		while( prev && !prev.GetMainWidget().IsVisible() )
		{
			if( --m_ActiveIndex > 0 )
			{
				prev = m_Body[m_ActiveIndex];
			}
		}
		if(prev)
		{
			prev.SetActive( true );
		}
		else
		{
			m_ActiveIndex = m_Body.Count() - 1;
			Container first = m_Body[m_ActiveIndex];
			first.SetActive( true );
		}
	}

	*/
	/*void LoadDefaultState()
	{
		m_Hidden = !ItemManager.GetInstance().GetDefaultHeaderOpenState( "VICINITY" );

		if( m_Hidden )
		{
			OnHide();
			Print(GetMainWidget().GetName());
			/*for ( int i = 1; i < m_Body.Count(); i++ )
			{
				m_Body.Get( i ).OnHide();
			}*/
	/*	}
		else
		{
			OnShow();
			/*for ( i = 1; i < m_Body.Count(); i++ )
			{
				m_Body.Get( i ).OnShow();
			}*/
		/*}

		//GetMainWidget().FindAnyWidget("opened").Show(!m_Hidden);
		//GetMainWidget().FindAnyWidget("closed").Show(m_Hidden);
	}*/

	void TraverseShowedItems()
	{
		map<string, bool> serialized_types = new map<string, bool>();
		for ( int i = 0; i < m_ShowedItems.Count(); i++ )
		{
			EntityAI entity = m_ShowedItems.GetKey( i );
			Container container = m_ShowedItems.GetElement( i );
			string type = entity.GetType();
			if( container.IsInherited( ContainerWithCargo ) )
			{
				if( !serialized_types.Contains( type ) )
				{
					ContainerWithCargo item_with_cargo = ContainerWithCargo.Cast( container );
					serialized_types.Insert( type, item_with_cargo.IsOpened() );
				}
			}
		}

		if( serialized_types.Count() > 0 )
		{
			FileSerializer file = new FileSerializer();
			if ( file.Open( "inventory_state.save", FileMode.APPEND ) )
			{
				file.Write( serialized_types.Count() );

				for ( i = 0; i < serialized_types.Count(); i++ )
				{
					file.Write( serialized_types.GetKey( i ) );
					file.Write( serialized_types.GetElement( i ) );
				}
			}
			file.Close();
		}
	}

	override void DraggingOverHeader( Widget w, int x, int y, Widget receiver )
	{
		if( w == NULL )
		{
			return;
		}
		
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		if( !ipw )
		{
			string name = w.GetName();
			name.Replace( "PanelWidget", "Render" );
			ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		}
		
		if( !ipw && ItemPreviewWidget.Cast( w ) )
		{
			ipw = ItemPreviewWidget.Cast( w );
		}
		else
		{
			return;
		}

		if( !ipw || !ipw.GetItem() )
		{
			return;
		}

		if( ipw.GetItem() && GetGame().GetPlayer().CanDropEntity( ipw.GetItem() ) )
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
			ItemManager.GetInstance().HideDropzones();
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
		}
		else
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
			ItemManager.GetInstance().ShowSourceDropzone( ipw.GetItem() );
		}
	}

	void DraggingOverIcon( Widget w, int x, int y, Widget receiver )
	{
		string name = receiver.GetName();
		name.Replace("PanelWidget", "Render");
		ItemPreviewWidget receiver_iw = ItemPreviewWidget.Cast( receiver.FindAnyWidget(name) );
		EntityAI receiver_item = receiver_iw.GetItem();

		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		if( !ipw )
		{
			name = w.GetName();
			name.Replace( "PanelWidget", "Render" );
			ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		}
		
		if( !ipw && ItemPreviewWidget.Cast( w ) )
		{
			ipw = ItemPreviewWidget.Cast( w );
		}
		else
		{
			return;
		}

		if( !ItemBase.Cast( receiver_item ) || !ipw.GetItem() )
		{
			return;
		}

		if ( ( ItemBase.Cast( receiver_item ) ).CanBeCombined( ItemBase.Cast( ipw.GetItem() ) ) )
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.COMBINE_COLOR );
			ItemManager.GetInstance().HideDropzones();
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
		}
		else if( GameInventory.CanSwapEntities( receiver_item, ipw.GetItem() ) )
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.SWAP_COLOR );
			ItemManager.GetInstance().HideDropzones();
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
		}
		else if( GetGame().GetPlayer().CanDropEntity( ipw.GetItem() ) )
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
			ItemManager.GetInstance().HideDropzones();
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).SetAlpha( 1 );
		}
		else
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
			ItemManager.GetInstance().ShowSourceDropzone( ipw.GetItem() );
		}
	}

	void OnDropReceivedFromIcon( Widget w, int x, int y, Widget receiver )
	{
		string name = receiver.GetName();
		name.Replace("PanelWidget", "Render");
		ItemPreviewWidget receiver_iw = ItemPreviewWidget.Cast( receiver.FindAnyWidget(name) );
		EntityAI receiver_item = receiver_iw.GetItem();

		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		if( !ipw )
		{
			name = w.GetName();
			name.Replace( "PanelWidget", "Render" );
			ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		}
		
		if( !ipw && ItemPreviewWidget.Cast( w ) )
		{
			ipw = ItemPreviewWidget.Cast( w );
		}
		else
		{
			return;
		}

		if( !ItemBase.Cast( receiver_item ) || !ipw.GetItem() )
		{
			return;
		}
		
		EntityAI item = ipw.GetItem();
		bool equal_typed = item.GetType() == receiver_item.GetType();
		if( !receiver_item.IsInherited( ItemBase ) || item == NULL || !equal_typed )
		{
			return;
		}
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );

		if ( ( ItemBase.Cast( receiver_item ) ).CanBeCombined( ItemBase.Cast( item ) ) )
		{
			( ItemBase.Cast( receiver_item ) ).CombineItemsClient( ItemBase.Cast( item ) );
		}
		else if( GameInventory.CanSwapEntities( receiver_item, item ) )
		{
			player.PredictiveSwapEntities( item, receiver_item );
		}
		else if( player.CanDropEntity( item ) )
		{
			player.PredictiveDropEntity( item );
		}
		
		ItemManager.GetInstance().SetIsDragging( false );
		ItemManager.GetInstance().PrepareTooltip( item );

		InventoryMenu menu = InventoryMenu.Cast( GetGame().GetUIManager().FindMenu( MENU_INVENTORY ) );
		if ( menu )
		{
			menu.RefreshQuickbar();
		}
	}
	
	void OnLeftPanelDropReceived( Widget w, int x, int y, Widget receiver )
	{
		OnDropReceivedFromHeader( w, x, y, receiver );
	}

	override void OnDropReceivedFromHeader( Widget w, int x, int y, Widget receiver )
	{
		if( !w )
			return;
		
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
		if( !ipw )
			return;
		
		EntityAI item = ipw.GetItem();
		if( !ipw.IsInherited( ItemPreviewWidget ) || !item )
		{
			return;
		}
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );

		if( player.CanDropEntity( item ) )
		{
			ItemBase item_base = ItemBase.Cast( item );
			if( item_base )
			{
				float stackable = item_base.ConfigGetFloat("varStackMax");
				if( stackable == 0 && item_base.GetQuantity() >= stackable )
				{
					player.PredictiveDropEntity( item );
				}
				else if( stackable != 0 && stackable <= item_base.GetQuantity() )
				{
					item_base.SplitIntoStackMaxClient( null, -1 );
				}
			}
		}

		InventoryMenu menu = InventoryMenu.Cast( GetGame().GetUIManager().FindMenu( MENU_INVENTORY ) );
		if ( menu )
		{
			menu.RefreshQuickbar();
		}
	}

	void OnDropReceivedFromEmptySpace( Widget w, int x, int y, Widget receiver )
	{
		OnDropReceivedFromHeader( w, x, y, receiver );
	}

	override void UpdateInterval()
	{
		// GetObjectsInVicinity
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		if(!player)
			return;
		
		vector pos = player.GetPosition();
		vector dir = player.GetDirection();
		ref array<Object> objects = new array<Object>;
		ref array<Object> objects_in_cone = new array<Object>;
		ref array<CargoBase> proxyCargos = new array<CargoBase>;
		GetGame().GetObjectsAtPosition3D( pos, 1.0, objects, proxyCargos );
		objects_in_cone = ActionTargets.GetVicinityObjects();

		for ( int i = 0; i < objects_in_cone.Count(); i++ )
		{
			Object object = objects_in_cone.Get( i );
			if( object && objects.Find( object ) == INDEX_NOT_FOUND )
			{
				objects.Insert( object );
			}
		}

		//GetItemsShowableInInventory
		array<Object> showable_items = new array<Object>;
		int m_OldShowedItemIconsCount = m_ShowedItemIcons.Count();
		m_ShowedItemIcons.Clear();
		
		for( i = 0; i < objects.Count(); i++ )
		{
			Object obj = objects.Get( i );
			bool showable_item = !objects.Get( i ).IsAnyInherited( { ScriptedEntity, Building, Camera, PlantSuper, PASReceiver, DayZAnimal, UndergroundStash, GardenBase } );
			if ( player.GetInventory().IsPlaceholderEntity(obj) )
				continue; // noproxy: ignore body placeholder
			if ( obj.GetParent() || ( EntityAI.Cast( obj ) && EntityAI.Cast( obj ).GetHierarchyParent() ) )
				continue; // noproxy: ignore owned items

			// Temporary solution for making GardenBase objects visible in vicinity
			//if (!showable_item )
				//showable_item = objects.Get( i ).IsAnyInherited( { GardenBase } );

			if( showable_item )
			{
				if( obj.IsInherited( EntityAI ) && obj != GetGame().GetPlayer() )
				{
					showable_items.Insert( obj );
				}

				if( obj.IsInherited( EntityAI ) && obj != GetGame().GetPlayer() && !obj.IsInherited( DayZInfected ) && !obj.IsInherited( PlayerBase ) )
				{
					m_ShowedItemIcons.Insert( obj );
				}
			}
		}

		if( m_Parent.m_Parent.GetMainWidget().IsVisible() && !m_Hidden )
		{
			ref map<EntityAI, ref Container> new_showed_items = new ref map<EntityAI, ref Container>;
			ref map<int, ref Container> showed_items_IDs = new ref map<int, ref Container>;
			for ( i = 0; i < showable_items.Count(); i++ )
			{
				EntityAI entity = EntityAI.Cast( showable_items.Get( i ) );
				if( entity )
				{
					if( !m_ShowedItems.Contains( entity ) && entity.GetInventory() )
					{
						string config = "CfgVehicles " + entity.GetType() + " GUIInventoryAttachmentsProps";

						if( GetGame().ConfigIsExisting( config ) )
						{
							AttachmentCategoriesContainer ac = new AttachmentCategoriesContainer( m_Parent );
							ac.SetEntity( entity );
							new_showed_items.Insert( entity, ac );
							showed_items_IDs.Insert( entity.GetID(), ac);
						}
						else if ( entity.GetSlotsCountCorrect() > 0 && entity.GetInventory().GetCargo() )
						{
							if( entity.IsInherited( DayZInfected ) )
							{
								if( entity.IsAlive() )
								{
									continue;
								}
								ZombieContainer zmb_cnt = new ZombieContainer( m_Parent );
								zmb_cnt.SetEntity( entity );
								new_showed_items.Insert( entity, zmb_cnt );
								showed_items_IDs.Insert( entity.GetID(), zmb_cnt );

							} else
							{
								ContainerWithCargoAndAttachments iwca = new ContainerWithCargoAndAttachments( this );
								iwca.SetEntity( entity );
								new_showed_items.Insert( entity, iwca );
								showed_items_IDs.Insert( entity.GetID(), iwca );
							}
						}
						else if( entity.GetInventory().GetCargo() )
						{
							{
								ContainerWithCargo iwc = new ContainerWithCargo( this );
								iwc.SetEntity( entity );
								new_showed_items.Insert( entity, iwc );
								showed_items_IDs.Insert( entity.GetID(), iwc );
								iwc.UpdateInterval();
							}
						}
						else if( entity.GetSlotsCountCorrect() > 0 )
						{
							if( entity.HasEnergyManager() )
							{
								ContainerWithElectricManager iwem = new ContainerWithElectricManager( this );
								iwem.SetEntity( entity );
								new_showed_items.Insert( entity, iwem );
								showed_items_IDs.Insert( entity.GetID(), iwem );
							}
							else
							{
								if ( entity.IsInherited( PlayerBase ) )
								{
									if( entity.IsAlive() && ( !PlayerBase.Cast( entity ).IsUnconscious() && !PlayerBase.Cast( entity ).IsRestrained() ) )
									{
										continue;
									}
									
									PlayerContainer plyr_cnt = new PlayerContainer( m_Parent );
									plyr_cnt.SetPlayer( PlayerBase.Cast( entity ) );
									( Container.Cast( m_Parent ) ).m_Body.Insert( plyr_cnt );
									( Container.Cast( m_Parent ) ).Refresh();
									new_showed_items.Insert( entity, plyr_cnt );
									showed_items_IDs.Insert( entity.GetID(), plyr_cnt);
								} else
								{
									ContainerWithCargoAndAttachments iwcas = new ContainerWithCargoAndAttachments( this );
									iwcas.SetEntity( entity );
									new_showed_items.Insert( entity, iwcas );
									showed_items_IDs.Insert( entity.GetID(), iwcas );
								}
							}
						}
					}
					else
					{
						if( m_ShowedItems.Get( entity ) )
						{
							if ( entity.IsInherited( PlayerBase ) )
							{
								if( entity.IsAlive() && ( !PlayerBase.Cast( entity ).IsUnconscious() && !PlayerBase.Cast( entity ).IsRestrained() ) )
								{
									continue;
								}
							}
							new_showed_items.Insert( entity, m_ShowedItems.Get( entity ) );
							showed_items_IDs.Insert( entity.GetID(), m_ShowedItemsIDs.Get( entity.GetID() ) );
						}
					}
				}
			}
			
			for ( i = 0; i < m_ShowedItems.Count(); i++ )
			{
				EntityAI ent = m_ShowedItems.GetKey( i );
				m_ShowedItems.GetElement( i ).UpdateInterval();
				if( !new_showed_items.Contains( ent ) )
				{
					Container con = m_ShowedItems.GetElement( i );
					( Container.Cast( con.m_Parent ) ).Remove( con );
					RecomputeOpenedContainers();
					GetMainWidget().Update();
				}
			}
			
			m_ShowedItems = new_showed_items;
			m_ShowedItemsIDs = showed_items_IDs;
			RecomputeOpenedContainers();
			m_VicinityIconsContainer.ShowItemsInContainers( m_ShowedItemIcons );
			
			if( m_ShowedItemIcons.Count() < m_OldShowedItemIconsCount )
			{
				Inventory in = Inventory.Cast( GetRoot() );
				if( in )
					in.UpdateConsoleToolbar();	
			}
		}
	}

	void ToggleContainer( Container conta )
	{
		Container cont = conta;

		if( cont && cont.IsInherited( ClosableContainer ) )
		{
			ClosableContainer c = ClosableContainer.Cast( cont );
			if( c.IsOpened() )
			{
				c.Close();
			}
			else
			{
				c.Open();
			}
		}
	}

	override void CollapseButtonOnMouseButtonDown(Widget w)
	{
		super.CollapseButtonOnMouseButtonDown(w);
		ItemManager.GetInstance().SetDefaultHeaderOpenState( "VICINITY", !m_Hidden );
	}
}
