class ModsMenuSimple extends ScriptedWidgetEventHandler
{
	protected Widget										m_Root;
	protected Widget										m_MoreButton;
	protected ref map<ModInfo, ref ModsMenuSimpleEntry>		m_Data;
	protected ModsMenuDetailed								m_DetailMenu;
	
	void ModsMenuSimple(array<ref ModInfo> data, Widget parent, ModsMenuDetailed detail_menu)
	{
		m_Root = GetGame().GetWorkspace().CreateWidgets("gui/layouts/new_ui/mods_menu/mods_menu_simple.layout", parent);
		m_MoreButton = m_Root.FindAnyWidget("ModMore");
		m_Data = new map<ModInfo, ref ModsMenuSimpleEntry>;
		m_DetailMenu = detail_menu;
		
		m_Root.SetHandler(this);
		LoadEntries(data);
	}
	
	void ~ModsMenuSimple()
	{
		delete m_Root;
	}

	void LoadEntries(array<ref ModInfo> data)
	{
		int count;
		if( data.Count() > 5 )
		{
			count = 4;
			m_Root.FindAnyWidget("ModMore").Show(true);
		}
		else
		{
			count = data.Count();
			m_Root.FindAnyWidget("ModMore").Show(false);
		}
		
		for (int i = count - 1; i >= 0; i--)
		{
			ref ModsMenuSimpleEntry entry = new ModsMenuSimpleEntry(data.Get(i), m_Root, this);
			m_Data.Insert(data.Get(i), entry);
		}
	}
	
	void Select( ModInfo mod )
	{
		m_DetailMenu.Open();
		m_DetailMenu.Highlight( mod );
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if( w == m_MoreButton )
		{
			if( m_DetailMenu.IsOpen() )
				m_DetailMenu.Close();
			else
				m_DetailMenu.Open();
			return true;
		}
		return false;
	}
}