class CharacterMenu extends UIScriptedMenu
{
	ButtonWidget	m_genderButton;
	//---------------------------
	ButtonWidget	m_skinNextButton;
	ButtonWidget	m_skinPreviousButton;
	int				m_skinIndex;
	ButtonWidget	m_topNextButton;
	ButtonWidget	m_topPreviousButton;
	int				m_topIndex;
	ButtonWidget	m_pantsNextButton;
	ButtonWidget	m_pantsPreviousButton;
	int				m_pantsIndex;
	ButtonWidget	m_shoesNextButton;
	ButtonWidget	m_shoesPreviousButton;
	int				m_shoesIndex;
	//---------------------------
	EditBoxWidget 	m_name_widget;
	
	protected MissionMainMenu m_MissionMainMenu;
	
	protected ref PlayerNameHandler m_PlayerNameHandler;
	
	//string m_format;
	//bool m_IntroScene.m_female;
	DayZIntroScene m_IntroScene;

	void CharacterMenu()
	{
		m_PlayerNameHandler = new PlayerNameHandler;
		m_MissionMainMenu = MissionMainMenu.Cast( GetGame().GetMission() );
		m_IntroScene = m_MissionMainMenu.GetIntroScenePC();
	}

	void ~CharacterMenu()
	{
		//sets name to widget in main menu
		if (g_Game.GetUIManager().FindMenu(MENU_MAIN) != null)
		{
			MainMenu menu = MainMenu.Cast( GetGame().GetUIManager().FindMenu(MENU_MAIN) );
			TextWidget name_widget = TextWidget.Cast( menu.layoutRoot.FindAnyWidget("character_name_text") );
			name_widget.SetText( g_Game.GetPlayerGameName() );
		}
	}

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_character_new.layout");

		Class.CastTo(m_genderButton, layoutRoot.FindAnyWidget("GenderButtonWidget"));
		//---------------------------
		Class.CastTo(m_skinNextButton, layoutRoot.FindAnyWidget("SkinNextButtonWidget"));
		Class.CastTo(m_skinPreviousButton, layoutRoot.FindAnyWidget("SkinPreviousButtonWidget"));
		Class.CastTo(m_topNextButton, layoutRoot.FindAnyWidget("TopNextButtonWidget"));
		Class.CastTo(m_topPreviousButton, layoutRoot.FindAnyWidget("TopPreviousButtonWidget"));
		Class.CastTo(m_pantsNextButton, layoutRoot.FindAnyWidget("PantsNextButtonWidget"));
		Class.CastTo(m_pantsPreviousButton, layoutRoot.FindAnyWidget("PantsPreviousButtonWidget"));
		Class.CastTo(m_shoesNextButton, layoutRoot.FindAnyWidget("ShoesNextButtonWidget"));
		Class.CastTo(m_shoesPreviousButton, layoutRoot.FindAnyWidget("ShoesPreviousButtonWidget"));
		//---------------------------
		
		MissionMainMenu mission;
		Class.CastTo(mission,  g_Game.GetMission() );
		m_IntroScene = mission.GetIntroScenePC();
		m_IntroScene.ResetIntroCamera();
		
		//DefaultCharacterScreen();
		Class.CastTo(m_name_widget ,layoutRoot.FindAnyWidget("CharacterNameEdit"));
		m_name_widget.SetText(g_Game.GetPlayerGameName());
		m_name_widget.SetHandler(m_PlayerNameHandler);
		
		if (m_MissionMainMenu.IsCharacterFemale())
		{
			m_genderButton.SetText("F");
		}
		else
		{
			m_genderButton.SetText("M");
		}
		
		ShowMenuButtons(g_Game.IsNewCharacter());
		
		LockControls();
		
		return layoutRoot;
	}
	
	override void OnShow()
	{
	}
		
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (!m_MissionMainMenu.IsClickEnabled())
		{
			return false;
		}
		
		super.OnClick(w, x, y, button);
		
		switch (w.GetUserID())
		{
		case IDC_OK:
			PlayerBase scene_char = m_MissionMainMenu.GetIntroSceneCharacter();
			
			if ( scene_char )
			{
				//saves demounit for further use
				m_MissionMainMenu.SaveCharName();
				if ( m_MissionMainMenu.GetIntroSceneCharacter().GetInventory().FindAttachment(InventorySlots.BODY) && m_MissionMainMenu.GetCurrentCharacterID() == -1)
				{
					m_MissionMainMenu.SaveCharacterSetup();
				}
				
				if (!g_Game.IsNewCharacter()) 
				{
					GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallByName(this, "ConnectLastSession");
				}
				else
				{
					GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallByName(this, "ConnectBestServer");
				}
			}
			
			return true;
			
		case IDC_CANCEL:
			m_MissionMainMenu.SaveCharName();
			
			Close();
			return true;
		
		case IDC_RESTART:
			RandomCharacter();
			return true;
		}
//------------------------------------------------------------------------
		switch (w)
		{
			case m_genderButton:
				if (!m_MissionMainMenu.IsCharacterFemale())
				{
					m_MissionMainMenu.SetCharacterFemale( true );
					m_genderButton.SetText("F");
				}
				else
				{
					m_MissionMainMenu.SetCharacterFemale( false );
					m_genderButton.SetText("M");
				}
				m_skinIndex = 0;
				
				SetCharacter();
			break;
			
			case m_skinNextButton:
				m_skinIndex++;
				SetCharacter();
			break;
			
			case m_skinPreviousButton:
				m_skinIndex--;
				SetCharacter();
			break;
			
			case m_topNextButton:
				m_MissionMainMenu.CharChangePart(Direction.RIGHT, InventorySlots.BODY);
			break;
			
			case m_topPreviousButton:
				m_MissionMainMenu.CharChangePart(Direction.LEFT, InventorySlots.BODY);
			break
			
			case m_pantsNextButton:
				m_MissionMainMenu.CharChangePart(Direction.RIGHT, InventorySlots.LEGS);
			break;
			
			case m_pantsPreviousButton:
				m_MissionMainMenu.CharChangePart(Direction.LEFT, InventorySlots.LEGS);
			break;
			
			case m_shoesNextButton:
				m_MissionMainMenu.CharChangePart(Direction.RIGHT, InventorySlots.FEET);
			break;
			
			case m_shoesPreviousButton:
				m_MissionMainMenu.CharChangePart(Direction.LEFT, InventorySlots.FEET);
			break;
		}
//------------------------------------------------------------------------
		return false;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		super.OnMouseButtonDown(w, x, y, button);
		
		if (w.GetName() == "CharacterRotationFrame")
		{
			m_IntroScene.CharacterRotationStart();
			return true;
		}

		return false;
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		super.OnMouseButtonUp(w, x, y, button);
		
		m_IntroScene.CharacterRotationStop();
		return true;
	}
	
	override bool OnMouseWheel(Widget  w, int  x, int  y, int wheel)
	{
	}
	
	void SetCharacter()
	{
		string character;
		//string params[2];

		if ( m_MissionMainMenu.IsCharacterFemale() )
		{
			if (m_skinIndex < 0)
			{
				m_skinIndex = m_IntroScene.m_CharPersonalityFemaleList.Count() - 1;
			}
			
			if (m_skinIndex > m_IntroScene.m_CharPersonalityFemaleList.Count() - 1)
			{
				m_skinIndex = 0;
			}
			
			/*params[0] = "F";
			params[1] = m_IntroScene.m_CharPersonalityFemaleList.Get(m_skinIndex);*/
			character = m_IntroScene.m_CharPersonalityFemaleList.Get(m_skinIndex);
		}
		else
		{
			if (m_skinIndex < 0)
			{
				m_skinIndex = m_IntroScene.m_CharPersonalityMaleList.Count() - 1;
			}
			
			if (m_skinIndex > m_IntroScene.m_CharPersonalityMaleList.Count() - 1)
			{
				m_skinIndex = 0;
			}
			
			/*params[0] = "M";
			params[1] = m_IntroScene.m_CharPersonalityMaleList.Get(m_skinIndex);*/
			character = m_IntroScene.m_CharPersonalityMaleList.Get(m_skinIndex);
		}
		
		//g_Game.FormatString(m_IntroScene.m_format, params, character);
		m_IntroScene.CreateNewCharacter(character);
		if (m_IntroScene.GetIntroSceneCharacter())
		{
			m_IntroScene.SetAttachment(m_IntroScene.m_CharShirtList.Get(m_topIndex), InventorySlots.BODY);
			m_IntroScene.SetAttachment(m_IntroScene.m_CharPantsList.Get(m_pantsIndex), InventorySlots.LEGS);
			m_IntroScene.SetAttachment(m_IntroScene.m_CharShoesList.Get(m_shoesIndex), InventorySlots.FEET);
			/////////allows for attachment of items to other slots, if needed
			if (g_Game.IsNewCharacter())
			{
				m_IntroScene.SetAttachment("", InventorySlots.SHOULDER);
				m_IntroScene.SetAttachment("", InventorySlots.BOW);
				m_IntroScene.SetAttachment("", InventorySlots.MELEE);
				m_IntroScene.SetAttachment("", InventorySlots.VEST);
				m_IntroScene.SetAttachment("", InventorySlots.HIPS);
				m_IntroScene.SetAttachment("", InventorySlots.BACK);
				m_IntroScene.SetAttachment("", InventorySlots.HEADGEAR);
				m_IntroScene.SetAttachment("", InventorySlots.MASK);
				m_IntroScene.SetAttachment("", InventorySlots.EYEWEAR);
				m_IntroScene.SetAttachment("", InventorySlots.GLOVES);
				m_IntroScene.SetAttachment("", InventorySlots.ARMBAND);
				m_IntroScene.SetAttachment("", InventorySlots.HANDS); //5 = hands slot
				//Print(InventorySlots.GetSlotIdFromString("BACK"));
			}
			
			m_IntroScene.SetClickEnable( false );
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(m_IntroScene.SceneCharacterSetPos, 250);
		}
	}

	void RandomCharacter()
	{
		g_Game.SetNewCharacter(true);
		//g_Game.SetPlayerGameName(DEFAULT_CHARACTER_NAME);
		ShowMenuButtons(true);
		m_IntroScene.SetCurrentCharacterID( -1 );
		g_Game.SetPlayerGameName(m_name_widget.GetText());
		//m_name_widget.SetText(g_Game.GetPlayerGameName());		
		
		// make random selection
		m_IntroScene.RandomSelectGender();
		
		if (m_IntroScene.IsCharacterFemale())
		{
			m_genderButton.SetText("F");
			m_skinIndex =  m_IntroScene.RandomSelectIndex(m_IntroScene.m_CharPersonalityFemaleList);
		}
		else
		{
			m_genderButton.SetText("M");
			m_skinIndex =  m_IntroScene.RandomSelectIndex(m_IntroScene.m_CharPersonalityMaleList);
		}
		
		m_topIndex = m_IntroScene.RandomSelectIndex(m_IntroScene.m_CharShirtList);
		m_pantsIndex = m_IntroScene.RandomSelectIndex(m_IntroScene.m_CharPantsList);
		m_shoesIndex = m_IntroScene.RandomSelectIndex(m_IntroScene.m_CharShoesList);
		
		// update character
		SetCharacter();
	}
	
	int ListLoad(string path, out TStringArray list);
	
	void ConnectLastSession()
	{
		//TODO fix code-side
		if ( !g_Game.ConnectLastSession(this, m_MissionMainMenu.GetCurrentCharacterID()) )
		{
			g_Game.GetUIManager().EnterServerBrowser(this);
		}
	}
	
	void ConnectBestServer()
	{
		//TODO add functionality! For now works as follows:
		ConnectLastSession();
	}
	
	void ShowMenuButtons(bool show)
	{
		layoutRoot.FindAnyWidget("CharacterPanel").Show(show);
	}
}