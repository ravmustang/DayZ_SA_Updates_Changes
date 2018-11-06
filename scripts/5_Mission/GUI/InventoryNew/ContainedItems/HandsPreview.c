class HandsPreview: LayoutHolder
{
	ref Icon m_Icon;
	protected EntityAI m_Item_has_attachments_initialized;


	private EntityAI m_Item

	void HandsPreview( LayoutHolder parent )
	{

	}

	void RefreshQuantity( EntityAI m_Item_to_refresh )
	{
		if( m_Icon )
		{
			m_Icon.RefreshQuantity();
		}
	}

	Icon GetIcon()
	{
		return m_Icon;
	}

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.HandsPreview;
	}

	void RemoveItem()
	{
		delete m_Icon;
	}

	override void UpdateInterval()
	{
		//TODO: create layout for HandsPreview and set ARGB there
		m_ParentWidget.SetColor( ARGB( 230, 20, 20, 20 ) );
		bool show_combine_swap = ItemManager.GetInstance().IsDragging();

		#ifndef PLATFORM_CONSOLE
		if( m_Icon && !m_Icon.IsDragged() )
		{
			m_Icon.GetMainWidget().FindAnyWidget( "Combine" ).Show(  show_combine_swap );
			m_Icon.GetMainWidget().FindAnyWidget( "Swap" ).Show( show_combine_swap );
		}
		#endif

		PlayerBase p = PlayerBase.Cast( GetGame().GetPlayer() );
		if( !p )
			return;
		m_Item = p.GetHumanInventory().GetEntityInHands();

		if ( ( !m_Item && m_Icon ) || ( m_Icon && m_Item != m_Icon.GetObject() ) )
		{
			RemoveItem();
			( HandsContainer.Cast( m_Parent ) ).DestroyAtt();
			( HandsContainer.Cast( m_Parent ) ).DestroyCargo();
			m_Item_has_attachments_initialized = NULL;
		}
		else
		if( m_Item && !m_Icon )
		{
			CreateNewIcon();
		}
		else if ( m_Icon )
		{
			m_Icon.Refresh();
			m_Icon.FullScreen();
		}

		HandsContainer parent = HandsContainer.Cast( m_Parent );
		Header header = parent.m_CollapsibleHeader;

		if( m_Item )
		{
			string display_name = m_Item.GetDisplayName();
			display_name.ToUpper();
			header.SetName( display_name );
		}
		else
		{
			header.SetName( "HANDS EMPTY" );
		}
	}

	protected void CreateNewIcon()
	{
		m_Icon = new Icon( this, true );
		m_Icon.Refresh();
		if( m_Icon )
		{
			Weapon_Base wpn;
			if ( Class.CastTo(wpn,  m_Item ) )
			{
				int mi = wpn.GetCurrentMuzzle();
				m_Icon.GetMainWidget().FindAnyWidget( "AmmoIcon" ).Show(  wpn.IsChamberFull( mi )  );
			}

			m_Icon.Init( m_Item );
			m_Icon.FullScreen();
			( HandsContainer.Cast( m_Parent ) ).Reselect();
			if( m_Item != m_Item_has_attachments_initialized )
			{
				( HandsContainer.Cast( m_Parent ) ).DestroyAtt();
				( HandsContainer.Cast( m_Parent ) ).DestroyCargo();
				( HandsContainer.Cast( m_Parent ) ).ShowAtt( m_Item );
				m_Item_has_attachments_initialized = m_Item;
			}
		}
	}
}
