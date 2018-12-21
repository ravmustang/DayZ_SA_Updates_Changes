class SurrenderDummyItem extends DummyItem
{
	protected PlayerBase m_player;
	
	void ~SurrenderDummyItem()
	{
		/*m_player = PlayerBase.Cast(GetHierarchyRootPlayer());
		if (m_player && m_player.m_EmoteManager)
			PlayerBase.Cast(m_player).m_EmoteManager.PlaySurrenderInOut(false); //has to be triggered separately in order to avoid locomotion state conflict ("playerballing")
		else
			Print("Error, no player owns  SurrenderDummyItem");*/
	}
	
	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{
		super.OnItemLocationChanged(old_owner,new_owner);
		
		//enters player's inventory
		if (new_owner)
		{
			//m_player = PlayerBase.Cast(new_owner);
			//PlayerBase.Cast(new_owner).m_EmoteManager.PlaySurrenderInOut(true);
		}
		// may not work on item destroy?
		else
		{
			Delete();
			//PlayerBase.Cast(old_owner).m_EmoteManager.PlaySurrenderInOut(false);
		}
	}
}