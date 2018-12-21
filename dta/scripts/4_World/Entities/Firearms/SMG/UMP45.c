class UMP45_Base : RifleBoltLock_Base
{ 
	void UMP_Base()
	{	
	}
	
	override RecoilBase SpawnRecoilObject()
	{
		return new Ump45Recoil(this);
	}
};