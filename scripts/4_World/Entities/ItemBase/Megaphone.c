class Megaphone extends ItemMegaphone
{
	override bool IsTransmitter()
	{
		return true;
	}	

	override void OnWorkStart()
	{
		//turn device on
		this.SwitchOn ( true ); // Note: This is not Energy Manager function. This is engine function.
	}
	
	override void OnWorkStop()
	{
		//turn off device
		this.SwitchOn ( false ); // Note: This is not Energy Manager function. This is engine function.
	}
}
