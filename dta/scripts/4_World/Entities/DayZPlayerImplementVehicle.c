class GearChangeActionCallback : HumanCommandActionCallback
{	
	HumanCommandVehicle m_hcv;
	
	void SetVehicleCommand(HumanCommandVehicle hcv)
	{
		m_hcv = hcv;
		m_hcv.SetClutchState(true);
	}
	
	//Command events
	override void OnFinish(bool pCanceled)	
	{
		m_hcv.SetClutchState(false);
	}	
};

