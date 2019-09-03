class AK101_Base : AKM_Base
{
	override RecoilBase SpawnRecoilObject()
	{
		return new Ak101Recoil(this);
	}
};

class AK101 : AK101_Base {};