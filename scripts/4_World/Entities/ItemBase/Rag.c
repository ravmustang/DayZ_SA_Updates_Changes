class Rag extends ItemBase
{
	override bool CanPutAsAttachment( EntityAI parent )
	{
		if(!super.CanPutAsAttachment(parent))
		{
			return false;
		}
		if(GetQuantity() > 1 && PlayerBase.Cast(parent))
		{
			return false;
		}
		return true;
	}
}