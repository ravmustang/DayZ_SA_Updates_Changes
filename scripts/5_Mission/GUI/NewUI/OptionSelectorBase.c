class OptionSelectorBase extends ScriptedWidgetEventHandler
{
	protected int							m_SelectorType = 0;
	protected Widget						m_Parent;
	protected Widget						m_Root;
	protected Widget						m_DisablePanel;
	
	protected bool							m_Enabled;
	
	protected ScriptedWidgetEventHandler	m_ParentClass;
	
	ref ScriptInvoker						m_OptionFocused = new ScriptInvoker;
	ref ScriptInvoker						m_OptionUnfocused = new ScriptInvoker;
	ref ScriptInvoker						m_OptionChanged = new ScriptInvoker;
	
	void ~OptionSelectorBase()
	{
		delete m_Root;
	}
	
	Widget GetParent()
	{
		return m_Parent;
	}
	
	bool IsFocusable( Widget w )
	{
		return w == m_Parent;
	}
	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			Darken( w, x, y );
			if( m_ParentClass )
				m_ParentClass.OnFocus( m_Root.GetParent(), -1, y );
			return true;
		}
		return false;
	}
	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		if( m_ParentClass )
			m_ParentClass.OnFocus( null, x, y );
		if( IsFocusable( w ) )
		{
			Lighten( w, enterW, x, y );
			
			return true;
		}
		return false;
	}
	
	override bool OnFocus( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			Darken( w, x, y );
			if( m_ParentClass )
				m_ParentClass.OnFocus( m_Root.GetParent(), -1, y );
			return true;
		}
		return false;
	}
	
	override bool OnFocusLost( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			Lighten( w, null, x, y );
			if( m_ParentClass )
				m_ParentClass.OnFocus( null, x, y );
			return true;
		}
		return false;
	}
	
	void Focus()
	{
		#ifdef PLATFORM_CONSOLE
			SetFocus( m_Parent );
		#else
		#ifdef PLATFORM_WINDOWS
			SetFocus( m_Root );
		#endif
		#endif
	}
	
	void Enable()
	{
		m_Enabled = true;
		
		m_Parent.ClearFlags( WidgetFlags.IGNOREPOINTER );
		
		m_DisablePanel.Show( false );
	}
	
	void Disable()
	{
		m_Enabled = false;
		
		m_Parent.SetFlags( WidgetFlags.IGNOREPOINTER );

		m_DisablePanel.Show( true );
	}
	
	//Coloring functions (Until WidgetStyles are useful)
	void Darken( Widget w, int x, int y )
	{
		if( !m_Enabled )
			return;
		if( w.GetParent() == m_Root )
		{
			m_Parent.SetAlpha( 1 );
		}
		
		#ifdef PLATFORM_CONSOLE
			SetFocus( m_Parent );
		#else
			SetFocus( m_Root );
		#endif
		m_Parent.SetAlpha( 1 );
	}
	
	void Lighten( Widget w, Widget enterW, int x, int y )
	{
		if( !m_Enabled )
			return;
		
		if( enterW && enterW.GetParent() == m_Root )
		{
			return;
		}

		if( w.GetParent() == m_Root )
		{
			m_Parent.SetAlpha( 0 );
			return;
		}
			
		m_Parent.SetAlpha( 0 );
	}
}