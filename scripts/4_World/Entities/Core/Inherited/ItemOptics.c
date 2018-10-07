class ItemOptics extends InventoryItemSuper
{
	bool 				m_data_set;
	bool 				m_allowsDOF;
	int 				m_reddot_index;
	float 				m_blur_float;
	string 				m_optic_sight_texture;
	string 				m_optic_sight_material;
	ref array<float> 	m_mask_array;
	ref array<float> 	m_lens_array;
		
	void ItemOptics()
	{
		m_mask_array = new array<float>;
		m_lens_array = new array<float>;
		InitReddotData();
		InitOpticsPPInfo();;
	}
	/**@fn		EnterOptics
	 * @brief	switches to optics mode if possible
	 * @return true if success, false otherwise
	 **/
	proto native bool EnterOptics ();
	/**@fn		IsInOptics
	 * @brief	is weapon in optics mode or not
	 * @return true if in optics mode, false otherwise
	 **/
	proto native bool IsInOptics ();
	/**@fn		ExitOptics
	 * @brief	switches out of optics mode (if possible)
	 * @return true if success, false otherwise
	 **/
	proto native bool ExitOptics ();

	/**@fn		GetStepFOVCount
	 * @brief	returns number of configured steps
	 **/
	proto native int GetStepFOVCount ();
	/**@fn		GetStepZoom
	 * @brief	returns position of currently used value in discreteFov config array
	 * @return	position of currently used value in discreteFov config array
	 **/
	proto native int GetStepFOVIndex ();
	/**@fn		SetStepZoom
	 * @brief sets zoom to fov value defined at given position in discreteFov config array
	 * @param[in] index of configured step, range [0..cfg_max]
	 * @return	true if zoom set
	 **/
	proto native bool SetStepFOVIndex (int step);
	/**@fn		StepZoomIn
	 * @brief sets zoom to next defined (respective to current) value in zoom fov config array
	 * @return	true if zoom set
	 **/
	proto native bool StepFOVUp ();
	/**@fn		StepZoomOut
	 * @brief	sets zoom to previous (respective to current) defined value in zoom fov config array
	 * @return	true if zoom set
	 **/
	proto native bool StepFOVDown ();

	/**@fn		GetCurrentStepFOV
	 * @brief	returns fov at current index, or _opticInfo._opticsZoomInit
	 **/
	proto native float GetCurrentStepFOV ();

	/**@fn		GetStepZeroing
	 * @brief	returns position of currently used value in zeroing config array
	 * @return	 position of currently used value in zeroing config array
	 **/
	proto native int GetStepZeroing ();
	/**@fn		SetZeroing
	 * @brief sets zeroing to value defined at given position in zeroing config array
	 * @param[in] index of zeroing to set [0..cfg_max]
	 * @return	true if zeroing set
	 **/
	proto native bool SetStepZeroing (int step);
	/**
	 * @fn		StepZeroingUp
	 * @brief sets zeroing to next defined (respective to current) value in zeroing config array
	 * @return true if zeroing set
	 **/
	proto native bool StepZeroingUp ();
	/**
	 * @fn		StepZeroingDown
	 * @brief	sets zeroing to previous (respective to current) defined value in zeroing config array
	 * @return true if zeroing set
	 **/
	proto native bool StepZeroingDown ();
	
	proto native void GetCameraPoint (out vector pos, out vector dir);
	
	/*override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);
		
		// could maybe just ask for energy component on item?
		if (slot_name == "BatteryD")
		{
			item.GetCompEM().SwitchOn();
		}
	}*/
	
	/*override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		
		if (slot_name == "BatteryD")
		{
			item.GetCompEM().SwitchOff();
		}
	}*/
	
	override void OnWorkStart()
	{
		ShowReddot(true);
	}
	
	override void OnWorkStop()
	{
		ShowReddot(false);
	}
	
	override void OnWasAttached( EntityAI parent, int slot_id )
	{
		super.OnWasAttached(parent, slot_id);
		
		SetTakeable(false);
	}

	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		super.OnWasDetached(parent, slot_id);
		
		PlayerBase player;
		if (PlayerBase.CastTo(player, GetHierarchyRootPlayer()))
		{
			player.SetReturnToOptics(false);
		}
		
		SetTakeable(true);
	}
	
	override void OnInventoryExit(Man player)
	{
		super.OnInventoryExit(player);
		
		PlayerBase playerPB;
		if (PlayerBase.CastTo(playerPB, player))
		{
			playerPB.SetReturnToOptics(false);
		}
		
		SetTakeable(true);
	}
	
	void InitReddotData()
	{
		string path = "cfgVehicles " + GetType() + " OpticsInfo";
		string temp;
		
		if (GetGame().ConfigIsExisting(path))
		{
			m_reddot_index = GetHiddenSelectionIndex("reddot");
			if (GetGame().ConfigIsExisting(path + " opticSightTexture"))
			{
				GetGame().ConfigGetText(path + " opticSightTexture", temp);
				m_optic_sight_texture = temp;
				temp = "";
			}
			if (GetGame().ConfigIsExisting(path + " opticSightMaterial"))
			{
				GetGame().ConfigGetText(path + " opticSightMaterial", temp);
				m_optic_sight_material = temp;
				temp = "";
			}
		}
		m_data_set = true;
	}
	
	void ShowReddot(bool state)
	{
		if (!m_data_set)
			InitReddotData();
		
		// does not have reddot
		if (m_reddot_index == -1)
			return;
		
		if (state)
		{
			if (m_optic_sight_texture != "")
				SetObjectTexture(m_reddot_index, m_optic_sight_texture);
			if (m_optic_sight_material != "")
				SetObjectMaterial(m_reddot_index, m_optic_sight_material);
		}
		else
		{
			SetObjectTexture(m_reddot_index, "");
			SetObjectMaterial(m_reddot_index, "");
		}
	}
	
	void InitOpticsPPInfo()
	{
		m_allowsDOF = InitDOFAvailability();
		InitOpticsPP(m_mask_array, m_lens_array, m_blur_float);
	}
	
	//! optics with more than 1x zoom do not allow DOF changes
	bool InitDOFAvailability()
	{
		float fov_max;
		string path = "cfgVehicles " + GetType() + " OpticsInfo";
		
		/*
		Weapon_Base weapon = Weapon_Base.Cast(GetHierarchyParent());
		if (!weapon)
			return false; // no DOF for handheld optics
		*/
		fov_max = GetGame().ConfigGetFloat(path + " opticsZoomMax");
		if (fov_max >= DZPLAYER_CAMERA_FOV_IRONSIGHTS)
		{
			return true;
		}
		return false;
	}
	
	//! initializes values for optics' post-processes
	void InitOpticsPP(out array<float> mask_array, out array<float> lens_array, out float blur_float)
	{
		string path = "cfgVehicles " + GetType() + " OpticsInfo";
		GetGame().ConfigGetFloatArray(path + " PPMaskProperties", mask_array);
		GetGame().ConfigGetFloatArray(path + " PPLensProperties", lens_array);
		blur_float = GetGame().ConfigGetFloat(path + " PPBlurProperties");
	}
	
	bool AllowsDOF()
	{
		return m_allowsDOF;
	}
	
	ref array<float> GetOpticsPPMask()
	{
		return m_mask_array;
	}
	ref array<float> GetOpticsPPLens()
	{
		return m_lens_array;
	}
	float GetOpticsPPBlur()
	{
		return m_blur_float;
	}
};	

typedef ItemOptics OpticBase;


