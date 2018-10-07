class OptionSelectorMultistate extends OptionSelector
{
	void OptionSelectorMultistate( Widget parent, int current_index, ScriptedWidgetEventHandler parent_c, bool disabled, notnull array<string> options )
	{
		m_SelectorType				= 2;
		m_Options					= options;
		if( options.Count() == 0 )
		{
			Error( "Invalid ServerBrowserEntry options" );
		}
		
		if( current_index < 0 || current_index >= m_Options.Count() )
		{
			m_SelectedOptionIndex = 0;
		}
		else
		{
			m_SelectedOptionIndex = current_index;
		}
		
		m_Enabled = !disabled;
		if( m_Enabled )
		{
			Enable();
		}
		else
		{
			Disable();
		}
		
		m_SelectedOption.SetText( m_Options.Get( m_SelectedOptionIndex ) );
	}
	
	void LoadNewValues( notnull array<string> options, int current_index )
	{
		m_Options = options;
		SetValue( current_index );
	}
	
	override void SetNextOption()
	{
		m_SelectedOptionIndex++;
		if( m_SelectedOptionIndex >= m_Options.Count() )
		{
			m_SelectedOptionIndex = 0;
		}
		
		m_SelectedOption.SetText( m_Options.Get( m_SelectedOptionIndex ) );
		m_OptionChanged.Invoke( m_SelectedOptionIndex );
	}
	
	override void SetPrevOption()
	{
		m_SelectedOptionIndex--;
		if( m_SelectedOptionIndex < 0 )
		{
			m_SelectedOptionIndex = m_Options.Count() - 1;
		}
		
		m_SelectedOption.SetText( m_Options.Get( m_SelectedOptionIndex ) );
		m_OptionChanged.Invoke( m_SelectedOptionIndex );
	}
	
	void SetValue( int value, bool fire_event = true )
	{
		if( value < m_Options.Count() && value >= 0 )
		{
			m_SelectedOptionIndex = value;
			m_SelectedOption.SetText( m_Options.Get( m_SelectedOptionIndex ) );
			if( fire_event )
				m_OptionChanged.Invoke( m_SelectedOptionIndex );
		}
	}
	
	void SetValue( string value, bool fire_event = true )
	{
		int index = m_Options.Find( value );
		if( index >= 0 )
		{
			m_SelectedOptionIndex = index;
			m_SelectedOption.SetText( m_Options.Get( m_SelectedOptionIndex ) );
			if( fire_event )
				m_OptionChanged.Invoke( m_SelectedOptionIndex );
		}
	}
	
	void SetRandomValue()
	{
		m_SelectedOptionIndex = Math.RandomInt( 0, m_Options.Count() );
		m_SelectedOption.SetText( m_Options.Get( m_SelectedOptionIndex ) );
		m_OptionChanged.Invoke( m_SelectedOptionIndex );
	}
	
	int GetValue()
	{
		return m_SelectedOptionIndex;
	}
	
	override void SetStringOption( string option, bool fire_event = true )
	{
		int index = m_Options.Find( option );
		if( index > -1 )
		{
			m_SelectedOptionIndex = index;
			m_SelectedOption.SetText( m_Options.Get( m_SelectedOptionIndex ) );
			
			if( fire_event )
				m_OptionChanged.Invoke( m_SelectedOptionIndex );
		}
	}
}