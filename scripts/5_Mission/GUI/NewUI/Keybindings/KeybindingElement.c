class KeybindingElement extends ScriptedWidgetEventHandler
{
	protected KeybindingsGroup				m_Group;
	
	protected Widget						m_Root;
	protected TextWidget					m_ElementName;
	protected TextWidget					m_ElementModifier;
	protected ButtonWidget					m_KBBindButton;
	protected ButtonWidget					m_ConsoleBindButton;
	
	protected int							m_ElementIndex;
	
	void KeybindingElement( int key_index, Widget parent, KeybindingsGroup group )
	{
		m_Root				= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/options/keybindings_selectors/keybinding_option.layout", parent );
		m_ElementName		= TextWidget.Cast( m_Root.FindAnyWidget( "setting_label" ) );
		m_ElementModifier	= TextWidget.Cast( m_Root.FindAnyWidget( "modifier_label" ) );
		m_KBBindButton		= ButtonWidget.Cast( m_Root.FindAnyWidget( "kb_bind" ) );
		m_ConsoleBindButton	= ButtonWidget.Cast( m_Root.FindAnyWidget( "console_bind" ) );
		
		m_Group				= group;
		m_ElementIndex		= key_index;
		
		Reload();
		m_Root.SetHandler( this );
	}
	
	void Reload()
	{
		UAInput input	= GetUApi().GetInputByID( m_ElementIndex );
		
		if( input.IsLimited() )
		{
			if( input.IsPressLimit() )
			{
				m_ElementModifier.SetText( "#keybind_press" );
			}
			if( input.IsReleaseLimit() )
			{
				m_ElementModifier.SetText( "#keybind_release" );
			}
			if( input.IsHoldLimit() )
			{
				m_ElementModifier.SetText( "#keybind_hold" );
			}
			if( input.IsDoubleClickLimit() )
			{
				m_ElementModifier.SetText( "#keybind_doubletap" );
			}
		}
		else
		{
			m_ElementModifier.SetText( "" );
		}
		
		string option_text;
		string name;
		
		GetGame().GetInput().GetActionDesc( m_ElementIndex, option_text );
		m_ElementName.SetText( option_text );
		string output;
		if( input.IsCombo() )
		{
			if( input.BindingCount() > 0 )
			{
				output = GetUApi().GetButtonName( input.Binding( 0 ) );
			}
			for( int i = 1; i < input.BindingCount(); i++ )
			{
				output += " + " + GetUApi().GetButtonName( input.Binding( i ) );
			}
		}
		else
		{
			
			if( input.BindingCount() > 0 )
			{
				output = GetUApi().GetButtonName( input.Binding( 0 ) );
			}
		}
		m_KBBindButton.SetText( output );
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if( w == m_KBBindButton || w == m_ConsoleBindButton )
		{
			m_Group.ShowKeybiningInputWindow( m_ElementIndex );
		}
		return false;
	}
}