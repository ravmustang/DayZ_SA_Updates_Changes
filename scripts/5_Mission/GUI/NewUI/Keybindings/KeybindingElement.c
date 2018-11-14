class KeybindingElement extends ScriptedWidgetEventHandler
{
	protected KeybindingsGroup				m_Group;
	
	protected Widget						m_Root;
	protected TextWidget					m_ElementName;
	protected TextWidget					m_ElementModifier;
	protected ButtonWidget					m_KBBindButton;
	protected ButtonWidget					m_ConsoleBindButton;
	
	protected int							m_ElementIndex;
	protected bool							m_IsEdited;
	protected bool							m_IsAlternateEdited;
	
	protected ref array<int>				m_CustomBind;
	protected ref array<int>				m_CustomAlternateBind;
	
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
	
	bool IsChanged()
	{
		return m_IsEdited;
	}
	
	bool IsAlternateChanged()
	{
		return m_IsAlternateEdited;
	}
	
	array<int> GetChangedBinds()
	{
		return m_CustomBind;
	}
	
	array<int> GetChangedAlternateBinds()
	{
		return m_CustomAlternateBind;
	}
	
	void Reload()
	{
		UAInput input			= GetUApi().GetInputByID( m_ElementIndex );
		m_IsEdited				= false;
		m_IsAlternateEdited		= false;
		m_CustomBind			= null;
		m_CustomAlternateBind	= null;
		
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
		string output;
		int i;
		
		GetGame().GetInput().GetActionDesc( m_ElementIndex, option_text );
		m_ElementName.SetText( option_text );
		
		if( input.AlternativeCount() > 0 )
		{
			input.SelectAlternative( 0 );
			if( input.IsCombo() )
			{
				if( input.BindingCount() > 0 )
				{
					output = GetUApi().GetButtonName( input.Binding( 0 ) );
				}
				for( i = 1; i < input.BindingCount(); i++ )
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
			
		if( input.AlternativeCount() > 1 )
		{
			input.SelectAlternative( 1 );
			if( input.IsCombo() )
			{
				if( input.BindingCount() > 0 )
				{
					output = GetUApi().GetButtonName( input.Binding( 0 ) );
				}
				for( i = 1; i < input.BindingCount(); i++ )
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
			m_ConsoleBindButton.SetText( output );
		}
	}
	
	void Reload( array<int> custom_binds, bool is_alternate )
	{
		string output;
		if( custom_binds.Count() > 1 )
		{
			output = GetUApi().GetButtonName( custom_binds.Get( 0 ) );
			for( int i = 1; i < custom_binds.Count(); i++ )
			{
				output += " + " + GetUApi().GetButtonName( custom_binds.Get( i ) );
			}
		}
		else if( custom_binds.Count() > 0 )
		{
			output = GetUApi().GetButtonName( custom_binds.Get( 0 ) );
		}
		
		if( is_alternate )
		{
			m_CustomAlternateBind = custom_binds;
			m_IsAlternateEdited = true;
			m_ConsoleBindButton.SetText( output );
		}
		else
		{
			m_CustomBind = custom_binds;
			m_IsEdited = true;
			m_KBBindButton.SetText( output );
		}
	}
	
	void StartEnteringKeybind()
	{
		m_Group.StartEnteringKeybind( m_ElementIndex );
		m_KBBindButton.SetText( "#layout_keybinding_new_keybind" );
	}
	
	void CancelEnteringKeybind()
	{
		Reload();
	}
	
	void StartEnteringAlternateKeybind()
	{
		m_Group.StartEnteringAlternateKeybind( m_ElementIndex );
		m_ConsoleBindButton.SetText( "#layout_keybinding_new_keybind" );
	}
	
	void CancelEnteringAlternateKeybind()
	{
		Reload();
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if( w == m_KBBindButton )
		{
			StartEnteringKeybind();
		}
		if( w == m_ConsoleBindButton )
		{
			StartEnteringAlternateKeybind();
		}
		return false;
	}
}