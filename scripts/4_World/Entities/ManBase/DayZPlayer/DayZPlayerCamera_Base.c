//! limit function
float 	Limit(float pV, float pMin, float pMax)
{
	if (pV >= pMin && pV <= pMax)
	{
		return pV;
	}
	else if (pV < pMin)
	{
		return pMin;
	}
	else
	{
		return pMax;
	}	
}


float fixAngle_PI_PI(float pAngle)
{
	while (pAngle > Math.PI)
	{

		pAngle -=  Math.PI2;
	}

	while (pAngle < -Math.PI)
	{

		pAngle +=  Math.PI2;
	}

	return pAngle;
}

float fixAngle_180_180(float pAngle)
{
	while (pAngle > 180)
	{

		pAngle -=  360;
	}

	while (pAngle < -180)
	{

		pAngle +=  360;
	}

	return pAngle;
}




class DayZPlayerCameraBase extends DayZPlayerCamera
{
	//! constructor must be same 
	void 	DayZPlayerCameraBase(DayZPlayer pPlayer, HumanInputController pInput)
	{
		m_fLRAngleVel[0] = 0;
		m_fUDAngleVel[0] = 0;
		m_fFovAbsVel[0] = 0;
		m_WeaponSwayModifier = 1;
		
		if (m_pPlayer.IsEyeZoom())
		{
			m_fFovAbsolute		= DZPLAYER_CAMERA_FOV_EYEZOOM;
		}
		else
		{
			m_fFovAbsolute		= g_Game.GetUserFOV();
		}
		
		//!
		/*
		{
			int perItemCamUD = pPlayer.GetCurrentPerItemCameraUD();
			string 		a	= "Per Item Camera User Data: " + perItemCamUD.ToString();
			Print(a);
		}
		*/
		m_CameraPPDelay = 0;
	};


	float UpdateUDAngle(out float pAngle, out float pAngleAdd, float pMin, float pMax, float pDt)
	{
		//! lr angle
		if (m_pInput.CameraIsFreeLook())	
		{
			pAngleAdd 	+= m_pInput.GetAimChange()[1] * Math.RAD2DEG;
			pAngleAdd	= Limit(pAngleAdd, pMin, pMax);

			m_fUDAngleVel[0]	= 0;
		}
		else
		{
			//! update it in degrees
			pAngle 	+= m_pInput.GetAimChange()[1] * Math.RAD2DEG;
			pAngle 	= Limit(pAngle, pMin, pMax);

			pAngleAdd	= Math.SmoothCD(pAngleAdd, 0, m_fUDAngleVel, 0.14, 1000, pDt);
		}


		return Limit(pAngle + pAngleAdd, pMin, pMax);
	}


	float UpdateLRAngle(float pAngle, float pMin, float pMax, float pDt)
	{
		//! lr angle
		if (m_pInput.CameraIsFreeLook() || m_bForceFreeLook)	
		{
			pAngle	+= m_pInput.GetAimChange()[0] * Math.RAD2DEG;
			pAngle	= Limit(pAngle, pMin, pMax);

			m_fLRAngleVel[0]	= 0;	// reset filter

		}
		else
		{
			// smooth value back to 0 
			pAngle		= Math.SmoothCD(pAngle, 0, m_fLRAngleVel, 0.14, 1000, pDt);
			// m_fLeftRightAngle	= 0.9 * (1.0 - pDt);			
		}

		return pAngle;
	}

	override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		super.OnUpdate(pDt, pOutResult);

		//! change abs FOV for naked eye zoom
		if (m_pPlayer.IsEyeZoom())
		{
			m_fFovAbsolute = Math.SmoothCD(m_fFovAbsolute, DZPLAYER_CAMERA_FOV_EYEZOOM, m_fFovAbsVel, 0.1, 1000, pDt);
		}
		else
		{
			m_fFovAbsolute = Math.SmoothCD(m_fFovAbsolute, g_Game.GetUserFOV(), m_fFovAbsVel, 0.1, 1000, pDt);
		}

		pOutResult.m_fFovAbsolute = m_fFovAbsolute;

		//! switch shooting from camera to weapon (firearms)
		if (!m_pPlayer.IsShootingFromCamera())
		{
			pOutResult.m_fShootFromCamera = 0.0;
		}
	}

	override void OnActivate(DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult)
	{
		//PrintString("OnActivate DayZPlayerCameraBase");
		PlayerBase.Cast(m_pPlayer).OnCameraChanged(this);
		SetCameraPPDelay(pPrevCamera);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SetCameraPP,m_CameraPPDelay*1000,false,true,this); // this takes care of weapon/optics postprocessing
	}
	
	float GetWeaponSwayModifier()
	{
		return m_WeaponSwayModifier;
	}
	
	override string GetCameraName()
	{
		return "DayZPlayerCameraBase";
	}
	
	void SetCameraPPDelay(DayZPlayerCamera pPrevCamera)
	{
	}
	
	//! by default sets camera PP to zero, regardless of parameter. Override if needed.
	void SetCameraPP(bool state, DayZPlayerCamera launchedFrom)
	{
		PPEffects.ResetPPMask();
		PPEffects.SetLensEffect(0, 0, 0, 0);
		PPEffects.OverrideDOF(false, 0, 0, 0, 0, 0);
		PPEffects.SetBlurOptics(0);
	}
	
	float GetCurrentPitch()
	{
		return m_CurrentCameraPitch;
	}
	
	protected float 	m_fLRAngleVel[1];
	protected float 	m_fUDAngleVel[1];
	protected float		m_fFovAbsVel[1];
	protected float		m_fFovAbsolute;
	protected bool		m_bForceFreeLook;
	protected float		m_WeaponSwayModifier;
	protected float 	m_CameraPPDelay;
	protected float 	m_CurrentCameraPitch;
}
