class CoughState extends StateBase
{
	//this is just for the state parameters set-up and is called even if the state doesn't execute, don't put any gameplay code in here
	override void OnInit()
	{
		m_StateType = StateTypes.PRIMARY;
		m_Priority = 0;
		m_ID = StateIDs.STATE_COUGH;
		m_DestroyOnAnimFinish = true;
		m_SyncToClient = false;
	}
	
	//!gets called every frame
	override void OnUpdateServer(PlayerBase player, float deltatime)
	{

	}

	override void OnUpdateClient(PlayerBase player, float deltatime)
	{
	}
	
	//!gets called once on an state which is being activated
	override void OnGetActivatedServer(PlayerBase player)
	{
		PlaySound( EPlayerSoundEventID.SYMPTOM_COUGH );
		player.SpreadAgents();
	}

	//!gets called once on a state which is being activated
	override void OnGetActivatedClient(PlayerBase player)
	{
	}

	//!only gets called once on an active state that is being deactivated
	override void OnGetDeactivatedServer(PlayerBase player)
	{
		Debug.Log("OnGetDeactivated CoughState called", "PlayerState");
	}

	//!only gets called once on an active state that is being deactivated
	override void OnGetDeactivatedClient(PlayerBase player)
	{
		Debug.Log("OnGetDeactivated CoughState called", "PlayerState");
	}
}
