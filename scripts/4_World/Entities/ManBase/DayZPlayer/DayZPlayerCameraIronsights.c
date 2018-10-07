
//! this is main camera class 
class DayZPlayerCameraIronsights extends DayZPlayerCameraBase
{
	static const float 	CONST_UD_MIN	= -85.0;		//!< down limit
	static const float 	CONST_UD_MAX	= 85.0;			//!< up limit

	static const float 	CONST_LR_MIN	= -160.0;		//!< down limit
	static const float 	CONST_LR_MAX	= 160.0;			//!< up limit
	
	protected 	bool 			m_isEntering 	= false;
	protected 	Weapon_Base		m_weaponUsed;
	protected 	ItemOptics 		m_opticsUsed;
	protected ref array<float> 	temp_array;

	void 	DayZPlayerCameraIronsights(DayZPlayer pPlayer, HumanInputController pInput)
	{
		if (!temp_array)
			temp_array = new array<float>;
		m_iBoneIndex = pPlayer.GetBoneIndexByName("RightHand_Dummy");
		if (m_iBoneIndex == -1)
		{
			Print("DayZPlayerCamera1stPerson: main bone not found");
		}

		GetCurrentSightInfo(m_OpticsCamPos, m_OpticsCamDir);
		m_isEntering = true;
		m_WeaponSwayModifier = 0.2;
	}
	
	//
	override void 		OnActivate (DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult)
	{
		super.OnActivate(pPrevCamera,pPrevCameraResult);
		vector 	f = pPrevCamera.GetBaseAngles();
		m_fUpDownAngle		= f[0]; 
		m_fLeftRightAngle	= f[1]; 
		m_fUpDownAngleAdd	= f[2];
	}

	EntityAI GetCurrentSightEntity ()
	{
		EntityAI inHands = m_pPlayer.GetHumanInventory().GetEntityInHands();
		if (inHands)
		{
			m_weaponUsed = Weapon_Base.Cast(inHands);
			return m_weaponUsed;
		}
		return null;
	}

	bool GetCurrentSightInfo (out vector camPos, out vector camDir)
	{
		EntityAI e = GetCurrentSightEntity();
		if (e)
		{
			m_weaponUsed = Weapon_Base.Cast(e);
			m_weaponUsed.GetCameraPoint(m_weaponUsed.GetCurrentMuzzle(), camPos, camDir);
			return true;
		}
		return false;
	}

	//	
	override void 		OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		//! update angles from input 
		float 	udAngle 	= UpdateUDAngle(m_fUpDownAngle, m_fUpDownAngleAdd, CONST_UD_MIN, CONST_UD_MAX, pDt);
		m_fLeftRightAngle	= UpdateLRAngle(m_fLeftRightAngle, CONST_LR_MIN, CONST_LR_MAX, pDt);

		// get model space transform matrix of the gun's eye vector 
		HumanItemAccessor hia = m_pPlayer.GetItemAccessor();
		if(GetCurrentSightEntity())
			hia.WeaponGetCameraPointBoneRelative(GetCurrentSightEntity(), m_OpticsCamPos, m_OpticsCamDir, m_iBoneIndex, pOutResult.m_CameraTM);
	
		AdjustCameraParameters(pDt, pOutResult);
	}

	//
	override vector GetBaseAngles()
	{
		vector a;
		a[0] = m_fUpDownAngle;
		a[1] = m_fLeftRightAngle;
		a[2] = m_fUpDownAngleAdd;
		return a;
	}
	
	void AdjustCameraParameters(float pDt, inout DayZPlayerCameraResult pOutResult)
	{
		pOutResult.m_iDirectBone 			= m_iBoneIndex;
		pOutResult.m_iDirectBoneMode 		= 4;
	
		pOutResult.m_fFovAbsolute = HoldBreathFOVEffect(pDt);
		
		pOutResult.m_bUpdateWhenBlendOut	= false;
		pOutResult.m_fDistance 				= 0;
		pOutResult.m_fUseHeading 			= 0;
		pOutResult.m_fInsideCamera 			= 1.0;
		pOutResult.m_fShootFromCamera		= m_fShootFromCamera;
		
		pOutResult.m_fNearPlane = 0.04; //0.07 default
	}
	
	float HoldBreathFOVEffect(float pDt)
	{
		if (m_pPlayer.IsHoldingBreath())
		{
			m_fFovAbsolute = Math.SmoothCD(m_fFovAbsolute, DZPLAYER_CAMERA_FOV_EYEZOOM, m_fFovAbsVel, 0.1, 1000, pDt);
		}
		else if (m_isEntering) //sets FOV immediatelly to avoid "floating camera" effect
		{
			m_fFovAbsolute = DZPLAYER_CAMERA_FOV_IRONSIGHTS;
			m_isEntering = false;
		}
		else
		{
			m_fFovAbsolute = Math.SmoothCD(m_fFovAbsolute, DZPLAYER_CAMERA_FOV_IRONSIGHTS, m_fFovAbsVel, 0.1, 1000, pDt);
		}

		return m_fFovAbsolute;
	}
	
	// sets PP for ironsights and optics (override) cameras
	override void SetCameraPP(bool state, DayZPlayerCamera launchedFrom)
	{
		if (!state || !m_weaponUsed || (PlayerBase.Cast(m_pPlayer) && launchedFrom != PlayerBase.Cast(m_pPlayer).m_CurrentCamera))
		{
			PPEffects.ResetPPMask();
			PPEffects.SetLensEffect(0, 0, 0, 0);
			PPEffects.OverrideDOF(false, 0, 0, 0, 0, 0);
			PPEffects.SetBlurOptics(0);
		}
		else
		{
			PPEffects.ResetPPMask();
			//PPEffects.AddPPMask(0.5, 0.5, 0.3, 0.05);
			PPEffects.SetLensEffect(0, 0, 0, 0);
			if (m_weaponUsed.GetWeaponDOF())
			{
				//temp_array = new array<float>;
				//temp_array.Clear();
				temp_array = m_weaponUsed.GetWeaponDOF();
				if (temp_array.Count() == 6)
				{
					#ifdef PLATFORM_WINDOWS
						PPEffects.OverrideDOF(temp_array[0],temp_array[1],temp_array[2],temp_array[3],temp_array[4],temp_array[5]);
					#endif
				}
			}
			else
				PPEffects.OverrideDOF(false, 0, 0, 0, 0, 0);
			PPEffects.SetBlurOptics(0);
		}
	}
	
	override string GetCameraName()
	{
		return "DayZPlayerCameraIronsights";
	}
	
	override void SetCameraPPDelay(DayZPlayerCamera pPrevCamera)
	{
		if (pPrevCamera.GetCameraName() == "DayZPlayerCamera1stPerson")
		{
			//Print("---ironsights---DayZPlayerCamera1stPerson");
			m_CameraPPDelay = DayZPlayerCameras.TIME_CAMERACHANGE_01;
		}
		else
		{
			//Print("---ironsights---else");
			m_CameraPPDelay = 0; //DayZPlayerCameras.TIME_CAMERACHANGE_02;
		}
	}

	//! settings
	protected	float	m_fShootFromCamera = 0.0;
	
	//! runtime values 
	protected	int 	m_iBoneIndex;				//!< right hand dummy bone index
	protected	vector	m_OpticsCamPos;
	protected	vector	m_OpticsCamDir;
	protected 	float 	m_fUpDownAngle;				//!< up down angle in rad
	protected 	float 	m_fUpDownAngleAdd;			//!< up down angle in rad
	protected 	float 	m_fLeftRightAngle;			//!< left right angle in rad (in freelook only)

}

class DayZPlayerCameraOptics : DayZPlayerCameraIronsights
{
	void DayZPlayerCameraOptics(DayZPlayer pPlayer, HumanInputController pInput)
	{
		if (!temp_array)
			temp_array = new array<float>;
		m_fShootFromCamera = 2.0;
	}
	
	override EntityAI GetCurrentSightEntity ()
	{
		EntityAI inHands = m_pPlayer.GetHumanInventory().GetEntityInHands();
		if (inHands)
		{
			m_opticsUsed = ItemOptics.Cast(inHands);
			if (m_opticsUsed)
				return m_opticsUsed;
			else
			{
				m_weaponUsed = Weapon_Base.Cast(inHands);
				if (m_weaponUsed)
				{
					m_opticsUsed = m_weaponUsed.GetAttachedOptics();
					return m_opticsUsed;
				}
			}
		}
		return null;
	}

	override bool GetCurrentSightInfo (out vector camPos, out vector camDir)
	{
		//super.GetCurrentSightInfo(inout camPos, inout CamDir);		
		
		EntityAI e = GetCurrentSightEntity();
		if (e)
		{
			m_opticsUsed = ItemOptics.Cast(e);
			m_opticsUsed.GetCameraPoint(camPos, camDir);
			return true;
		}
		return false;
	}
	
	override void AdjustCameraParameters(float pDt, inout DayZPlayerCameraResult pOutResult)
	{
		pOutResult.m_iDirectBone 			= m_iBoneIndex;
		pOutResult.m_iDirectBoneMode 		= 4;
	
		pOutResult.m_fFovAbsolute = HoldBreathFOVEffect(pDt);
		
		pOutResult.m_bUpdateWhenBlendOut	= false;
		pOutResult.m_fDistance 				= 0;
		pOutResult.m_fUseHeading 			= 0;
		pOutResult.m_fInsideCamera 			= 1.0;
		pOutResult.m_fShootFromCamera		= m_fShootFromCamera;
		
		pOutResult.m_fNearPlane = 0.06; //0.07 default
	}
	
	override float HoldBreathFOVEffect(float pDt)
	{
		ItemOptics optics = ItemOptics.Cast( GetCurrentSightEntity() );
		if (optics)
		{
			if (optics.GetCurrentStepFOV() >= DZPLAYER_CAMERA_FOV_EYEZOOM && (m_pPlayer.IsHoldingBreath() || m_pPlayer.IsEyeZoom()))
			{
				m_fFovAbsolute = Math.SmoothCD(m_fFovAbsolute, DZPLAYER_CAMERA_FOV_EYEZOOM, m_fFovAbsVel, 0.1, 1000, pDt);
			}
			else if (m_isEntering) //sets FOV immediatelly to avoid "floating camera" effect
			{
				m_fFovAbsolute = optics.GetCurrentStepFOV();
				m_isEntering = false;
			}
			else
			{
				m_fFovAbsolute = Math.SmoothCD(m_fFovAbsolute, optics.GetCurrentStepFOV(), m_fFovAbsVel, 0.1, 1000, pDt);
				//m_fFovAbsolute = optics.GetCurrentStepFOV();
			}
			return m_fFovAbsolute;
		}
		else
		{
			Error("optic camera, but there is no optic item or optic on weapon");
			return 0.6;  //TODO figure some other way to get original FOV here!
		}
	}
	
	override void SetCameraPP(bool state,DayZPlayerCamera launchedFrom)
	{
		//Print("-----optics camera-----");
		if (!state || !m_opticsUsed || (PlayerBase.Cast(m_pPlayer) && launchedFrom != PlayerBase.Cast(m_pPlayer).m_CurrentCamera))
		{
			PPEffects.ResetPPMask();
			PPEffects.SetLensEffect(0, 0, 0, 0);
			PPEffects.OverrideDOF(false, 0, 0, 0, 0, 0);
			PPEffects.SetBlurOptics(0);
		}
		else
		{
			PPEffects.ResetPPMask();
			
			// 1x scopes only
			if (m_opticsUsed.AllowsDOF())
			{
				PPEffects.SetLensEffect(0, 0, 0, 0);
				if (m_weaponUsed)
				{
					temp_array = m_weaponUsed.GetWeaponDOF(); //TODO should some optics have own DOF settings (different eye point)?
					if (temp_array.Count() == 6)
						PPEffects.OverrideDOF(temp_array[0],temp_array[1],temp_array[2],temp_array[3],temp_array[4],temp_array[5]);
				}
				else
					PPEffects.OverrideDOF(false, 0, 0, 0, 0, 0);
			}
			else
			{
				// optics mask
				if (m_opticsUsed.GetOpticsPPMask() && m_opticsUsed.GetOpticsPPMask().Count() == 4)
				{
					//temp_array = new array<float>;
					//temp_array.Clear();
					temp_array = m_opticsUsed.GetOpticsPPMask();
					
					PPEffects.AddPPMask(temp_array[0], temp_array[1], temp_array[2], temp_array[3]);
					//PPEffects.AddPPMask(0.5, 0.5, 0.4, 0.05);
				}
				
				//optics lens
				if (m_opticsUsed.GetOpticsPPLens() && m_opticsUsed.GetOpticsPPLens().Count() == 4)
				{
					//temp_array = new array<float>;
					//temp_array.Clear();
					temp_array = m_opticsUsed.GetOpticsPPLens();
					
					PPEffects.SetLensEffect(temp_array[0], temp_array[1], temp_array[2], temp_array[3]);
					//PPEffects.SetLensEffect(1, 0.15, 0, 0 );
				}
				else
				{
					PPEffects.SetLensEffect(0, 0, 0, 0);
				}
				
				//optics DOF (1x optics only)
				if (m_opticsUsed.AllowsDOF())
				{
					//temp_array = new array<float>;
					//temp_array.Clear();
	
				}
				else
				{
					PPEffects.OverrideDOF(false, 0, 0, 0, 0, 0);
				}
				
				//optics blur
				if (m_opticsUsed.GetOpticsPPBlur() != 0)
				{
					#ifdef PLATFORM_WINDOWS
						PPEffects.SetBlurOptics(m_opticsUsed.GetOpticsPPBlur());
					#endif
				}
				else
					PPEffects.SetBlurOptics(0);
			}
			
		}
	}
	
	override string GetCameraName()
	{
		return "DayZPlayerCameraOptics";
	}
	
	override void SetCameraPPDelay(DayZPlayerCamera pPrevCamera)
	{
		if (pPrevCamera.GetCameraName() == "DayZPlayerCameraIronsights")
		{
			//Print("---optics---DayZPlayerCamera1stPerson || DayZPlayerCameraIronsights");
			m_CameraPPDelay = DayZPlayerCameras.TIME_CAMERACHANGE_01;
		}
		else if (pPrevCamera.GetCameraName() == "DayZPlayerCamera1stPerson")
		{
			m_CameraPPDelay = 0; //changed to accomodate handheld optics, had to be changed here also
		}
		else
		{
			//Print("---optics---else");
			m_CameraPPDelay = DayZPlayerCameras.TIME_CAMERACHANGE_015;
		}
	}
};

