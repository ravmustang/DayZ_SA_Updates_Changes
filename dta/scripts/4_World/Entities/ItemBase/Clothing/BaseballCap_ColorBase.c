class BaseballCap_ColorBase extends Clothing
{
	override bool CanPutAsAttachment( EntityAI parent )
	{
		if(!super.CanPutAsAttachment(parent)) {return false;}
		bool is_mask_only = false;
		
		if ( parent.FindAttachmentBySlotName( "Mask" ) )
		{
			is_mask_only = parent.FindAttachmentBySlotName( "Mask" ).ConfigGetBool( "noHelmet" );
		}
		
		if ( ( GetNumberOfItems() == 0 || !parent || parent.IsMan() ) && !is_mask_only )
		{
			return true;
		}
		return false;
	}
};


class BaseballCap_Blue extends BaseballCap_ColorBase {};
class BaseballCap_Beige extends BaseballCap_ColorBase {};
class BaseballCap_Black extends BaseballCap_ColorBase {};
class BaseballCap_Olive extends BaseballCap_ColorBase {};
class BaseballCap_Pink extends BaseballCap_ColorBase {};
class BaseballCap_Red extends BaseballCap_ColorBase {};
class BaseballCap_Camo extends BaseballCap_ColorBase {};
class BaseballCap_CMMG_Black extends BaseballCap_ColorBase {};
class BaseballCap_CMMG_Pink extends BaseballCap_ColorBase {};