class TAdvanceArray<Class T> extends array<string>
{
	void InsertArray(array<T> other)
	{
		for (int i = 0; i < other.Count(); i++)
		{
			T item = other.Get(i);
			Insert(item);
		}
	}
}

typedef TAdvanceArray<string> TStringAdvanceArray;

class DayZIntroScene: Managed
{
	protected string m_lastCharacter;
	protected bool m_IsCharFemale;
	protected int m_LastShavedSeconds;
	protected int m_CachedPlaytime;
	protected int m_currentCharacterID;
	
	ref TStringArray 			m_CharAllCharacters;
	ref TStringAdvanceArray 	m_CharGenderList;
	ref TStringAdvanceArray 	m_CharPersonalityMaleList;
	ref TStringAdvanceArray 	m_CharPersonalityFemaleList;
	ref TStringAdvanceArray		m_CharShirtList;
	int 						m_CharShirtIndex;
	ref TStringAdvanceArray 	m_CharPantsList;
	int 						m_CharPantsIndex;
	ref TStringAdvanceArray 	m_CharShoesList;
	int 						m_CharShoesIndex;
	
	protected ref EntityAnimEndEventHandler 	m_anim_end_event_handler;
	protected ref PlayerNameHandler 			m_PlayerNameHandler;

	protected Camera 	m_Camera;
	protected PlayerBase m_DemoUnit;
	protected Weather	m_Weather;
	protected vector 	m_DemoPos;
	protected vector 	m_DemoRot;
	protected vector 	m_CameraTrans[4];
	protected vector 	m_Target;
	protected bool		m_EnableClick;
	protected bool 		m_RotatingCamera;
	protected bool 		m_RotatingCharacter;
	protected int 		m_RotatingCharacterMouseX;
	protected int 		m_RotatingCharacterMouseY;
	protected float 	m_RotatingCharacterRot;
	protected float 	m_Radius;
	protected float 	m_Radius_original;
	protected float 	m_DiffX;
	protected float 	m_DeltaX;
	protected float 	m_DeltaZ;
	protected float 	m_Angle;
	protected float 	m_Angle_offset = 0;
	protected float 	m_NewX = 0;
	protected float 	m_NewZ = 0;
	protected float 	m_BlurValue;

	protected ref array<Man> m_Preloaded;
	protected ref OptionsMenu m_optmenu = new OptionsMenu;
	protected MenuData m_data;

	protected ref Timer m_timer;
	protected EntityAI m_entity_to_take;

	void DayZIntroScene()
	{
		m_timer = new Timer();
		m_currentCharacterID = -1;
		m_DemoPos = "0 0 0";
		m_DemoRot = "0 0 0";
		m_lastCharacter = "";
		m_LastShavedSeconds = 0;
		m_CachedPlaytime = 0;
		SetClickEnable( true );
		//g_Game.GetPlayerName(m_player_name);
		
		m_Preloaded = new array<Man>;
		
		string cached_playtime_str = "";
		g_Game.GetProfileString("cachedPlaytime", cached_playtime_str);
		if ( cached_playtime_str != "" )
		{
			m_CachedPlaytime = cached_playtime_str.ToInt();
		}
		World w = g_Game.GetWorld();
		m_data = g_Game.GetMenuData();
		m_data.ClearCharacters();
		m_data.LoadCharacters();
		
		//g_Game.m_PlayerName = "Survivor"; //default
		
		if (m_data.GetLastPlayedCharacter() > -1 )
		{
			m_currentCharacterID = m_data.GetLastPlayedCharacter();
			m_data.GetCharacterName(m_currentCharacterID, g_Game.GetPlayerGameName());
		}

		string worldName;
		g_Game.GetWorldName(worldName);
	
		string root_path = "cfgCharacterScenes " + worldName;
		
		int count = g_Game.ConfigGetChildrenCount(root_path);
		int index = Math.RandomInt(0, count - 1);
		string childName;
		g_Game.ConfigGetChildName(root_path, index, childName);
	
		string scene_path = root_path + " " + childName;
		m_Target = SwapYZ(g_Game.ConfigGetVector(scene_path + " target"));
		vector position = SwapYZ(g_Game.ConfigGetVector(scene_path + " position"));
		TIntArray date = new TIntArray;
		TFloatArray storm = new TFloatArray;
		g_Game.ConfigGetIntArray(scene_path + " date", date);
		float fov = g_Game.ConfigGetFloat(scene_path + " fov");
		float overcast = g_Game.ConfigGetFloat(scene_path + " overcast");
		float rain = g_Game.ConfigGetFloat(scene_path + " rain");
		float fog = g_Game.ConfigGetFloat(scene_path + " fog");
		float windspeed = -1;
		if ( g_Game.ConfigIsExisting(scene_path + " windspeed") ) 	windspeed = g_Game.ConfigGetFloat(scene_path + " windspeed");
		g_Game.ConfigGetFloatArray(scene_path + " storm", storm);
		
		World world = g_Game.GetWorld();
	
		if (world && date.Count() >= 5)
		{
			world.SetDate(date.Get(0), date.Get(1), date.Get(2), date.Get(3), date.Get(4));
		}
	
		GetGame().ObjectDelete( m_Camera );
		Class.CastTo(m_Camera, g_Game.CreateObject("staticcamera", SnapToGround(position), true)); //Vector(position[0] , position[1] + 1, position[2])
		
		Math3D.MatrixIdentity4(m_CameraTrans);
		
		if (m_Camera)
		{
			//m_Camera.SetPosition(Vector(m_Camera.GetPosition()[0],m_Camera.GetPosition()[1]+0,m_Camera.GetPosition()[2]));
			m_Camera.LookAt(m_Target);
			m_Camera.SetFOV(fov);
			m_Camera.SetFocus(5.0, 0.0); //5.0, 1.0
			
			m_Camera.SetActive(true);
			
			Math3D.DirectionAndUpMatrix(m_Target - SnapToGround(position), "0 1 0", m_CameraTrans);
			m_CameraTrans[3] = m_Camera.GetPosition();
			m_DemoPos = Vector(0.685547, -0.988281, 3.68823).Multiply4(m_CameraTrans);

			float pos_x = m_DemoPos[0];
			float pos_z = m_DemoPos[2];
			float pos_y = GetGame().SurfaceY(pos_x, pos_z);
			vector ground_demo_pos = Vector(pos_x, pos_y, pos_z);
			m_DemoPos = ground_demo_pos;

			m_DemoRot = "0 0 0";
			vector to_cam_dir = m_Camera.GetPosition() - m_DemoPos;
			m_DemoRot[0] = Math.Atan2(to_cam_dir[0], to_cam_dir[2]) * Math.RAD2DEG;
		}
		
		m_DeltaX = Math.AbsFloat(m_DemoPos[0] - m_Camera.GetPosition()[0]);
		m_DeltaZ = Math.AbsFloat(m_DemoPos[2] - m_Camera.GetPosition()[2]);
		if (!m_Radius || m_Radius == 0)
		{
			m_Radius = Math.Sqrt (Math.Pow(m_DeltaX, 2) + Math.Pow(m_DeltaZ, 2));
			m_Radius_original = m_Radius;
		}
		
		m_Weather = g_Game.GetWeather();
		m_Weather.GetOvercast().SetLimits( overcast, overcast );
		m_Weather.GetRain().SetLimits( rain, rain );
		m_Weather.GetFog().SetLimits( fog, fog );
		
		m_Weather.GetOvercast().Set( overcast, 0, 0);
		m_Weather.GetRain().Set( rain, 0, 0);
		m_Weather.GetFog().Set( fog, 0, 0);
		
		//Vignette
		PPEffects.SetVignette(0.3, 0,0,0);
		
		if ( storm.Count() == 3 )
		{
			m_Weather.SetStorm(storm.Get(0),storm.Get(1),storm.Get(2));
		}
		
		if ( windspeed != -1 )
		{
			m_Weather.SetWindSpeed(windspeed);
			m_Weather.SetWindMaximumSpeed(windspeed);
			m_Weather.SetWindFunctionParams(1,1,1);
		}
		
		Init();
	}
	
	void Init()
	{
		//fill default lists
		m_CharGenderList = new TStringAdvanceArray;
		m_CharPersonalityMaleList = new TStringAdvanceArray;
		m_CharPersonalityFemaleList = new TStringAdvanceArray;
		m_CharAllCharacters = new TStringArray;
		m_CharShirtList = new TStringAdvanceArray;
		m_CharPantsList = new TStringAdvanceArray;
		m_CharShoesList = new TStringAdvanceArray;
		
		string character_CfgName;
		string root_path = "cfgCharacterCreation";
		
		g_Game.ConfigGetTextArray(root_path + " gender", m_CharGenderList);
		g_Game.ConfigGetTextArray(root_path + " top", m_CharShirtList);
		g_Game.ConfigGetTextArray(root_path + " bottom", m_CharPantsList);
		g_Game.ConfigGetTextArray(root_path + " shoe", m_CharShoesList);

		m_CharAllCharacters = GetGame().ListAvailableCharacters();
		for (int i = 0; i < m_CharAllCharacters.Count(); i++)
		{
			character_CfgName = m_CharAllCharacters.Get(i);
			if (GetGame().IsKindOf(character_CfgName, "SurvivorMale_Base"))
			{
				m_CharPersonalityMaleList.Insert(character_CfgName);
			}
			else
			{
				m_CharPersonalityFemaleList.Insert(character_CfgName);
			}
		}
		
		ChangeCharacter(m_currentCharacterID);
		
		PPEffects.Init();
		PPEffects.DisableBurlapSackBlindness(); //HOTFIX
		//PPEffects.ResetAll();
	}
	
	Camera GetCamera()
	{
		return m_Camera;
	}
	
	void SetCharacterFemale(bool fem)
	{
		m_IsCharFemale = fem;
	}
	
	bool IsCharacterFemale()
	{
		return m_IsCharFemale;
	}
	
	void SetClickEnable( bool enable )
	{
		m_EnableClick = enable;
	}
	
	bool IsClickEnabled()
	{
		return m_EnableClick;
	}
	
	void ResetIntroCamera()
	{
		GetCamera().LookAt( GetIntroSceneCharacter().GetPosition() + Vector( 0, 1, 0 ) );
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater( SceneCharacterSetPos, 250 );
	}
	
	PlayerBase GetIntroSceneCharacter()
	{
		return m_DemoUnit;
	}
	
	void RandomSelectGender()
	{
		m_IsCharFemale = Math.RandomInt(0, 2);
	}
	
	// ------------------------------------------------------------
	void CreateRandomCharacter()
	{
		string character_name;
		string params[2];
		
		RandomSelectGender();
		
		if (IsCharacterFemale())
		{
			character_name = m_CharPersonalityFemaleList.GetRandomElement();
		}
		else
		{
			character_name = m_CharPersonalityMaleList.GetRandomElement();
		}
		CreateNewCharacter(character_name);
		
		if (m_DemoUnit)
		{
			SetAttachment(m_CharShirtList.GetRandomElement(), InventorySlots.BODY);
			SetAttachment(m_CharPantsList.GetRandomElement(), InventorySlots.LEGS);
			SetAttachment(m_CharShoesList.GetRandomElement(), InventorySlots.FEET);
		}
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SceneCharacterSetPos, 250);
	}
	
	int RandomSelectIndex(TStringAdvanceArray list)
	{
		if (list)
		{
			int rnd = Math.RandomInt(0, list.Count());
			return rnd;
		}
		return -1;
	}
	
	void SetAttachment(string type, int slot)
	{
		if (!m_DemoUnit) return;
		g_Game.ObjectDelete(m_DemoUnit.GetInventory().FindAttachment(slot));
		EntityAI entity;
		Class.CastTo(entity, g_Game.CreateObject(type, "0 2000 0", true));
		if (entity)
		{
			m_DemoUnit.LocalTakeEntityAsAttachmentEx(entity, slot);
		}
	}
	
	void GetSelectedUserName()
	{
		string name;
		BiosUserManager user_manager = GetGame().GetUserManager();
		if( user_manager )
		{
			BiosUser user = user_manager.GetSelectedUser();
			if( user )
			{
				g_Game.SetPlayerGameName( user.GetName() );
				return;
			}
		}
		g_Game.SetPlayerGameName(DEFAULT_CHARACTER_NAME);
	}
	
	void CharChangePart( Direction dir, int inv_slot )
	{
		TStringAdvanceArray list;
		int list_index;
		
		switch ( inv_slot )
		{
			case InventorySlots.BODY:
			{
				list = m_CharShirtList;
				m_CharShirtIndex = AdjustListIndex(list, dir, m_CharShirtIndex);
				list_index = m_CharShirtIndex;
				break;
			}
			case InventorySlots.LEGS:
			{
				list = m_CharPantsList;
				m_CharPantsIndex = AdjustListIndex(list, dir, m_CharPantsIndex);
				list_index = m_CharPantsIndex;
				break;
			}
			case InventorySlots.FEET:
			{
				list = m_CharShoesList;
				m_CharShoesIndex = AdjustListIndex(list, dir, m_CharShoesIndex);
				list_index = m_CharShoesIndex;
				break;
			}
		}		
		
		SetAttachment(list.Get(list_index), inv_slot);
	}
	
	int AdjustListIndex(TStringAdvanceArray list, Direction dir, int index)
	{
		if ( dir == Direction.RIGHT )
		{
			index++;
			
			if (index >= list.Count() )
			{
				index = 0;
			}
		}
		else if ( dir == Direction.LEFT )
		{
			index--;
			
			if (index < 0)
			{
				index = list.Count() - 1;
			}
		}
		
		return index;
	}
	
	// ------------------------------------------------------------
	void ChangeCharacter(int characterID)
	{
		string name;
		if (m_DemoUnit)
		{
			g_Game.ObjectDelete(m_DemoUnit);
			m_DemoUnit = NULL;
		}
		m_currentCharacterID = characterID;
		BiosUserManager user_manager;
		
		
		//random character
		if(characterID == - 1)
		{
#ifdef PLATFORM_CONSOLE
			GetSelectedUserName();
#else
			g_Game.SetPlayerGameName(DEFAULT_CHARACTER_NAME);
#endif
			CreateRandomCharacter();
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SceneCharacterSetPos, 250);
			g_Game.SetNewCharacter(true);
			return;
		}
		
		Print("CreateCharacterPerson -> "+ characterID);
		m_DemoUnit = PlayerBase.Cast(m_data.CreateCharacterPerson(characterID));
		
		if (m_DemoUnit)
		{
			g_Game.SetNewCharacter(false);
			m_DemoUnit.PlaceOnSurface();
			m_DemoUnit.SetPosition(m_DemoPos);
			m_DemoUnit.SetOrientation(m_DemoRot);
			m_DemoUnit.SetEventHandler(m_anim_end_event_handler);
			m_DemoUnit.SetLastShavedSeconds(m_LastShavedSeconds);
	
			//InitCharacterAnimations(m_DemoUnit.GetHumanInventory().GetEntityInHands());
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SceneCharacterSetPos, 250);
		}

#ifdef PLATFORM_CONSOLE
			GetSelectedUserName();
#else
		m_data.GetCharacterName(m_currentCharacterID, name);
#endif
		g_Game.SetPlayerGameName(name);
		//Print(g_Game.m_PlayerName);
		//GetGame().GetPlayerName(m_player_name);
	}
	//-------------------------------------------------------------
	int NextCharacterID()
	{
		int count = m_data.GetCharactersCount();
		if ( count == 0 )	return -1;
		
		if ( m_currentCharacterID + 1 < count )
		{
			return m_currentCharacterID++; 
		}
		else
		{
			return -1;
		}
	}
	//-------------------------------------------------------------
	int PrevCharacterID()
	{
		int count = m_data.GetCharactersCount();
		if ( count == 0 )	return -1;
		if ( m_currentCharacterID > -1 )
		{
			return m_currentCharacterID - 1;
		}
		return count - 1;
	}
	//-------------------------------------------------------------
	void SetCurrentCharacterID( int id )
	{
		m_currentCharacterID = id;
	}
	
	//-------------------------------------------------------------
	int GetCurrentCharacterID()
	{
		return m_currentCharacterID;
	}
	// ------------------------------------------------------------
	void CreateNewCharacter(string type)
	{
		if (m_DemoUnit)
		{
			g_Game.ObjectDelete(m_DemoUnit);
			m_DemoUnit = NULL;
		}

		g_Game.PreloadObject(type, 1.0);
		m_DemoUnit = PlayerBase.Cast(g_Game.CreateObject(type, SnapToGround(Vector(m_DemoPos[0],m_DemoPos[1],m_DemoPos[2]) + "0 0 333"), true));
		
		if (m_DemoUnit)
		{
			//GetGame().SetPlayerName("Survivor");
			//GetGame().GetPlayerName(m_player_name);
			m_DemoUnit.PlaceOnSurface();
			m_DemoUnit.SetOrientation(m_DemoRot);
			m_DemoUnit.SetEventHandler(m_anim_end_event_handler);
			m_DemoUnit.SetLastShavedSeconds(m_LastShavedSeconds);

			// NEW STATS API
			string lvalue = "";
			m_DemoUnit.StatGetCounter("playtime", lvalue);

			PluginLifespan module_lifespan;
			Class.CastTo(module_lifespan, PluginLifespan.Cast( GetPlugin( PluginLifespan ) ));
			module_lifespan.UpdateLifespanParam( m_DemoUnit, lvalue, true );
		}
	}
	
	void SceneCharacterSetPos()
	{
		if (m_DemoUnit)
		{
			m_DemoUnit.SetPosition(m_DemoPos);
			m_DemoUnit.SetOrientation(m_DemoRot);
		}
		SetClickEnable( true );
	}
	
	// ------------------------------------------------------------
	void AnimateCharacter()
	{
		// Mirek: Switch move was related to legacy animation system
		//if (m_DemoUnit && m_characterAnimations.Count() > 0)
		//{
		//	int rnd = Math.RandomInt(0, (m_characterAnimations.Count() / 2) - 1 ) * 2;
		//	//Print("DayZIntroScene::AnimateCharacter()");
		//	//Print(m_characterAnimations.Get(rnd));
		//	m_DemoUnit.PlayMoveNowGlobal(m_characterAnimations.Get(rnd));
		//}
	}
	
	// ------------------------------------------------------------
	void GetAllItemsInInventory(out array<InventoryItem> items)
	{
		items.Clear();
		
		if (!m_DemoUnit) return;
		
		for (int i = 0; i < InventorySlots.COUNT; i++)
		{
			InventoryItem item;
			Class.CastTo(item, m_DemoUnit.GetInventory().FindAttachment(i));
	
			if (!item) continue;
	
			items.Insert(item);
	
			if (item.GetInventory().GetCargo())
			{
				CargoBase cargo = item.GetInventory().GetCargo();
	
				for (int j = 0; j < cargo.GetItemCount(); j++)
				{
					InventoryItemBase inventoryItem;
					Class.CastTo(inventoryItem, cargo.GetItem(j));
					items.Insert(inventoryItem);
				}
			}
	
			for (j = 0; j < item.GetInventory().AttachmentCount(); j++)
			{
				Class.CastTo(inventoryItem, item.GetInventory().GetAttachmentFromIndex(j));
				items.Insert(inventoryItem);
			}
		}
		
		Class.CastTo(item, m_DemoUnit.GetHumanInventory().GetEntityInHands());
		if (item) items.Insert(item);
	}
	
	// ------------------------------------------------------------
	void ~DayZIntroScene()
	{
		//g_Game.GetMenuData().UnloadCharacters();
	}
	
	// ------------------------------------------------------------
	void CharacterRotationStart()
	{
		m_RotatingCharacter = true;
		g_Game.GetMousePos(m_RotatingCharacterMouseX, m_RotatingCharacterMouseY);
		if (m_DemoUnit) 
		{
			m_RotatingCharacterRot = m_DemoRot[0];
		}
	}
	
	// ------------------------------------------------------------
	void CharacterRotationStop()
	{
		if (m_RotatingCharacter)
		{
			m_RotatingCharacter = false;
		}
	}
	
	// ------------------------------------------------------------
	void CharacterRotate()
	{
		int actual_mouse_x;
		int actual_mouse_y;
		float coef;
		g_Game.GetMousePos(actual_mouse_x, actual_mouse_y);
	
		m_DiffX = m_RotatingCharacterMouseX - actual_mouse_x;
		
		if (m_DemoUnit)
		{
			coef = ( m_RotatingCharacterRot + (m_DiffX * 0.5) ) / 360;
			coef = coef - Math.Floor(coef);
			//m_DemoRot[0] = m_RotatingCharacterRot + (diff_x * 0.5);
			m_DemoRot[0] = coef * 360;
			
			m_DemoUnit.SetOrientation(m_DemoRot);
		}
	}
	
	// ------------------------------------------------------------
	void SaveDefaultCharacter()
	{
		if (m_DemoUnit)
		{
			g_Game.SetProfileString("defaultCharacter", m_DemoUnit.GetType());
			
			InventoryItem item = NULL;
			TStringArray inventory = new TStringArray;
			
			for (int i = 0; i < InventorySlots.COUNT; i++)
			{
				Class.CastTo(item, m_DemoUnit.GetInventory().FindAttachment(i));
				if (item)
				{
					inventory.Insert(item.GetType());
				}
			}
			
			g_Game.SetProfileStringList("defaultInventory", inventory);
			g_Game.SaveProfile();
		}
	}
	
	// ------------------------------------------------------------
	void Update()
	{
		if (m_DemoUnit && m_RotatingCharacter)
		{
			CharacterRotate();
		}
		else if (m_DemoUnit && m_RotatingCamera)
		{
			CameraRotate();
		}
	}
	
	// ------------------------------------------------------------
	vector SwapYZ(vector vec)
	{
		vector tmp;
		tmp[0] = vec[0];
		tmp[1] = vec[2];
		tmp[2] = vec[1];
	
		return tmp;
	}
	
	// ------------------------------------------------------------
	vector SnapToGround(vector pos)
	{
		float pos_x = pos[0];
		float pos_z = pos[2];
		float pos_y = GetGame().SurfaceY(pos_x, pos_z);
		vector tmp_pos = Vector(pos_x, pos_y, pos_z);
		tmp_pos[1] = tmp_pos[1] + pos[1];
	
		return tmp_pos;
	}
	bool IsRotatingCharacter() {
		return m_RotatingCharacter;
	}
	bool IsRotatingCamera() {
		return m_RotatingCamera;
	}

	//Camera rotation
	// ------------------------------------------------------------
	void CameraRotationStart()
	{
		m_RotatingCamera = true;
		if (m_DemoUnit)
		{
			if (m_Angle)
			{
				//angle_offset = angle + angle_offset;
				m_Angle_offset = Math.DEG2RAD*(Math.NormalizeAngle(Math.RAD2DEG*(m_Angle + m_Angle_offset)));
			}
			g_Game.GetMousePos(m_RotatingCharacterMouseX, m_RotatingCharacterMouseY);

			m_optmenu.m_Options = new GameOptions;
			NumericOptionsAccess numeric;
			Class.CastTo(numeric, NumericOptionsAccess.Cast( m_optmenu.m_Options.GetOption(21) )); //21 = Radial blur option
			m_BlurValue = numeric.ReadValue();
			numeric.WriteValue(0.0); //sets rotation blur to 0
		}
	}
	
	// ------------------------------------------------------------
	void CameraRotationStop()
	{
		if (m_RotatingCamera)
		{
			m_optmenu.m_Options = new GameOptions;
			NumericOptionsAccess numeric;
			Class.CastTo(numeric, NumericOptionsAccess.Cast( m_optmenu.m_Options.GetOption(21) )); //21 = Radial blur option
			numeric.WriteValue(m_BlurValue); //restores original rotation blur value
			
			m_RotatingCamera = false;
		}
	}
	
	// ------------------------------------------------------------
	void CameraRotate()
	{
		if (m_DemoUnit && m_RotatingCamera)
		{
			int actual_mouse_x_camera;
			int actual_mouse_y;
			g_Game.GetMousePos(actual_mouse_x_camera, actual_mouse_y);

			m_DiffX = m_RotatingCharacterMouseX * 0.05 - actual_mouse_x_camera * 0.05;//obvod vysece

			m_Angle = m_DiffX;//Math.DEG2RAD*(Math.NormalizeAngle(Math.RAD2DEG*(((diff_x - (2 * r)) / r)))); //in Rad
			m_NewX = Math.Cos(m_Angle + m_Angle_offset) * m_Radius;
			m_NewZ = Math.Sin(m_Angle + m_Angle_offset) * m_Radius;
			
			//set camera pos and dir
			m_Camera.SetPosition(Vector(m_DemoPos[0] + m_NewX, m_DemoPos[1] + 2, m_DemoPos[2] + m_NewZ));
			if(!GetGame().GetUIManager().IsMenuOpen(MENU_OPTIONS))
			{
				// obsolete
				m_Camera.LookAt(Vector(m_DemoPos[0] + Math.Cos(m_Angle + m_Angle_offset + Math.PI*4/3), m_DemoPos[1] + 0.75, m_DemoPos[2] + Math.Sin(m_Angle + m_Angle_offset + Math.PI*4/3)));
			}
			m_Camera.SetFocus(m_Radius, 0); //(distance, blur)
			//Print(m_Camera.GetPosition());
		}
	}

	//camera zoom
	void ZoomCamera(Widget w, int wheel)
	{
		if (w.GetName() == "CharacterRotationFrame" && m_DemoUnit && m_Camera)
		{
			m_DeltaX = Math.AbsFloat(m_DemoPos[0] - m_Camera.GetPosition()[0]);
			m_DeltaZ = Math.AbsFloat(m_DemoPos[2] - m_Camera.GetPosition()[2]);
			
			m_Radius = Math.Clamp(wheel/5 + Math.Sqrt (Math.Pow(m_DeltaX, 2) + Math.Pow(m_DeltaZ, 2)), m_Radius_original*0.75, m_Radius_original*1.5);
			
			m_NewX = Math.Cos(m_Angle + m_Angle_offset) * m_Radius;
			m_NewZ = Math.Sin(m_Angle + m_Angle_offset) * m_Radius;
			m_Camera.SetPosition(Vector(m_DemoPos[0] + m_NewX, m_DemoPos[1] + 2, m_DemoPos[2] + m_NewZ));
		}
	}
	
	//preload of characters, may stay created! TODO cleanup
	void PreloadCharacters()
	{
		string path = "cfgVehicles";
		string child_name = ""; 
		int count = GetGame().ConfigGetChildrenCount ( path );
		Man character;
		vector preloadPos = Vector(m_DemoPos[0], m_DemoPos[1] - 10, m_DemoPos[2]);
		
		for (int p = 0; p < count; p++)
		{
			GetGame().ConfigGetChildName ( path, p, child_name );
			
			if (GetGame().ConfigGetInt(path + " " + child_name + " scope") == 2 && GetGame().IsKindOf(child_name,"SurvivorBase"))
			{
				GetGame().PreloadObject(child_name, 1.0);
				Class.CastTo(character, GetGame().CreateObject(child_name, preloadPos));
				m_Preloaded.Insert(character);
				if(character)
				{
					character.SetPosition(preloadPos);
					character.DisableSimulation(false);
				}
			}
		}
	}
	
	void SaveCharName()
	{
		//TODO cleanup, spaghetti stuff
		g_Game.InitCharacterMenuDataInfo(m_data.GetCharactersCount());
		if (!g_Game.IsNewCharacter() && m_currentCharacterID > -1)
		{
			m_data.SetCharacterName(m_currentCharacterID, g_Game.GetPlayerGameName());
			//Print(g_Game.GetPlayerGameName());
		}
		/*if (m_isNewCharacter || m_currentCharacterID == -1)
		{
			g_Game.InitCharacterMenuDataInfo(m_data.GetCharactersCount(), g_Game.m_PlayerName);
		}
		else
		{
			//m_data.SetCharacterName(m_currentCharacterID, g_Game.m_PlayerName);
		}*/
	}
	
	void SaveCharacterSetup()
	{
		int topIndex; 
		int bottomIndex;
		int shoesIndex;
		int characterIndex;
		
		if( m_DemoUnit )
		{
			Object obj = m_DemoUnit.GetInventory().FindAttachment(InventorySlots.BODY);
			if( obj )
				topIndex = m_CharShirtList.Find( obj.GetType() );
			
			obj = m_DemoUnit.GetInventory().FindAttachment(InventorySlots.LEGS);
			if( obj )
				bottomIndex = m_CharPantsList.Find( obj.GetType() );
			
			obj = m_DemoUnit.GetInventory().FindAttachment(InventorySlots.FEET);
			if( obj )
				shoesIndex = m_CharShoesList.Find( obj.GetType() );
			
			characterIndex = GetGame().ListAvailableCharacters().Find( m_DemoUnit.GetType() );
		}		
		
		//saves player' type and clothes to g_Game to sync with server
		GetGame().SetCharacterInfo(topIndex, bottomIndex, shoesIndex, characterIndex);
	}
};
