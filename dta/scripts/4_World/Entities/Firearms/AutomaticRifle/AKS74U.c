class AKS74U : AK74_Base
{
	override RecoilBase SpawnRecoilObject()
	{
		return new Aks74uRecoil(this);
	}
};