class VomitSymptom extends SymptomBase
{
	//just for the Symptom parameters set-up and gets called even if the Symptom doesn't execute, don't put any gameplay code in here
	override void OnInit()
	{
		m_SymptomType = SymptomTypes.PRIMARY;
		m_Priority = 100;
		m_ID = SymptomIDs.SYMPTOM_VOMIT;
		m_DestroyOnAnimFinish = true;
		m_SyncToClient = true;
		m_Duration = 5;
		m_QuitAnimOnSymptomDestroy = true;
	}
	
	//!gets called every frame
	override void OnUpdateServer(PlayerBase player, float deltatime)
	{

	}

	override void OnUpdateClient(PlayerBase player, float deltatime)
	{

	}
	
	override bool CanActivate()
	{
		return ( m_Manager.GetCurrentCommandID() == DayZPlayerConstants.COMMANDID_MOVE || m_Manager.GetCurrentCommandID() == DayZPlayerConstants.COMMANDID_ACTION );
	}	
	
	//!gets called once on an Symptom which is being activated
	override void OnGetActivatedServer(PlayerBase player)
	{
		//timer.Run(10, this, "Destroy");
		Debug.Log("OnGetActivated VomitSymptom called", "PlayerSymptom");
		player.GetStatStomachVolume().Set(0);
		player.GetStatStomachWater().Set(0);
		player.GetStatStomachEnergy().Set(0);
		PlayAnimation(DayZPlayerConstants.CMD_ACTIONFB_VOMIT,DayZPlayerConstants.STANCEMASK_CROUCH, GetDuration() );
	}

	//!only gets called once on an active Symptom that is being deactivated
	override void OnGetDeactivatedServer(PlayerBase player)
	{
		Debug.Log("OnGetDeactivated VomitSymptom called", "PlayerSymptom");
	}

	override void OnGetDeactivatedClient(PlayerBase player)
	{
		Debug.Log("OnGetDeactivated VomitSymptom called", "PlayerSymptom");
	}
}