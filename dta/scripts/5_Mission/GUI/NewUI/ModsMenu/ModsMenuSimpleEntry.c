class ModsMenuSimpleEntry extends ScriptedWidgetEventHandler
{
	protected ImageWidget		m_Icon;
	protected ImageWidget		m_Hover;

	protected bool				m_HasLogoOver;
	protected ModInfo			m_Data;
	protected ModsMenuSimple	m_ParentMenu;

	void ModsMenuSimpleEntry(ModInfo data, Widget parent, ModsMenuSimple parent_menu)
	{
		m_Icon			= ImageWidget.Cast(GetGame().GetWorkspace().CreateWidgets("gui/layouts/new_ui/mods_menu/mods_menu_simple_entry.layout", parent));
		m_Hover			= m_Icon.FindAnyWidget("Overlay");
		m_Data			= data;
		m_ParentMenu	= parent_menu;
		
		m_Icon.SetHandler(this);

		LoadData();
	}
	
	void ~ModsMenuSimpleEntry()
	{
		delete m_Icon;
	}

	void LoadData()
	{
		string logo = m_Data.GetLogo();
		string logo_over = m_Data.GetLogoOver();
		
		if (logo != "")
		{
			m_Icon.LoadImageFile(0, logo);
		}
		else
		{
			//Load default image
		}
		
		if (logo_over != "")
		{
			m_HasLogoOver = true;
			m_Icon.LoadImageFile(1, logo_over);
		}
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if( w == m_Icon )
		{
			m_ParentMenu.Select( m_Data );
			return true;
		}
		return false;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if( w == m_Icon )
		{
			if( m_HasLogoOver )
				m_Icon.SetImage( 1 );
			m_Hover.Show( true );
			return true;
		}
		return false;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if( enterW != m_Icon )
		{
			if( m_HasLogoOver )
				m_Icon.SetImage( 0 );
			m_Hover.Show( false );
			return true;
		}
		return false;
	}
}