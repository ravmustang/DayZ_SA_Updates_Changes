class OptionSelectorSlider extends OptionSelectorBase
{
	protected SliderWidget			m_Slider;
	protected float					m_MinValue;
	protected float					m_MaxValue;
	
	void OptionSelectorSlider( Widget parent, float value, ScriptedWidgetEventHandler parent_menu, bool disabled, float min, float max )
	{
		m_Root					= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/option_slider.layout", parent );
		#ifdef PLATFORM_CONSOLE
			m_Parent				= parent.GetParent().GetParent();
		#else
		#ifdef PLATFORM_WINDOWS
			m_Parent				= parent.GetParent();
		#endif
		#endif
		
		m_SelectorType				= 1;
		m_ParentClass				= parent_menu;
		m_Slider					= SliderWidget.Cast( m_Root.FindAnyWidget( "option_value" ) );
		m_Slider.SetCurrent( value );
		//m_Parent.SetFlags( WidgetFlags.NOFOCUS );
		m_DisablePanel				= m_Parent.GetParent().FindAnyWidget( m_Parent.GetName() + "_disable" );
		
		m_MinValue					= min;
		m_MaxValue					= max;
		
		SetValue( value );
		
		m_Enabled = !disabled;
		if( m_Enabled )
		{
			Enable();
		}
		else
		{
			Disable();
		}
		
		m_Parent.SetHandler( this );
	}
	
	void ~OptionSelectorSlider()
	{
		delete m_Root;
	}
	
	override void Enable()
	{
		super.Enable();
		
		m_Slider.ClearFlags( WidgetFlags.IGNOREPOINTER );
	}
	
	override void Disable()
	{
		super.Disable();
		
		m_Slider.SetFlags( WidgetFlags.IGNOREPOINTER );
	}
	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		if( button == MouseState.LEFT && w == m_Slider )
		{
			Print( m_Slider.GetCurrent() );
		}
		return false;
	}
	
	override bool OnChange( Widget w, int x, int y, bool finished )
	{
		if( w == m_Slider )
		{
			m_OptionChanged.Invoke( GetValue() );
			return true;
		}
		return false;
	}
	
	override bool IsFocusable( Widget w )
	{
		return ( w == m_Parent || w == m_Slider );
	}
	
	float NormalizeInput( float value )
	{
		float ret = ( value - m_MinValue) / ( m_MaxValue - m_MinValue );
		return ret;
	}
	
	void SetValue( float value, bool update = true )
	{
		m_Slider.SetCurrent( NormalizeInput( value ) );
		if( update )
			m_OptionChanged.Invoke( GetValue() );
	}
	
	float GetValue()
	{
		float ret = ( m_Slider.GetCurrent() * ( m_MaxValue - m_MinValue ) ) + m_MinValue;
		return ret;
	}
	
	//Coloring functions (Until WidgetStyles are useful)
	override void Darken( Widget w, int x, int y )
	{
		if( !m_Enabled )
			return;
		
		if( w.GetParent() == m_Root )
		{
			m_Parent.SetAlpha( 1 );
		}
		
		if( GetFocus() != m_Slider )
		{
			SetFocus( m_Slider );
			return;
		}
		
		m_Parent.SetFlags( WidgetFlags.NOFOCUS );
		
		m_Parent.SetAlpha( 1 );
		
		TextWidget text_label;
		if( w.GetParent().GetParent() == m_Root )
		{
			text_label = TextWidget.Cast( m_Parent.FindAnyWidget( m_Parent.GetName() + "_label" ) );
			if( text_label )
				text_label.SetColor( ARGB( 255, 255, 0, 0 ) );
			m_Slider.SetColor( ARGB( 255, 255, 0, 0 ) );
			return;
		}
		m_Slider.SetColor( ARGB( 255, 255, 0, 0 ) );
		
		text_label = TextWidget.Cast( w.FindAnyWidget( w.GetName() + "_label" ) );
		if( text_label )
			text_label.SetColor( ARGB( 255, 255, 0, 0 ) );
	}
	
	override void Lighten( Widget w, Widget enterW, int x, int y )
	{
		if( !m_Enabled )
			return;
		
		super.Lighten( w, enterW, x, y );
		
		if( IsFocusable( enterW ) )
		{
			return;
		}
		
		m_Parent.ClearFlags( WidgetFlags.NOFOCUS );
		
		TextWidget text_label;
		if( w.GetParent().GetParent() == m_Root )
		{
			text_label = TextWidget.Cast( m_Parent.FindAnyWidget( m_Parent.GetName() + "_label" ) );
			if( text_label )
				text_label.SetColor( ARGB( 255, 255, 255, 255 ) );
			m_Slider.SetColor( ARGB( 255, 255, 255, 255 ) );
			return;
		}
			
		m_Slider.SetColor( ARGB( 255, 255, 255, 255 ) );
		
		text_label = TextWidget.Cast( w.FindAnyWidget( w.GetName() + "_label" ) );
		if( text_label )
			text_label.SetColor( ARGB( 255, 255, 255, 255 ) );
	}
}