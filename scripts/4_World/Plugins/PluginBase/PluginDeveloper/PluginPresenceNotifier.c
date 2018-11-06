class PluginPresenceNotifier extends PluginBase
{
	const int windowPosX = 0;
	const int windowPosY = 10;
	const int mainPanelSizeX = 200;
	const int mainPanelSizeY = 1;
	const int margin = 10;
	
	protected static float 		m_LandNoise;
	
	protected PlayerBase 		m_pPlayer;

	void PluginPresenceNotifier() {}
	
	void ~PluginPresenceNotifier() {}

	void UpdatePresenceNotifier(PlayerBase pPlayer, bool pEnabled)
	{
		m_pPlayer = pPlayer;
		
		ShowCoefsDbg(pEnabled);
	}
	
	protected void ShowCoefsDbg(bool pEnabled)
	{
		ref HumanMovementState	hms = new HumanMovementState();
		m_pPlayer.GetMovementState(hms);

		DbgUI.BeginCleanupScope();

		if (pEnabled)
		{
			float visualMean = (m_pPlayer.GetVisibilityCoef() + GetMovementSpeedVisualCoef() + GetMovementStanceVisualCoef()) / 3;
			float noiseMean = (GetSurfaceNoiseCoef() + GetMovementSpeedNoiseCoef() + GetBootsNoiseCoef() + GetLandNoise()) / 4;
			float resultMean = (visualMean + noiseMean) / 2;

			DbgUI.Begin("Presence Notifier", windowPosX + 10, windowPosY);
			DbgUI.Panel("MinimumSize", mainPanelSizeX, mainPanelSizeY);

			DbgUI.Text("Visual: ");
			DbgUI.Text("Visibility: " + m_pPlayer.GetVisibilityCoef());
			DbgUI.Text("Speed: " + GetMovementSpeedVisualCoef());
			DbgUI.Text("Stance: " + GetMovementStanceVisualCoef());
			DbgUI.Spacer(10);
		
			DbgUI.Panel("-- Noises", mainPanelSizeX, 2);
			DbgUI.Text("Noises: ");
			DbgUI.Text("Speed:  " + GetMovementSpeedNoiseCoef());
			DbgUI.Text("Boots: " + GetBootsNoiseCoef());
			DbgUI.Text("Surface: " + GetSurfaceNoiseCoef());
			DbgUI.Spacer(10);

			DbgUI.Panel("-- Result", mainPanelSizeX, 2);
			DbgUI.Text("Result: ");
			DbgUI.Text("Visual coef: " + visualMean);
			DbgUI.Text("Noise coef: " + noiseMean);
			DbgUI.Text("Result coef: " + resultMean);
			DbgUI.Spacer(20);
			DbgUI.Text("Alert Level:");
			DbgUI.Spacer(20);
			DbgUI.SameLine();

			if (resultMean <= 0.5)
			{
				DbgUI.Text("!");
			}
			else if (resultMean <= 0.75)
			{
				DbgUI.Text("!!");
			}
			else
			{
				DbgUI.Text("!!!");
			}
			
			DbgUI.End();
			
			//! HumanMovementState content
			DbgUI.Begin("HumanMovementState", windowPosX + 250, windowPosY);
			DbgUI.Panel("MinimumSize", mainPanelSizeX, mainPanelSizeY);
			DbgUI.Text("Command ID: " + hms.m_CommandTypeId);
			DbgUI.Text("Stance: " + hms.m_iStanceIdx);
			DbgUI.Text("Movement: " + hms.m_iMovement);
			DbgUI.End();
		}

		DbgUI.EndCleanupScope();
	}
	
	
	protected float GetMovementSpeedVisualCoef()
	{
		ref HumanMovementState	hms = new HumanMovementState();
		float speedCoef = 1.0;

		m_pPlayer.GetMovementState(hms);
		switch(AITargetCallbacksPlayer.StanceToMovementIdxTranslation(hms))
		{
			case DayZPlayerConstants.MOVEMENTIDX_RUN:
				speedCoef = 0.66;
				break;
			case DayZPlayerConstants.MOVEMENTIDX_WALK:
				speedCoef = 0.33;
				break;
			case DayZPlayerConstants.MOVEMENTIDX_IDLE:
				speedCoef = 0;
				break;
		}
		
		return speedCoef;
	}

	protected float GetMovementStanceVisualCoef()
	{
		ref HumanMovementState	hms = new HumanMovementState();
		float stanceCoef = 1.0;

		m_pPlayer.GetMovementState(hms);
		switch(hms.m_iStanceIdx)
		{
			case DayZPlayerConstants.STANCEIDX_CROUCH:
			case DayZPlayerConstants.STANCEIDX_RAISEDCROUCH:
				stanceCoef = 0.33;
				break;
				
			case DayZPlayerConstants.STANCEIDX_PRONE:
			case DayZPlayerConstants.STANCEIDX_RAISEDPRONE:
				stanceCoef = 0.11;
				break;
		}
		
		return stanceCoef;
	}

		
	protected float GetMovementSpeedNoiseCoef()
	{
		float speedCoef = 0;
		
		ref HumanMovementState	hms = new HumanMovementState();

		//! noise multiplier based on player speed
		m_pPlayer.GetMovementState(hms);
		switch(AITargetCallbacksPlayer.StanceToMovementIdxTranslation(hms))
		{
			case DayZPlayerConstants.MOVEMENTIDX_SPRINT:
				speedCoef = 1.0;
				break;
			case DayZPlayerConstants.MOVEMENTIDX_RUN:
				speedCoef = 0.33;
				break;
			case DayZPlayerConstants.MOVEMENTIDX_WALK:
				speedCoef = 0.11;
				break;
		}

		return speedCoef;
	}

	protected float GetBootsNoiseCoef()
	{
		float bootsCoef = 0.0;

		ref HumanMovementState	hms = new HumanMovementState();
		m_pPlayer.GetMovementState(hms);

		if (hms.m_iMovement == DayZPlayerConstants.MOVEMENT_IDLE)
			return bootsCoef;
		
		//! noise multiplier based on type of boots
		switch(m_pPlayer.GetBootsType())
		{
			case AnimBootsType.Boots:
				bootsCoef = 1.0;
				break;
			case AnimBootsType.Sneakers:
				bootsCoef = 0.66;
				break;
		}
		
		return bootsCoef;
	}
	
	protected float GetLandNoise()
	{
		return m_LandNoise;
	}
	
	protected float GetSurfaceNoiseCoef()
	{
		ref HumanMovementState	hms = new HumanMovementState();
		m_pPlayer.GetMovementState(hms);

		if (hms.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_IDLE)
			return 0;
		
		return m_pPlayer.GetSurfaceNoise();
	}
	
	static void SetLandNoise()
	{
		m_LandNoise = 2.0;
	}

	static void ClearLandNoise()
	{
		m_LandNoise = 0.0;
	}
}