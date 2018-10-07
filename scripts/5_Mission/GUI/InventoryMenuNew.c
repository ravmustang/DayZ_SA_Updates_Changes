class InventoryMenuNew extends UIScriptedMenu
{
	ref Inventory m_Inventory;
	private ref ContextMenu m_context_menu;
	protected bool m_IsOpened;
	protected bool m_OnlyFirstTime;

	void InventoryMenuNew()
	{
		m_Inventory = new Inventory(NULL);
		m_Inventory.Reset();
		m_Inventory.UpdateInterval();
		m_context_menu = new ContextMenu;
	}

	void ~InventoryMenuNew()
	{
		UnlockControls();
	}
	
	override Widget Init()
	{
		m_Inventory.Init();
		m_context_menu.Init(layoutRoot);
		layoutRoot = m_Inventory.GetMainPanel();
		return layoutRoot;
	}
	
	void RefreshQuickbar()
	{
		m_Inventory.RefreshQuickbar();
	}
	
	void RefreshQuantity( EntityAI item_to_refresh )
	{
		m_Inventory.RefreshQuantity( item_to_refresh );
	}
	
	void RefreshItemPosition( EntityAI item_to_refresh )
	{
		m_Inventory.RefreshItemPosition( item_to_refresh );
	}
	
	override ContextMenu GetContextMenu()
	{
		return m_context_menu;
	}

	void InitContainers(EntityAI target)
	{
	}

	override void OnShow()
	{
		super.OnShow();
		m_IsOpened = true;
		PPEffects.SetBlurInventory(1);
		if(m_Inventory)
			m_Inventory.OnShow();
		LockControls();
		SetFocus( layoutRoot );
		MissionGameplay mission = MissionGameplay.Cast( GetGame().GetMission() );
		if( mission )
		{
			mission.MoveHudForInventory( true );
		}
	}
	
	override bool OnController( Widget w, int control, int value )
	{
		return m_Inventory.Controller( w, control, value );
	}
	
	int Reset()
	{
		m_Inventory.Reset();
		return 1;
	}
	
	bool IsOpened()
	{
		return m_IsOpened;
	}
	
	override bool UseMouse()
	{
#ifdef PLATFORM_CONSOLE
		return false;
#endif
		return true;
	}

	override void OnHide()
	{
		super.OnHide();
		m_IsOpened = false;
		PPEffects.SetBlurInventory(0);
		if(m_Inventory)
			m_Inventory.OnHide();
		UnlockControls();
		MissionGameplay mission = MissionGameplay.Cast( GetGame().GetMission() );
		if( mission )
		{
			mission.MoveHudForInventory( false );
		}
	}
}
