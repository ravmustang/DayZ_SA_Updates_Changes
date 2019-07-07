class AK74 : AKM_Base
{
	override RecoilBase SpawnRecoilObject()
	{
		return new Ak74Recoil(this);
	}
};