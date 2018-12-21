class MapMenu extends UIScriptedMenu
{
	void ~MapMenu()
	{
		//CloseMap();
	}
	
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_map.layout");
		
		/*MapWidget m = MapWidget.Cast(layoutRoot.FindAnyWidget("Map"));
		if (m)
		{
			m.AddUserMark("2681 4.7 1751", "Lalal", ARGB(255,255,0,0), "\\dz\\gear\\navigation\\data\\map_tree_ca.paa");
			m.AddUserMark("2683 4.7 1851", "Lala2", ARGB(255,0,255,0), "\\dz\\gear\\navigation\\data\\map_bunker_ca.paa");
			m.AddUserMark("2670 4.7 1651", "Lala3", ARGB(255,0,0,255), "\\dz\\gear\\navigation\\data\\map_busstop_ca.paa");
		}*/
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
	
//currently unused
	void CloseMap()
	{
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		if (!player || !player.m_hac)
			return;
		
		ScriptInputUserData ctx = new ScriptInputUserData;
		//player.m_MapOpen = false;
		if (ctx.CanStoreInputUserData())
		{
			player.m_hac.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_END);
			if (GetGame().IsMultiplayer() && GetGame().IsClient())
			{
				ctx.Write(INPUT_UDT_STANDARD_ACTION);
				ctx.Write(DayZPlayerConstants.CMD_ACTIONINT_END);
				ctx.Send();
			}
			if (GetGame().IsClient())
			{
				GetGame().GetMission().PlayerControlEnable();
			}
		}
	}
}
