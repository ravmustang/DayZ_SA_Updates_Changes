/*! Xbox menu */
class TitleScreenMenu extends UIScriptedMenu
{
	void TitleScreenMenu()
	{
		g_Game.SetGameState( DayZGameState.MAIN_MENU );
		g_Game.SetLoadState( DayZLoadState.MAIN_MENU_START );
	}
	
	void ~TitleScreenMenu()
	{
	}
	
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/xbox/day_z_title_screen.layout");
		
		MissionMainMenu mission = MissionMainMenu.Cast( g_Game.GetMission() );
		
		RichTextWidget text_widget = RichTextWidget.Cast( layoutRoot.FindAnyWidget("InputPromptText") );
		if (text_widget)
		{
			string gamertag;
			GetGame().GetPlayerName(gamertag);
#ifdef PLATFORM_XBOX
		text_widget.SetText("Press <image set=\"xbox_buttons\" name=\"A\" /> to start the game");
#endif
		
#ifdef PLATFORM_PS4
		text_widget.SetText("Press <image set=\"playstation_buttons\" name=\"cross\" /> to start the game");
#endif
		}
		return layoutRoot;
	}
	
	override void OnShow()
	{
		if( g_Game.GetGameState() != DayZGameState.CONNECTING )
		{
			g_Game.SelectUser();
		}
	}
	
	override void OnHide()
	{
		
	}
	
	override void Update(float timeslice)
	{
		#ifdef PLATFORM_WINDOWS
			if( GetGame().GetInput().GetActionDown( UAUISelect, false ) )
			{
				EnterScriptedMenu(MENU_MAIN);
			}
		#endif
	}
}