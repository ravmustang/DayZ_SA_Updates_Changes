class BoonieHat_ColorBase extends Clothing
{
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		if ( parent != this )
		{
			return true;
		}
		return false;
	}
}

class BoonieHat_Black extends BoonieHat_ColorBase {};
class BoonieHat_Blue extends BoonieHat_ColorBase {};
class BoonieHat_DPM extends BoonieHat_ColorBase {};
class BoonieHat_Dubok extends BoonieHat_ColorBase {};
class BoonieHat_Flecktran extends BoonieHat_ColorBase {};
class BoonieHat_NavyBlue extends BoonieHat_ColorBase {};
class BoonieHat_Olive extends BoonieHat_ColorBase {};
class BoonieHat_Orange extends BoonieHat_ColorBase {};
class BoonieHat_Red extends BoonieHat_ColorBase {};
class BoonieHat_Tan extends BoonieHat_ColorBase {};