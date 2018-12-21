class GiftBox_Base extends Container_Base
{
	override bool IsContainer()
	{
		return true;
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}	
		if ( parent != this && ( this.GetType() != parent.GetType() ) )
		{
			if ( (parent.IsInherited(GiftBox_Small) && IsInherited(GiftBox_Small)) || (parent.IsInherited(GiftBox_Medium) && IsInherited(GiftBox_Medium)) || (parent.IsInherited(GiftBox_Large) && IsInherited(GiftBox_Large)))
			{
				if ( GetNumberOfItems() != 0 )
				{
					return false;
				}
				return true;
			}
			return true;
		}
		
		return false;
	}
}

class GiftBox_Small extends GiftBox_Base {}
class GiftBox_Medium extends GiftBox_Base {}
class GiftBox_Large extends GiftBox_Base {}