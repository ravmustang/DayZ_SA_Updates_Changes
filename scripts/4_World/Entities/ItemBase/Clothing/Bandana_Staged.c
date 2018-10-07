enum EUniversal_Bandana_Redpattern_stages
{
	UNTIED,
	MASK,
	HEADHEAR,
	MAX
}

class Universal_Bandana_Redpattern extends ClothingBase
{	
	void Universal_Bandana_Redpattern()
	{
		//SetItemStage(EUniversal_Bandana_Redpattern_stages.UNTIED); //starts at stage 0
	}
	
	override bool CanPutAsAttachment( EntityAI parent )
	{
		if(!super.CanPutAsAttachment(parent)) {return false;}
		if ( parent.IsMan() )// TODO
		{
			return true;
		}
		return false;
	}
	
	override void OnWasAttached( EntityAI parent, int slot_id )
	{
		super.OnWasAttached(parent, slot_id);
		
		switch (slot_id)
		{
			case InventorySlots.MASK:
				SetItemStage(EUniversal_Bandana_Redpattern_stages.MASK);
			break;
			case InventorySlots.HEADGEAR:
				SetItemStage(EUniversal_Bandana_Redpattern_stages.HEADHEAR);
			break;
		}
		Print("stage_attach = " + m_Item_Stage);
	}
	
	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		super.OnWasDetached(parent, slot_id);
		
		SetItemStage(EUniversal_Bandana_Redpattern_stages.UNTIED);
		Print("stage_unattach = " + m_Item_Stage);
	}
}