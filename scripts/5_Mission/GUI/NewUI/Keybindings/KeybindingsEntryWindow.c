class KeybindingsEntryWindow extends ScriptedWidgetEventHandler
{
	protected Widget									m_Root;
	protected Widget									m_ButtonGrid;
	protected TextWidget								m_Caption;
	protected TextWidget								m_KeybindEntry;
	protected TextListboxWidget							m_KeybindList;
	protected ButtonWidget								m_Confirm;
	protected ButtonWidget								m_Add;
	protected ButtonWidget								m_Delete;
	protected ButtonWidget								m_Cancel;
	
	protected KeybindingsMenu							m_Menu;
	protected int										m_ElementIndex;
	
	protected int										m_CurrentEnteredKeyIndex = -1;
	protected int										m_CurrentEnteredSpecialKeyIndex = -1;
	protected bool										m_Listening;
	
	protected ref Timer									m_CloseTimer;
	
	void KeybindingsEntryWindow( int index, Widget parent, KeybindingsMenu menu )
	{
		m_Menu			= menu;
		m_ElementIndex	= index;
		
		m_Root			= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/options/keybindings_selectors/keybinding_enter_window.layout", parent );
		m_ButtonGrid	= m_Root.FindAnyWidget( "ButtonGrid" );
		m_Caption		= TextWidget.Cast( m_Root.FindAnyWidget( "Caption" ) );
		m_KeybindEntry	= TextWidget.Cast( m_Root.FindAnyWidget( "KeybindEntry" ) );
		m_KeybindList	= TextListboxWidget.Cast( m_Root.FindAnyWidget( "KeybindList" ) );
		m_Confirm		= ButtonWidget.Cast( m_Root.FindAnyWidget( "Confirm" ) );
		m_Add			= ButtonWidget.Cast( m_Root.FindAnyWidget( "Add" ) );
		m_Delete		= ButtonWidget.Cast( m_Root.FindAnyWidget( "Delete" ) );
		m_Cancel		= ButtonWidget.Cast( m_Root.FindAnyWidget( "Cancel" ) );
		
		m_CloseTimer	= new Timer( CALL_CATEGORY_GUI );
		
		LoadKeybinds();
		
		m_Root.SetHandler( this );
	}
	
	void ~KeybindingsEntryWindow()
	{
		delete m_Root;
	}
	
	void LoadKeybinds()
	{
		UAInput input	= GetUApi().GetInputByID( m_ElementIndex );
		string option_text;
		string name;
		
		GetGame().GetInput().GetActionDesc( m_ElementIndex, option_text );
		m_Caption.SetText( option_text );
		
		string output;
		array<int> binds = GetBindings( input );
		if( GetBindingText( binds, output ) )
		{
			m_KeybindList.AddItem( output, binds, 0, 1 );
		}
		else
		{
			m_Add.Show( true );
		}
	}
	
	array<int> GetBindings( UAInput input )
	{
		ref array<int> binds = new array<int>;
		if( input.BindingCount() > 0 )
		{
			for( int i = 0; i < input.BindingCount(); i++ )
			{
				binds.Insert( input.Binding( i ) );
			}
		}
		return binds;
	}
	
	bool GetBindingText( array<int> binds, out string text )
	{
		if( binds.Count() > 1 )
		{
			text = GetUApi().GetButtonName( binds.Get( 0 ) );
			for( int i = 1; i < binds.Count(); i++ )
			{
				text += " + " + GetUApi().GetButtonName( binds.Get( i ) );
			}
			return true;
		}
		else if( binds.Count() > 0 )
		{
			text = GetUApi().GetButtonName( binds.Get( 0 ) );
			return true;
		}
		return false;
	}
	
	bool IsListening()
	{
		return m_Listening;
	}
	
	void Confirm()
	{
		ref array<int> binds = new array<int>;
		for( int i = 0; i < m_KeybindList.GetNumItems(); i++ )
		{
			array<int> binds2 = new array<int>;
			m_KeybindList.GetItemData( i, 0, binds2 );
			binds.InsertAll( binds2 );
		}
		m_Menu.ConfirmKeybindEntry( binds );
	}
	
	void ConfirmSet()
	{
		string name;
		GetGame().GetInput().GetActionKeyName( m_CurrentEnteredKeyIndex, name );
		m_KeybindList.SetItem( 0, name, GetBindings( GetUApi().GetInputByID( m_CurrentEnteredKeyIndex ) ), 0 );
		
		string option_text;
		GetGame().GetInput().GetActionDesc( m_ElementIndex, option_text );
		m_Caption.SetText( option_text );
		
		m_CurrentEnteredKeyIndex		= -1;
		m_CurrentEnteredSpecialKeyIndex	= -1;
		
		m_Add.Show( true );
		m_ButtonGrid.Show( true );
		m_KeybindList.Show( true );
		m_KeybindEntry.Show( false );
	}
	
	void Add()
	{
		m_Listening = true;
		m_ButtonGrid.Show( false );
		m_KeybindList.Show( false );
		m_KeybindEntry.Show( true );
		
		m_Caption.SetText( "#layout_keybinding_new_keybind" );
	}
	
	void Delete()
	{
		int index = m_KeybindList.GetSelectedRow();
		if( index > -1 )
		{
			m_KeybindList.RemoveRow( index );
			m_KeybindList.SelectRow( -1 );
			m_Delete.Show( false );
		}
		
		m_Caption.SetText( "#layout_keybinding_new_keybind" );
	}
	
	void Cancel()
	{
		if( m_Listening )
		{
			m_ButtonGrid.Show( true );
			m_KeybindList.Show( true );
			m_KeybindEntry.Show( false );
			m_Listening = false;
			
			string option_text;
			GetGame().GetInput().GetActionDesc( m_ElementIndex, option_text );
			m_Caption.SetText( option_text );
		}
		else
		{
			m_Menu.CancelKeybindEntry();
		}
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if( button == MouseState.LEFT )
		{
			if( w == m_Confirm )
			{
				Confirm();
				return true;
			}
			else if( w == m_Add )
			{
				Add();
				return true;
			}
			else if( w == m_Delete )
			{
				Add();
				return true;
			}
			else if( w == m_Cancel )
			{
				Cancel();
				return true;
			}
		}
		return false;
	}
	
	override bool OnMouseButtonDown( Widget w, int x, int y, int button )
	{
		if( w == m_KeybindList )
		{
			if( m_KeybindList.GetSelectedRow() > -1 )
			{
				m_Delete.Show( true );
			}
			else
			{
				m_Delete.Show( false );
			}
		}
		else if( !ButtonWidget.Cast( w ) )
		{
			m_KeybindList.SelectRow( -1 );
			m_Delete.Show( false );
		}
		return false;
	}
	
	void Update( float timeslice )
	{
		UAInputAPI ua_api = GetUApi();
		string name;
		string text;
		
		if( m_CurrentEnteredKeyIndex > -1 )
		{
			GetGame().GetInput().GetActionKeyName( m_CurrentEnteredKeyIndex, name );
			text += name;
			m_Caption.SetText( "#layout_keybinding_keybind_set" );
			m_CloseTimer.Run( 2, this, "ConfirmSet" );
			m_Listening = false;
		}
		
		m_KeybindEntry.SetText( text );
	}
	
	override bool OnKeyDown( Widget w, int x, int y, int key )
	{
		
		return false;
	}
	
	override bool OnKeyUp( Widget w, int x, int y, int key )
	{
		
		return false;
	}
}