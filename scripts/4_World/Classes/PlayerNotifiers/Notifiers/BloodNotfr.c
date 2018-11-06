class BloodNotfr: NotifierBase
{
	protected const float 	DEC_TRESHOLD_LOW 	= -0.001;
	protected const float 	DEC_TRESHOLD_MED 	= -1;
	protected const float 	DEC_TRESHOLD_HIGH	= -2;
	protected const float 	INC_TRESHOLD_LOW 	= 0.001;
	protected const float 	INC_TRESHOLD_MED 	= 1;
	protected const float 	INC_TRESHOLD_HIGH	= 2;

	void BloodNotfr(NotifiersManager manager)
	{
	}

	override int GetNotifierType()
	{
		return eNotifiers.NTF_BLOOD;
	}

	override void DisplayTendency(float delta)
	{
		int tendency = CalculateTendency(delta, INC_TRESHOLD_LOW, INC_TRESHOLD_MED, INC_TRESHOLD_HIGH, DEC_TRESHOLD_LOW, DEC_TRESHOLD_MED, DEC_TRESHOLD_HIGH);
		
		//PrintString(delta.ToString());
		//GetVirtualHud().SetStatus(eDisplayElements.DELM_TDCY_BLOOD,tendency);
		
		DSLevels level = DetermineLevel( GetObservedValue(), PlayerConstants.THRESHOLD_BLOOD_WARNING,  PlayerConstants.THRESHOLD_BLOOD_CRITICAL, PlayerConstants.THRESHOLD_BLOOD_EMPTY );
		DisplayElementTendency dis_elm = DisplayElementTendency.Cast(GetVirtualHud().GetElement(eDisplayElements.DELM_TDCY_BLOOD));
		
		if( dis_elm )
		{
			dis_elm.SetTendency(tendency);
			dis_elm.SetSeriousnessLevel(level);
			
		}
		
	}

	override float GetObservedValue()
	{
		float blood = m_Player.GetHealth("GlobalHealth","Blood");
		return blood;
	}
};