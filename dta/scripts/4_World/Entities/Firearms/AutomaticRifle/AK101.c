class AK101 : AKM_Base
{
	override RecoilBase SpawnRecoilObject()
	{
		return new Ak101Recoil(this);
	}
};