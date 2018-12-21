class M4_RISHndgrd extends Inventory_Base
{
	override bool CanDetachAttachment (EntityAI parent)
	{
		if ( Weapon_Base.Cast(parent) && parent.FindAttachmentBySlotName("weaponFlashlight")/*.IsKindOf("UniversalLight")*/ )
		{
			return false;
		}
		return true;
	}
	
	/*override bool CanReleaseAttachment( EntityAI attachment )
	{
		if ( FindAttachmentBySlotName("weaponHandguardM4").IsKindOf("M4_RISHndgrd")
		
		return false;
	}*/
}