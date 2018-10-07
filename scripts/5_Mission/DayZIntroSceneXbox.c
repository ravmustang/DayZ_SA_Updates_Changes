class DayZIntroSceneXbox: Managed
{
	protected bool m_IsCharFemale;
	protected int m_LastShavedSeconds;
	protected int m_LastPlayedCharacterID;
	
	protected ref TStringAdvanceArray 	m_genderList;
	protected ref TStringAdvanceArray 	m_CharPersonalityMaleList;
	protected ref TStringAdvanceArray 	m_CharPersonalityFemaleList;
	protected ref TStringAdvanceArray 	m_CharShirtList;
	protected ref TStringAdvanceArray 	m_CharPantsList;
	protected ref TStringAdvanceArray 	m_CharShoesList;
	protected ref TStringArray			m_AllCharacters;
	
	protected ref EntityAnimEndEventHandler 	m_anim_end_event_handler;

	protected Camera		m_SceneCamera;
	protected PlayerBase	m_SceneCharacter;
	protected Weather		m_Weather;
	protected vector		m_CharacterPos;
	protected vector		m_CharacterDir;
	protected ref TIntArray m_Date = new TIntArray;

	protected MenuData m_MenuData;
	
	protected ref MenuCarEngineSmoke	m_FXParticleCarSmoke;
	protected ref MenuEvaporation		m_FXParticleStreamLeft;
	protected ref MenuEvaporation		m_FXParticleStreamRight;
	
	ref Timer m_TimerUpdate = new Timer( CALL_CATEGORY_GAMEPLAY );
	ref Timer m_TimerParticle = new Timer( CALL_CATEGORY_GAMEPLAY );
	ref Timer m_TimerDate = new Timer( CALL_CATEGORY_GAMEPLAY );

	//==================================
	// DayZIntroSceneXbox
	//==================================
	void DayZIntroSceneXbox()
	{
		Print("DayZIntroSceneXbox Start");
		
		m_MenuData = g_Game.GetMenuData();
		m_MenuData.LoadCharacters();
		
		m_LastPlayedCharacterID = m_MenuData.GetLastPlayedCharacter();
		m_CharacterPos = "0 0 0";
		m_CharacterDir = "0 0 0";
		m_LastShavedSeconds = 0;
		
		//g_Game.m_PlayerName = "Survivor"; //default
		if( m_MenuData.GetCharactersCount() == 0 )
		{
			m_LastPlayedCharacterID = -1;
		}
		
		if ( m_LastPlayedCharacterID > -1 )
		{
			m_MenuData.GetCharacterName(m_LastPlayedCharacterID, g_Game.GetPlayerGameName());
		}
	
		// Camera Setup
		vector camera_position;
		camera_position[0] 			= 1323.0;	// X
		camera_position[1] 			= 1.0;		// Y
		camera_position[2] 			= 1590.37;	// Z
		float camera_rotation_h		= 100;
		float camera_rotation_v		= -3;
		float camera_fov			= 0.85;
		float camera_focus_distance	= 0.0;
		float camera_focus_streght	= 0.0;
		
		// Character
		float character_distance = 2.1;
		
		// Date		
		m_Date.Insert(2017);	// Year
		m_Date.Insert(10);	// Month
		m_Date.Insert(11);	// Day
		m_Date.Insert(9);	// Hour
		m_Date.Insert(20);	// Minite
		
		// Weather
		float weather_overcast			= 0.45;
		float weather_rain				= 0.0;
		float weather_fog				= 0.0;
		float weather_storm_density		= 0.0;
		float weather_storm_threshold	= 0.0;
		float weather_storm_time_out	= 0.0;
		float weather_windspeed			= 0.3;
		
		// Weather Setup 
		m_Weather = g_Game.GetWeather();
		m_Weather.GetOvercast().SetLimits( weather_overcast, weather_overcast );
		m_Weather.GetOvercast().SetForecastTimeLimits(weather_overcast, weather_overcast);
		m_Weather.GetOvercast().Set( weather_overcast, 1.0, 1000);		
		m_Weather.GetOvercast().SetNextChange( 1 );
		m_Weather.GetRain().SetLimits( weather_rain, weather_rain );
		m_Weather.GetRain().Set( weather_rain, 0, 1000);
		m_Weather.GetFog().SetLimits( weather_fog, weather_fog );
		m_Weather.GetFog().Set( weather_fog, 0, 1000);
		m_Weather.SetStorm(weather_storm_density, weather_storm_threshold, weather_storm_time_out);
		m_Weather.SetWindSpeed(weather_windspeed);
		m_Weather.SetWindMaximumSpeed(weather_windspeed);
		m_Weather.SetWindFunctionParams(1, 1, 1);
		
		// Date Setup - Clouds HACK
		g_Game.GetWorld().SetDate(2020, 10, 15, 9, 00);
		
		// Camera Setup
		m_SceneCamera = CameraCreate(camera_position, camera_rotation_h, camera_rotation_v, camera_fov, camera_focus_distance, camera_focus_streght);
		m_SceneCamera.SetActive(true);
		
		//Vignette
		PPEffects.SetVignette(0.3, 0, 0, 0);
		
		// Character Setup
		vector cam_dir = m_SceneCamera.GetDirection();
		m_CharacterPos = camera_position + ( cam_dir * character_distance );
		m_CharacterPos[1] = GetGame().SurfaceY(m_CharacterPos[0], m_CharacterPos[2]);
		m_CharacterDir = (camera_position - m_CharacterPos);
		
		Init();
	
		m_TimerParticle.Run(0.1, this, "SetupParticles", NULL, false);
		m_TimerDate.Run(1.0, this, "SetupDate", NULL, true);
		m_TimerUpdate.Run(0.5, this, "SetupCharacter", NULL, true);
		
		Material material = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/chromaber");
		material.SetParam("PowerX", 0.002);
		material.SetParam("PowerY", 0.002);
	}
	
	void ~DayZIntroSceneXbox()
	{
		if ( m_TimerUpdate )
		{
			m_TimerUpdate.Stop();
			delete m_TimerUpdate;
			m_TimerUpdate = null;
		}
		
		if ( m_TimerParticle )
		{
			m_TimerParticle.Stop();
			delete m_TimerParticle;
			m_TimerParticle = null;
		}
		
		
		if ( m_TimerDate )
		{
			m_TimerDate.Stop();
			delete m_TimerDate;
			m_TimerDate = null;
		}
		
		GetGame().ObjectDelete( m_SceneCharacter );
		GetGame().ObjectDelete( m_SceneCamera );
		m_MenuData.ClearCharacters();
		Material material = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/chromaber");
		material.SetParam("PowerX", 0.0);
		material.SetParam("PowerY", 0.0);
	}
	
	//==================================
	// Init
	//==================================
	void Init()
	{
		//fill default lists
		m_genderList = new TStringAdvanceArray;
		m_CharPersonalityMaleList = new TStringAdvanceArray;
		m_CharPersonalityFemaleList = new TStringAdvanceArray;
		m_AllCharacters = new TStringArray;
		m_CharShirtList = new TStringAdvanceArray;
		m_CharPantsList = new TStringAdvanceArray;
		m_CharShoesList = new TStringAdvanceArray;
		
		string character_CfgName;
		string root_path = "cfgCharacterCreation";
		
		g_Game.ConfigGetTextArray(root_path + " gender", m_genderList);
		g_Game.ConfigGetTextArray(root_path + " top", m_CharShirtList);
		g_Game.ConfigGetTextArray(root_path + " bottom", m_CharPantsList);
		g_Game.ConfigGetTextArray(root_path + " shoe", m_CharShoesList);

		m_AllCharacters = GetGame().ListAvailableCharacters();
		for (int i = 0; i < m_AllCharacters.Count(); i++)
		{
			character_CfgName = m_AllCharacters.Get(i);
			if (GetGame().IsKindOf(character_CfgName, "SurvivorMale_Base"))
			{
				m_CharPersonalityMaleList.Insert(character_CfgName);
			}
			else
			{
				m_CharPersonalityFemaleList.Insert(character_CfgName);
			}
		}
		
		ChangeCharacter(m_LastPlayedCharacterID);
		
		PPEffects.Init();
		PPEffects.SetBlur(0);
		PPEffects.DisableBurlapSackBlindness(); //HOTFIX
	}
	
	//==================================
	// SetupCharacter
	//==================================
	void SetupCharacter()
	{
		if ( m_SceneCharacter )
		{
			vector v = m_SceneCharacter.GetOrientation();
			v[0] = -75;
			m_SceneCharacter.SetOrientation(v);
			m_SceneCharacter.SetPosition(m_CharacterPos);
		}
	}
	
	//==================================
	// SetupParticles
	//==================================
	void SetupParticles()
	{
		m_FXParticleCarSmoke = new MenuCarEngineSmoke();
		//SEffectManager.PlayInWorld(m_FXParticleCarSmoke, "1330.36 2.11628 1594.31");
		//SEffectManager.PlayInWorld(m_FXParticleCarSmoke, "1333.88 1.51392 1594.88");
		SEffectManager.PlayInWorld(m_FXParticleCarSmoke, "1331.52 2.34052 1593.55");
		
		vector pos = m_SceneCamera.GetPosition() + m_SceneCamera.GetDirection() * 1.5;
		vector dir = m_SceneCamera.GetDirection();
		float temp = dir[0];
		dir[0] = dir[2];
		dir[2] = -temp;
			
		vector pos_right = pos + (dir * 1.5);
		vector pos_left = pos + (-dir * 1.5);
		
		m_FXParticleStreamLeft = new MenuEvaporation();
		SEffectManager.PlayInWorld(m_FXParticleStreamLeft, pos_right);
		
		m_FXParticleStreamRight = new MenuEvaporation();
		SEffectManager.PlayInWorld(m_FXParticleStreamRight, pos_left);
	}
	
	//==================================
	// SetupDate
	//==================================
	void SetupDate()
	{
		g_Game.GetWorld().SetDate(m_Date.Get(0), m_Date.Get(1), m_Date.Get(2), m_Date.Get(3), m_Date.Get(4));
	}
	
	//==================================
	// CameraCreate
	//==================================
	protected Camera CameraCreate(vector cam_pos, float cam_rot_h, float cam_rot_v, float cam_fov, float cam_focus_dist, float cam_focus_strg)
	{
		Camera cam = Camera.Cast( g_Game.CreateObject("staticcamera", SnapToGround(cam_pos), true));
		cam.SetOrientation( Vector( cam_rot_h, cam_rot_v, 0) );
		cam.SetFOV( cam_fov );
		cam.SetFocus(cam_focus_dist, cam_focus_strg);
		
		return cam;
	}
	
	//==================================
	// GetCamera
	//==================================
	Camera GetCamera()
	{
		return m_SceneCamera;
	}
	
	//==================================
	// ResetIntroCamera
	//==================================
	void ResetIntroCamera()
	{
		
	}
	
	//==================================
	// SetCharacterFemale
	//==================================
	void SetCharacterFemale(bool fem)
	{
		m_IsCharFemale = fem;
	}
	
	//==================================
	// IsCharacterFemale
	//==================================
	bool IsCharacterFemale()
	{
		return m_IsCharFemale;
	}
	
	//==================================
	// GetIntroSceneCharacter
	//==================================
	PlayerBase GetIntroSceneCharacter()
	{
		return m_SceneCharacter;
	}
	
	//==================================
	// CreateRandomCharacter
	//==================================
	void CreateRandomCharacter()
	{
		string character_name;
		string params[2];
		
		m_IsCharFemale = Math.RandomInt(0, 2);
		
		if (m_IsCharFemale)
		{
			character_name = m_CharPersonalityFemaleList.GetRandomElement();
		}
		else
		{
			character_name = m_CharPersonalityMaleList.GetRandomElement();
		}
		CreateNewCharacter(character_name);
		
		if (m_SceneCharacter)
		{
			SetAttachment(m_CharShirtList.GetRandomElement(), InventorySlots.BODY);
			SetAttachment(m_CharPantsList.GetRandomElement(), InventorySlots.LEGS);
			SetAttachment(m_CharShoesList.GetRandomElement(), InventorySlots.FEET);
		}
		
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateCharacterPos, 250);
	}
	
	void SetAttachment(string type, int slot)
	{
		if (!m_SceneCharacter) return;
		g_Game.ObjectDelete(m_SceneCharacter.GetInventory().FindAttachment(slot));
		EntityAI entity;
		Class.CastTo(entity, g_Game.CreateObject(type, "0 2000 0", true));
		if (entity)
		{
			m_SceneCharacter.LocalTakeEntityAsAttachmentEx(entity, slot);
		}
	}
	
	void UpdateSelectedUserName()
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
	
	// ------------------------------------------------------------
	void ChangeCharacter(int characterID)
	{
		string name;
		if (m_SceneCharacter)
		{
			g_Game.ObjectDelete(m_SceneCharacter);
			m_SceneCharacter = NULL;
		}
		m_LastPlayedCharacterID = characterID;
		BiosUserManager user_manager;
		
		//random character
		if(characterID == - 1)
		{
			UpdateSelectedUserName();

			CreateRandomCharacter();
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateCharacterPos, 250);
			g_Game.SetNewCharacter(true);
			return;
		}
		
		m_SceneCharacter = PlayerBase.Cast(m_MenuData.CreateCharacterPerson(characterID));
		
		if (m_SceneCharacter)
		{
			g_Game.SetNewCharacter(false);
			m_SceneCharacter.PlaceOnSurface();
			m_SceneCharacter.SetPosition( m_CharacterPos );
			m_SceneCharacter.SetDirection(m_CharacterDir);
			m_SceneCharacter.SetEventHandler(m_anim_end_event_handler);
			m_SceneCharacter.SetLastShavedSeconds(m_LastShavedSeconds);
	
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateCharacterPos, 250);
		}

		UpdateSelectedUserName();

		g_Game.SetPlayerGameName(name);
	}
	
	// ------------------------------------------------------------
	void CreateNewCharacter(string type)
	{
		if (m_SceneCharacter)
		{
			g_Game.ObjectDelete(m_SceneCharacter);
			m_SceneCharacter = NULL;
		}

		g_Game.PreloadObject(type, 1.0);
		Class.CastTo(m_SceneCharacter, g_Game.CreateObject(type, m_CharacterPos, true));
		Print("CreateNewCharacter");
		
		if (m_SceneCharacter)
		{
			m_SceneCharacter.PlaceOnSurface();
			m_SceneCharacter.SetDirection(m_CharacterDir);
			m_SceneCharacter.SetEventHandler(m_anim_end_event_handler);
			m_SceneCharacter.SetLastShavedSeconds(m_LastShavedSeconds);

			// NEW STATS API
			string lvalue = "";
			m_SceneCharacter.StatGetCounter("playtime", lvalue);

			PluginLifespan module_lifespan;
			Class.CastTo(module_lifespan, PluginLifespan.Cast( GetPlugin( PluginLifespan ) ));
			module_lifespan.UpdateLifespanParam( m_SceneCharacter, lvalue, true );
		}
	}
	
	void UpdateCharacterPos()
	{
		if (m_SceneCharacter)
		{
			m_SceneCharacter.SetPosition(m_CharacterPos);
			m_SceneCharacter.SetDirection(m_CharacterDir.Normalized() );
		}
	}
	
	void SaveCharName()
	{
		GetDayZGame().InitCharacterMenuDataInfo(m_MenuData.GetCharactersCount());
		
		if (!GetDayZGame().IsNewCharacter() && m_LastPlayedCharacterID > -1)
		{
			m_MenuData.SetCharacterName(m_LastPlayedCharacterID, GetDayZGame().GetPlayerGameName());			
		}
	}
	
	// ------------------------------------------------------------
	void Update()
	{
	
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
};
