class AircraftKeybindings extends ScriptedWidgetEventHandler
{
	protected Widget						m_Root;
	
	protected KeybindingsMenu				m_Menu;
	
	void AircraftKeybindings( Widget parent, KeybindingsMenu menu )
	{
		m_Root		= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/options/keybinding_tabs/aircraft_keybindings.layout", parent );
	}
}