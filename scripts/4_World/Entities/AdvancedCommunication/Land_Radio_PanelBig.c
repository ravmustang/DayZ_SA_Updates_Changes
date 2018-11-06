class Land_Radio_PanelBig extends StaticTransmitter
{
	// --- SYSTEM EVENTS
	override void OnStoreSave( ParamsWriteContext ctx )
	{   
		super.OnStoreSave( ctx );
		
		//store tuned frequency
		ctx.Write( GetTunedFrequencyIndex() );
	}
	
	override void OnStoreLoad( ParamsReadContext ctx )
	{
		super.OnStoreLoad( ctx );
		
		//load and set tuned frequency
		int tuned_frequency_idx;
		ctx.Read( tuned_frequency_idx );
		SetFrequencyByIndex( tuned_frequency_idx );
	}	
	
	//--- BASE
	override bool IsStaticTransmitter()
	{
		return true;
	}
	
	void SetNextFrequency( PlayerBase player = NULL )
	{
		SetNextChannel();
		
		if ( player )
		{
			DisplayRadioInfo( GetTunedFrequency().ToString(), player );
		}
	}

	//--- POWER EVENTS
	override void OnSwitchOn()
	{
		if ( !GetCompEM().CanWork() )
		{
			GetCompEM().SwitchOff();
		}
	}	
	
	override void OnWorkStart()
	{
		//turn on broadcasting/receiving
		EnableBroadcast ( true );
		EnableReceive ( true );
		SwitchOn ( true );
	}

	override void OnWorkStop()
	{
		//auto switch off (EM)
		GetCompEM().SwitchOff();
		
		//turn off broadcasting/receiving
		EnableBroadcast ( false );
		EnableReceive ( false );	
		SwitchOn ( false );		
	}
}
