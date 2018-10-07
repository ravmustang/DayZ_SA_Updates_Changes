class VehicleKeybindings extends ScriptedWidgetEventHandler
{
	protected Widget						m_Root;
	
	protected KeybindingsMenu				m_Menu;
	
	void VehicleKeybindings( Widget parent, KeybindingsMenu menu )
	{
		m_Root		= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/options/keybinding_tabs/vehicle_keybindings.layout", parent );
	}
}