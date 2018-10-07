class Land_Radio_PanelPAS extends PASBroadcaster
{
	//--- BASE
	override bool IsStaticTransmitter()
	{
		return true;
	}

	//--- ACTION CONDITION
	bool CanOperate()
	{
		return GetCompEM().IsSwitchedOn();
	}
	
	//--- POWER EVENTS
	override void OnWorkStart()
	{
		//turn off device
		SwitchOn ( true ); // start send/receive voice
	}
	
	override void OnWorkStop()
	{
		//turn off device
		SwitchOn ( false ); // stop send/receive voice
	}
	
	//--- ACTION EVENTS
	override void OnSwitchOn()
	{
		
	}
	
	override void OnSwitchOff()
	{
		
	}
}
