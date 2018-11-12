class KeybindingsGroup extends ScriptedWidgetEventHandler
{
	protected Widget									m_Root;
	protected KeybindingsMenu							m_Menu;
	
	protected ref map<int, ref KeybindingElement>		m_KeyWidgets;
	
	protected ref DropdownPrefab						m_KBDropdown;
	protected ref DropdownPrefab						m_ConsoleDropdown;
	
	void KeybindingsGroup( int index, Input input, Widget parent, KeybindingsMenu menu )
	{
		m_KeyWidgets	= new map<int, ref KeybindingElement>;
		m_Menu			= menu;
		
		string group_name;
		input.GetActionGroupName( index, group_name );
		
		m_Root	= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/options/keybindings_selectors/keybinding_group.layout", parent );
		Widget subgroup	= m_Root.FindAnyWidget( "group_root" );
		
		for( int i = 0; i < 1; i++ )
		{
			AddSubgroup( index, subgroup, input );
		}
		
		InitPresets( index, m_Root.FindAnyWidget( "group_header" ), input );
		
		subgroup.Update();
		
		m_Root.SetHandler( this );
	}
	
	void InitPresets( int index, Widget parent, Input input )
	{
		Widget kb_root = parent.FindAnyWidget( "keyboard_dropown" );
		Widget console_root = parent.FindAnyWidget( "controller_dropown" );
		
		string profile_text;
		input.GetProfileName( input.GetCurrentProfile(), profile_text );
		
		m_KBDropdown		= new DropdownPrefab( kb_root, profile_text );
		m_ConsoleDropdown	= new DropdownPrefab( console_root, "" );
		
		m_KBDropdown.m_OnSelectItem.Insert( OnSelectKBPreset );
		m_ConsoleDropdown.m_OnSelectItem.Insert( OnSelectConsolePreset );
		
		for( int i = 0; i < input.GetProfilesCount(); i++ )
		{
			input.GetProfileName( i, profile_text );
			m_KBDropdown.AddElement( profile_text );
		}
	}
	
	void OnSelectKBPreset( int index )
	{
		string profile_text;
		GetGame().GetInput().GetProfileName( index, profile_text );
		m_KBDropdown.SetText( profile_text );
		GetGame().GetInput().SetProfile( index );
		ReloadProfiles();
		m_KBDropdown.Close();
	}
	
	void OnSelectConsolePreset( int index )
	{
		string profile_text;
		GetGame().GetInput().GetProfileName( index, profile_text );
		m_ConsoleDropdown.SetText( profile_text );
		GetGame().GetInput().SetProfile( index );
		ReloadProfiles();
	}
	
	void ReloadProfiles()
	{
		foreach( int index,  KeybindingElement element : m_KeyWidgets )
		{
			element.Reload();
		}
	}
	
	void AddSubgroup( int index, Widget parent, Input input )
	{
		Widget subgroup				= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/options/keybindings_selectors/keybinding_subgroup.layout", parent );
		TextWidget subgroup_name	= TextWidget.Cast( subgroup.FindAnyWidget( "subgroup_text" ) );
		
		subgroup_name.SetText( "TestSubgroup" );
		Widget subgroup_content = subgroup.FindAnyWidget( "subgroup_content" );
		
		TIntArray actions = new TIntArray;
		input.GetActionGroupItems( index, actions );
		for( int i = 0; i < actions.Count(); i++ )
		{
			AddAction( actions.Get( i ), subgroup_content, input );
		}
		
		subgroup_content.Update();
	}
	
	void AddAction( int index, Widget parent, Input input )
	{
		m_KeyWidgets.Insert( index, new KeybindingElement( index, parent, this ) );
	}
	
	void ReloadAction( int index )
	{
		if( m_KeyWidgets.Contains( index ) )
		{
			m_KeyWidgets.Get( index ).Reload();
		}
	}
	
	void ShowKeybiningInputWindow( int key_index )
	{
		m_Menu.ShowKeybiningInputWindow( key_index );
	}
	
	override bool OnMouseButtonDown( Widget w, int x, int y, int button )
	{
		if( !ButtonWidget.Cast( w ) )
		{
			m_KBDropdown.Close();
			m_ConsoleDropdown.Close();
		}
		return false;
	}
}