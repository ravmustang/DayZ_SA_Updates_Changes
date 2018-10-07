class GeneralKeybindings extends ScriptedWidgetEventHandler
{
	protected Widget						m_Root;
	
	protected KeybindingsMenu				m_Menu;
	
	void GeneralKeybindings( Widget parent, KeybindingsMenu menu )
	{
		m_Root		= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/options/keybinding_tabs/general_keybindings.layout", parent );
	}
}