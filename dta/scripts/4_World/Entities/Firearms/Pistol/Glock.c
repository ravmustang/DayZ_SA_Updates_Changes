class Glock19_Base : Pistol_Base
{
	override RecoilBase SpawnRecoilObject()
	{
		return new GlockRecoil(this);
	}
};