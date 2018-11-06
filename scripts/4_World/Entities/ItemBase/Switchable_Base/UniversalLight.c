class UniversalLight extends Switchable_Base
{
	override bool CanPutAsAttachment( EntityAI parent )
	{
		if(!super.CanPutAsAttachment(parent)) {return false;}
		bool m4_rishndgrd   = false;
		bool ak_railhndgrd  = false;
		bool mp5_railhndgrd = false;

		if ( parent.FindAttachmentBySlotName("weaponHandguardM4") != NULL )
		{
			m4_rishndgrd = parent.FindAttachmentBySlotName("weaponHandguardM4").IsKindOf("M4_RISHndgrd");
		}
		
		if ( parent.FindAttachmentBySlotName("weaponHandguardAK") != NULL )
		{
			ak_railhndgrd = parent.FindAttachmentBySlotName("weaponHandguardAK").IsKindOf("AK_RailHndgrd");
		}

		if ( parent.FindAttachmentBySlotName("weaponHandguardMP5") != NULL )
		{
			mp5_railhndgrd = parent.FindAttachmentBySlotName("weaponHandguardMP5").IsKindOf("MP5_RailHndgrd");
		}

		if ( m4_rishndgrd || ak_railhndgrd || mp5_railhndgrd || parent.IsKindOf("UMP45_Base") )
		{
			return true;
		}
		
		return false;
	}
	
	//copied from Flashlight, TODO redo
	bool ignore1cycle = true;
	
	void UniversalLight()
	{
	}
	
	//--- POWER EVENTS
	override void OnWorkStart()
	{
		ignore1cycle = true;
		SetPilotLight(true);
	}

	override void OnWorkStop()
	{
		ignore1cycle = true;
		SetPilotLight(false);
	}
	
	/*override void OnWork( float consumed_energy)
	{
	}*/
	
	void FixColdContact()
	{
		ItemBase battery = ItemBase.Cast( GetInventory().GetAttachmentFromIndex(0) );
		
		if (battery)
		{
			ignore1cycle = true;
			GetCompEM().PlugThisInto(battery);
		}
	}
}