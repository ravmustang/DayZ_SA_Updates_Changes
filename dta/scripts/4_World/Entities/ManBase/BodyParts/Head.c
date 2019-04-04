class Head_Default extends EntityAI
{
	string selections_to_hide[HAIR_SELECTION_COUNT];
	
	void Head_Default()
	{
		Init();
	}
	
	void Init()
	{
		selections_to_hide[HAIR_SELECTION_COUNT] = {"Clipping_GhillieHood", "Clipping_grathelm", "Clipping_ConstructionHelmet", "Clipping_Hockey_hekmet", "Clipping_Maska", "Clipping_ProtecSkateHelmet2", "Clipping_BandanaFace", "Clipping_NioshFaceMask", "Clipping_NBC_Hood", "Clipping_MotoHelmet", "Clipping_FireHelmet", "Clipping_ushanka", "Clipping_TankerHelmet", "Clipping_SantasBeard", "Clipping_Surgical_mask", "Clipping_PumpkinHelmet", "Clipping_Balaclava_3holes", "Clipping_Balaclava", "Clipping_GP5GasMask", "Clipping_BoonieHat", "Clipping_prison_cap", "Clipping_MilitaryBeret_xx", "Clipping_Policecap", "Clipping_OfficerHat", "Clipping_Hat_leather", "Clipping_CowboyHat", "Clipping_BandanaHead", "Clipping_SantasHat", "Clipping_FlatCap", "Clipping_MxHelmet", "Clipping_baseballcap", "Clipping_BeanieHat", "Clipping_MedicalScrubs_Hat", "Clipping_RadarCap", "Clipping_ZmijovkaCap", "Clipping_HeadTorch", "Clipping_pilotka", "Clipping_MxHelmet", "Clipping_HelmetMich", "Clipping_Ssh68Helmet", "Clipping_Mich2001", "Clipping_Welding_Mask", "Clipping_VintageHockeyMask", "Clipping_mouth_rags", "Clipping_Gasmask"};
	}
	
	string GetHeadHideableSelections()
	{
		return selections_to_hide;
	}
};