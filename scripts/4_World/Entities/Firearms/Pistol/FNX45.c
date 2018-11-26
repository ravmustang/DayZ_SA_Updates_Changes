class FNX45 : Pistol_Base
{
	override RecoilBase SpawnRecoilObject()
	{
		return new Fnx45Recoil(this);
	}
	
	void FNX45 ()
	{
	}
};