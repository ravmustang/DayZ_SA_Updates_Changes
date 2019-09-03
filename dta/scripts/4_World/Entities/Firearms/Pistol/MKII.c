class MKII_Base : Pistol_Base
{
	override RecoilBase SpawnRecoilObject()
	{
		return new MkiiRecoil(this);
	}
};
