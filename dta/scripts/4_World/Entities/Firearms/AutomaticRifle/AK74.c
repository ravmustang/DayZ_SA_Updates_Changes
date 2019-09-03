class AK74_Base : AKM_Base
{
	override RecoilBase SpawnRecoilObject()
	{
		return new Ak74Recoil(this);
	}
};

class AK74 : AK74_Base {};