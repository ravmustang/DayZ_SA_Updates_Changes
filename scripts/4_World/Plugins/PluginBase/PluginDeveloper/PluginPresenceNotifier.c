class PluginPresenceNotifier extends PluginBase
{
	const int windowPosX = 0;
	const int windowPosY = 50;
	const int mainPanelSizeX = 200;
	const int mainPanelSizeY = 1;
	const int margin = 10;
	
	protected PlayerBase m_pPlayer;

	void PluginPresenceNotifier() {}
	
	void ~PluginPresenceNotifier() {}

	void UpdatePresenceNotifier(PlayerBase pPlayer, bool pEnabled)
	{
		m_pPlayer = pPlayer;
		
		ShowCoefsDbg(pEnabled);
	}
	
	protected void ShowCoefsDbg(bool pEnabled)
	{
		DbgUI.BeginCleanupScope();

		if (pEnabled)
		{
			float visualMean = (m_pPlayer.GetVisibilityCoef() + GetMovementSpeedVisualCoef() + GetMovementStanceVisualCoef()) / 3;
			float noiseMean = (GetSurfaceNoiseCoef() + GetMovementSpeedNoiseCoef() + GetBootsNoiseCoef()) / 3;
			float resultMean = (visualMean + noiseMean) / 2;

			DbgUI.Begin("Presence Notifier", windowPosX, windowPosY);
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
		}

		DbgUI.EndCleanupScope();
	}
	
	
	protected float GetMovementSpeedVisualCoef()
	{
		ref HumanMovementState	hms = new HumanMovementState();
		float speedCoef = 1.0;

		m_pPlayer.GetMovementState(hms);
		switch(hms.m_iMovement)
		{
			case DayZPlayerConstants.MOVEMENTIDX_RUN:
				speedCoef = 0.66;
				break;
			case DayZPlayerConstants.MOVEMENTIDX_WALK:
				speedCoef = 0.33;
				break;
			case DayZPlayerConstants.MOVEMENTIDX_IDLE:
				speedCoef = 0.11;
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
				stanceCoef = 0.66;
				break;
				
			case DayZPlayerConstants.STANCEIDX_PRONE:
			case DayZPlayerConstants.STANCEIDX_RAISEDPRONE:
				stanceCoef = 0.33;
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
		switch(hms.m_iMovement)
		{
			case DayZPlayerConstants.MOVEMENTIDX_SPRINT:
				speedCoef = 1.0;
				break;
			case DayZPlayerConstants.MOVEMENTIDX_RUN:
				speedCoef = 0.66;
				break;
			case DayZPlayerConstants.MOVEMENTIDX_WALK:
				speedCoef = 0.33;
				break;
		}

		return speedCoef;
	}

	protected float GetBootsNoiseCoef()
	{
		float bootsCoef = 0;

		ref HumanMovementState	hms = new HumanMovementState();
		m_pPlayer.GetMovementState(hms);

		if (hms.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_IDLE)
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
			case AnimBootsType.None:
				bootsCoef = 0.33;
				break;
		}
		
		return bootsCoef;
	}
	
	protected float GetSurfaceNoiseCoef()
	{
		ref HumanMovementState	hms = new HumanMovementState();
		m_pPlayer.GetMovementState(hms);

		if (hms.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_IDLE)
			return 0;
		
		return m_pPlayer.GetSurfaceNoise();
	}
}