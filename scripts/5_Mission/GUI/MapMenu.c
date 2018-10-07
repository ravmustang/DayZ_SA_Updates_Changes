class MapMenu extends UIScriptedMenu
{
	void ~MapMenu()
	{
		CloseMap();
	}
	
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_map.layout");
		
		return layoutRoot;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		switch (w.GetUserID())
		{
			case IDC_CANCEL:
				Close();
				return true;
		}

		return false;
	}
	
	void CloseMap()
	{
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		ScriptInputUserData ctx = new ScriptInputUserData;
		//player.m_MapOpen = false;
		player.m_hac.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_END);
		if (GetGame().IsMultiplayer() && GetGame().IsClient() && ctx.CanStoreInputUserData())
		{
			ctx.Write(INPUT_UDT_STANDARD_ACTION);
			ctx.Write(DayZPlayerConstants.CMD_ACTIONINT_END);
			ctx.Send();
		}
	}
}
