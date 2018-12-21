class WidgetCache
{
	private ref static WidgetCache s_instance;

	protected ref map<string, ref array<ref WidgetCacheObject>> m_WidgetsCache;

	const int INVENTORY_CACHES = 1;
	const int INVENTORY_ICON_CACHES = 50;
	const int INVENTORY_ICON_CONTAINER_CACHES = 50;
	const int COLLAPSIBLE_HEADER_CACHES = 50;
	const int GRID_CONTAINER_CACHES = 50;
	
	ref Inventory m_Inventory;

	static WidgetCache GetInstance()
	{
		return s_instance;
	}

	Inventory GetInv()
	{
		return m_Inventory;
	}

	void WidgetCache()
	{
		s_instance = this;
		m_WidgetsCache = new map<string, ref array<ref WidgetCacheObject>>;

		//CreateWidgetsCache();
		/*m_Inventory = new Inventory( NULL );
		m_Inventory.Reset();
		m_Inventory.OnHide();*/
	}

	void Reset()
	{
		//m_Inventory.Reset();
	}

	void CreateWidgetsCache()
	{
		CreateWidgetCache( WidgetLayoutName.Inventory, INVENTORY_CACHES );
		CreateWidgetCache( WidgetLayoutName.InventorySlotsContainer, 5 );
		CreateWidgetCache( WidgetLayoutName.CollapsibleHeader, COLLAPSIBLE_HEADER_CACHES );
		CreateWidgetCache( WidgetLayoutName.LeftArea, 1 );
		CreateWidgetCache( WidgetLayoutName.CollapsibleContainer, COLLAPSIBLE_HEADER_CACHES );
		CreateWidgetCache( WidgetLayoutName.ClosableHeader, COLLAPSIBLE_HEADER_CACHES );
		CreateWidgetCache( WidgetLayoutName.HandsHeader, 1 );
		CreateWidgetCache( WidgetLayoutName.HandsArea, 1 );
		CreateWidgetCache( WidgetLayoutName.ClosableContainer, COLLAPSIBLE_HEADER_CACHES );
		CreateWidgetCache( WidgetLayoutName.GridContainer, GRID_CONTAINER_CACHES );
	}

	void CreateWidgetCache( string layoutName, int numberOfCaches )
	{
		ref array<ref WidgetCacheObject> widgets = new array<ref WidgetCacheObject>;

		for( int i = 0; i < numberOfCaches; i++)
		{
			ref Widget w = GetGame().GetWorkspace().CreateWidgets( layoutName, NULL );
			w.Show( false );
			ref WidgetCacheObject w_cache_object = new WidgetCacheObject( w );
			widgets.Insert( w_cache_object );
		}

		m_WidgetsCache.Insert( layoutName, widgets );
	}

	WidgetCacheObject Get( string layoutName )
	{
		array<ref WidgetCacheObject> objects = m_WidgetsCache.Get( layoutName );
		ref WidgetCacheObject w;
		if( objects && objects.Count() > 0 )
		{
			w = objects.Get( 0 );
			objects.Remove(0);
	
			w.SaveOriginalValeus();
		}
		return w;
	}

	void Return( string layoutName, ref WidgetCacheObject w )
	{
		Widget w_p = w.GetWidget().GetParent();
		if( w_p )
			w_p.RemoveChild( w.GetWidget() );
		w.RestoreOriginalValeus();
		m_WidgetsCache.Get( layoutName ).Insert( w );
	}
}
