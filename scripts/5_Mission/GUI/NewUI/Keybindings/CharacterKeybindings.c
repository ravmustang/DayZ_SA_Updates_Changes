class CharacterKeybindings extends ScriptedWidgetEventHandler
{
	protected Widget						m_Root;
	
	protected KeybindingsMenu				m_Menu;
	
	void CharacterKeybindings( Widget parent, KeybindingsMenu menu )
	{
		m_Root		= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/options/keybinding_tabs/character_keybindings.layout", parent );
	}
}