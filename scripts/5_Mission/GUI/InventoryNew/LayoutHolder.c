class LayoutHolder extends ScriptedWidgetEventHandler
{
	protected Widget				m_MainWidget;
	protected Widget				m_ParentWidget;
	protected LayoutHolder			m_Parent;
	protected string				m_LayoutName;
	
	protected WidgetCache			m_Cache;
	protected WidgetCacheObject		m_CacheObject;
	
	protected bool					m_IsActive;

	void LayoutHolder( LayoutHolder parent )
	{		
		m_Parent = parent;
		
		this.SetLayoutName();
		this.SetParentWidget();
		
		if( m_LayoutName )
		{
			if( GetWidgetCache() )
			{
				m_CacheObject = GetWidgetCache().Get( m_LayoutName );
			}
			if( m_CacheObject )
			{
				m_MainWidget = m_CacheObject.GetWidget();
				m_MainWidget.Show( true );
			}
			else
			{
				m_MainWidget = GetGame().GetWorkspace().CreateWidgets( m_LayoutName );
				m_MainWidget.Show( true );
			}
		}
		
		if( m_ParentWidget )
		{
			m_ParentWidget.Show( true );
			if( m_MainWidget )
			{
				m_ParentWidget.AddChild( m_MainWidget );
			}
		}
	}
	
	void ~LayoutHolder()
	{
		if( m_CacheObject )
		{
			if( GetWidgetCache() )
				GetWidgetCache().Return( m_LayoutName, m_CacheObject );
		}
		else
		{
			delete m_MainWidget;
		}
	}
	
		
	LayoutHolder GetRoot()
	{
		if( m_Parent )
		{
			ref LayoutHolder container = m_Parent.GetRoot();
			return container;
		}
		else
		{
			return this;
		}
	}
	
	void UpdateInterval() {}

	void SetLayoutName() {}
	
	void SetParentWidget()
	{
		if( m_Parent != NULL )
		{
			m_ParentWidget = m_Parent.GetMainWidget();
		}
	}
	
	LayoutHolder GetParent()
	{
		return m_Parent;
	}

	void SetActive( bool active )
	{
	}

	bool IsActive()
	{
		return m_IsActive;
	}

	Widget GetMainWidget()
	{
		return m_MainWidget;
	}

	void OnShow()
	{
		m_MainWidget.Show( true );
		if( m_ParentWidget )
		{
			m_ParentWidget.Show( true );
		}
	}

	void OnHide()
	{
		m_MainWidget.Show( false );
	}

	void Refresh()
	{
		m_MainWidget.Update();
	}
	
	void InspectItem(InventoryItem item)
	{
		InventoryMenu menu = InventoryMenu.Cast( GetGame().GetUIManager().FindMenu(MENU_INVENTORY) );
		InspectMenuNew inspect_menu = InspectMenuNew.Cast( menu.EnterScriptedMenu(MENU_INSPECT) );
		if ( inspect_menu )
		{
			inspect_menu.SetItem(item);
		}
	}
	
	
	WidgetCache GetWidgetCache()
	{
		if( !m_Cache )
		{
			MissionGameplay mission = MissionGameplay.Cast( GetGame().GetMission() );
			m_Cache = mission.GetWidgetCache();
		}
		return m_Cache;
	}
}
